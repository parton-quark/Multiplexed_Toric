std::vector<bool> make_erasure_errors(int num_photons, float prob_e, std::mt19937& engine, std::uniform_real_distribution<double>& dist);
std::vector<bool> convert_photon_loss_to_qubit_loss(int num_photons, int num_qubits, int multiplexing, std::vector<std::vector<int> > photons, std::vector<bool> photon_loss);
std::vector<bool> make_xerrors(std::vector<bool> erased_qubits, int num_qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist);
std::vector<bool> make_zerrors(std::vector<bool> erased_qubits, int num_qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist);