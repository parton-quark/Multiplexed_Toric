#include <iostream>
#include <random>
#include <vector>

std::vector<int> vertex_to_coordinate(int l2, int v){
    // Z stabilizer
    int x,y;
    x = v % l2;
    y = v / l2;
    std::vector<int> coordicate{x,y};
    return coordicate;
}

std::vector<std::vector<int>> edge_to_coordinate(int l2, int v){
    // qubit
    int v_0_x,v_0_y,v_1_x,v_1_y;
    if(v % 2 == 0){
        // Horizontal
        v_0_x = v % l2;
        v_0_y = v / l2;
        v_1_x = (v % l2) + 1;
        v_1_y = v / l2;
    }
    else if (v % 2 == 1){
        // Vertical
        v_0_x = v % l2;
        v_0_y = v / l2;
        v_1_x = v % l2;
        v_1_y = (v / l2)+ 1;
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