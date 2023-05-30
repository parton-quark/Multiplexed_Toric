#include <random>
// #include <iostream>

bool probabilistic(float prob, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // returns bool with probability prob
    bool result;
    double val;
    val = dist(engine);
    // std::cout << val;
    // std::cout << "\n";
    if (val < prob){
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}