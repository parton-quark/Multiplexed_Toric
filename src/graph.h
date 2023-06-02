class graph{
    // G = (V,E)
    public:
        std::vector<int> vertices;
        std::vector<int> edges;
        // member functions
        int num_vertices();
        int num_edges();
        // constructors
        graph(){
            std::vector<int> vs = {0};
            std::vector<int> es = {0};
            vertices = vs;
            edges = es;
        }
        graph(std::vector<int> vs, std::vector<int> es){
            vertices = vs;
            edges = es;
        }
        graph(const graph& g){
        vertices = g.vertices;
        edges = g.edges;
        }
};

std::vector<int> extract_group(std::vector<int> group);
std::vector<graph> devide_graph(graph G, int l2);
std::vector<int> extract_vertices(std::vector<int> edges, int l2);