// make X error vector
#include <random>
#include <vector>
#include "prob.h"

std::vector<bool> make_xerrors(std::vector<bool> erased_qubits, int num_qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    std::vector<bool> xerrors(num_qubits);
    int qubit_index_x = 0; 
    for (bool qubit : erased_qubits){
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

std::vector<bool> make_zerrors(std::vector<bool> erased_qubits, int num_qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    std::vector<bool> zerrors(num_qubits);
    int qubit_index_z = 0; 
    for (bool qubit : erased_qubits){
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