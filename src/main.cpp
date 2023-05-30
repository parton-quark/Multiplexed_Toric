/* l1 × l2 Toric codes over erasure channel*/
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include "lattice.h"
#include "prob.h"

int main()
{   //　lattice size: l1 times l2
    int l1,l2;
    std::cout << "l1:";
    std::cin >> l1;
    std::cout << "\nl2:";
    std::cin >> l2;

    // number of qubits per photon
    int multiplexing; 
    std::cout  << "\nmultiplexing:";
    std::cin >> multiplexing;

    // photons: vector of photon(vector of qubits) 
    int num_qubits = l1*l2;
    std::cout << "\nnum_qubits:";
    std::cout << num_qubits;

    // randomly assign qubits to the photons
    int num_photons;
    num_photons = num_qubits / multiplexing;
    if (num_qubits % multiplexing > 0){
        num_photons  = num_photons  + 1;
    }
    std::cout  << "\nnum_photons:";
    std::cout  << num_photons;
 
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
    
    // input erasure probability
    float prob;
    std::cout << "\nprob:";
    std::cin >> prob;

    // make erasure vector for photons
    std::vector<bool> erased_photons;
    // make random distribution
    std::random_device rd;
    std::mt19937 engine;
    std::uniform_real_distribution<double> dist(0,1);

    for (int phys_ph = 0; phys_ph < num_photons; phys_ph++){
        bool is_erased = false;
        is_erased = probabilistic(prob, engine, dist);
        erased_photons.push_back(is_erased);
    }

    // print erased_photons
    std::cout << "\nerasure vector for photons:";
    for (bool c : erased_photons) {
        std::cout << c << ",";
    }

    // make erasure vector for qubits
    std::vector<bool> erased_qubits(num_qubits);

    for (int ph = 0; ph < num_photons; ph++){
        if (erased_photons[ph] == true){
            for (int qu = 0 ; qu < num_qubits; qu++){
                int qubitlabel;
                qubitlabel = photons[ph][qu];
                erased_qubits[qubitlabel] = 1;
            }
        }else if (erased_photons[ph] == false){
            for (int qu = 0 ; qu < num_qubits; qu++){
                int qubitlabel;
                qubitlabel = photons[ph][qu];
                erased_qubits[qubitlabel] = 0;
            }
        }
    } 
    // print erased_qubits
    std::cout << "\nerasure vector for qubits :";
    for (bool c : erased_qubits) {
        std::cout << c << ",";
    }

    // make X error vector
    std::vector<char> xerrors(num_qubits);
    int qubit_index_x = 0; 
    for (bool qubit : erased_qubits){
        if (qubit == true){
            bool is_xerror = false;
            is_xerror = probabilistic(0.5, engine, dist);
            xerrors[qubit_index_x]  = is_xerror;
        }else if (qubit == false){
            bool is_xerror = false;
            xerrors[qubit_index_x]  = is_xerror;
        }
        qubit_index_x = qubit_index_x + 1;
    }
    // print xerrors
    std::cout << "\nX errors on qubits        :";
    for (bool c : xerrors) {
        std::cout << c << ",";
    }
    // make Z error vector
    std::vector<char> zerrors(num_qubits);
    int qubit_index_z = 0; 
    for (bool qubit : erased_qubits){
        if (qubit == true){
            bool is_zerror = false;
            is_zerror = probabilistic(0.5, engine, dist);
            zerrors[qubit_index_z]  = is_zerror;
        }else if (qubit == false){
            bool is_zerror = false;
            zerrors[qubit_index_z]  = is_zerror;
        }
        qubit_index_z = qubit_index_z + 1;
    }
    // print zerrors
    std::cout << "\nZ errors on qubits        :";
    for (bool c : zerrors) {
        std::cout << c << ",";
    }

    // input the measurement error
    // decoding
    // show the result of decoding 
    // destructive measurement
    return 0;
}