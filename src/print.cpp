#include <iostream>
#include <vector>
#include "print.hpp"

void print_ind_of_bool_vec(std::vector<bool>  bv){
    bool is_first_elem;
    for (int index = 0; index < bv.size(); index++){
        if (bv[index]){
            std::cout << index << "," << std::flush;
        }
    }
}

void print_vec_of_vec(std::vector<std::vector<int> > vec_vec){
    for (std::vector<int> vec: vec_vec){
        std::cout << "[";
        print_vec(vec);
        std::cout << "], ";
    }
}

void print_vec_of_vec(std::vector<std::vector<float> > vec_vec){
    for (std::vector<float> vec: vec_vec){
        std::cout << "[";
        print_vec(vec);
        std::cout << "], ";
    }
}

std::string vec_to_str(std::vector<bool> vec){
    std::string vecstr;
    bool is_first_elem;
    is_first_elem = true;
    for (bool elem: vec){
        int x (elem);
        if (is_first_elem){
            vecstr = vecstr + std::to_string(x);
            is_first_elem = false;
        }
        vecstr = vecstr + ", " + std::to_string(x);
    }
    return vecstr;
}