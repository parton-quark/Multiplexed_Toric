/* l1 × l2 Toric codes over erasure channel*/
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
// #include "print.hpp"
#include "lattice.hpp"
#include "prob.hpp"
#include "error.hpp"
#include "stabilizer.hpp"
#include "graph.hpp"
#include "spanning_forest.hpp"
#include "peeling_decoder.hpp"

template <class T> void print_vec(std::vector<T> x){
    bool is_first_elem;
    is_first_elem = true;
    for (T elem: x){ 
        if (is_first_elem){
            std::cout << elem << std::flush;
            is_first_elem = false;
        }
        std::cout << "," << elem << std::flush;
    }
}

int main()
{   // lattice size: l1 times l2
    int l1,l2;
    std::cout << "l1:";
    std::cin >> l1;
    std::cout << "\nl2:";
    std::cin >> l2;
    // number of qubits per one photon
    int multiplexing;
    std::cout << "\nmultiplexing:";
    std::cin >> multiplexing;
    // photons: vector of photon(vector of qubits) 
    int num_qubits = (l1*l2) * 2;
    std::cout << "\nnum_qubits:" << num_qubits;
    // randomly assign qubits to the photons
    int num_photons;
    num_photons = num_qubits / multiplexing;
    if (num_qubits % multiplexing > 0){
        num_photons  = num_photons  + 1;
    }
    std::cout  << "\nnum_photons:" << num_photons;
 
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }

    std::vector<std::vector<int> > photons;
    for (int ph = 0; ph < num_photons; ph++){
        // std::cout << "\nphoton ";
        // print_vec(ph);
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
    
    // input erasure probability
    float prob_e;
    std::cout << "\nerasure probability:";
    std::cin >> prob_e;
    // make random device
    std::random_device rd;
    std::mt19937 engine;
    std::uniform_real_distribution<double> dist(0,1);

    std::vector<bool> erased_photons;
    erased_photons = make_erasure_errors(num_photons, prob_e, engine, dist);
    // print erased_photons
    std::cout << "\nerasure vector for photons:";
    print_vec(erased_photons);

    // make erasure vector for qubits
    std::vector<bool> erased_qubits(num_qubits);
    
    for (int ph = 0; ph < num_photons; ph++){
        if (erased_photons[ph]){
            for (int qu = 0 ; qu < multiplexing; qu++){
                int qubitlabel;
                qubitlabel = photons[ph][qu];
                erased_qubits[qubitlabel] = 1;
            }
        }else if (erased_photons[ph] == false){
            for (int qu = 0 ; qu < multiplexing; qu++){
                int qubitlabel;
                qubitlabel = photons[ph][qu];
                erased_qubits[qubitlabel] = 0;
            }
        }
    } 
    // print erased_qubits
    std::cout << "\nerasure vector for qubits :";
    print_vec(erased_qubits);

    // replace the erased qubits with mixed state -> random pauli
    // make X error vector
    std::vector<bool> xerrors;
    xerrors = make_xerrors(erased_qubits, num_qubits, engine, dist);
    // print xerrors
    // std::cout << "\nX errors on qubits        :";
    // print_vec(xerrors);

    // make Z error vector
    std::vector<bool> zerrors;
    zerrors = make_zerrors(erased_qubits, num_qubits, engine, dist);
    // print zerrors
    std::cout << "\nZ errors on qubits        :";
    print_vec(zerrors);
    // make stabilizers
    std::vector<int> xstabs, zstabs;
    xstabs = make_x_stabilzers(l1, l2);
    zstabs = make_x_stabilzers(l1, l2);
    // vector of x stabilizers which returns -1
    std::vector<bool> x_syndromes;
    x_syndromes = x_stab_measurement(l1, l2, xstabs, zerrors);
    // std::cout << "\nX syndromes               :";
    // print_vec(x_syndromes);
    
    // option: input measurement error probability
    // add measurement error to the syndrome
    // float prob_m;
    // std::cout << "\nmeasurement probability:";
    // std::cin >> prob_m;

    // decoding
    std::vector<bool> z_correction;
    z_correction = peeling_decoder_for_z_errors(l1, l2, num_qubits, erased_qubits, x_syndromes);
    std::cout << "\nZ correction              :";
    print_vec(z_correction);
    
    // show the result of decoding
    std::vector<bool> z_errors_after_decoding(num_qubits);
    for (int qubit = 0; qubit < num_qubits; qubit++){
        z_errors_after_decoding[qubit] = zerrors[qubit] ^ z_correction[qubit];
    }
    std::cout << "\nZ error after correction  :";
    print_vec(z_errors_after_decoding);
    std::cout << "\n";

    // destructive measurement

    std::cout << "\n";
    return 0;
}