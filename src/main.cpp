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

int main()
{   // lattice size: l1 times l2
    int l1,l2, multiplexing;
    l1 = 5;
    l2 = 5;
    // number of qubits per one photon
    multiplexing = 5;
    std::cout << "\nmultiplexing              :" << multiplexing;
    // input erasure probability
    float prob_e;
    prob_e = 0.6;
    std::cout << "\nphoton loss probability   :" << prob_e;
    // random device
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<double> dist(0,1);
    // photons: vector of photon(vector of qubits) 
    int num_qubits = (l1*l2) * 2;
    std::cout << "\nnumber of qubits          :" << num_qubits;
    int num_photons;
    num_photons = num_qubits / multiplexing;
    if (num_qubits % multiplexing > 0){
        num_photons  = num_photons  + 1;
    }
    std::cout  << "\nnumber of photons         :" << num_photons;
    std::vector<std::vector<int> > photons;

    int strategy;
    strategy =  4;
    if (strategy == 0){
        // strategy 0: no multiplexing 
        multiplexing = 1;
        photons = assign_without_multiplexing(l1, l2, num_photons, num_qubits);
    } else if (strategy == 1){
        // strategy 1: randomly assign qubits to the photons
        photons = assign_random(l1, l2, multiplexing, num_photons, num_qubits);
    } else if (strategy == 2){
        // strategy 2: randomly assign qubits and accept if qubits have enough distance -> update threshold if it stuck
        std::pair<std::vector<std::vector<int> >, int>  photons_and_threshold;
        int threshold;
        photons_and_threshold = assign_random_distance(l1, l2, multiplexing, num_photons, num_qubits, engine);
        photons = photons_and_threshold.first;
        threshold = photons_and_threshold.second;
        std::cout << "\nthreshold                 :" << threshold;
    } else if (strategy == 3){
        multiplexing = 2;
        photons = assign_deterministic(l1, l2, multiplexing, num_photons, num_qubits);
    } else if (strategy == 4){
        photons = assign_deterministic_shrink(l1, l2, multiplexing, num_photons, num_qubits);
    }
    std::cout << "\nphotons                   :";
    print_vec_of_vec(photons);

    // check assignment works correctly 
    std::vector<int> assigned_qubits;
    for (std::vector<int> photon: photons){
        for (int qb_in_ph: photon){
            assigned_qubits.push_back(qb_in_ph);
        }
    }
    std::sort(assigned_qubits.begin(), assigned_qubits.end() );
    std::cout << "\nassigned qubits           :";
    print_vec(assigned_qubits);

    std::vector<bool> photon_loss;
    photon_loss = make_erasure_errors(num_photons, prob_e, engine, dist);
    std::cout << "\nphoton loss               :";
    print_ind_of_bool_vec(photon_loss);
    int num_photon_loss;
    num_photon_loss = 0;
    for (bool photon: photon_loss){
        if (photon){
            num_photon_loss = num_photon_loss + 1;
        } 
    }
    std::cout << "\nnum photon loss           :" << num_photon_loss;
    // make erasure vector for qubits
    std::vector<bool> qubit_loss(num_qubits);
    qubit_loss = convert_photon_loss_to_qubit_loss(num_photons, num_qubits, multiplexing, photons, photon_loss);
    std::cout << "\nqubit loss                :";
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
    std::vector<int> x_stabs, z_stabs;
    x_stabs = make_x_stabilzers(l1, l2);
    z_stabs = make_z_stabilzers(l1, l2);

    // make parity check matrix
    std::vector<std::vector<int> > x_pcm;
    x_pcm = make_x_pcm(l1, l2, num_qubits, x_stabs);

    // vector of x stabilizers which returns -1
    std::vector<bool> x_syndromes;
    x_syndromes = x_stab_measurement(l1, l2, x_stabs, zerrors);
    std::cout << "\nX syndromes               :";
    print_ind_of_bool_vec(x_syndromes);

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
    std::cout << "\nX_1 logical basis qubits  :";
    print_vec(x_1);
    std::cout << "\nX_2 logical basis qubits  :";
    print_vec(x_2);

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
            num_error_in_x_2  = num_error_in_x_2 + 1;// kokogamatigatteta
        }
    }
    if (num_error_in_x_1 % 2 == 0){
        std::cout << "\nno error in x_1 basis";
    } else {
        std::cout << "\nnum_error_in_x_1          :" << num_error_in_x_1;
    }
    if (num_error_in_x_2 % 2 == 0){
        std::cout << "\nno error in x_2 basis";
    } else {
        std::cout << "\nnum_error_in_x_2          :" << num_error_in_x_2;
    }
    
    bool is_success;
    if (num_error_in_x_1 % 2 == 0 && num_error_in_x_2 % 2 == 0){
        is_success = true;
    } else {
        is_success = false;
    }
    std::cout << "\nis_success                :";
    std::cout << is_success;

    std::vector<bool> x_syndromes_after_dec;
    x_syndromes_after_dec = x_stab_measurement(l1, l2, x_stabs,z_errors_after_decoding);
    
    // save result
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");
    std::ofstream writing_file;
    // std::string filename = ss.str() + std::to_string(l1) + "_" + std::to_string(l2) + "_" + std::to_string(multiplexing) + "_" + std::to_string(prob_e) + "_" + ".json";
    std::string filename = "result.json";
    nlohmann::json result_json;
    std::vector<int> lattice_size(l1, l2);
    result_json["time"] = ss.str();
    result_json["lattice_size"] = lattice_size;
    result_json["num_qubits"] = num_qubits;
    result_json["num_photons"] = num_photons;
    result_json["multiplexing"] = multiplexing;
    result_json["qubit list for each photon"] = photons;
    result_json["erasure vector for photons"] = photon_loss;
    result_json["erasure vector for qubits"] = qubit_loss;
    result_json["Z errors"] = zerrors;
    result_json["X syndromes"] = x_syndromes;
    result_json["X parity check matrix"] = x_pcm;
    result_json["Z correction"] = z_correction;
    result_json["Z errors after correction"] = z_errors_after_decoding;
    result_json["X syndromes after decoding"] = x_syndromes_after_dec;
    result_json["X_1 Logical operator"] = x_1;
    result_json["X_2 Logical operator"] = x_2;
    result_json["is success"] = is_success;
    // if (strategy == 2){
    //     result_json["threshold"] = threshold;
    // }
    std::ofstream file(filename);
    file << result_json;
    
    std::cout << "\n";
    return 0;
}


