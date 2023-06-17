/* l1 × l2 Toric codes over erasure channel*/
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <nlohmann/json.hpp>
#include "lattice.hpp"
#include "prob.hpp"
#include "error.hpp"
#include "stabilizer.hpp"
#include "graph.hpp"
#include "spanning_forest.hpp"
#include "peeling_decoder.hpp"
#include "assign_qubits.hpp"

void print_ind_of_bool_vec(std::vector<bool>  bv){
    bool is_first_elem;
    for (int index = 0; index < bv.size(); index++){
        if (bv[index]){
            std::cout << index << "," << std::flush;
        }
    }
}

template <class T> void print_vec(std::vector<T> vec){
    bool is_first_elem;
    is_first_elem = true;
    for (T elem: vec){ 
        if (is_first_elem){
            std::cout << elem << std::flush;
            is_first_elem = false;
        } else {
            std::cout << "," << elem << std::flush;
        }
    }
}

void print_vec_of_vec(std::vector<std::vector<int> > vec_vec){
    for (std::vector<int> vec: vec_vec){
        print_vec(vec);
        std::cout << "\n";
    }
}

std::string vec_to_str(std::vector<bool> vec){
    std::string vecstr;
    bool is_first_elem;
    is_first_elem = true;
    for (bool elem: vec){
        int x (elem);
        if (is_first_elem){
            vecstr = vecstr + std::to_string(x);
            is_first_elem = false;
        }
        vecstr = vecstr + ", " + std::to_string(x);
    }
    return vecstr;
}

int main()
{   // lattice size: l1 times l2
    int l1,l2;
    // std::cout << "l1:";
    // std::cin >> l1;
    // std::cout << "\nl2:";
    // std::cin >> l2;
    l1 = 10;
    l2 = 10;
    // number of qubits per one photon
    int multiplexing;
    // std::cout << "\nmultiplexing:";
    // std::cin >> multiplexing;
    multiplexing = 5;
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
    std::vector<std::vector<int>> photons;

    // photons = assign_random(l1, l2, multiplexing, num_photons, num_qubits);
    photons = assign_random_distance(l1, l2, multiplexing, num_photons, num_qubits);
    // input erasure probability
    float prob_e;
    // std::cout << "\nerasure probability:";
    // std::cin >> prob_e;
    prob_e = 0.2;
    // make random device
    std::random_device rd;
    std::mt19937 engine;
    std::uniform_real_distribution<double> dist(0,1);

    std::vector<bool> photon_loss;
    photon_loss = make_erasure_errors(num_photons, prob_e, engine, dist);
    std::cout << "\nphoton loss:  ";
    print_ind_of_bool_vec(photon_loss);
    // make erasure vector for qubits
    std::vector<bool> qubit_loss(num_qubits);
    qubit_loss = convert_photon_loss_to_qubit_loss(num_photons, num_qubits, multiplexing, photons, photon_loss);
    // print qubit_loss
    std::cout << "\nerasure vector for qubits :";
    print_ind_of_bool_vec(qubit_loss);
    // replace the erased qubits with mixed state -> random pauli
    // make X error vector
    std::vector<bool> xerrors;
    xerrors = make_xerrors(qubit_loss, num_qubits, engine, dist);
    // print xerrors
    std::cout << "\nX errors on qubits        :";
    print_ind_of_bool_vec(xerrors);

    // make Z error vector
    std::vector<bool> zerrors;
    zerrors = make_zerrors(qubit_loss, num_qubits, engine, dist);
    // print zerrors
    std::cout << "\nZ errors on qubits        :";
    // print_vec(zerrors);
    print_ind_of_bool_vec(zerrors);
    // make stabilizers
    std::vector<int> xstabs, zstabs;
    xstabs = make_x_stabilzers(l1, l2);
    zstabs = make_z_stabilzers(l1, l2);
    // vector of x stabilizers which returns -1
    std::vector<bool> x_syndromes;
    x_syndromes = x_stab_measurement(l1, l2, xstabs, zerrors);
    std::cout << "\nX syndromes               :";
    print_ind_of_bool_vec(x_syndromes);

    // option: input measurement error probability
    // add measurement error to the syndrome
    // float prob_m;
    // std::cout << "\nmeasurement probability:";
    // std::cin >> prob_m;

    // decoding
    std::vector<bool> z_correction;
    z_correction = peeling_decoder_for_z_errors(l1, l2, num_qubits, qubit_loss, x_syndromes);
    std::cout << "\nZ correction              :";
    print_ind_of_bool_vec(z_correction);
    
    // show the result of decoding
    std::vector<bool> z_errors_after_decoding(num_qubits);
    for (int qubitind = 0; qubitind < num_qubits; qubitind++){
        z_errors_after_decoding[qubitind] = zerrors[qubitind] ^ z_correction[qubitind];
    }
    std::cout << "\nZ errors after correction :";
    print_ind_of_bool_vec(z_errors_after_decoding);
    std::cout << "\n";
    
    // destructive measurement
    // Z_L measurement
    
    
    // X_L measurement
    // Masure all data qubits in X basis
    int product_XL;
    product_XL = 1;
    for (bool z_e: z_errors_after_decoding){
        if (z_e){
            product_XL = product_XL * -1;
        } else {
            // do nothing
        }
    }

    std::cout << "\nis success:  ";
    bool correction_res;
    if (product_XL==1){
        correction_res = true;
        std::cout << "Success";
    } else {
        correction_res = false;
        std::cout << "Failed";
    }
    // save result
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");
    std::ofstream writing_file;
    std::string filename = ss.str() + std::to_string(l1) + "_" + std::to_string(l2) + "_" + std::to_string(multiplexing) + "_" + std::to_string(prob_e) + "_" + ".json";

    nlohmann::json result_json;
    std::vector<int> lattice_size(l1, l2);
    result_json["lattice_size"] = lattice_size;
    result_json["num_qubits"] = num_qubits;
    result_json["num_photons"] = num_photons;
    result_json["multiplexing"] = multiplexing;
    result_json["qubit list for each photon"] = photons;
    result_json["erasure vector for photons"] = photon_loss;
    result_json["erasure vector for qubits"] = qubit_loss;
    result_json["Z errors"] = zerrors;
    result_json["X syndromes"] = x_syndromes;
    result_json["Z correction"] = z_correction;
    result_json["Z errors after correction"] = z_errors_after_decoding;
    result_json["success/fail"] = correction_res;
    std::ofstream file(filename);
    file << result_json;
    std::cout << "\n";
    return 0;
}