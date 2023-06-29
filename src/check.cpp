#include <vector>

//　訂正している量子ビットが消失量子ビットに含まれているか確認する関数
bool is_correction_involved_in_loss(std::vector<bool> correction, std::vector<bool> qubit_loss){
    bool is_error;
    is_error = false;
    
    std::vector<int> corrected_qubits, erased_qubits;
    
    int qubit_index;
    qubit_index = 0;
    for (bool corr: correction){
        if (corr){
            corrected_qubits.push_back(qubit_index);
        }
        qubit_index = qubit_index + 1;
    }

    int qubit_index_e;
    qubit_index_e = 0;
    for (bool erase_qubit: qubit_loss){
        if (erase_qubit){
            erased_qubits.push_back(qubit_index_e);
        }
        qubit_index_e = qubit_index_e + 1;
    }  

    for (int corrected_qubit: corrected_qubits){
        if (std::count(erased_qubits.begin(), erased_qubits.end(), corrected_qubit)){
            // do nothing
        } else { 
            is_error = true;
        }
    }
    return is_error;
}