// for plotting data
// int main(){   // lattice size: l1 times l2
//     // for (int lattice_size = 5; lattice_size < 20; lattice_size =  lattice_size + 5){
//     std::random_device rd;
//     std::mt19937 engine(rd());
//     std::uniform_real_distribution<double> dist(0,1);
//     // std::uniform_real_distribution<double> dist(0,1);
//     std::vector<int> lattice_sizes;
//     lattice_sizes = {16};
//     for (int lattice_size: lattice_sizes){
//     // for (int lattice_size = 12; lattice_size < 16; lattice_size =  lattice_size + 4){
//         std::cout << "\nlattice_size: " << lattice_size;
//         int l1,l2;
//         l1 = lattice_size;
//         l2 = lattice_size;
//         int num_qubits = (l1*l2) * 2;
//         int num_shots = 10000;
//         std::vector<int> strategies;
//         strategies = {0};
//         for (int strategy: strategies){
//         // for (int strategy = 0; strategy < 5; strategy = strategy + 1){
//             std::cout << "\nstrategy: " << strategy;
//             auto now = std::chrono::system_clock::now();
//             auto now_c = std::chrono::system_clock::to_time_t(now);
//             std::stringstream ss;
//             ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");

//             int multiplexing;
//             if (strategy == 0){
//                 multiplexing = 1;
//             } else if (strategy == 1){
//                 // random
//                 multiplexing = 16;
//             } else if (strategy == 2){
//                 // random with distance
//                 multiplexing = 2;
//             } else if (strategy == 3){
//                 // deterministic min dist
//                 multiplexing = 2;
//             } else if (strategy == 4){
//                 // deterministic max dist
//                 multiplexing = 16;
//             }

