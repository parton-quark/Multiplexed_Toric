#include <random>
#include <iostream>
#include "print.hpp"

bool probabilistic(float prob, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // returns bool with probability prob
    bool result;
    double val;
    val = dist(engine);
    if (val < prob){
        result = true;
    }
    else {
        result = false;
    }
    return result;
}

double probabilistic_float(std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    double val;
    val = dist(engine);
    return val;
}


std::pair<int, int> weighted_random(std::vector<int> values,std::vector<float> weights, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    std::pair<int, int> selected_item_and_index;
    std::vector<float> cumulative_sums;
    // make a vector of cumulative sum for each element
    float cumulative_sum;
    cumulative_sum = 0;
    for (float weight: weights){
        cumulative_sum = cumulative_sum + weight;
        cumulative_sums.push_back(cumulative_sum);
    }
    double val;
    val = dist(engine);
    int index, selected_index, selected_item;
    index = 0;
    for (float cumulative_sum: cumulative_sums){
        if (val <= cumulative_sum){
            selected_index = index;
            break;
        }
        index = index + 1;
    }

    selected_item = values[selected_index];
    selected_item_and_index = {selected_item, selected_index};

    // std::cout << "\nselected item " << selected_item << " selected index " << selected_index;
    return selected_item_and_index;
}