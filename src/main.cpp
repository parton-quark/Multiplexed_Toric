/* l1 × l2 Toric codes over erasure channel*/
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include "lattice.h"
#include "prob.h"

int main()
{   //　Lattice size l1 times l2
    int l1,l2;
    std::cout << "l1:";
    std::cin >> l1;
    std::cout << "\nl2:";
    std::cin >> l2;

    // Number of qubits per photon
    int multiplexing; 
    std::cout  << "\nmultiplexing:";
    std::cin >> multiplexing;

    // randomly assign qubits to the photons
    int num_photons;
    num_photons = (l1*l2) / multiplexing;
    if ((l1*l2) % multiplexing > 0){
        num_photons  = num_photons  + 1;
    }
    std::cout  << "\nnum_photons:";
    std::cout  << num_photons;

    // photons: vector of photon(vector of qubits) 
    int num_qubits = l1*l2;
    std::cout << "\nnum_qubits:";
    std::cout << num_qubits;
 
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }

    std::vector<std::vector<int>> photons;
    for (int ph = 0; ph < num_photons; ph++){
        std::cout << "\nphoton ";
        std::cout << ph;
        std::cout << ": ";
        std::vector<int> photon;
        for (int qb = 0; qb < multiplexing; qb++){
            std::srand(time(NULL));
            if (left_qubits.empty()== true){
                break;
            }else{
            int qubit;
            qubit = left_qubits[rand() % left_qubits.size()];
            left_qubits.erase(std::remove(std::begin(left_qubits), std::end(left_qubits), qubit), std::cend(left_qubits));
            photon.push_back(qubit);
            std::cout << qubit;
            std::cout << ",";
            }
        }
        photons.push_back(photon);
    }
    // photonsに入ってる全ての量子ビット
    // for (std::vector<int> a : photons) {
    //     for (int b : a) {
    //         std::cout << b << "\n";
    //     }
    // }
    
    // input erasure probability
    float prob;
    std::cout << "\nprob:";
    std::cin >> prob;

    // make erasure vector for photons
    std::vector<bool> erased_photons;

    std::random_device rd;
    std::mt19937 engine;
    std::uniform_real_distribution<double> dist(0,1);


    for (int phys_ph = 0; phys_ph < num_photons; phys_ph++){
        bool is_erased = false;
        is_erased = probabilistic(prob, engine, dist);
        erased_photons.push_back(is_erased);
        // std::cout << "\nis_erased:";
        // std::cout << is_erased << "\n";
    }

    // print erased_photons
    std::cout << "\nerased photons:";
    for (bool c : erased_photons) {
        std::cout << c << "\n";
    }
    // make erasure vector for qubits
    // make pauli error vector for X and Z
    // input the measurement error 
    // decoding
    // show the result of decoding 
    // destructive measurement

    return 0;
}