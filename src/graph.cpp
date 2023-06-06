#include <vector>
#include "graph.hpp"
#include "lattice.hpp"

int graph::num_vertices(){
    int num;
    num = vertices.size();
    return num;
}

int graph::num_edges(){
    int num;
    num = edges.size();
    return num;
}

void graph::add_vertex(int vertex){
    vertices.push_back(vertex);
}

void graph::add_edge(int edge){
    edges.push_back(edge);
}

void graph::remove_vertex(int vertex){
    vertices.erase(remove(vertices.begin(), vertices.end(), vertex), vertices.end());
}

void graph::remove_edge(int edge){
    edges.erase(remove(edges.begin(), edges.end(), edge), edges.end());
}

std::vector<int> extract_group(std::vector<int> group){
    // Sort and remove duplicate elements
    std::vector<int> res;
    res = group;
    std::sort(res.begin(), res.end());
    res.erase(std::unique(res.begin(), res.end()), res.end());
    return res;
}

std::vector<graph> devide_graph(graph G, int l2){
    // input graph (can be disconnected graph)
    // output vector of connected graphs 
    // assign each vertices to groups 
    std::vector<int> group;
    for (int v; v<= G.num_edges(); v++){
        group[v] = v;
    }

    // update groups
    for (int e: G.edges){
        // e has v1 and v2
        // Compare groups of v1 and v2 and merge them into the one with the smaller group number
        std::vector<int> v1_v2;
        int v1, v2;
        v1_v2 = edge_to_vertices(l2, e);
        v1 = v1_v2[0];
        v2 = v1_v2[0];
        int g1, g2;
        g1 = group[v1];
        g2 = group[v2];
        if (g1 > g2){
            group[v1_v2[0]] = g2;
        } else if (g1 < g2){
            group[v1_v2[1]] = g1;
        }
    }
    std::vector<int> groups;
    groups = extract_group(group); 
    std::vector<graph> graphs; /* vector of connected graphs*/
    for (int c_grph: groups/* connected graph*/){
        std::vector<int> c_grph_vert, c_grph_edg;
        for (int vert: G.vertices){
            if (c_grph == group[vert]){
                c_grph_vert.push_back(vert);
            }
        }
        for (int edg: G.edges){
            std::vector<int> vv;
            vv = edge_to_vertices(l2, edg);
            if (c_grph == group[vv[0]]){
                c_grph_edg.push_back(edg);
            } 
        }
        graph cg;
        cg = graph(c_grph_vert, c_grph_edg);
        graphs.push_back(cg);
    } 
    return graphs;
}

std::vector<int> extract_vertices(std::vector<int> edges, int l2){
    // input: edges
    // output: vector of vertices in the edges

    std::vector<int> vertices; /* vector of vertices which spanning forest spans */
    for (int edge: edges){
        // If the vertex in an edge contains is not included in vertices, add it.
        std::vector<std::vector<int>> v1v2_coordinates;
        std::vector<int> v1_coordinate, v2_coordinate;
        v1v2_coordinates = edge_to_coordinate(l2, edge); 
        v1_coordinate = v1v2_coordinates[0];
        v2_coordinate = v1v2_coordinates[1];
        
        int v1_x, v1_y, v2_x, v2_y;
        v1_x = v1_coordinate[0];
        v1_y = v1_coordinate[1];
        v1_x = v2_coordinate[0];
        v1_y = v2_coordinate[1];
        int v1, v2;
        v1 = coordinate_to_vertex(l2, v1_x, v1_y);
        v2 = coordinate_to_vertex(l2, v2_x, v2_y);
        
        if (std::count(vertices.begin(), vertices.end(), v1)) {
            // element found in vertices
        }else{
            vertices.push_back(v1);
        }

        if (std::count(vertices.begin(), vertices.end(), v2)) {
            // element found in vertices
        }else{
            vertices.push_back(v2);
        }
    // sort vertices
    }
    std::sort(vertices.begin(), vertices.end());
    return vertices;
}