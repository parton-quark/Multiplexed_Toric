template <class T> void print_vec(std::vector<T> vec){
    bool is_first_elem;
    is_first_elem = true;
    for (T elem: vec){ 
        if (is_first_elem){
            std::cout << elem << std::flush;
            is_first_elem = false;
        } else {
            std::cout << "," << elem << std::flush;
        }
    }
}

void print_vec_of_vec(std::vector<std::vector<int> > vec_vec);
void print_vec_of_vec(std::vector<std::vector<float> > vec_vec);
void print_ind_of_bool_vec(std::vector<bool>  bv);
std::string vec_to_str(std::vector<bool> vec);