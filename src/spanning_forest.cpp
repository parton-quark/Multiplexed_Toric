#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "lattice.hpp"
#include "graph.hpp"

int pick_min_w_edge(graph left_graph, std::vector<int> edge_weights){
    // pick an edge with minimal weight randomly
    std::vector<int> left_edges;
    left_edges = left_graph.edges; std::cout << "\nle_size" << left_edges.size() << std::flush;
    // std::cout << "\nHere 15"<< std::flush;
    if (left_edges.size() != edge_weights.size()){
        std::cout << "CAUTION left ed != ed_we" << std::flush;
    } else {
        // std::cout << "NP" << std::flush;
    }
    // std::cout << "\nHere 21" << std::flush;
    int min_w; /* minimum weight of left edges */
    min_w = *min_element(edge_weights.begin(), edge_weights.end());
    std::vector<int> min_w_indexes;
    // std::cout << "\nHere 25" << std::flush;
    for (int ind; ind < edge_weights.size(); ind++){
        if (edge_weights[ind] == min_w){
            min_w_indexes.push_back(ind);
        }
    }
    int max;
    // std::cout << "\nHere 32" << std::flush;
    max = min_w_indexes.size() - 1;  // なんかおかしいぞ！！！！！！！
    std::cout << "\nmax: " << max << std::flush;
    std::random_device rd;
    std::mt19937 engine;
    std::uniform_int_distribution<int> dist(0, max); // ここをmin_w_indexのサイズの一様分布にしたい
    // std::cout << "\nHere 37" << std::flush;
    int candidate_index, candidate_edge;
    candidate_index =  dist(engine); // std::cout << "\nHere 39" << std::flush;
    std::cout << "\ncandidate_index" << candidate_index << std::flush;
    std::cout << "\nleft_edges size" << left_edges.size() << std::flush;
    candidate_edge = left_edges[candidate_index]; // std::cout << "\nHere 40" << std::flush;
    return candidate_edge;
}

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
    // std::cout << "\nHere 67"<< std::flush;
    while(left_graph.vertices.size() != 0){/* until all vertices are included in the tree */
        std::cout << "\nleft_graph: "<< std::flush;
        left_graph.print_graph();
        std::cout << "\nleft_vertices_size: " <<  left_graph.vertices.size() << std::flush;
        bool is_cycle = true;
        int candidate_e; // std::cout << "\nHere 71"<< std::flush;
        candidate_e = pick_min_w_edge(left_graph, edge_weights); //std::cout << "\nHere 72"<< std::flush; //どうやら辺が空のグラフを渡しているらしい
        std::cout << "\ncandidate_e: " << candidate_e << std::flush;
        std::vector<int> candidate_e_vertices;
        candidate_e_vertices = edge_to_vertices(l1, l2, candidate_e);
        int candidate_v0, candidate_v1;
        candidate_v0 = candidate_e_vertices[0];
        candidate_v1 = candidate_e_vertices[1];

        int num_u_in_tree, num_v_in_tree;
        num_u_in_tree = std::count(mm_st.vertices.begin(), mm_st.vertices.end(), candidate_v0);
        num_v_in_tree = std::count(mm_st.vertices.begin(), mm_st.vertices.end(), candidate_v1);

        std::cout << "\nnum_u_in_tree: " << num_u_in_tree << std::flush;
        std::cout << "\nnum_v_in_tree: " << num_v_in_tree << std::flush;
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
            // this edge makes cycle in tree
            // discard the candidate edge
            std::cout << "makes cycle";
        } else {
            // update edge weight (remove the weight of candidate)
            std::vector<int>::iterator itr;
            int num_edge_in_left_g, wanted_index;
            num_edge_in_left_g = left_graph.num_edges();
            for (int ind = 0; ind < num_edge_in_left_g; ind++){
                if (left_graph.edges[ind] == candidate_e){
                    wanted_index = ind;
                    break;
                } else {
                    std::cout << "edge_weight not found" << std::flush;
                    wanted_index = 0;
                }
            }            
            // const int wanted = candidate_e; std::cout << "Here 85" << std::flush;
            // itr = std::find(left_graph.edges.begin(), left_graph.edges.end(), wanted); std::cout << "Here 86" << std::flush;
            // const int wanted_index = std::distance(left_graph.edges.begin(), itr); std::cout << "Here 87" << std::flush;
            std::cout << "\nedge_weights: " << std::flush;
            for (int ed: edge_weights){
                std::cout << ed << "," << std::flush;
            }
            edge_weights.erase(edge_weights.begin() + wanted_index);
            std::cout << "\nedge_weights: " << std::flush;
            for (int ed: edge_weights){
                std::cout << ed << "," << std::flush;
            }
            // wanted index: can_eはedgesの中で何番目か？
            // wanted index番目のweighted_edgesを削除して詰める
            // edge_weights.erase(edge_weights.begin() + wanted_index - 1); /* remove method必要？*/ std::cout << "Here 88" << std::flush;
            //std::cout << "Here 92" << std::flush;
            // add this edge to tree
            mm_st.add_edge(candidate_e);
            mm_st.add_vertex(candidate_v0);
            mm_st.add_vertex(candidate_v1);      
            //std::cout << "Here 94" << std::flush;
            // remove two vertices of candidate_e from left_vertices
            left_graph.remove_vertex(candidate_v0);
            left_graph.remove_vertex(candidate_v1);
            // remove candidate_e from the left_graph
            left_graph.remove_edge(candidate_e);
            //std::cout << "\nHere 100" << std::flush;
            std::cout << "\nmm_st:";
            mm_st.print_graph();

        }
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
    std::cout << "\nN:" << N << std::flush;
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
    // 連結成分ごとにグラフを分割する
    // std::cout <<  "\nspanning forest function";
    // G.print_graph();
    // std::cout << "done.";
    std::vector<graph> msf; /* maximal spanning forest */
    std::vector<graph> connected_graphs; //std::cout <<  "\nHere 185" << std::flush;
    connected_graphs =  devide_graph(G, l1, l2); // std::cout <<  "\nHere 186" << std::flush;
    for (graph connected_graph: connected_graphs){
        std::cout << "\nconnected graph: ";
        connected_graph.print_graph();
        // ここまでは良い。
        graph tree;
        tree = maximal_spanning_tree(connected_graph, l1, l2);
        std::cout << "\ntree: ";
        tree.print_graph();
        // add tree to spanning forest
        msf.push_back(tree);
    }
    return msf;
}

