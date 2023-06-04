#include <vector>
#include <algorithm>
#include "lattice.h"
#include "graph.h"
#include "spanning_forest.h"
// Peeling decoder has been proposed by Nicolas Delfosse and Gilles Zémor in "Linear-time maximum likelihood decoding of surface codes over the quantum erasure channel", Phys. Rev. Research 2, 033042 – Published 9 July 2020
// This implementation is based on Algorithm 1 from the original paper.

std::pair<int, std::pair<int, int>> pick_leaf_edge(graph G, int l1, int l2){
    std::vector<int> edges, vertices;
    edges = G.edges;
    vertices = G.vertices;

    for (int edge: edges){
        std::vector<int> uv;
        int u, v;
        uv = edge_to_vertices(l2, edge);
        u = uv[0];
        v = uv[1];
        std::vector<int> eu_list, ev_list;
        std::vector<int> edges_u, edges_v; /* u,vに接続されている辺 */
        edges_u = vertex_to_edges(l1, l2, u);
        edges_v = vertex_to_edges(l1, l2, v);
        
        for (int eu: edges_u){
            if (eu != edge){
                eu_list.push_back(eu);
            }
        }
        for (int ev: edges_v){
            if (ev != edge){
                ev_list.push_back(ev);
            }
        }

        int degree_u, degree_v;
        degree_u = 1;
        degree_v = 1;
        for (int eu: eu_list){
            degree_u = degree_u + std::count(edges.begin(), edges.end(), eu);
        }
        for (int ev: ev_list){
            degree_v = degree_v + std::count(edges.begin(), edges.end(), ev);
        }

        bool is_pendant_u, is_pendant_v;
        if (degree_u == 1){
            is_pendant_u = true;
        }
        if (degree_v == 1){
            is_pendant_v = true;
        }

        if (is_pendant_u || is_pendant_v){
            /* edgeがleaf edgeである：uvのどちらかは他の辺を持たない。 */
            int leaf;
            leaf = edge;
            int pendant_vertex, connected_vertex;
            if (is_pendant_u){
                pendant_vertex = u;
                connected_vertex = v;
            } else if (is_pendant_v){
                pendant_vertex = v;
                connected_vertex = u;
            }
            std::pair<int, int> leaf_vertices{pendant_vertex, connected_vertex};
            std::pair<int, std::pair<int, int>> leaf_and_pendant{leaf, leaf_vertices};
            break;
        }
    }
    return leaf_and_pendant;
}

std::vector<bool> peeling_decoder_for_z_errors(int l1, int l2, int num_qubits, std::vector<bool> erased_qubits, std::vector<bool> x_syndrome){
    // inputs: 
    // lattice size (int l1, int l2)
    // erasure vector(std::vector<bool> erased_qubits)
    // x stabilizer syndrome (z error syndrome, std::vector<bool> x_syndrome) 
    // outputs: 
    // Z correction (std::vector<bool> p_z)
    
    graph epsilon; /* graph which has erased edges */
    for (int erased_qubit: erased_qubits){
        epsilon.add_edge(erased_qubit);
    }

    // 1. Construct spanning forest f_eps of 
    std::vector<graph>  msf; /* vector of maximal spanning trees */
    msf = spanning_forest(epsilon, l2);
    // 2. Initialize A 
    graph A;
    for (graph f_eps: msf){
        // 3. While f_eps = ∅, pick a leaf edge e={u,v} with pendant vertex u,remove e from f_eps and apply the two rules: 
        while (f_eps.num_edges() != 0){
            std::pair<int, std::pair<int, int>> leaf_and_pendant; /* leaf edge */
            // pick a leaf edge e={u,v} with pendant vertex u
            leaf_and_pendant = pick_leaf_edge(f_eps, l1, l2);
            int leaf, pendant, connected;
            leaf = leaf_and_pendant.first;
            pendant = leaf_and_pendant.second.first;
            connected = leaf_and_pendant.second.second;
            // remove e from f_eps
            f_eps.remove_edge(leaf);
            // 4. (R1)If u∈σ, add e to A,remove u from σ and flip v in σ.
            if (x_syndrome[pendant] == true){
                A.add_edge(leaf);
                x_syndrome[pendant] = false;
                x_syndrome[connected] = !x_syndrome[connected];
            } else{
                // 5. (R2)If u/∈σ do nothing. 
            }
        }
    }

    // 6. Return P = Prod_{e∈A} Ze.
    std::vector<bool> p_z(num_qubits); /* Z correction*/
    for (int edge: A.edges){
        p_z[edge] = true;
    }
    return p_z;
}

// Peeling decoder for X correction (std::vector<bool> p_x) has not yet implemented