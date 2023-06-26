#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include "lattice.hpp"

template<typename T> void pop_front(std::vector<T> &v){
    if (v.size() > 0) {
        v.erase(v.begin());
    }
}

std::vector<std::vector<int> > assign_without_multiplexing(int l1, int l2, int num_photons, int num_qubits){
    std::vector<std::vector<int> > photons;
    for (int ph = 0; ph < num_photons; ph++){
        std::vector<int> photon;
        photon.push_back(ph);
        photons.push_back(photon);
    }
    return photons;
}

std::vector<std::vector<int> > assign_random(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // randomly assign qubits
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }

    for (int ph = 0; ph < num_photons; ph++){
        std::vector<int> photon;
        for (int qb = 0; qb < multiplexing; qb++){
            std::srand(time(NULL));
            if (left_qubits.empty()== true){
                break;
            }else{
            int qubit;
            qubit = left_qubits[rand() % left_qubits.size()];
            left_qubits.erase(std::remove(std::begin(left_qubits), std::end(left_qubits), qubit), std::end(left_qubits));
            photon.push_back(qubit);
            }
        }
        // std::cout << "\n photon: ";
        // for (int ph: photon){
        //     std::cout << ph << ", ";
        // }
        photons.push_back(photon);
    }
    return photons;
}

std::pair<std::vector<std::vector<int> >, int> assign_random_distance(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // Adopt or reject multiple qubits that are more than a threshold distance apart and assign them to the same photon
    // If allocation is not possible within the threshold, update the threshold
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    int threshold;
    threshold = (l1 + l2) / 2;
    std::cout << "\nthreshold                 :" << threshold;

    for (int ph = 0; ph < num_photons; ph++){
        std::vector<int> photon;
        std::random_device rd;
        std::mt19937 engine(rd());
        std::shuffle(left_qubits.begin(), left_qubits.end(), engine);
        int first_qubit_in_photon;
        first_qubit_in_photon = left_qubits[left_qubits.size() - 1];
        photon.push_back(first_qubit_in_photon);
        // std::cout << "\nfirst qubit in this photon:" << first_qubit_in_photon;
        // std::cout << "\nleft_qubits        ";
        // print_vec(left_qubits);
        left_qubits.erase(std::remove(left_qubits.begin(), left_qubits.end(),first_qubit_in_photon), left_qubits.end());
        // std::cout << "\nleft_qubits updated";
        // print_vec(left_qubits);

        bool is_photon_sat;
        is_photon_sat = false;
        while (!is_photon_sat){
            std::shuffle(left_qubits.begin(), left_qubits.end(), engine);
            for (int candidate_qb: left_qubits){
                bool has_enough_distance;
                has_enough_distance = false;
                
                int distance;
                distance = 0;
                for (int other_qb_in_same_ph: photon){
                    int dist_other_qb;
                    dist_other_qb = distance_e(l1, l2, candidate_qb, other_qb_in_same_ph);
                    if (dist_other_qb > distance){
                        distance = dist_other_qb;
                    }
                }
                if (distance >= threshold){
                    photon.push_back(candidate_qb);

                    if (photon.size() == multiplexing){
                        is_photon_sat = true;
                        photons.push_back(photon);
                        for (int qubit: photon){
                            left_qubits.erase(std::remove(left_qubits.begin(), left_qubits.end(), qubit), left_qubits.end());
                        }
                    }
                } 
            }
            if (!is_photon_sat){
                threshold = threshold - 1;
                std::cout << "\nthreshold update          :" << threshold << std::flush;

            }
        }
    }
    // std::cout << "\nleft_qubits               :";
    // for (int lq: left_qubits){
    //     std::cout << lq;
    //     std::cout << ",";
    // }
    // std::cout << "\nnum_assigned_qubits: " << num_assigned_qubits;
    std::pair<std::vector<std::vector<int> >, int> photon_and_threshold;
    photon_and_threshold.first = photons;
    photon_and_threshold.second = threshold;
    return photon_and_threshold;
}

// assign_deterministic
// 決定論的に量子ビットを配置する

std::vector<std::vector<int> > assign_deterministic(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // 量子ビットを割り当てる...
    // l１, l2ともに偶数
    // multiplexing = 2
    std::vector<int> photon_of_qubits;

    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    for (int left_qubit: left_qubits){
        if (left_qubit % 2 == 0){
            // vertical
            // std::cout << " vertical"<< std::flush;
            if (left_qubit < l1 * l2){
                // lower block
                // std::cout << " lower"<< std::flush;
                int photon_index;
                photon_index = left_qubit  / 2; 
                photon_index = photon_index + (num_photons) / 2; 
                photon_of_qubits.push_back(photon_index);
                
            } else {
                // upper block
                // std::cout << " upper"<< std::flush;
                int photon_index;
                photon_index = (left_qubit - (l1 * l2)) / 2; /*上段と同じ並び ここから半分ずらす*/
                int row_index, column_index;
                row_index = photon_index / l2;
                column_index =  photon_index % l2;
                if (column_index < (l2 / 2)){
                    // 左側なので右側にずらす
                    photon_index =  photon_index + (l2/2);
                } else {
                    // 右側
                    photon_index = photon_index - (l2/2);
                }
                photon_index = photon_index + (num_photons) / 2;
                // photons[photon_index].push_back(left_qubit);
                photon_of_qubits.push_back(photon_index);
            }
        } else if (left_qubit % 2 == 1){
            //　horizontal
            // std::cout << " horizontal"<< std::flush;
            if (left_qubit < l1 * l2){
                // lower block
                // std::cout << " lower"<< std::flush;
                int photon_index;
                photon_index = left_qubit / 2;
                // photons[photon_index].push_back(left_qubit);
                photon_of_qubits.push_back(photon_index);
            } else {
                // upper block
                // std::cout << " upper"<< std::flush;
                int photon_index;
                photon_index = (left_qubit - (l1 * l2)) / 2;
                int row_index, column_index;
                row_index = photon_index / l2;
                column_index =  photon_index % l2;
                if (column_index < (l2 / 2)){
                    // 左側なので右側にずらす
                    photon_index =  photon_index + (l2/2);
                } else {
                    // 右側なので左側にずらす
                    photon_index =  photon_index - (l2/2);
                }
                // photons[photon_index].push_back(left_qubit);
                photon_of_qubits.push_back(photon_index);
            }
        }    
    }
    std::vector<std::vector<int> > photons = {};

    for (int i = 0; i < num_photons; i++){
        std::pair<int, int> photon;
        bool is_first_qubit;
        is_first_qubit = true;
        for (int j = 0; j < num_qubits; j++){
            if (photon_of_qubits[j] == i){
                if (is_first_qubit){
                    photon.first = j;
                    is_first_qubit = false;
                } else {
                    photon.second = j;
                }
            } else {
                //
            }
        }
        // photonをvectorに変換
        std::vector<int> photon_vec = {photon.first, photon.second};
        photons.push_back(photon_vec);
    }
    return photons;
}