//             int num_photons;
//             num_photons = num_qubits / multiplexing;
//             if (num_qubits % multiplexing > 0){
//                 num_photons  = num_photons  + 1;
//             }
//             std::vector<std::vector<int> > photons;
//             int threthold;
//             threthold = 0;
//             if (strategy == 0){
//                 photons = assign_without_multiplexing(l1, l2, num_photons, num_qubits);
//             } else if (strategy == 1){
//                 photons = assign_random(l1, l2, multiplexing, num_photons, num_qubits);
//             } else if (strategy == 2){
//                 std::pair<std::vector<std::vector<int> >, int> photon_and_threshold;
//                 photon_and_threshold = assign_random_distance(l1, l2, multiplexing, num_photons, num_qubits, engine);
//                 photons = photon_and_threshold.first;
//                 threthold = photon_and_threshold.second;
//             } else if (strategy == 3){
//                 photons = assign_deterministic(l1, l2, multiplexing, num_photons, num_qubits);
//             } else if (strategy == 4){
//                 photons = assign_deterministic_shrink(l1, l2, multiplexing, num_photons, num_qubits);
//             }
//             std::vector<std::pair<int,int>> success_rate;
//             std::pair <float, std::vector<std::pair<int,int> > > prob_e_and_success_rate;

//             std::vector<float> prob_e_vec;
//             std::vector<double> vec_loss_rate_average, vec_z_error_rate_average;

//             // for (float prob_e = 0.40; prob_e < 0.61; prob_e = prob_e + 0.01){
//             for (float prob_e = 0.3000; prob_e < 0.56000; prob_e = prob_e + 0.0100){
//                 prob_e_vec.push_back(prob_e);
//                 int num_success;
//                 int num_fail;
//                 num_success = 0;
//                 num_fail = 0;
//                 int num_error_happens;
//                 num_error_happens = 0;
//                 std::vector<float> vec_loss_rate, vec_zerror_rate;

//                 for (int i = 0; i < num_shots; i++){
//                     std::vector<bool> photon_loss;
//                     photon_loss = make_erasure_errors(num_photons, prob_e, engine, dist);
//                     std::vector<bool> qubit_loss(num_qubits);
//                     qubit_loss = convert_photon_loss_to_qubit_loss(num_photons, num_qubits, multiplexing, photons, photon_loss);

//                     int num_qubit_loss;
//                     num_qubit_loss = 0;
//                     for (bool one_qubit_loss: qubit_loss){
//                         if (one_qubit_loss){
//                             num_qubit_loss = num_qubit_loss + 1;
//                         } 
//                     }
//                     float f_num_qubit_loss, f_num_qubits, loss_rate;
//                     f_num_qubit_loss = (float) num_qubit_loss;
//                     f_num_qubits = (float) num_qubits;
//                     loss_rate = f_num_qubit_loss / f_num_qubits;
//                     vec_loss_rate.push_back(loss_rate);

//                     // make Z error vector
//                     std::vector<bool> zerrors;
//                     zerrors = make_zerrors(qubit_loss, num_qubits, engine, dist);

//                     int num_z_errors;
//                     num_z_errors = 0;
//                     for (bool zerror: zerrors){
//                         if (zerror){
//                             num_z_errors = num_z_errors + 1;
//                         } 
//                     }
//                     float f_num_z_errors, z_error_rate;
//                     f_num_z_errors = (float) num_z_errors;
//                     z_error_rate = f_num_z_errors / f_num_qubits;
//                     vec_zerror_rate.push_back(z_error_rate);

