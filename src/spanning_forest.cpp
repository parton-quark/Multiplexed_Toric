#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "lattice.h"
#include "graph.h"

int pick_min_w_edge(graph left_graph, std::vector<int> edge_weights){
    // pick an edge with minimal weight randomly
    std::vector<int> left_vertices, left_edges;
    left_vertices = left_graph.vertices
    left_edges = left_graph.edges
    int min_w; /* minimum weight of left edges */
    min_w = *min_element(edge_weights.begin(), edge_weights.end());
    itr = std::find(vec.begin(), vec.end(), min_w);
    const int min_w_index = std::distance(vec.begin(), itr);
    int min_w_edge;
    min_w_edge = min_w_index[rand()%min_w_index.size()];
    // int edge_index;
    // // min_w_edgeはG.edges内で何番目の辺だったか
    // edge_index =  
    return min_w_edge;
}

graph kruskal(graph G, int l2, std::vector<int> edge_weights){
    // Kruskal's algorithm
    // input: graph G
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
                const int wanted_index = std::distance(vec.begin(), itr);
                // wanted_index番目のedge_weightを削除して詰める
                // ここから作業する!!
                // remove two vertices of searching_e from left_vertices
                left_graph.remove_vertex(uv[0]);
                left_graph.remove_vertex(uv[1]);
                // remove searching_e from left_edges
                left_graph.remove_edge(searching_e);

                // add two vertices of searching_e to mm_st
                bool is_u_in_mm_st, is_v_in_mm_st;
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

graph maximal_spanning_tree(graph G){
    // input: graph G
    // output: graph T (maximal spanning tree of G)

    // 辺の重みの最大値よりも大きい数Nを用意： グラフの持つ辺の数+1をNとする
    // prepare a int N greater than the maximum of the edge weights of the tree
    // let N be (the number of edges that G has) + 1.
    int N;
    N = G.num_edges() + 1;
    // all edges has weight
    // replace the weights on each side by N-w where w = 1 here
    std::vector<int> edge_weights = {};
    for (int e: G.edges){
        edge_weights.push_back(N-1); 
    }
    // Finding the minimum spanning tree of weight-replaced graphs
    // Since the input is a graph with equal edge weights, it can be replaced by a faster algorithm (depth-first search),
    // but for simplicity, Kruskal's algorithm is used. 
    graph mst;
    mxst = kruskal(G, edge_weight); /* maximal spanning tree */
    // The weights of each edge should be returned to w 
    // Here all edge weights are 1, so no such transformation is necessary
    return mx_st;
}

std::vector<int> spanning_forest(std::vector<int> edges, int l2){
    // input: edges
    // output: maximal spanning forest of edges 
    // (maximal subset of edges that contains no cycle and span all the vertices in edges)

    // 連結成分ごとにグラフを分割する
    connected_graphs =  devide_forest() 
    for connected_graph: connected graphs
        tree = spanning_tree(色々)
        spanning_forestにspanning treeを追加
    }
    return msf;
} 

