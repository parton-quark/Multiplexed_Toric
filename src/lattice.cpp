#include <iostream>
#include <random>
#include <vector>
#include <cstdlib>
// vertex
std::vector<int> vertex_to_coordinate(int l2, int v){
    // Z stabilizer
    int x,y;
    x = v % l2;
    y = v / l2;
    std::vector<int> coordinate{x,y};
    return coordinate;
}

int coordinate_to_vertex(int l2, int x, int y){
    int vertex;
    vertex = x + y * l2;
    return vertex;
}

std::vector<int> vertex_to_edges(int l1, int l2, int v){
    int e0, e1, e2, e3;
    e0 = 2 * (v - l2);
    e1 = (2 * v) - 1;
    e2 = 2 * v;
    e3 = (2 * v) + 1;

    if ((v % l2) == 0){ // most left 
        if (v == 0){
            e0 = 2 * l2 * (l1 - 1);
            e1 = (2 * l2) - 1;
        } else {
            e1 = (2 * v) + (2 * l2) - 1;
        }
    } else if (v < l2){// bottom row
        e0 = 2 * v + 2 * l2 * (l1 - 1);
    }

    std::vector<int>  edges{e0, e1, e2, e3};
    return edges;
}

// edge
std::vector<std::vector<int>> edge_to_coordinate(int l2, int e){
    // qubit
    int v_0_x,v_0_y,v_1_x,v_1_y;
    if(e % 2 == 0){
        // Vertical
        v_0_x = e % l2;
        v_0_y = e / l2;
        v_1_x = (e % l2) + 1;
        v_1_y = e / l2;
    }
    else if (e % 2 == 1){
        // Horizontal
        v_0_x = e % l2;
        v_0_y = e / l2;
        v_1_x = e % l2;
        v_1_y = (e / l2)+ 1;
    }
    else{   
        v_0_x = 0;
        v_0_y = 0;
        v_1_x = 0;
        v_1_y = 0;
        std::cout << "ERROR:edge_to_coordinate";
    }
    std::vector<std::vector<int>> coordinates{{v_0_x,v_0_y},{v_1_x,v_1_y}};
    return coordinates;
}

std::vector<int> edge_to_vertices(int l1, int l2, int e){
    int u, v;
    if (e%2 == 0){
        // vertical edge
        u = e / 2;
        if (e >= 2 * (l1 - 1) * l2){// 上端
            v = (e % (2 * l2)) / 2;
        } else {
            v = u + l2;
        }
    } else {
        // horizontal edge
        u = (e - 1) / 2;
        if (e % (2 * l2) == (2 * l2) - 1){ // 右端
            v = (e / (2 * l2)) * l2;
        } else {
            v = u + 1;
        }
    }
    std::vector<int> vertices{u, v};
    return vertices;
}

int coordinates_to_edge(int l2, int x1, int y1, int x2, int y2){
    int edge;
    if (x1 == x2 && y1 - 1 == y2){
        // vertical, lower to upper
        edge = 2 * x1;
    } else if (x1 == x2 && y1 == y2 - 1){
        // vertical, upper to lower
        edge = 2 * x1;
    } else if (x1 == x2 - 1 && y1 == y2){
        // horizontal, left to right
        edge = (2 * x1) + 1;
    } else if (x1 - 1  == x2 && y1 ==y2){
        // horizontal, right to left
        edge = (2 * x2) + 1;
    } else {
        std::cout << "Error, this is not an edge";
        edge = 0;
    }
    return edge;
}

std::vector<std::vector<int>> face_to_coordinates(int l2,int v){
    // X stabilizer
    int v_0_x,v_0_y,v_1_x,v_1_y,v_2_x,v_2_y,v_3_x,v_3_y;
    v_0_x = v % l2;
    v_0_y = v / l2;
    v_1_x = v % l2 + 1;
    v_1_y = v / l2;
    v_2_x = v % l2;
    v_2_y = v / l2 + 1;
    v_3_x = v % l2 + 1;
    v_3_y = v / l2 + 1;
    std::vector<std::vector<int>> coordinates{{v_0_x,v_0_y},{v_1_x,v_1_y},{v_2_x,v_2_y},{v_3_x,v_3_y}};
    return coordinates;
}

