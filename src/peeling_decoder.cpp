#include <vector>
#include "spanning_tree.h"
// Peeling decoder has been proposed by Nicolas Delfosse and Gilles Zémor in "Linear-time maximum likelihood decoding of surface codes over the quantum erasure channel", Phys. Rev. Research 2, 033042 – Published 9 July 2020
// This implementation is based on Algorithm 1 from the original paper.

std::vector<bool> peeling_decoder_for_z_errors(int l1, int l2, std::vector<bool> erased_qubits, std::vector<bool> x_syndrome){
    // inputs: 
    // lattice size (int l1, int l2)
    // erasure vector(std::vector<bool> erased_qubits)
    // x stabilizer syndrome (z error syndrome, std::vector<bool> x_syndrome) 
    // outputs: 
    // Z correction (std::vector<bool> p_z)
    
    // 1. Construct spanning forest f_eps

    // 2. Initialize A 
    // 3. While f_eps = ∅, pick a leaf edge e={u,v} with pendant vertex u,remove e from f_eps and apply the two rules: 
    // 4. (R1)If u∈σ, add e to A,remove u from σ and flip v in σ. 
    // 5. (R2)If u/∈σ do nothing. 
    // 6. Return P = Prod_{e∈A} Ze.
    std::vector<bool> p_z;
    return p_z
}

// Peeling decoder for X correction (std::vector<bool> p_x) has not yet implemented