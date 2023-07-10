#include <random>

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


weighted_random(std::vector<float> vec_weight, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    int result;
    val = dist(engine);
    
    //累積和に基づいて量子
    
}