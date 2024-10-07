/* l1 × l2 Toric codes over erasure channel*/
#include "main_functions.hpp"

int main(){
    int res;
    // loss error model
    // combined error (random pauli -> erasure -> random pauli)
    res = main_with_combined_error();
    // multiple shots for plotting
    // res = main_with_combined_error_multiple_shots();
    return res;
}