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

int main()
{   // lattice size: l1 times l2
    int l1,l2, multiplexing;
    l1 = 8;
    l2 = 8;
    // number of qubits per one photon
    multiplexing = 4;
    std::cout << "\nmultiplexing              :" << multiplexing;
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
    strategy = 11;
    std::cout  << "\nstrategy                  :" << strategy;
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
        photons_and_threshold = assign_random_plus_threshold(l1, l2, multiplexing, num_photons, num_qubits, engine);
        photons = photons_and_threshold.first;
        threshold = photons_and_threshold.second;
        std::cout << "\nthreshold                 :" << threshold;
    } else if (strategy == 3){
        multiplexing = 2;
        photons = assign_min_pair(l1, l2, multiplexing, num_photons, num_qubits);
    } else if (strategy == 4){
        photons = assign_max_pair(l1, l2, multiplexing, num_photons, num_qubits);
    } else if (strategy == 5){
        int force;
        force = 15;
        photons = assign_random_with_occupation_enhancement_for_each_photon(l1, l2, multiplexing, num_photons, num_qubits, force, engine, dist);
    } else if (strategy == 6){
        int force;
        force = 15;
        photons = assign_random_with_occupation_enhancement(l1, l2, multiplexing, num_photons, num_qubits, force, engine, dist);
    } else if (strategy == 7){
        multiplexing = 4;
        photons = assign_by_star_stabilizer_even_coordinate(l1, l2, multiplexing, num_photons, num_qubits);
    } else if (strategy == 8){
        multiplexing = 4;
        photons = assign_by_star_stabilizer_odd_coordinate(l1, l2, multiplexing, num_photons, num_qubits);
    } else if (strategy == 9){
        multiplexing = 4;
        photons = assign_by_plaquette_stabilizer_even_coordinate(l1, l2, multiplexing, num_photons, num_qubits);
    } else if (strategy == 10){
        multiplexing = 4;
        photons = assign_by_plaquette_stabilizer_odd_coordinate(l1, l2, multiplexing, num_photons, num_qubits);
    } else if (strategy == 11){
        multiplexing = 4;
        photons = assign_by_mixed_stabilizer(l1,l2, multiplexing, num_photons, num_qubits);
    }

    std::cout << "\nphotons                   :";
    print_vec_of_vec(photons);
    std::size_t size = photons.size();
    std::cout << "\nnumber of photons         :" << size << std::endl;
    if (size != num_photons){
        std::cout << "\nError! number of photons is not num_photons";
    }

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

    // input erasure probability
    std::vector<bool> photon_loss;
    int error_model;
    error_model = 0;
    if (error_model == 0){
        float prob_e;
        prob_e = 0.3;
        std::cout << "\nphoton loss probability   :" << prob_e;
        photon_loss = make_erasure_errors(num_photons, prob_e, engine, dist);
    } else if (error_model == 1){
        //gilbert model (burst error)
        float p_good, p_burst;
        p_good = 0.1;
        p_burst = 0.3;
        std::cout << "\np_good                    :" << p_good;
        std::cout << "\np_burst                   :" << p_burst;
        photon_loss = make_burst_errors(num_photons, p_good, p_burst, engine, dist);
    }

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
    std::pair<int,int> lattice_size(l1,l2);
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
    std::ofstream file(filename);
    file << result_json;
    
    std::cout << "\n";
    return 0;
}