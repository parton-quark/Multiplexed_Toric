#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include "main_functions.hpp"
#include "main_functions_for_plots.hpp"
#include "main_with_argv.hpp"

int main(int argc, char** argv){
    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Current Time and Date: " << std::ctime(&end_time) << std::endl;

    int res;

    // 1. loss error model
    // res = main_with_loss_error();

    // 2. loss error model, multiple shots for plotting
    // res = main_with_loss_error_for_plot();

    // 3. combined error model (random pauli -> erasure -> random pauli)
    // res = main_with_combined_error();

    // 4. combined error model (random pauli -> erasure -> random pauli) multiple shots for plotting
    // std::vector<float> prob_z_vec;
    // prob_z_vec = {0, 0.0001, 0.001, 0.01, 0.1};
    // prob_z_vec = {0, 0.0001, 0.001, 0.01, 0.1};
    // for (float prob_z: prob_z_vec){
    //     res = main_with_combined_error_for_plot(prob_z);
    //     auto now1 = std::chrono::system_clock::now();
    //     std::time_t end_time1 = std::chrono::system_clock::to_time_t(now1);
    //     std::cout << "\nCurrent Time and Date: " << std::ctime(&end_time1) << std::endl;
    // }
    int prob_z = atoi(argv[1]);
    // prob_z_vec = {0, 0.0001, 0.001, 0.01, 0.1};
    // for (float prob_z: prob_z_vec){
    res = main_with_combined_error_for_plot(prob_z);
    auto now1 = std::chrono::system_clock::now();
    std::time_t end_time1 = std::chrono::system_clock::to_time_t(now1);
    std::cout << "\nCurrent Time and Date: " << std::ctime(&end_time1) << std::endl;
    // }
    return 0;
}

// for using command line argument with argv as inputs
// int main(int argc, char** argv){
//     // loss error model with sh argvs
//     int res;
//     res = main_with_loss_sh_argvs(argc, argv);
// }