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