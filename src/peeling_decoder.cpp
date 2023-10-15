#include <iostream>
#include <vector>
#include <algorithm>
#include "lattice.hpp"
#include "graph.hpp"
#include "spanning_forest.hpp"

// Peeling decoder has been proposed by Nicolas Delfosse and Gilles Zémor in "Linear-time maximum likelihood decoding of surface codes over the quantum erasure channel", Phys. Rev. Research 2, 033042 – Published 9 July 2020
// This implementation is based on Algorithm 1 from the original paper.

std::pair<int, std::pair<int, int> > pick_leaf_edge(graph G, int l1, int l2){
    std::vector<int> edges, vertices;
    edges = G.edges;
    vertices = G.vertices;
    bool find_leaf;
    find_leaf = false;
    std::pair<int, std::pair<int, int> > leaf_and_pendant;

    for (int edge: edges){
        std::vector<int> v0_v1;
        int v0, v1;
        v0_v1 = edge_to_vertices(l1 , l2, edge);
        v0 = v0_v1[0];
        v1 = v0_v1[1];
        // std::cout << "\nv0: " << v0;
        // std::cout << "\nv1: " << v1;
        std::vector<int> es_of_v0, es_of_v1; // v0, v1が持ちうる辺
        es_of_v0 = vertex_to_edges(l1, l2, v0);
        es_of_v1 = vertex_to_edges(l1, l2, v1);
         std::vector<int> other_es_of_v0, other_es_of_v1; // 選んでいる辺以外のv0, v1が持ちうる辺

        for (int e_of_v0: es_of_v0){
            if (e_of_v0 != edge){
                other_es_of_v0.push_back(e_of_v0);
            } 
        }
        for (int e_of_v1: es_of_v1){
            if (e_of_v1 != edge){
                other_es_of_v1.push_back(e_of_v1);
            } 
        }

        int degree_v0, degree_v1;
        degree_v0 = 1;
        degree_v1 = 1;

        for (int other_e_of_v0: other_es_of_v0){
            if (std::count(edges.begin(), edges.end(), other_e_of_v0)){
                degree_v0 = degree_v0 + 1;
            }
        }
        for (int other_e_of_v1: other_es_of_v1){
            if (std::count(edges.begin(), edges.end(), other_e_of_v1)){
                degree_v1 = degree_v1 + 1;
            }
        }
        // std::cout << "\n degree_v0: " << degree_v0 << std::flush;
        // std::cout << "\n degree_v1: " << degree_v1 << std::flush;
        bool is_pendant_v0, is_pendant_v1;
        is_pendant_v0 = true;
        is_pendant_v1 = true;
        if (degree_v0 > 1){
            is_pendant_v0 = false;
        }
        if (degree_v1 > 1){
            is_pendant_v1 = false;
        }
        // std::cout << "\n is_pendant_v0: " << is_pendant_v0 << std::flush;
        // std::cout << "\n is_pendant_v1: " << is_pendant_v1 << std::flush;
        if (is_pendant_v0 || is_pendant_v1){
            /* edgeがleaf edgeである：uvのどちらかは他の辺を持たない。 */
            int leaf;
            leaf = edge;
            int pendant_vertex, connected_vertex;
            if (is_pendant_v0){
                pendant_vertex = v0;
                connected_vertex = v1;
            } else if (is_pendant_v1){
                pendant_vertex = v1;
                connected_vertex = v0;
            }
            std::pair<int, int> leaf_vertices{pendant_vertex, connected_vertex};
            leaf_and_pendant.first = leaf;
            leaf_and_pendant.second = leaf_vertices;
            find_leaf = true;
        }
        if (find_leaf == true){
            break;
        }
    }
    if (find_leaf == false){
        int leaf;
        leaf = 0;
        std::pair<int, int> leaf_vertices{0, 0};
        static std::pair<int, std::pair<int, int> > leaf_and_pendant;
        leaf_and_pendant.first = leaf;
        leaf_and_pendant.second = leaf_vertices;
        find_leaf = true;
        // std::cout << "Couldn't find leaf!!";
    }
    return leaf_and_pendant;
}

std::vector<bool> peeling_decoder_for_z_errors(int l1, int l2, int num_qubits, std::vector<bool> qubit_loss, std::vector<bool> x_syndrome){
    // inputs: 
    // lattice size (int l1, int l2)
    // erasure vector(std::vector<bool> qubit_loss)
    // x stabilizer syndrome (z error syndrome, std::vector<bool> x_syndrome) 
    // outputs: 
    // Z correction (std::vector<bool> p_z)
    graph epsilon; /* graph which has erased edges */
    // convert erasure qubits into edges
    for (int i = 0; i < num_qubits; i++){
        if (qubit_loss[i] == true){
            epsilon.add_edge(i);
            std::vector<int> v0_v1;
            int v0, v1;
            v0_v1 = edge_to_vertices(l1, l2, i);
            v0 = v0_v1[0];
            v1 = v0_v1[1];
            epsilon.add_vertex(v0);
            epsilon.add_vertex(v1);
        }
    }
    // std::cout << "\nepsilon" << std::flush;
    // epsilon.print_graph();
    // 1. Construct spanning forest f_eps of 
    std::vector<graph>  msf; /* vector of maximal spanning trees */
    msf = spanning_forest(epsilon, l1, l2);
    // 2. Initialize A
    graph A;
    for (graph f_eps: msf){
        // std::cout << "\nspanning tree:";
        // f_eps.print_graph();
        // 3. While f_eps = ∅, pick a leaf edge e={u,v} with pendant vertex u,remove e from f_eps and apply the two rules: 
        while (f_eps.num_edges() != 0){
            std::pair<int, std::pair<int, int>> leaf_and_pendant; /* leaf edge */
            // pick a leaf edge e={u,v} with pendant vertex u
            leaf_and_pendant = pick_leaf_edge(f_eps, l1, l2);
            int leaf, pendant, connected;
            leaf = leaf_and_pendant.first;
            pendant = leaf_and_pendant.second.first;
            connected = leaf_and_pendant.second.second;
            // std::cout << "\nleaf: ";
            // std::cout << leaf;
            // std::cout << "\n    pendant: ";
            // std::cout << pendant;
            // std::cout << "\n    connected: ";
            // std::cout << connected;
            // remove e from f_eps
            f_eps.remove_edge(leaf);
            // std::cout << "\nx_syndrome: ";
            int x_syndrome_index;

            // x_syndrome_index = 0;
            // for (bool x_stab:x_syndrome){
            //     if (x_stab){
            //         std::cout << x_syndrome_index << ", ";
            //     } 
            //     x_syndrome_index = x_syndrome_index + 1;
            // }
            if (x_syndrome[pendant]){
                // std::cout << "\nu is in sigma";
                // 4. (R1)If u∈σ, add e to A,remove u from σ and flip v in σ.
                A.add_edge(leaf);
                x_syndrome[pendant] = false;
                x_syndrome[connected] = !x_syndrome[connected];
            } else {
                // 5. (R2)If u/∈σ do nothing. 
                // std::cout << "\nu is not in sigma";
            }
        }
    }
    // 6. Return P = Prod_{e∈A} Ze.
    std::vector<bool> p_z(num_qubits); /* Z correction*/
    if (!A.edges.empty()){
        for (int edge: A.edges){
        p_z[edge] = true;
        }
    } else {
        // No correction
        // std::cout << "\n No correction!";
    }
    return p_z;
}

// Peeling decoder for X correction (std::vector<bool> p_x) has not yet implemented