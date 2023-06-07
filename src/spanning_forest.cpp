#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "lattice.hpp"
#include "graph.hpp"

int pick_min_w_edge(graph left_graph, std::vector<int> edge_weights){
    // pick an edge with minimal weight randomly
    std::vector<int> left_vertices, left_edges;
    left_vertices = left_graph.vertices;
    left_edges = left_graph.edges;
    int min_w; /* minimum weight of left edges */
    min_w = *min_element(edge_weights.begin(), edge_weights.end());
    std::vector<int>::iterator itr;
    itr = std::find(edge_weights.begin(), edge_weights.end(), min_w);
    // const int min_w_index = std::distance(edge_weights.begin(), itr);
    std::vector<int> min_w_index;
    // 最小重みの辺のインデックスのベクトルを求める
    // インデックスのベクトルからランダムに一つ選ぶ
    for (int ind; ind < edge_weights.size(); ind++){
        if (edge_weights[ind] == min_w){
            min_w_index.push_back(ind);
        }
    }
    // min_w_index = std::distance(edge_weights.begin(), itr);
    int min_w_edge;
    srand(10);
    min_w_edge = min_w_index[rand() % min_w_index.size()];
    return min_w_edge;
}

graph kruskal(graph G, int l2, std::vector<int> edge_weights){
    // Kruskal's algorithm
    // input: graph G (connected g)
    // output: graph T (minimal spanning tree of G)
    std::vector<int> left_vertices;
    std::vector<int> left_edges;
    left_vertices = G.vertices;
    left_edges = G.edges;
    graph left_graph(left_vertices, left_edges);

    graph mm_st; /* minimal spanning tree */
    
    while(left_vertices.size() != 0){/* 全ての頂点が選び終わったら終了, empty使ったほうが良いか？ */
        bool is_cycle = true;
        while (is_cycle == true){
            int searching_e;
            searching_e = pick_min_w_edge(left_graph, edge_weights);
            std::vector<int> uv;
            uv = edge_to_vertices(l2, searching_e);

            bool num_u_in_left_v, num_v_in_left_v;
            num_u_in_left_v = std::count(left_vertices.begin(), left_vertices.end(), uv[0]);
            num_u_in_left_v = std::count(left_vertices.begin(), left_vertices.end(), uv[1]);

            if (num_u_in_left_v + num_v_in_left_v == 0){
                // carry on!
                // update edge weight (remove searching_e)
                std::vector<int>::iterator itr;
                const int wanted = searching_e; 
                itr = std::find(left_graph.edges.begin(), left_graph.edges.end(), wanted);
                const int wanted_index = std::distance(left_graph.edges.begin(), itr); // edge_weights.begin()かもしれん..
                // wanted_index番目のedge_weightを削除して詰める
                // update edge_weights (remove the weight with wanted_index)
                edge_weights.erase(edge_weights.begin() + wanted_index - 1);
                // remove two vertices of searching_e from left_vertices
                left_graph.remove_vertex(uv[0]);
                left_graph.remove_vertex(uv[1]);
                // remove searching_e from left_edges
                left_graph.remove_edge(searching_e);

                // add two vertices of searching_e to mm_st
                bool is_u_in_mm_st, is_v_in_mm_st;
                int num_u_in_mm_st, num_v_in_mm_st;
                num_u_in_mm_st = std::count(mm_st.vertices.begin(), mm_st.vertices.end(), uv[0]);
                num_v_in_mm_st = std::count(mm_st.vertices.begin(), mm_st.vertices.end(), uv[1]);
                if (num_u_in_left_v == 0){
                    mm_st.add_vertex(uv[0]);
                }
                if (num_v_in_left_v == 0){
                    mm_st.add_vertex(uv[1]);
                }
                // add searching_e to the mm_st
                mm_st.add_edge(searching_e);
                is_cycle = false;
            }else{
                is_cycle = true;
            }
        }
    }
    return mm_st;
}

graph maximal_spanning_tree(graph G, int l2){
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
    // ここまでは良さそう
    // Finding the minimum spanning tree of weight-replaced graphs
    // Since the input is a graph with equal edge weights, it can be replaced by a faster algorithm (depth-first search),
    // but for simplicity, Kruskal's algorithm is used. 
    graph mxst;
    mxst = kruskal(G, l2, edge_weights); /* maximal spanning tree */
    // The weights of each edge should be returned to w 
    // Here all edge weights are 1, so no such transformation is necessary
    return mxst;
}

std::vector<graph> spanning_forest(graph G, int l2){
    // input: edges
    // output: maximal spanning forest of edges 
    // (maximal subset of edges that contains no cycle and span all the vertices in edges)
    // 連結成分ごとにグラフを分割する
    std::cout <<  "\nspanning forest function";
    G.print_graph();
    std::vector<graph> msf; /* maximal spanning forest */
    std::vector<graph> connected_graphs;
    connected_graphs =  devide_graph(G, l2);
    for (graph connected_graph: connected_graphs){
        std::cout << "\nconnected graph: ";
        connected_graph.print_graph();
        // ここまでは良い。
        graph tree;
        tree = maximal_spanning_tree(connected_graph, l2);
        std::cout << "\ntree: ";
        tree.print_graph();
        // add tree to spanning forest
        msf.push_back(tree);
    }
    return msf;
}