//                     // make stabilizers
//                     std::vector<int> x_stabs;
//                     x_stabs = make_x_stabilzers(l1, l2);
//                     // make parity check matrix
//                     std::vector<std::vector<int> > x_pcm;
//                     x_pcm = make_x_pcm(l1, l2, num_qubits, x_stabs);
//                     // vector of x stabilizers which returns -1
//                     std::vector<bool> x_syndromes;
//                     x_syndromes = x_stab_measurement(l1, l2, x_stabs, zerrors);
//                     // decoding
//                     std::vector<bool> z_correction;
//                     z_correction = peeling_decoder_for_z_errors(l1, l2, num_qubits, qubit_loss, x_syndromes);
//                     // show the result of decoding
//                     std::vector<bool> z_errors_after_decoding(num_qubits);
//                     for (int qubitind = 0; qubitind < num_qubits; qubitind++){
//                         z_errors_after_decoding[qubitind] = zerrors[qubitind] ^ z_correction[qubitind];
//                     }
//                     // destructive measurement
//                     std::vector<int> x_1, x_2;
//                     for (int q = 0; q < l2; q++){
//                         int qubit;
//                         qubit = q * 2;
//                         x_1.push_back(qubit);
//                     }
//                     for (int r = 0; r < l1; r++){
//                         int qubit;
//                         qubit = r *(2 * l2) + 1;
//                         x_2.push_back(qubit);
//                     }

//                     int num_error_in_x_1,num_error_in_x_2;
//                     num_error_in_x_1 = 0;
//                     num_error_in_x_2 = 0;
//                     for (int x_m: x_1){
//                         if (z_errors_after_decoding[x_m]){
//                             num_error_in_x_1  = num_error_in_x_1 + 1;
//                         }
//                     }
//                     for (int x_m: x_2){
//                         if (z_errors_after_decoding[x_m]){
//                             num_error_in_x_2  = num_error_in_x_2 + 1;
//                         }
//                     }
//                     bool is_success;
//                     if (num_error_in_x_1 % 2 == 0 && num_error_in_x_2 % 2 == 0){
//                         is_success = true;
//                         num_success = num_success + 1;
//                     } else {
//                         is_success = false;
//                         num_fail = num_fail + 1;
//                     }
//                     bool is_error_happens;
//                     is_error_happens = is_correction_involved_in_loss(z_correction,qubit_loss);

//                     if (is_error_happens){
//                         num_error_happens = num_error_happens + 1;
//                     }
//                 }


//                 //vec_loss_rateから平均値を導出する
//                 double loss_rate_average = accumulate(vec_loss_rate.begin(), vec_loss_rate.end(), 0.0) / vec_loss_rate.size();
//                 double z_error_rate_average = accumulate(vec_zerror_rate.begin(), vec_zerror_rate.end(), 0.0) / vec_zerror_rate.size();
//                 vec_loss_rate_average.push_back(loss_rate_average);
//                 vec_z_error_rate_average.push_back(z_error_rate_average);

//                 std::pair<int, int> num_success_fail;
//                 num_success_fail.first = num_success;
//                 num_success_fail.second = num_fail;
//                 success_rate.push_back(num_success_fail);

//                 std::ofstream writing_file;
//                 std::string filename = "result_" + std::to_string(l1) + "_" + std::to_string(l2) + "_" + std::to_string(strategy) + "_" + std::to_string(multiplexing) + "_" + std::to_string(num_shots) + ".json";

//                 auto now = std::chrono::system_clock::now();
//                 auto now_c = std::chrono::system_clock::to_time_t(now);
//                 std::stringstream sf;
//                 sf << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");
//                 nlohmann::json result_json;
//                 std::vector<int> lattice_size(l1, l2);
//                 result_json["time_start"] = ss.str();
//                 result_json["time_fin"] = sf.str();
//                 result_json["lattice_size"] = lattice_size;
//                 result_json["num_qubits"] = num_qubits;
//                 result_json["num_photons"] = num_photons;
//                 result_json["num_error_happens"] = num_error_happens;
//                 result_json["strategy"] = strategy;
//                 result_json["multiplexing"] = multiplexing;
//                 result_json["qubit list for each photon"] = photons;
//                 result_json["prob_e_vec"] = prob_e_vec;
//                 result_json["success_rate"] = success_rate;
//                 if (strategy == 2){
//                     result_json["threshold"] = threthold;
//                 }
//                 result_json["loss rate average"] = vec_loss_rate_average;
//                 result_json["z error rate average"] =  vec_z_error_rate_average;
//                 std::ofstream file(filename);
//                 file << result_json;
//             }
//         }
//     }   
//     std::cout << "\n";
//     return 0;
// }