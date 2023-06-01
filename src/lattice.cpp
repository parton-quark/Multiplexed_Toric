#include <iostream>
#include <random>
#include <vector>
#include <cstdlib>

std::vector<int> vertex_to_coordinate(int l2, int v){
    // Z stabilizer
    int x,y;
    x = v % l2;
    y = v / l2;
    std::vector<int> coordicate{x,y};
    return coordicate;
}

int coordinate_to_vertex(int l2, int x, int y){
    int vertex;
    vertex = x + y * l2;
    return vertex;
}

std::vector<std::vector<int>> edge_to_coordinate(int l2, int e){
    // qubit
    int v_0_x,v_0_y,v_1_x,v_1_y;
    if(e % 2 == 0){
        // Horizontal
        v_0_x = e % l2;
        v_0_y = e / l2;
        v_1_x = (e % l2) + 1;
        v_1_y = e / l2;
    }
    else if (e % 2 == 1){
        // Vertical
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

std::vector<int> edge_to_vertices(int l2, int e){
    std::vector<std::vector<int>> v1_v2;
    v1_v2 = edge_to_coordinate(l2, e);
    v1_coordinate = v1_v2[0];
    v2_coordinate = v1_v2[1];
    v1 = coordinate_to_vertex(l2, v1_coordinate[0], v1_coordinate[1]);
    v2 = coordinate_to_vertex(l2, v2_coordinate[0], v2_coordinate[1]);
    std::vector<int> vertices{v1, v2};
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

int distance(int v_1_x, int v_1_y, int v_2_x, int v_2_y){
    // returns the distance between v1 and v2
    int distance;
    distance = abs((v_1_x - v_2_x)) + abs((v_2_x - v_2_y));
    return distance;
}