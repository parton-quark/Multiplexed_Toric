class graph{
    // G = (V,E)
    std::vector<int> vertices;
    std::vector<int> edges;
    // member functions
    int num_vertices();
    int num_edges();
};

std::vector<int> extract_group(std::vector<int> group);
std::vector<graph> devide_graph(graph G);
std::vector<int> extract_vertices(std::vector<int> edges, int l2);