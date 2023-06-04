/* l1 × l2 Toric codes over erasure channel*/
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include "lattice.h"
#include "prob.h"
#include "error.h"
#include "stabilizer.h"
#include "graph.h"
#include "spanning_forest.h"
#include "peeling_decoder.h"

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
    int num_qubits = (l1*l2) * 2;
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
    float prob_e;
    std::cout << "\nerasure probability:";
    std::cin >> prob_e;
    //　この後たまにSegmentation fault: 11 を吐く。なんでだ？
    // make random device and distribution
    std::random_device rd;
    std::mt19937 engine;
    std::uniform_real_distribution<double> dist(0,1);

    std::vector<bool> erased_photons;
    erased_photons = make_erasure_errors(num_photons, prob_e, engine, dist);

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
    // replace the erased qubits with mixed state -> random pauli
    // make X error vector
    std::vector<bool> xerrors;
    xerrors = make_xerrors(erased_qubits, num_qubits, engine, dist);
    // print xerrors
    std::cout << "\nX errors on qubits        :";
    for (bool c : xerrors) {
        std::cout << c << ",";
    }

    // make Z error vector
    std::vector<bool> zerrors;
    zerrors = make_zerrors(erased_qubits, num_qubits, engine, dist);
    // print zerrors
    std::cout << "\nZ errors on qubits        :";
    for (bool c : zerrors) {
        std::cout << c << ",";
    }

    std::vector<int> xstabs;
    xstabs = make_x_stabilzers(l1, l2);
    // std::cout << "\nxstabs: ";
    // for (int stab : xstabs) {
    //     std::cout << stab << ",";
    // }

    std::vector<int> zstabs;
    zstabs = make_x_stabilzers(l1, l2);
    // std::cout << "\nzstabs: ";
    // for (int stab : zstabs) {
    //     std::cout << stab << ",";
    // }

    // vector of x stabilizers which returns -1
    std::vector<bool> x_syndromes;
    x_syndromes = x_stab_measurement(l1, l2, xstabs, zerrors);

    std::cout << "\nZ error syndrome (X stabs syndrome): ";
    for (int synd : x_syndromes){
        std::cout << synd << ",";
    }

    // option: vector of z stabilizers which returns -1

    // option: input measurement error probability
    // add measurement error to the syndrome
    // float prob_m;
    // std::cout << "\nmeasurement probability:";
    // std::cin >> prob_m;

    // decoding
    std::vector<bool> z_correction;
    z_correction = peeling_decoder_for_z_errors(l1, l2, num_qubits, erased_qubits, x_syndromes);
    std::cout << "\nZ correction: ";
    for (int correct : z_correction){
        std::cout << correct << ",";
    }
    // show the result of decoding 
    std::vector<bool> z_errors_after_decoding[num_qubits];
    for (int qubit; qubit <= num_qubits; qubit++){
        z_errors_after_decoding[qubit] = zerrors[qubit] ^ z_correction[qubit];
    }
    std::cout << "\nZ error after correction: ";
    for (bool z_error_after_decoding : z_errors_after_decoding){
        std::cout << z_error_after_decoding << ",";
    }
    // destructive measurement
    char d_measurement_basis;
    std::cout << "X or Z destructive_measurement";
    std::cin >> d_measurement_basis; /* x or z */

    return 0;
}