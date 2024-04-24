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
#include "json.hpp"
#include "print.hpp"
#include "lattice.hpp"
#include "prob.hpp"
#include "error.hpp"
#include "stabilizer.hpp"
#include "graph.hpp"
#include "spanning_forest.hpp"
#include "peeling_decoder.hpp"
#include "assign_qubits.hpp"
#include "parity_check_matrix.hpp"
#include "check.hpp"
// run for multiple error rate for plotting data
int main_for_plot(){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<double> dist(0,1);
    
    std::vector<int> lattice_sizes;
    lattice_sizes = {10};
    for (int lattice_size: lattice_sizes){
        std::cout << "\nlattice_size: " << lattice_size;
        int l1,l2;
        l1 = lattice_size;
        l2 = lattice_size;
        int num_qubits = (l1*l2) * 2;
        int num_shots = 100;
        std::vector<int> strategies;
        strategies = {7,8,9,10};
        for (int strategy: strategies){
            std::cout << "\nstrategy: " << strategy;
            auto now = std::chrono::system_clock::now();
            auto now_c = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");

            int multiplexing;
            if (strategy == 0){
                multiplexing = 1;
            } else if (strategy == 1){
                // random
                multiplexing = 2;
            } else if (strategy == 2){
                // random with distance
                multiplexing = 2;
            } else if (strategy == 3){
                // deterministic min dist
                multiplexing = 2;
            } else if (strategy == 4){
                // deterministic max dist
                multiplexing = 16;
            } else if (strategy == 5){
                // deterministic max dist
                multiplexing = 4;
            } else if (strategy == 6){
                multiplexing  = 1;
            } else if (strategy == 7){
                multiplexing = 4;
            } else if (strategy == 8){
                multiplexing = 4;
            } else if (strategy == 9){
                multiplexing = 4;
            } else if (strategy == 10){
                multiplexing = 4;
            } else if (strategy == 11){
                multiplexing = 4;
            }  

            int num_photons;
            num_photons = num_qubits / multiplexing;
            if (num_qubits % multiplexing > 0){
                num_photons  = num_photons  + 1;
            }
            std::vector<std::vector<int> > photons;
            int threthold;
            threthold = 0;
            int force;
            force = 15;

            if (strategy == 0){
                photons = assign_without_multiplexing(l1, l2, num_photons, num_qubits);
            } else if (strategy == 1){
                photons = assign_random(l1, l2, multiplexing, num_photons, num_qubits);
            } else if (strategy == 2){
                std::pair<std::vector<std::vector<int> >, int> photon_and_threshold;
                photon_and_threshold = assign_random_distance(l1, l2, multiplexing, num_photons, num_qubits, engine);
                photons = photon_and_threshold.first;
                threthold = photon_and_threshold.second;
            } else if (strategy == 3){
                photons = assign_deterministic(l1, l2, multiplexing, num_photons, num_qubits);
            } else if (strategy == 4){
                photons = assign_deterministic_shrink(l1, l2, multiplexing, num_photons, num_qubits);
            } else if (strategy == 5){
                photons = assign_random_with_occupation_enhancement_for_each_photon(l1, l2, multiplexing, num_photons, num_qubits, force, engine, dist);
            } else if (strategy == 6){
                photons = assign_random_with_occupation_enhancement(l1, l2, multiplexing, num_photons, num_qubits, force, engine, dist);
            } else if (strategy == 7){
                photons = assign_by_star_stabilizer_even_coordinate(l1, l2, multiplexing, num_photons, num_qubits);
            } else if (strategy == 8){
                photons = assign_by_star_stabilizer_odd_coordinate(l1, l2, multiplexing, num_photons, num_qubits);
            } else if (strategy == 9){
                photons = assign_by_plaquette_stabilizer_even_coordinate(l1, l2, multiplexing, num_photons, num_qubits);
            } else if (strategy == 10){
                photons = assign_by_plaquette_stabilizer_odd_coordinate(l1, l2, multiplexing, num_photons, num_qubits);
            } else if (strategy == 11){
                photons = assign_by_mixed_stabilizer(l1, l2, multiplexing, num_photons, num_qubits);
            }

            std::vector<std::pair<int,int>> success_rate;
            std::pair <float, std::vector<std::pair<int,int> > > prob_e_and_success_rate;

            std::vector<float> prob_e_vec;
            std::vector<double> vec_loss_rate_average, vec_z_error_rate_average;

            for (float prob_e = 0.0000; prob_e < 1.0500; prob_e = prob_e + 0.0500){
                prob_e_vec.push_back(prob_e);
                int num_success;
                int num_fail;
                num_success = 0;
                num_fail = 0;
                int num_error_happens;
                num_error_happens = 0;
                std::vector<float> vec_loss_rate, vec_zerror_rate;
                for (int i = 0; i < num_shots; i++){
                    std::string error_model("uniform");
                    std::vector<bool> photon_loss;
                    // error_model = 'burst';
                    if (error_model == "uniform"){
                    photon_loss = make_erasure_errors(num_photons, prob_e, engine, dist);
                    } else if (error_model == "burst"){
                    float p_good, p_burst;
                    p_good = 0.10;
                    p_burst = prob_e;
                    photon_loss = make_burst_errors(num_photons, p_good, p_burst, engine, dist);
                    }
                    std::vector<bool> qubit_loss(num_qubits);
                    qubit_loss = convert_photon_loss_to_qubit_loss(num_photons, num_qubits, multiplexing, photons, photon_loss);
                    int num_qubit_loss;
                    num_qubit_loss = 0;
                    for (bool one_qubit_loss: qubit_loss){
                        if (one_qubit_loss){
                            num_qubit_loss = num_qubit_loss + 1;
                        } 
                    }
                    float f_num_qubit_loss, f_num_qubits, loss_rate;
                    f_num_qubit_loss = (float) num_qubit_loss;
                    f_num_qubits = (float) num_qubits;
                    loss_rate = f_num_qubit_loss / f_num_qubits;
                    vec_loss_rate.push_back(loss_rate);
                    // make Z error vector
                    std::vector<bool> zerrors;
                    zerrors = make_zerrors(qubit_loss, num_qubits, engine, dist);
                    int num_z_errors;
                    num_z_errors = 0;
                    for (bool zerror: zerrors){
                        if (zerror){
                            num_z_errors = num_z_errors + 1;
                        } 
                    }
                    float f_num_z_errors, z_error_rate;
                    f_num_z_errors = (float) num_z_errors;
                    z_error_rate = f_num_z_errors / f_num_qubits;
                    vec_zerror_rate.push_back(z_error_rate);
                    // make stabilizers
                    std::vector<int> x_stabs;
                    x_stabs = make_x_stabilzers(l1, l2);
                    // make parity check matrix
                    std::vector<std::vector<int> > x_pcm;
                    x_pcm = make_x_pcm(l1, l2, num_qubits, x_stabs);
                    // vector of x stabilizers which returns -1
                    std::vector<bool> x_syndromes;
                    x_syndromes = x_stab_measurement(l1, l2, x_stabs, zerrors);
                    // decoding
                    std::vector<bool> z_correction;
                    z_correction = peeling_decoder_for_z_errors(l1, l2, num_qubits, qubit_loss, x_syndromes);
                    // show the result of decoding
                    std::vector<bool> z_errors_after_decoding(num_qubits);
                    for (int qubitind = 0; qubitind < num_qubits; qubitind++){
                        z_errors_after_decoding[qubitind] = zerrors[qubitind] ^ z_correction[qubitind];
                    }
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
                            num_error_in_x_2  = num_error_in_x_2 + 1;
                        }
                    }
                    bool is_success;
                    if (num_error_in_x_1 % 2 == 0 && num_error_in_x_2 % 2 == 0){
                        is_success = true;
                        num_success = num_success + 1;
                    } else {
                        is_success = false;
                        num_fail = num_fail + 1;
                    }
                    bool is_error_happens;
                    is_error_happens = is_correction_involved_in_loss(z_correction,qubit_loss);

                    if (is_error_happens){
                        num_error_happens = num_error_happens + 1;
                    }
                }
                //get average from vec_loss_rate
                double loss_rate_average = accumulate(vec_loss_rate.begin(), vec_loss_rate.end(), 0.0) / vec_loss_rate.size();
                double z_error_rate_average = accumulate(vec_zerror_rate.begin(), vec_zerror_rate.end(), 0.0) / vec_zerror_rate.size();
                vec_loss_rate_average.push_back(loss_rate_average);
                vec_z_error_rate_average.push_back(z_error_rate_average);
                std::pair<int, int> num_success_fail;
                num_success_fail.first = num_success;
                num_success_fail.second = num_fail;
                success_rate.push_back(num_success_fail);
                std::ofstream writing_file;
                std::string filename = "result_" + std::to_string(l1) + "_" + std::to_string(l2) + "_" + std::to_string(strategy) + "_" + std::to_string(multiplexing) + "_" + std::to_string(num_shots) + ".json";
                if (strategy == 5 || strategy == 6){
                    filename = "result_" + std::to_string(l1) + "_" + std::to_string(l2) + "_" + std::to_string(strategy) + "_" + std::to_string(multiplexing) + "_" + std::to_string(num_shots) + "_f" +std::to_string(force) + ".json";
                }
                auto now = std::chrono::system_clock::now();
                auto now_c = std::chrono::system_clock::to_time_t(now);
                std::stringstream sf;
                sf << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");
                nlohmann::json result_json;
                std::vector<int> lattice_size(l1, l2);
                result_json["time_start"] = ss.str();
                result_json["time_fin"] = sf.str();
                result_json["lattice_size"] = lattice_size;
                result_json["num_qubits"] = num_qubits;
                result_json["num_photons"] = num_photons;
                result_json["num_error_happens"] = num_error_happens;
                result_json["strategy"] = strategy;
                result_json["multiplexing"] = multiplexing;
                result_json["qubit list for each photon"] = photons;
                result_json["prob_e_vec"] = prob_e_vec;
                result_json["success_rate"] = success_rate;
                if (strategy == 2){
                    result_json["threshold"] = threthold;
                }
                if (strategy == 5 || strategy == 6){
                    result_json["force"] = force;
                }
                result_json["loss rate average"] = vec_loss_rate_average;
                result_json["z error rate average"] =  vec_z_error_rate_average;
                std::ofstream file(filename);
                file << result_json;
            }
        }
    }   
    std::cout << "\n";
    return 0;
}