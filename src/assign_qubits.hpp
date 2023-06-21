template<typename T> void pop_front(std::vector<T> &v);
std::vector<std::vector<int> > assign_without_multiplexing(int l1, int l2, int num_photons, int num_qubits);
std::vector<std::vector<int> > assign_random(int l1, int l2, int multiplexing, int num_photons, int num_qubits);
std::pair<std::vector<std::vector<int> >, int> assign_random_distance(int l1, int l2, int multiplexing, int num_photons, int num_qubits);
// std::vector<std::vector<int> > assign_random_distance(int l1, int l2, int multiplexing, int num_photons, int num_qubits);