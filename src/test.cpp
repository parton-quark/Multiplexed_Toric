// #include <iostream>
// #include <random>
// #include <vector>
// #include <algorithm>
// #include <fstream>
// #include <stdio.h>
// #include <string>
// #include <sstream>
// #include <iomanip>
// #include <chrono>
// #include "json.hpp"
// #include "print.hpp"
// #include "lattice.hpp"
// #include "prob.hpp"
// #include "error.hpp"
// #include "stabilizer.hpp"
// #include "graph.hpp"
// #include "spanning_forest.hpp"
// #include "peeling_decoder.hpp"
// #include "assign_qubits.hpp"
// #include "parity_check_matrix.hpp"
// #include "check.hpp"
// #include "union_find_decoder.hpp"
// #include "save.hpp"
// test growning function
// int main(){
//     graph G,H;
//     std::vector<int> vs, es;
//     vs = {13,17,18};
//     es = {26,35};
//     G = graph(vs, es);
//     H = grow_cluster(G, 5, 5);
//     H.print_graph();
//     return 0;
// }

//test collision detection
// int main(){
//     graph G,H;
//     std::vector<int> gv, ge, hv, he;
//     gv = {13,17,18};
//     ge = {26,35};
//     G = graph(gv, ge);

//     hv = {8,13,14};
//     he = {16,27};
//     H = graph(hv, he);

//     bool result;
//     result = detect_collision(H, G);
//     std::cout << "\nresult: " << result;
//     return 0;
// }

// // test union find decoder
// int main(){
//     std::random_device rd;
//     std::mt19937 engine(rd());
//     std::uniform_real_distribution<double> dist(0,1);
//     int l1, l2, num_qubits;
//     std::vector<bool> qubit_loss;
//     std::vector<bool> x_syndrome;
//     l1 = 5;
//     l2 = 5;
//     num_qubits = 50;
//     qubit_loss = make_erasure_errors(50, 0, engine, dist);
//     std::vector<bool> zerrors(50);
//     std::vector<int> zeposition{7,22,15};
//     for (int i; i < zerrors.size() - 1; i++){
//         zerrors[i] = false;
//     }
//     for (int zepos: zeposition){
//         zerrors[zepos] = true;
//     }
//     print_vec(zerrors);
//     std::vector<int> x_stabs;
//     std::vector<std::vector<int> > x_pcm;
//     std::vector<bool> x_syndromes;
//     x_stabs = make_x_stabilzers(l1, l2);
//     x_pcm = make_x_pcm(l1, l2, num_qubits, x_stabs);
//     x_syndromes = x_stab_measurement(l1, l2, x_stabs, zerrors);

//     std::vector<bool> z_correction;
//     z_correction = union_find_decoder_for_z_errors(l1, l2, num_qubits, qubit_loss, x_syndromes);
//     std::cout << "\nZ correction              :";
//     print_vec(z_correction);
//     return 0;
// }
