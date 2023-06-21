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
        for (int ph: photon){
            std::cout << ph << ", ";
        }
        photons.push_back(photon);
    }
    return photons;
}

std::pair<std::vector<std::vector<int> >, int> assign_random_distance(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
// std::vector<std::vector<int> > assign_random_distance(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // Adopt or reject multiple qubits that are more than a threshold distance apart and assign them to the same photon
    // If allocation is not possible within the threshold, update the threshold
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    int threshold;
    threshold = (l1 + l2) / 2 ;
    // std::cout << "\nthreshold: " << threshold << std::flush;
    for (int ph = 0; ph < num_photons; ph++){
        std::random_device rd;
        std::mt19937 engine(rd());
        // std::mt19937 get_rand_mt;
        std::shuffle(left_qubits.begin(), left_qubits.end(), engine);
        std::vector<int> photon;

        int first_qubit_in_photon;
        first_qubit_in_photon = left_qubits[0];
        photon.push_back(first_qubit_in_photon);
        pop_front(left_qubits);

        bool is_photon_sat;
        is_photon_sat = false;
        while (!is_photon_sat){
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

                        // std::cout << "\n photon: ";
                        // for (int ph: photon){
                            // std::cout << ph << ", ";
                        // }
                    }
                } 
            }
            if (!is_photon_sat){
                threshold = threshold - 1;
                // std::cout << "\nthreshold update: " << threshold << std::flush;

            }
        }
    }
    std::pair<std::vector<std::vector<int> >, int> photon_and_threshold;
    photon_and_threshold.first = photons;
    photon_and_threshold.second = threshold;
    return photon_and_threshold;
}