#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "lattice.hpp"
#include "graph.hpp"

graph kruskal(graph G, int l1, int l2, std::vector<int> edge_weights){
    // Kruskal's algorithm
    // input: graph G (connected g)
    // output: graph T (minimal spanning tree of G)
    std::vector<int> left_vertices;
    std::vector<int> left_edges;
    left_vertices = G.vertices;
    left_edges = G.edges;
    graph left_graph(left_vertices, left_edges);

    graph mm_st; /* minimal spanning tree */
    std::cout << "\nleft_graph:";
    left_graph.print_graph();

    while(left_graph.vertices.size() != 0){/* until all vertices are included in the tree */
        std::cout << "\nleft_graph: "<< std::flush;
        left_graph.print_graph();
        std::cout << "\nleft_vertices_size: " <<  left_graph.vertices.size() << std::flush;
        std::cout << "\nedge_weights: " << std::flush;
        for (int ed_we: edge_weights){
            std::cout << ed_we <<  ",";
        }

        int min_w; /* minimum weight of left edges */
        bool is_first_w;
        is_first_w = true;
        for (int ed_we: edge_weights){
            if (is_first_w){
                min_w = ed_we;
                is_first_w = false;
            } else {
                if (min_w > ed_we){
                    min_w = ed_we;
                }
            }
        }
        std::cout << "\nmin_w:" << min_w;
        std::vector<int> min_w_indexes;
        for (int ind = 0 ; ind < edge_weights.size(); ind++){
            if (edge_weights[ind] == min_w){
                min_w_indexes.push_back(ind);
            }
        }
        std::cout << "\nmin_w_indexes: " << std::flush;
        for (int mwi: min_w_indexes){
            std::cout << mwi << "," << std::flush;
        }

        int index_max;
        index_max = min_w_indexes.size() - 1; 
        std::cout << "\nindex_max: " << index_max << std::flush;
        std::random_device rd;
        std::mt19937 engine;
        std::uniform_int_distribution<int> dist(0, index_max);
        bool is_cycle = true;
        int candidate_index, candidate_edge;
        std::vector<int> candidate_e_vertices;
        int candidate_v0, candidate_v1;
        while (is_cycle == true){// 候補を選び、サイクルになってしまったらまた候補を選ぶ
            candidate_index =  dist(engine);
            std::cout << "\ncandidate_index" << candidate_index << std::flush;
            candidate_edge = left_graph.edges[candidate_index];    
            std::cout << "\ncandidate_edge: " << candidate_edge << std::flush;
            candidate_e_vertices = edge_to_vertices(l1, l2, candidate_edge);
            candidate_v0 = candidate_e_vertices[0];
            candidate_v1 = candidate_e_vertices[1];

            int num_u_in_tree, num_v_in_tree;
            num_u_in_tree = std::count(mm_st.vertices.begin(), mm_st.vertices.end(), candidate_v0);
            num_v_in_tree = std::count(mm_st.vertices.begin(), mm_st.vertices.end(), candidate_v1);
            bool u_in_tree, v_in_tree;
            if (num_u_in_tree > 0){
                u_in_tree = true;
            } else {
                u_in_tree = false;
            }
            if (num_v_in_tree > 0){
                v_in_tree = true;
            } else {
                v_in_tree = false;
            }
            if (u_in_tree && v_in_tree){
                std::cout << "This candidate edge makes cycle. Reject and pick an edge again.";
                is_cycle = true;
            } else {
                is_cycle = false;
            }
        }
        // update edge weight (remove the weight of candidate)
        std::vector<int>::iterator itr;
        int num_edge_in_left_g, wanted_index;
        num_edge_in_left_g = left_graph.num_edges();
        for (int ind = 0; ind < num_edge_in_left_g; ind++){
            if (left_graph.edges[ind] == candidate_edge){
                wanted_index = ind;
                break;
            } else {
                wanted_index = 0;
            }
        }            
        edge_weights.erase(edge_weights.begin() + wanted_index);
        // add this edge to tree
        mm_st.add_edge(candidate_edge);
        mm_st.add_vertex(candidate_v0);
        mm_st.add_vertex(candidate_v1);
        // remove two vertices of candidate_e from left_vertices
        left_graph.remove_vertex(candidate_v0);
        left_graph.remove_vertex(candidate_v1);
        // remove candidate_e from the left_graph
        left_graph.remove_edge(candidate_edge);
    }
    return mm_st;
}

graph maximal_spanning_tree(graph G, int l1, int l2){
    // input: graph G (connected graph)
    // output: graph T (maximal spanning tree of G)

    // prepare a int N greater than the possible maximum value of the edge weights of the tree
    // let N be (the number of edges that G has) + 1.
    int N;
    N = G.num_edges() + 1;
    // all edges has weight
    // replace the weights on each side by N-w where w = 1 here
    std::vector<int> edge_weights;
    for (int e: G.edges){
        edge_weights.push_back(N-1); 
    }
    // Finding the minimum spanning tree of weight-replaced graphs
    // Since the input is a graph with equal edge weights, it can be replaced by a faster algorithm (depth-first search),
    // but for simplicity, Kruskal's algorithm is used. 
    graph mxst;
    mxst = kruskal(G, l1, l2, edge_weights); /* maximal spanning tree */
    // The weights of each edge should be returned to w 
    // Here all edge weights are 1, so no such transformation is necessary
    return mxst;
}

std::vector<graph> spanning_forest(graph G, int l1, int l2){
    // input: edges
    // output: maximal spanning forest of edges 
    // (maximal subset of edges that contains no cycle and span all the vertices in edges)
    std::vector<graph> msf; /* maximal spanning forest */
    std::vector<graph> connected_graphs;
    connected_graphs =  devide_graph(G, l1, l2); 
    for (graph connected_graph: connected_graphs){
        graph tree;
        tree = maximal_spanning_tree(connected_graph, l1, l2);
        // add tree to spanning forest
        msf.push_back(tree);
    }
    return msf;
}

