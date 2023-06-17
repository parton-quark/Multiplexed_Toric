// make X error vector
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
    std::vector<bool> xerrors(num_qubits);
    int qubit_index_x = 0; 
    for (bool qubit : qubit_loss){
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
    return xerrors;
}

std::vector<bool> make_zerrors(std::vector<bool> qubit_loss, int num_qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    std::vector<bool> zerrors(num_qubits);
    int qubit_index_z = 0; 
    for (bool qubit : qubit_loss){
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
    return zerrors;
}