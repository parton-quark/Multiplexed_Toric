std::vector<int> make_x_stabilzers(int l1, int l2);
std::vector<int> make_z_stabilzers(int l1, int l2);
std::vector<int> xstab_to_qubits(int l1, int l2,int x_stab);
std::vector<int> zstab_to_qubits(int l1, int l2,int z_stab);
std::vector<bool> x_stab_measurement(int l1, int l2, std::vector<int> x_stabs, std::vector<bool> zerrors);