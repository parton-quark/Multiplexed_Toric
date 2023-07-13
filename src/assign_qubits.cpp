#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <math.h>
#include "print.hpp"
#include "lattice.hpp"
#include "prob.hpp"


template<typename T> void remove_with_index(std::vector<T>& vector, unsigned int index){
    vector.erase(vector.begin() + index);
}

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
        photons.push_back(photon);
    }
    return photons;
}

std::pair<std::vector<std::vector<int> >, int> assign_random_distance(int l1, int l2, int multiplexing, int num_photons, int num_qubits, std::mt19937& engine){
    // Adopt or reject multiple qubits that are more than a threshold distance apart and assign them to the same photon
    // If allocation is not possible within the threshold, update the threshold
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    int threshold;
    threshold = ((l1 + l2) / 4) - 1;
    // std::cout << "\nthreshold                 :" << threshold;
    for (int ph = 0; ph < num_photons; ph++){
        std::vector<int> photon;
        std::shuffle(left_qubits.begin(), left_qubits.end(), engine);
        int first_qubit_in_photon;
        first_qubit_in_photon = left_qubits[left_qubits.size() - 1];
        photon.push_back(first_qubit_in_photon);
        left_qubits.erase(std::remove(left_qubits.begin(), left_qubits.end(),first_qubit_in_photon), left_qubits.end());
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
    std::pair<std::vector<std::vector<int> >, int> photon_and_threshold;
    photon_and_threshold.first = photons;
    photon_and_threshold.second = threshold;
    return photon_and_threshold;
}

// assign_deterministic
std::vector<std::vector<int> > assign_deterministic(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // multiplexing = 2
    std::vector<int> photon_of_qubits;

    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    for (int left_qubit: left_qubits){
        if (left_qubit % 2 == 0){
            // vertical
            if (left_qubit < l1 * l2){
                // lower block
                int photon_index;
                photon_index = left_qubit  / 2; 
                photon_index = photon_index + (num_photons) / 2; 
                photon_of_qubits.push_back(photon_index);
                
            } else {
                // upper block
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
                int photon_index;
                photon_index = left_qubit / 2;
                photon_of_qubits.push_back(photon_index);
            } else {
                // upper block
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

std::vector<std::vector<int> > assign_deterministic_shrink(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    for (int ph = 0; ph  < num_photons; ph++){
        std::vector<int> photon;
        for (int qubit = multiplexing * ph; qubit < multiplexing * (ph + 1); qubit++){
            photon.push_back(left_qubits[qubit]);
        }
        photons.push_back(photon);
    }
    return photons;
}

std::vector<float> weights_from_degrees(std::vector<int> degrees, int force){
    // 次数rのベクトルを受け取って、AF^rに直す
    std::vector<int> vec_fr;
    
    for (int degree: degrees){
        int fr;
        fr = pow(force, degree);
        // std::cout << "\nfr: ";
        // std::cout << fr;
        vec_fr.push_back(fr);
    }
    // 規格化するために和を取る
    int sum;
    sum = 0;
    for (int fr: vec_fr){
        sum = sum  + fr;
    }
    // std::cout << "\nsum: ";
    // std::cout << sum;

    // 規格化する
    std::vector<float> vec_afr;
    for (int fr: vec_fr){
        float afr;
        afr = (float) fr / (float) sum;
        vec_afr.push_back(afr);
    }
    return vec_afr;
}

std::vector<std::vector<int> > assign_random_with_occupation_enhancement(int l1, int l2, int multiplexing, int num_photons, int num_qubits, int force, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // Inspired by L. J. Duckers, and R. G. Ross. "Percolation with non-random site occupation." Physics Letters A 49.5 (1974): 361-362. 
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    std::cout << "\nassignment random with occupation enhancement" << std::flush;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }

    // std::cout << "\nleft qubits:" << std::flush;
    // print_vec(left_qubits);

    for (int ph = 0; ph < num_photons; ph++){
        // initialize degree vector
        std::vector<int> degrees;
        for (int left_qubit: left_qubits){
            int deg = 0;
            degrees.push_back(deg);
        }
        // convert degree vector into weight vector (AF^r)
        std::vector<float> weights;
        weights = weights_from_degrees(degrees, force);

        std::vector<int> photon;
        for (int qb = 0; qb < multiplexing; qb++){
            std::cout << "\nleft qubits:" << std::flush;
            print_vec(left_qubits);
            std::cout << "\ndegrees:" << std::flush;
            print_vec(degrees);

            // pick weighted random
            std::pair <int, int> selected_item_and_index;
            int selected_qubit, selected_index;
            selected_item_and_index = weighted_random(left_qubits, weights, engine, dist);
            selected_qubit = selected_item_and_index.first;
            selected_index = selected_item_and_index.second;

            photon.push_back(selected_qubit);
            // remove degree of selected_qubit

            degrees.erase(degrees.begin() + selected_index);
            // remove_with_index(degrees, selected_qubit_index);
            std::cout << "\nThis is 293" << std::flush;
            // remove selected_qubit from left_qubits
            left_qubits.erase(remove(left_qubits.begin(), left_qubits.end(), selected_qubit), left_qubits.end());

            // update degrees
            std::vector<int> vertices;// vertices of the selected qubit
            vertices =  edge_to_vertices(l1, l2, selected_qubit);
            std::vector<int> adjacent_qubits; 
            for (int vertex: vertices){
                std::vector<int> edges;
                edges = vertex_to_edges(l1, l2, vertex);
                for (int edge: edges){
                    adjacent_qubits.push_back(edge);
                }
            }
            adjacent_qubits.erase(remove(adjacent_qubits.begin(), adjacent_qubits.end(), selected_qubit), adjacent_qubits.end());
            adjacent_qubits.erase(remove(adjacent_qubits.begin(), adjacent_qubits.end(), selected_qubit), adjacent_qubits.end());

            for (int adjacent_qubit: adjacent_qubits){
                size_t adj_count = std::count(left_qubits.begin(), left_qubits.end(), adjacent_qubit);

                if (adj_count > 0){
                    // update degree
                    int adjacent_qubits_index;
                    adjacent_qubits_index = 0;
                    for (int left_q: left_qubits){
                        if (left_q == adjacent_qubit){
                            break;
                        }
                    }
                    degrees[adjacent_qubits_index] = degrees[adjacent_qubits_index] + 1;
                }
            }
            // update weights
            weights = weights_from_degrees(degrees, force);
        }
        photons.push_back(photon);
    }
    return photons;
}