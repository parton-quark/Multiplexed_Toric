#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include "json.hpp"
bool save_results(std::string filename, int l1, int l2, int num_qubits, int num_photons, int multiplexing, int strategy, std::vector<std::vector<int> > photons, std::vector<bool> photon_loss, std::vector<bool> qubit_loss, std::vector<bool> zerrors, std::vector<bool> x_syndromes, std::vector<std::vector<int> > x_pcm, std::vector<bool> z_correction, std::vector<bool> z_errors_after_decoding, std::vector<bool> x_syndromes_after_dec, std::vector<int> x_1, std::vector<int> x_2, bool is_success){
    std::ofstream writing_file;
    nlohmann::json result_json;
    std::pair<int,int> lattice_size(l1,l2);
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);    
    std::stringstream ss;
    ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");
    result_json["time"] = ss.str();
    result_json["lattice_size"] = lattice_size;
    result_json["num_qubits"] = num_qubits;
    result_json["num_photons"] = num_photons;
    result_json["multiplexing"] = multiplexing;
    result_json["strategy"] = strategy;
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
    return true;
}

// save function for combined case


    // // save result
    // // auto now = std::chrono::system_clock::now();
    // // auto now_c = std::chrono::system_clock::to_time_t(now);
    // // std::stringstream ss;
    // // ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");
    // std::ofstream writing_file;
    // // std::string filename = ss.str() + std::to_string(l1) + "_" + std::to_string(l2) + "_" + std::to_string(multiplexing) + "_" + std::to_string(prob_e) + "_" + ".json";
    // std::string filename = "result.json";
    // nlohmann::json result_json;
    // std::pair<int,int> lattice_size(l1,l2);
    // result_json["time"] = ss.str();
    // result_json["lattice_size"] = lattice_size;
    // result_json["num_qubits"] = num_qubits;
    // result_json["num_photons"] = num_photons;
    // result_json["multiplexing"] = multiplexing;
    // result_json["qubit list for each photon"] = photons;
    // result_json["erasure vector for photons"] = photon_loss;
    // result_json["erasure vector for qubits"] = qubit_loss;
    // result_json["Z errors"] = zerrors;
    // result_json["X syndromes"] = x_syndromes;
    // result_json["X parity check matrix"] = x_pcm;
    // result_json["Z correction"] = z_correction;
    // result_json["Z errors after correction"] = z_errors_after_decoding;
    // result_json["X syndromes after decoding"] = x_syndromes_after_dec;
    // result_json["X_1 Logical operator"] = x_1;
    // result_json["X_2 Logical operator"] = x_2;
    // result_json["is success"] = is_success;
    // std::ofstream file(filename);
    // file << result_json;