// 全てのスタビライザを生成する
#include <vector>
#include "lattice.hpp"

std::vector<int> make_x_stabilzers(int l1, int l2){
    // face corresponds to X stabilizer
    // there are l1 * l2 stabilziers
    std::vector<int> x_stabs; 
    for (int i = 0; i < l1 * l2; i++){
        x_stabs.push_back(i);
    }
    return x_stabs;
}

std::vector<int> make_z_stabilzers(int l1, int l2){
    // vertex(star) corresponds to Z stabilizer
    // there are l1 * l2 stabilziers
    std::vector<int> z_stabs; 
    for (int i = 0; i < l1 * l2; i++){
        z_stabs.push_back(i);
    }
    return z_stabs;
}

// stabilzer to qubits
std::vector<int> xstab_to_qubits(int l1, int l2,int xstab){
    // returns qubits in a X stabilizer
    int q0, q1, q2, q3;
    if (xstab >= (l1 * (l2 - 1))){ /* is top edge */
        if ((xstab % l1) == (l1 - 1)){ /* is rightmost */
            q0 = 2 * xstab;
            q1 = (2 * xstab) + 1;
            q2 = 2 * (l1 * (l2 - 1));
            q3 = (l1 * 2) - 1;
        }else{
            q0 = 2 * xstab;
            q1 = 2 * xstab + 1;
            q2 = 2 * xstab + 2;
            q3 = 2 * (xstab - (l1 * (l2 - 1))) + 1;
        }
    }else if ((xstab % l1) == (l1 - 1)){ /* is rightmost */
        q0 = 2 * xstab;
        q1 = 2 * xstab + 1;
        q2 = 2 * (xstab - (l1 - 1));
        q3 = 2 * xstab + 1 + (2 * l1);
    }else{
        q0 = 2 * xstab;
        q1 = 2 * xstab + 1;
        q2 = 2 * xstab + 2;
        q3 = 2 * xstab + 1 + (2 * l1);
    }
    std::vector<int> qubits{q0,q1,q2,q3};
    return qubits;
}

// std::vector<std::vector<int>> zstab_to_qubits(int l2,int v){
//     return qubits;
// }


std::vector<bool> x_stab_measurement(int l1, int l2, std::vector<int> xstabs, std::vector<bool> zerrors){
    std::vector<bool> x_stabs_syndrome;
    for (int xstab: xstabs){
        std::vector<int> qubits_in_xstab;
        qubits_in_xstab = xstab_to_qubits(l1, l2, xstab);

        int num_zerrors_in_xstab = 0;
        for (int qubit: qubits_in_xstab){
            if (zerrors[qubit] == true){
                num_zerrors_in_xstab = num_zerrors_in_xstab + 1;
            }
        }
        if (num_zerrors_in_xstab % 2 == 0){
            x_stabs_syndrome.push_back(false);
        }else if (num_zerrors_in_xstab % 2 != 0){
            x_stabs_syndrome.push_back(true);
        }
    }
    return x_stabs_syndrome;
}

// std::vector<bool> z_stab_measurement(int l1, int l2, std::vector<int> zstabs, std::vector<bool> xerrors){
// }