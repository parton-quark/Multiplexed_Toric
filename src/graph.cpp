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
        // graph already has vertex
    }
}

void graph::add_edge(int edge){
    size_t num_e;
    num_e = std::count(edges.begin(), edges.end(), edge);
    if (num_e == 0){
        edges.push_back(edge);
    } else {
        // graph already has edge
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
    std::vector<int> res;
    // std::cout << "\ngroup: " << std::flush;
    // for (int g: group){
    //     std::cout << g << "," << std::flush;
    // }
    res = group;
    std::sort(res.begin(), res.end());
    res.erase(std::unique(res.begin(), res.end()), res.end());
    // std::cout << "\nextracted group: " << std::flush;
    // for (int r: res){
    //     std::cout << r << "," << std::flush;
    // }
    return res;
}

std::vector<graph> devide_graph(graph G, int l1, int l2){
    // input graph
    // output vector of connected graphs
    std::vector<int> v_group(G.num_vertices());
    for (int v = 0; v < G.num_vertices(); v++){
        v_group[v] = v + 1;
    }
    std::vector<int> e_group(G.num_edges());
        for (int e = 0; e < G.num_edges(); e++){
        e_group[e] = 0;
    }

    int e_index;
    e_index = 0;

    for (int e: G.edges){
        std::vector<int> uv;
        uv = edge_to_vertices(l1, l2, e);
        int u, v;
        u = uv[0];
        v = uv[1];

        int index_u, index_v;
        for (int i=0; i < G.num_vertices(); i++){
            if (G.vertices[i] == u){
                index_u = i;
            }
            if (G.vertices[i] == v){
                index_v = i;
            }
        }

        int gu, gv;
        gu = v_group[index_u];
        gv = v_group[index_v];

        if (gu == gv){
            // do nothing
            e_group[e_index] = gu;
        } else if (gu < gv){
            // update groups of e
            e_group[e_index] = gu;
            // update groups of all edges which has group gv
            for (int e_ind = 0; e_ind < G.num_edges(); e_ind++){
                if (e_group[e_index] == gv){
                    e_group[e_index] = gu;
                }
            }
            // update groups of all vertices which has same group of v
            std::replace(v_group.begin(), v_group.end(), gv, gu);
        } else if (gu > gv){
            // update groups of e
            e_group[e_index] = gv;
            // update groups of all edges which has group gu
            for (int e_ind = 0; e_ind < G.num_edges(); e_ind++){
                if (e_group[e_index] == gu){
                    e_group[e_index] = gv;
                }
            }
            // update groups of all vertices which has same group of u
            std::replace(v_group.begin(), v_group.end(), gu, gv);
        }
        e_index = e_index + 1;
    }
    std::cout << "\nv_group: " << std::flush;
    for (int a: v_group){
        std::cout << a << "," << std::flush;
    } 
    std::cout << "\ne_group: " << std::flush;
    for (int b: e_group){
        std::cout << b << "," << std::flush;
    } 

    std::vector<int> v_group_no_dupl;
    v_group_no_dupl = extract_group(v_group);

    std::vector<graph> connected_graphs;
    for (int group : v_group_no_dupl){
        graph connected_graph;
        // 頂点を追加する
        int vertex_index;
        vertex_index = 0;
        for (int vertex: v_group){
            if (vertex == group){
                connected_graph.add_vertex(G.vertices[vertex_index]);
            }
            vertex_index = vertex_index + 1;
        }
        // 辺を追加する
        int edge_index;
        edge_index = 0;
        for (int edge: e_group){
            if(edge == group){
                connected_graph.add_edge(G.edges[edge_index]);
            }
            edge_index = edge_index + 1;
        }
        connected_graphs.push_back(connected_graph);
        std::cout << "\nconnected_graph" << std::flush;
        connected_graph.print_graph();
    }
    return connected_graphs;
}

std::vector<int> extract_vertices(std::vector<int> edges, int l1, int l2){
    // input: edges
    // output: vector of vertices in the edges

    std::vector<int> vertices; /* vector of vertices which spanning forest spans */
    for (int edge: edges){
        // If the vertex in an edge contains is not included in vertices, add it.
        std::vector<int> v1v2;
        int v1, v2;
        v1v2 = edge_to_vertices(l1, l2, edge);
        v1 = v1v2[0];
        v2 = v1v2[1];
        std::vector<int> v1_coordinate, v2_coordinate;
        v1_coordinate = vertex_to_coordinate(l2, v1);
        v2_coordinate = vertex_to_coordinate(l2, v2);
        int v1_x, v1_y, v2_x, v2_y;
        v1_x = v1_coordinate[0];
        v1_y = v1_coordinate[1];
        v1_x = v2_coordinate[0];
        v1_y = v2_coordinate[1];
        
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