bool v_is_in_g(int v, graph G);
graph prim(graph G, int  l1, int l2, std::vector<int> edge_weights);
graph maximal_spanning_tree(graph G, int l1, int l2);
std::vector<graph> spanning_forest(graph G, int l1, int l2);