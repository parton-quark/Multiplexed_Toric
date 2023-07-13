template<typename T> void pop_front(std::vector<T> &v);
std::vector<std::vector<int> > assign_without_multiplexing(int l1, int l2, int num_photons, int num_qubits);
std::vector<std::vector<int> > assign_random(int l1, int l2, int multiplexing, int num_photons, int num_qubits);
std::pair<std::vector<std::vector<int> >, int> assign_random_distance(int l1, int l2, int multiplexing, int num_photons, int num_qubits, std::mt19937& engine);
std::vector<std::vector<int> > assign_deterministic(int l1, int l2, int multiplexing, int num_photons, int num_qubits);
std::vector<std::vector<int> > assign_deterministic_shrink(int l1, int l2, int multiplexing, int num_photons, int num_qubits);
std::vector<float> weights_from_degrees(std::vector<int> degrees, int force);
std::vector<std::vector<int> > assign_random_with_occupation_enhancement(int l1, int l2, int multiplexing, int num_photons, int num_qubits, int force, std::mt19937& engine, std::uniform_real_distribution<double>& dist);