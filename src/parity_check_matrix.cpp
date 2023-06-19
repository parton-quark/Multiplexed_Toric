#include <vector>
#include "stabilizer.hpp"

std::vector<std::vector<int> > make_x_pcm(int l1, int l2, int num_qubits, std::vector<int> x_stabs){
    // make X parity check matrix of toric codes
    int num_stabs;
    num_stabs = x_stabs.size();
    std::vector<std::vector<int> > x_pcm(num_stabs, std::vector<int>(num_qubits));
    int x_stab_index;
    x_stab_index = 0;
    for (int x_stab: x_stabs){
        std::vector<int>  supported_qubits;
        supported_qubits =  xstab_to_qubits(l1, l2, x_stab);
        for (int qubit: supported_qubits){
            x_pcm[x_stab_index][qubit] = 1;
        }
        x_stab_index = x_stab_index + 1;
    }
    return x_pcm;
}

