std::vector<bool> make_erasure_errors(int num_photons, float prob_e, std::mt19937& engine, std::uniform_real_distribution<double>& dist);
std::vector<bool> make_burst_errors(int num_photons, float p_good, float p_burst, std::mt19937& engine, std::uniform_real_distribution<double>& dist);
std::vector<bool> convert_photon_loss_to_qubit_loss(int num_photons, int num_qubits, int multiplexing, std::vector<std::vector<int> > photons, std::vector<bool> photon_loss);
std::vector<bool> make_xerrors(std::vector<bool> erased_qubits, int num_qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist);
std::vector<bool> make_zerrors(std::vector<bool> erased_qubits, int num_qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist);
std::vector<bool> random_x_error(int num_qubits, float prob_e, std::mt19937& engine, std::uniform_real_distribution<double>& dist);
std::vector<bool> random_z_error(int num_qubits, float prob_e, std::mt19937& engine, std::uniform_real_distribution<double>& dist);
std::vector<bool> combine_errors(std::vector<bool> errorvec1, std::vector<bool> errorvec2);