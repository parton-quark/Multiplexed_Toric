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
#include <iostream>
#include <nlohmann/json.hpp>
#include "lattice.hpp"
#include "prob.hpp"
#include "error.hpp"
#include "stabilizer.hpp"
#include "graph.hpp"
#include "spanning_forest.hpp"
#include "peeling_decoder.hpp"
#include "assign_qubits.hpp"
#include "parity_check_matrix.hpp"

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
    l1 = 20;
    l2 = 20;
    // number of qubits per one photon
    int multiplexing;
    // std::cout << "\nmultiplexing:";
    // std::cin >> multiplexing;
    multiplexing = 1;
    // photons: vector of photon(vector of qubits) 
    int num_qubits = (l1*l2) * 2;
    // std::cout << "\nnum_qubits:" << num_qubits;
    // randomly assign qubits to the photons
    int num_photons;
    num_photons = num_qubits / multiplexing;
    if (num_qubits % multiplexing > 0){
        num_photons  = num_photons  + 1;
    }
    // std::cout  << "\nnum_photons:" << num_photons;
    std::vector<std::vector<int>> photons;

    // photons = assign_random(l1, l2, multiplexing, num_photons, num_qubits);
    // std::pair<std::vector<std::vector<int> >, int> photons_and_threshold;
    // photons_and_threshold = assign_random_distance(l1, l2, multiplexing, num_photons, num_qubits);
    // photons = photons_and_threshold.first;
    // int threshold;
    // threshold = photons_and_threshold.second;
    photons = assign_without_multiplexing(l1, l2, num_photons, num_qubits);

    // input erasure probability
            // make random device

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<double> dist(0,1);

    std::vector<std::pair<int,int>> result_for_0p1_to_0p9;

    for (float prob_e = 0; prob_e < 1.0; prob_e = prob_e + 0.1){
        int num_success;
        int num_fail;
        num_success = 0;
        num_fail = 0;
        for (int i = 0; i < 10000; i++){
            std::vector<bool> photon_loss;
            photon_loss = make_erasure_errors(num_photons, prob_e, engine, dist);
            // std::cout << "\nphoton loss:  ";
            // print_ind_of_bool_vec(photon_loss);
            // make erasure vector for qubits
            std::vector<bool> qubit_loss(num_qubits);
            qubit_loss = convert_photon_loss_to_qubit_loss(num_photons, num_qubits, multiplexing, photons, photon_loss);

            // print qubit_loss
            // std::cout << "\nerasure vector for qubits :";
            // print_ind_of_bool_vec(qubit_loss);
            // replace the erased qubits with mixed state -> random pauli

            // make X error vector
            // std::vector<bool> xerrors;
            // xerrors = make_xerrors(qubit_loss, num_qubits, engine, dist);
            // print xerrors
            // std::cout << "\nX errors on qubits        :";
            // print_ind_of_bool_vec(xerrors);

            // make Z error vector
            std::vector<bool> zerrors;
            zerrors = make_zerrors(qubit_loss, num_qubits, engine, dist);
            // print zerrors
            // std::cout << "\nZ errors on qubits        :";
            // print_vec(zerrors);
            // print_ind_of_bool_vec(zerrors);
            // make stabilizers
            std::vector<int> x_stabs, z_stabs;
            x_stabs = make_x_stabilzers(l1, l2);
            // z_stabs = make_z_stabilzers(l1, l2);

            // make parity check matrix
            std::vector<std::vector<int> > x_pcm;
            x_pcm = make_x_pcm(l1, l2, num_qubits, x_stabs);

            // vector of x stabilizers which returns -1
            std::vector<bool> x_syndromes;
            x_syndromes = x_stab_measurement(l1, l2, x_stabs, zerrors);
            // std::cout << "\nX syndromes               :";
            // print_ind_of_bool_vec(x_syndromes);

            // decoding
            std::vector<bool> z_correction;
            z_correction = peeling_decoder_for_z_errors(l1, l2, num_qubits, qubit_loss, x_syndromes);
            // std::cout << "\nZ correction              :";
            // print_ind_of_bool_vec(z_correction);
            
            // show the result of decoding
            std::vector<bool> z_errors_after_decoding(num_qubits);
            for (int qubitind = 0; qubitind < num_qubits; qubitind++){
                z_errors_after_decoding[qubitind] = zerrors[qubitind] ^ z_correction[qubitind];
            }
            // std::cout << "\nZ errors after correction :";
            // print_ind_of_bool_vec(z_errors_after_decoding);
            // std::cout << "\n";
            
            // destructive measurement
            std::vector<int> x_1, x_2;
            for (int q = 0; q < l2; q++){
                int qubit;
                qubit = q * 2;
                x_1.push_back(qubit);
            }
            
            for (int r = 0; r < l1; r++){
                int qubit;
                qubit = r *(2 * l2) + 1;
                x_2.push_back(qubit);
            }
            // std::cout << "\nx_1: ";
            // print_vec(x_1);
            // std::cout << "\nx_2: ";
            // print_vec(x_2);

            int num_error_in_x_1,num_error_in_x_2;
            num_error_in_x_1 = 0;
            num_error_in_x_2 = 0;
            for (int x_m: x_1){
                if (z_errors_after_decoding[x_m]){
                    num_error_in_x_1  = num_error_in_x_1 + 1;
                }
            }
            for (int x_m: x_2){
                if (z_errors_after_decoding[x_m]){
                    num_error_in_x_1  = num_error_in_x_2 + 1;
                }
            }
            // if (num_error_in_x_1 % 2 == 0){
            //     std::cout << "\nno error in x_1 basis";
            // } else {
            //     std::cout << "\nnum_error_in_x_1: " << num_error_in_x_1;
            // }
            // if (num_error_in_x_2 % 2 == 0){
            //     std::cout << "\nno error in x_2 basis";
            // } else {
            //     std::cout << "\nnum_error_in_x_2: " << num_error_in_x_2;
            // }
            
            bool is_success;
            if (num_error_in_x_1 % 2 == 0 && num_error_in_x_2 % 2 == 0){
                is_success = true;
                num_success = num_success + 1;
            } else {
                is_success = false;
                num_fail = num_fail + 1;
            }

        }
        std::pair<int, int> num_success_fail;
        num_success_fail.first = num_success;
        num_success_fail.second = num_fail;
        result_for_0p1_to_0p9.push_back(num_success_fail);
    }



    
    // save result
    // auto now = std::chrono::system_clock::now();
    // auto now_c = std::chrono::system_clock::to_time_t(now);
    // std::stringstream ss;
    // ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");
    std::ofstream writing_file;
    // std::string filename = ss.str() + std::to_string(l1) + "_" + std::to_string(l2) + "_" + std::to_string(multiplexing) + "_" + std::to_string(prob_e) + "_" + ".json";
    std::string filename = "result_20_20_1_10000.json";
    nlohmann::json result_json;
    std::vector<int> lattice_size(l1, l2);
    result_json["lattice_size"] = lattice_size;
    result_json["num_qubits"] = num_qubits;
    result_json["num_photons"] = num_photons;
    result_json["multiplexing"] = multiplexing;
    result_json["qubit list for each photon"] = photons;
    result_json["result_for_0p1_to_0p9"] = result_for_0p1_to_0p9;

    // result_json["erasure vector for photons"] = photon_loss;
    // result_json["erasure vector for qubits"] = qubit_loss;
    // result_json["Z errors"] = zerrors;
    // result_json["X syndromes"] = x_syndromes;
    // result_json["X parity check matrix"] = x_pcm;
    // result_json["Z correction"] = z_correction;
    // result_json["Z errors after correction"] = z_errors_after_decoding;
    // result_json["X syndromes after decoding"] = x_syndromes_after_dec;
    // result_json["is success"] = is_success;
    std::ofstream file(filename);
    file << result_json;    
    // std::cout << "\n";
    return 0;
}