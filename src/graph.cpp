#include <vector>
#include "graph.h"
#include "lattice.h"

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

// constructor
graph::graph(std::vector<int> v, std::vector<int> e){
    vertices = v;
    edges = e;
}

// copy constructor
graph::graph(const graph& g){
    vertices = g.vertices;
    edges = g.edges;
}

std::vector<int> extract_group(std::vector<int> group){
    // Sort and remove duplicate elements
    std::vector<int> res;
    res = group;
    std::sort(res.begin(), res.end());
    res.erase(std::unique(res.begin(), res.end()), res.end());
    return res;
}

std::vector<graph> devide_graph(graph G){
    // input graph (can be disconnected graph)
    // output vector of connected graphs 
    
    // assign each vertices to groups 
    std::vector<int> group;
    for (int v; v<= graph.num_edges(); v++){
        group[v] = v;
    }
    // update groups
    for (int e: G.edges){
        // e has v1 and v2
        // Compare groups of v1 and v2 and merge them into the one with the smaller group number
        std::vector<int> v1_v2;
        v1_v2 = edge_to_vertices(e);
        int g1 = group[v1_v2[0]];
        int g2 = group[v1_v2[1]];

        if (g1 > g2){
            group[v1_v2[0]] = g2;
        } else if (g1 < g2){
            group[v1_v2[1]] = g1;
        }
    }
    groups = extract_group(std::vector<int> group);    
    std::vector<graph> graphs; /* vector of connected graphs*/

    for (int c_grph: groups/* connected graph*/){
        std::vector<int> c_grph_vert, c_grph_edg;
        for (int vert: G.vertices){
            if (c_grph == group[vert]){
                c_grph_vert.push_back(vert);
            }
        }
        for (int edg: G.edges){
            vv = edge_to_vertices(e);
            if (c_grph == group[vv[0]]){
                c_grph_edg.push_back(edg);
            } 
        }
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
        std::vector<std::vector<int>> v1_coordinates, v2_coordinates;
        v1_coordinates = edge_to_coordinate(l2, edge)[0];
        v2_coordinates = edge_to_coordinate(l2, edge)[1];
        
        int v1, v2;
        v1 = coordinate_to_vertex(l2, v1_coordinates[0], v1_coordinates[1]);
        v2 = coordinate_to_vertex(l2, v2_coordinates[0], v2_coordinates[1]);
        
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
    std::sort(vertices.begin(), vertices,end());
    return vertices;
}