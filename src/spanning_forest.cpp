#include <vector>
#include <algorithm>
#include "lattice.h"
#include "graph.h"

std::vector<int> maximal_spanning_tree(graph G){
    // input: graph G
    // output: graph T (maximal spanning tree of G)

    // 辺の重みの最大値よりも大きい数Nを用意： グラフの持つ辺の数+1をNとする
    // prepare a int N greater than the maximum of the edge weights of the tree
    // let N be (the number of edges that G has) + 1.
    int N;
    N = G.num_edges() + 1;
    // all edges has weight
    // replace the weights on each side by N-w where w = 1 here
    std::vector<int> edge_weight = {};
    for (int e: G.edges){
        edge_weight.push_back(N-1); 
    }
    // Finding the minimum spanning tree of weight-replaced graphs
    // Since the input is a graph with equal edge weights, it can be replaced by a faster algorithm (depth-first search),
    // but for simplicity, Kruskal's algorithm is used. 
    tree = kruskal(G, edge_weight);

    // 各辺の重みを戻すと最大全域木

}

std::vector<int> kruskal(graph G, std::vector<int> edge_weight){
    // Kruskal's algorithm
    // input: graph G
    // output: graph T (minimal spanning tree of G)

    // 重みが最小の辺を一つ選ぶ
    // すでに選んだ辺と合わせてもサイクルができない辺の中から最小の辺を選ぶ
    // 全ての頂点を選び終わったら終了

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