int coordinates_to_face(int l2, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3){
    // x0,y0 left, lower
    // x1,y1 right, lower
    // x2,y2 left, upper
    // x3,y3 right, upper
    int face;
    face = x0 + l2 * y0;
    return face;
}

int distance_c(int l1, int l2, int v_1_x, int v_1_y, int v_2_x, int v_2_y){
    // returns the distance between two coordinates
    int distance;
    int x_distance, x_distance_reverse, y_distance, y_distance_reverse;

    x_distance = abs((v_1_x - v_2_x));
    x_distance_reverse = l1 - x_distance;

    y_distance = abs((v_1_y - v_2_y));
    y_distance_reverse = l2 - x_distance;

    if (x_distance > x_distance_reverse){
        x_distance = x_distance_reverse;
    }
    if (y_distance > y_distance_reverse){
        y_distance = y_distance_reverse;
    }
    distance = x_distance + y_distance;
    return distance;
}


int distance_v(int l1, int l2, int v_1, int v_2){
    // returns the distance between v1 and v2
    std::vector<int> v_1_c, v_2_c;
    int v_1_x, v_1_y, v_2_x, v_2_y;
    v_1_c = vertex_to_coordinate(l2, v_1);
    v_1_x = v_1_c[0];
    v_1_x = v_1_c[1];

    v_2_c = vertex_to_coordinate(l2, v_2);
    v_2_x = v_2_c[0];
    v_2_x = v_2_c[1];

    int distance;
    int x_distance, x_distance_reverse, y_distance, y_distance_reverse;

    x_distance = abs((v_1_x - v_2_x));
    x_distance_reverse = l1 - x_distance;

    y_distance = abs((v_1_y - v_2_y));
    y_distance_reverse = l2 - x_distance;

    if (x_distance > x_distance_reverse){
        x_distance = x_distance_reverse;
    }
    if (y_distance > y_distance_reverse){
        y_distance = y_distance_reverse;
    }
    distance = x_distance + y_distance;
    return distance;
}

int distance_e(int l1, int l2, int e1, int e2){
    std::vector<int> e1_uv, e2_uv;
    int e1_u, e1_v, e2_u, e2_v;
    e1_uv = edge_to_vertices(l1, l2, e1);
    e1_u = e1_uv[0];
    e1_v = e1_uv[1];
    e2_uv = edge_to_vertices(l1, l2, e2);
    e2_u = e2_uv[0];
    e2_v = e2_uv[1];

    std::vector<int> e1_u_c, e1_v_c, e2_u_c, e2_v_c;
    e1_u_c = vertex_to_coordinate(l2, e1_u);
    e1_v_c = vertex_to_coordinate(l2, e1_v);
    e2_u_c = vertex_to_coordinate(l2, e2_u);
    e2_v_c = vertex_to_coordinate(l2, e2_v);

    int e1_u_x, e1_u_y, e1_v_x, e1_v_y, e2_u_x, e2_u_y, e2_v_x, e2_v_y;
    e1_u_x = e1_u_c[0];
    e1_u_y = e1_u_c[1];
    e1_v_x = e1_v_c[0];
    e1_v_y = e1_v_c[1];
    e2_u_x = e2_u_c[0];
    e2_u_y = e2_u_c[1];
    e2_v_x = e2_v_c[0];
    e2_v_y = e2_v_c[1];

    int e1_u_e2_u, e1_u_e2_v, e1_v_e2_u, e1_v_e2_v;
    e1_u_e2_u = distance_c(l1,l2, e1_u_x, e1_u_y, e2_u_x, e2_u_y);
    e1_u_e2_v = distance_c(l1,l2, e1_u_x, e1_u_y, e2_v_x, e2_v_y);
    e1_v_e2_u = distance_c(l1,l2, e1_v_x, e1_v_y, e2_u_x, e2_u_y);
    e1_v_e2_v = distance_c(l1,l2, e1_v_x, e1_v_y, e2_v_x, e2_v_y);
    std::vector<int> e1_e2_vec{e1_u_e2_u, e1_u_e2_v, e1_v_e2_u, e1_v_e2_v};
    int e1_e2;
    e1_e2 = *min_element(e1_e2_vec.begin(), e1_e2_vec.end());
    return e1_e2;
}