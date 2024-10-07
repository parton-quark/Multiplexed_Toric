#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "lattice.hpp"
#include "graph.hpp"
#include "print.hpp"

bool v_is_in_g(int v, graph G){
    bool result;
    if (std::find(G.vertices.begin(), G.vertices.end(), v) != G.vertices.end()){
        result = true;
    } else {
        result = false;
    }
    return result;
}

graph prim(graph G, int  l1, int l2, std::vector<int> edge_weights){
    std::cout << "\nprim start! G: " << std::flush;
    G.print_graph();
    std::vector<int> left_vertices, left_edges;
    left_vertices = G.vertices;
    left_edges = G.edges;
    graph left_graph(left_vertices, left_edges);
    graph mm_st; /* minimal spanning tree */

    int first_vertex;
    first_vertex = left_graph.vertices[0];
    mm_st.add_vertex(first_vertex);
    left_graph.remove_vertex(first_vertex);

    std::vector<int> candidates, candidates_edge_weights;
    int left_edge_index = 0;
    bool find_leaf=false;
    while (left_graph.num_vertices() > 0){
        std::vector<int> candidates;
        for (int  left_edge: left_graph.edges){
            std::vector<int> left_edge_vertices;
            int left_edge_v0, left_edge_v1;
            left_edge_vertices = edge_to_vertices(l1, l2, left_edge);
            left_edge_v0 = left_edge_vertices[0];
            left_edge_v1 = left_edge_vertices[1];
            // std::cout << "\nleft_edge" << left_edge << std::flush;
            // std::cout << "\nleft_edge_v0: " << left_edge_v0 << std::flush;
            // std::cout << "\nleft_edge_v1: " << left_edge_v1 << std::flush;
            int num_v0_in_tree, num_v1_in_tree;
            // mm_st.print_graph();
            std::vector<int> mm_st_vertices;
            mm_st_vertices = mm_st.vertices;
            num_v0_in_tree = std::count(mm_st_vertices.begin(), mm_st_vertices.end(), left_edge_v0);
            num_v1_in_tree = std::count(mm_st_vertices.begin(), mm_st_vertices.end(), left_edge_v1);
            // std::cout << "\nnum_v0_in_tree: " << num_v0_in_tree << std::flush;
            // std::cout << "\nnum_v1_in_tree: " << num_v1_in_tree << std::flush;
            if (num_v0_in_tree == 1 && num_v1_in_tree == 0){/* v0 is in tree, v1 is not in tree */
                candidates_edge_weights.push_back(edge_weights[left_edge_index]);
                candidates.push_back(left_edge);
                find_leaf = true;
                // std::cout << "\nfind leaf!" << std::flush;
                // std::cout << left_edge << std::flush;
            }
            else if (num_v0_in_tree == 0 && num_v1_in_tree == 1){/* v0 is not in tree, v1 is in tree */
                candidates_edge_weights.push_back(edge_weights[left_edge_index]);
                candidates.push_back(left_edge);
                find_leaf = true;
                // std::cout << "\nfind leaf!" << std::flush;
                // std::cout << left_edge << std::flush;
            }
            else if (num_v0_in_tree != 0 && num_v1_in_tree != 0){/* v0 and v1 are in tree */
                // std::cout << "\nv0 and v1 are in tree. weird!" << std::flush;
            }
            else if (num_v0_in_tree == 0 && num_v1_in_tree == 0){/* v0 and v1 are not in tree */
                // std::cout << "\nv0 and v1 are not in tree." << std::flush;
            }
            left_edge_index = left_edge_index + 1;
        }
        // std::cout << "\n(65)find_leaf? " << find_leaf << std::flush;
        // std::cout << "\n(66)Candidates? " << std::flush;
        print_vec(candidates);
        if (find_leaf && candidates.size() != 0){
            // std::cout << "\nfind leaf: true " << std::flush;
            // find minimum weight in candidates
            int min_w_in_candidates;
            bool is_first_candidate;
            int cand_index;
            is_first_candidate = true;
            cand_index = 0;
            for (int candidate: candidates){
                if (is_first_candidate){
                    min_w_in_candidates = candidates_edge_weights[cand_index];
                    is_first_candidate = false;
                } else {
                    if (min_w_in_candidates > candidates_edge_weights[cand_index]){
                        min_w_in_candidates = candidates_edge_weights[cand_index];
                    }
                }
                cand_index = cand_index + 1;
            }
            std::vector<int> candidates_with_min_w;
            cand_index = 0;
            // print candidates
            // std::cout << "\ncandidates: ";
            // for (int candidate: candidates){
            //     std::cout << candidate << " ";
            // }
            std::cout << std::flush;
            for (int candidate: candidates){
                if (min_w_in_candidates == candidates_edge_weights[cand_index]){
                    candidates_with_min_w.push_back(candidate);
                }
                cand_index = cand_index + 1;
            }
            //print candidates_with_min_w
            // std::cout << "\ncandidates_with_min_w: ";   
            // for (int candidate: candidates_with_min_w){
            //     std::cout << candidate << " ";
            // }
            // std::cout << std::flush;
            int winner;
            int random_index;
            random_index = rand()%candidates_with_min_w.size();
            // candidates_with_min_w.size()が0になっており、おかしい
            // std::cout << "\ncandidates_with_min_w.size(): " << candidates_with_min_w.size() << std::flush;
            // std::cout << "\nrandom_index: " << random_index << std::flush;
            if (candidates_with_min_w.size() == 0){
                std::cout << "\nError! candidates_with_min_w is empty." << std::flush;
            }

            winner = candidates_with_min_w[random_index];
            std::vector<int> winner_vertices;
            int winner_v0, winner_v1;
            winner_vertices = edge_to_vertices(l1, l2, winner);
            winner_v0 = winner_vertices[0];
            winner_v1 = winner_vertices[1];
            int num_winner_v0_in_tree, num_winner_v1_in_tree;
            num_winner_v0_in_tree = std::count(mm_st.vertices.begin(), mm_st.vertices.end(), winner_v0);
            num_winner_v1_in_tree = std::count(mm_st.vertices.begin(), mm_st.vertices.end(), winner_v1);
            // u in mm_st, v is not in mm_st
            int winner_u, winner_v;
            if (num_winner_v1_in_tree == 0){
                winner_u = winner_v0;
                winner_v = winner_v1;
            } else if (num_winner_v0_in_tree == 0){
                winner_u = winner_v1;
                winner_v = winner_v0;
            } else {
                std::cout << "Error! Candidate edge is not connected to the spanning tree." << std::flush;
            }
            mm_st.add_edge(winner);
            mm_st.add_vertex(winner_v);

            // update edge_weights
            std::vector<int>::iterator itr;
            int wanted_index;
            for (int ind = 0; ind < left_graph.num_edges(); ind++){
                if (left_graph.edges[ind] == winner){
                    wanted_index = ind;
                    break;
                } else {
                    wanted_index = 0;
                }
            }
            left_graph.remove_edge(winner);
            left_graph.remove_vertex(winner_v);
            // edge_weights.erase(remove(edge_weights.begin(), edge_weights.end(), 2), edge_weights.end());
            // std::cout << "\nedge_weights.size()" << std::flush;
            // std::cout << edge_weights.size() << std::flush;
            // std::cout << "\nwanted_index" << std::flush;
            // std::cout << wanted_index << std::flush;
            edge_weights.erase(edge_weights.begin() + wanted_index);
            // edge_weights.shrink_to_fit();
        } else {
            std::cout << "\ncoudnt find..." << std::flush;
            break;
        }
    }
    std::cout << "\nprim finish" << std::flush;
    mm_st.print_graph();
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
    graph mxst;
    // std::cout << "\nprim start" << std::flush;
    mxst = prim(G, l1, l2, edge_weights);
    std::cout << "\nprim finish" << std::flush;
    // mxst = kruskal(G, l1, l2, edge_weights); /* maximal spanning tree */
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