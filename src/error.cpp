// make X error vector
#include <iostream>
#include <random>
#include <vector>
#include "prob.hpp"

std::vector<bool> make_erasure_errors(int num_photons, float prob_e, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // make erasure vector for photons
    std::vector<bool> photon_loss;
    for (int phys_ph = 0; phys_ph < num_photons; phys_ph++){
        bool is_erased = false;
        is_erased = probabilistic(prob_e, engine, dist);
        photon_loss.push_back(is_erased);
    }
    return photon_loss;
}

std::vector<bool> make_burst_errors(int num_photons, float p_good, float p_burst, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // Gilbert model of burst error
    // based on automata with two states
    // true: good state
    // false: burst state

    bool gilbert_state;
    gilbert_state = true;
    std::vector<bool> vec_error;

    for (int i = 0; i < num_photons; i++){
        float random_number;
        random_number = probabilistic_float(engine, dist);
        // std::cout << "\nrandom number" << random_number;
        
        if(gilbert_state){
            // goodからp_goodの確率でbadに遷移
            // (1-p_good)の確率でgoodに留まる
            if (random_number <= p_good){
                // burstに遷移
                gilbert_state = false;
            } 
        } else {
            // burst から1 - p_burstの確率でgoodに遷移
            // p_burstの確率でburstに留まる
            if (random_number > p_burst){
                gilbert_state = true;
            } 
        }
        // if the state is good, error is false
        // otherwise error is true
        vec_error.push_back(!gilbert_state); 
    }
    return vec_error;
}

std::vector<bool> convert_photon_loss_to_qubit_loss(int num_photons, int num_qubits, int multiplexing, std::vector<std::vector<int> > photons, std::vector<bool> photon_loss){
    std::vector<bool> qubit_loss(num_qubits);
    for (int ph = 0; ph < num_photons; ph++){
        if (photon_loss[ph]){
            for (int qu = 0 ; qu < multiplexing; qu++){
                int qubitlabel;
                qubitlabel = photons[ph][qu];
                qubit_loss[qubitlabel] = 1;
            }
        }else if (photon_loss[ph] == false){
            for (int qu = 0 ; qu < multiplexing; qu++){
                int qubitlabel;
                qubitlabel = photons[ph][qu];
                qubit_loss[qubitlabel] = 0;
            }
        }
    }
    return qubit_loss;
}

std::vector<bool> make_xerrors(std::vector<bool> qubit_loss, int num_qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // convert qubit loss to x errors
    std::vector<bool> xerrors(num_qubits);
    int qubit_index_x = 0; 
    for (bool qubit : qubit_loss){
        if (qubit == true){
            bool is_xerror = false;
            is_xerror = probabilistic(0.500, engine, dist);
            xerrors[qubit_index_x]  = is_xerror;
        }else if (qubit == false){
            bool is_xerror = false;
            xerrors[qubit_index_x]  = is_xerror;
        }
        qubit_index_x = qubit_index_x + 1;
    }
    return xerrors;
}

std::vector<bool> make_zerrors(std::vector<bool> qubit_loss, int num_qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // convert qubit loss to x errors
    std::vector<bool> zerrors(num_qubits);
    int qubit_index_z = 0; 
    for (bool qubit : qubit_loss){
        if (qubit == true){
            bool is_zerror = false;
            is_zerror = probabilistic(0.500, engine, dist);
            zerrors[qubit_index_z]  = is_zerror;
        }else if (qubit == false){
            bool is_zerror = false;
            zerrors[qubit_index_z]  = is_zerror;
        }
        qubit_index_z = qubit_index_z + 1;
    }
    return zerrors;
}


std::vector<bool> random_x_error(int num_qubits, float prob_e, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // make erasure vector for photons
    std::vector<bool> x_errors;
    for (int physical_qubit = 0; physical_qubit < num_qubits; physical_qubit++){
        bool has_error = false;
        has_error = probabilistic(prob_e, engine, dist);
        x_errors.push_back(has_error);
    }
    return x_errors;
}

std::vector<bool> random_z_error(int num_qubits, float prob_e, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // make erasure vector for photons
    std::vector<bool> z_errors;
    for (int physical_qubit = 0; physical_qubit < num_qubits; physical_qubit++){
        bool has_error = false;
        has_error = probabilistic(prob_e, engine, dist);
        z_errors.push_back(has_error);
    }
    return z_errors;
}

std::vector<bool> combine_errors(std::vector<bool> errorvec1, std::vector<bool> errorvec2){
    std::vector<bool> combined_errors;
    if (errorvec1.size() != errorvec2.size()){
        std::cout << "Error: the size of two error vectors are different!";
    }
    for (int i = 0; i < errorvec1.size(); i++){
        bool combined_error;
        combined_error = errorvec1[i] ^ errorvec2[i];
        combined_errors.push_back(combined_error);
    }
    return combined_errors;
}