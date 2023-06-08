#include <iostream>
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

void graph::print_graph(){    
    std::cout << "\nV: ";
    bool is_first_vertex;
    is_first_vertex = true;
    for(int vertex: vertices){
        if (is_first_vertex){
            std::cout << vertex << std::flush;
            is_first_vertex = false;
        } else {
            std::cout << "," << vertex  << std::flush;  
        }
    }
    std::cout << "\nE: ";
    bool is_first_edge;
    is_first_edge = true;
    for(int edge: edges){
        if (is_first_edge){
            std::cout << edge  << std::flush;
            is_first_edge = false;
        } else {
            std::cout << "," << edge << std::flush;  
        }
    }
}

void graph::print_vertices(){
    bool is_first_vertex;
    is_first_vertex = true;
    for(int vertex: vertices){
        if (is_first_vertex){
            std::cout << vertex << std::flush;
            is_first_vertex = false;
        } else {
            std::cout << "," << vertex << std::flush;  
        }
    }
}

void graph::print_edges(){
    bool is_first_edge;
    is_first_edge = true;
    for(int edge: edges){
        if (is_first_edge){
            std::cout << edge << std::flush;
            is_first_edge = false;
        } else {
            std::cout << "," << edge << std::flush;  
        }
    }
}

void graph::add_vertex(int vertex){
    size_t num_v;
    num_v = std::count(vertices.begin(), vertices.end(), vertex);
    if (num_v == 0){
        vertices.push_back(vertex);
    } else {
        // すでにグラフが持つ辺頂点の時は何もせずcout
        std::cout << "vertex " << vertex << " already exits." << std::flush;
    }
}

void graph::add_edge(int edge){
    size_t num_e;
    num_e = std::count(edges.begin(), edges.end(), edge);
    if (num_e == 0){
        edges.push_back(edge);
    } else {
        // すでにグラフが持つ辺の時は何もせずcout
        std::cout << "edge " << edge << " already exits." << std::flush;
    }
}

void graph::remove_vertex(int vertex){
    vertices.erase(remove(vertices.begin(), vertices.end(), vertex), vertices.end());
}

void graph::remove_edge(int edge){
    edges.erase(remove(edges.begin(), edges.end(), edge), edges.end());
}

std::vector<int> extract_group(std::vector<int> group){
    // seg fault
    // Sort and remove duplicate elements
    std::cout << "\nHere 102" << std::flush;
    std::vector<int> res;
    res = group;
    std::cout << "\ngroup: " << std::flush;
    for (int g: group){
        std::cout << g << "," << std::flush;
    }

    std::sort(res.begin(), res.end());
    res.erase(std::unique(res.begin(), res.end()), res.end());

    std::cout << "\nres: " << std::flush;
    for (int r: res){
        std::cout << r << "," << std::flush;
    }
    return res;
}

std::vector<graph> devide_graph(graph G, int l2){
    // input graph
    // output vector of connected graphs
    std::vector<int> group(G.num_vertices());
    for (int v; v < G.num_vertices(); v++){
        group[v] = v;
    }
    // update group based on edges
    for (int e: G.edges){
        std::vector<int> uv;
        uv = edge_to_vertices(l2, e);
        int u, v;
        u = uv[0];
        v = uv[1];
        int gu, gv;
        gu = group[u];
        gv = group[v];
        if (gu == gv){
            // do nothing
        } else if (gu < gv){
            // update groups of the all edges which has same group of v
            std::replace(group.begin(), group.end(), gv, gu);
        } else if (gu > gv){
            // update groups of the all edges which has same group of u 
            std::replace(group.begin(), group.end(), gu, gv);
        }
    }
    std::vector<int> groups;std::cout << "\nHere 140" << std::flush;
    groups = extract_group(group); std::cout << "\nHere 141" << std::flush;
    std::vector<graph> graphs; /* vector of connected graphs*/
    for (int c_grph: groups /* connected graph*/){
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