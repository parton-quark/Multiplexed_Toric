bool is_even_cluster(graph cluster, std::vector<bool> syndrome);
bool is_included_in_any_cluster(int qubit_index_for_a_syndrome, std::vector<graph> forest);
graph grow_cluster(graph cluster, int l1, int l2);
graph fuse_cluster(graph clusterA, graph clusterB);
bool detect_collision(graph clusterA, graph clusterB);
std::vector<bool> union_find_decoder_for_z_errors(int l1, int l2, int num_qubits, std::vector<bool> qubit_loss, std::vector<bool> x_syndrome);