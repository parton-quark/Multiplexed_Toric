bool probabilistic(float prob, std::mt19937& engine, std::uniform_real_distribution<double>& dist);
double probabilistic_float(std::mt19937& engine, std::uniform_real_distribution<double>& dist);
std::pair<int, int> weighted_random(std::vector<int> values,std::vector<float> weights, std::mt19937& engine, std::uniform_real_distribution<double>& dist);