#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <math.h>
#include "print.hpp"
#include "lattice.hpp"
#include "prob.hpp"

template<typename T> void pop_front(std::vector<T> &v){
    if (v.size() > 0) {
        v.erase(v.begin());
    }
}

std::vector<std::vector<int> > assign_without_multiplexing(int l1, int l2, int num_photons, int num_qubits){
    std::vector<std::vector<int> > photons;
    for (int ph = 0; ph < num_photons; ph++){
        std::vector<int> photon;
        photon.push_back(ph);
        photons.push_back(photon);
    }
    return photons;
}

std::vector<std::vector<int> > assign_random(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // randomly assign qubits
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }

    for (int ph = 0; ph < num_photons; ph++){
        std::vector<int> photon;
        for (int qb = 0; qb < multiplexing; qb++){
            std::srand(time(NULL));
            if (left_qubits.empty()== true){
                break;
            }else{
            int qubit;
            qubit = left_qubits[rand() % left_qubits.size()];
            left_qubits.erase(std::remove(std::begin(left_qubits), std::end(left_qubits), qubit), std::end(left_qubits));
            photon.push_back(qubit);
            }
        }
        photons.push_back(photon);
    }
    return photons;
}

std::pair<std::vector<std::vector<int> >, int> assign_random_distance(int l1, int l2, int multiplexing, int num_photons, int num_qubits, std::mt19937& engine){
    // Adopt or reject multiple qubits that are more than a threshold distance apart and assign them to the same photon
    // If allocation is not possible within the threshold, update the threshold
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    int threshold;
    threshold = ((l1 + l2) / 4) - 1;
    // std::cout << "\nthreshold                 :" << threshold;
    for (int ph = 0; ph < num_photons; ph++){
        std::vector<int> photon;
        std::shuffle(left_qubits.begin(), left_qubits.end(), engine);
        int first_qubit_in_photon;
        first_qubit_in_photon = left_qubits[left_qubits.size() - 1];
        photon.push_back(first_qubit_in_photon);
        left_qubits.erase(std::remove(left_qubits.begin(), left_qubits.end(),first_qubit_in_photon), left_qubits.end());
        bool is_photon_sat;
        is_photon_sat = false;
        while (!is_photon_sat){
            std::shuffle(left_qubits.begin(), left_qubits.end(), engine);
            for (int candidate_qb: left_qubits){
                bool has_enough_distance;
                has_enough_distance = false;
                
                int distance;
                distance = 0;
                for (int other_qb_in_same_ph: photon){
                    int dist_other_qb;
                    dist_other_qb = distance_e(l1, l2, candidate_qb, other_qb_in_same_ph);
                    if (dist_other_qb > distance){
                        distance = dist_other_qb;
                    }
                }
                if (distance >= threshold){
                    photon.push_back(candidate_qb);

                    if (photon.size() == multiplexing){
                        is_photon_sat = true;
                        photons.push_back(photon);
                        for (int qubit: photon){
                            left_qubits.erase(std::remove(left_qubits.begin(), left_qubits.end(), qubit), left_qubits.end());
                        }
                    }
                } 
            }
            if (!is_photon_sat){
                threshold = threshold - 1;
                std::cout << "\nthreshold update          :" << threshold << std::flush;

            }
        }
    }
    std::pair<std::vector<std::vector<int> >, int> photon_and_threshold;
    photon_and_threshold.first = photons;
    photon_and_threshold.second = threshold;
    return photon_and_threshold;
}

// assign_deterministic
std::vector<std::vector<int> > assign_deterministic(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
     // deterministic min dist
    std::vector<int> photon_of_qubits;

    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    for (int left_qubit: left_qubits){
        if (left_qubit % 2 == 0){
            // vertical
            if (left_qubit < l1 * l2){
                // lower block
                int photon_index;
                photon_index = left_qubit  / 2; 
                photon_index = photon_index + (num_photons) / 2; 
                photon_of_qubits.push_back(photon_index);
                
            } else {
                // upper block
                int photon_index;
                photon_index = (left_qubit - (l1 * l2)) / 2; /*上段と同じ並び ここから半分ずらす*/
                int row_index, column_index;
                row_index = photon_index / l2;
                column_index =  photon_index % l2;
                if (column_index < (l2 / 2)){
                    // 左側なので右側にずらす
                    photon_index =  photon_index + (l2/2);
                } else {
                    // 右側
                    photon_index = photon_index - (l2/2);
                }
                photon_index = photon_index + (num_photons) / 2;
                // photons[photon_index].push_back(left_qubit);
                photon_of_qubits.push_back(photon_index);
            }
        } else if (left_qubit % 2 == 1){
            //　horizontal
            // std::cout << " horizontal"<< std::flush;
            if (left_qubit < l1 * l2){
                // lower block
                int photon_index;
                photon_index = left_qubit / 2;
                photon_of_qubits.push_back(photon_index);
            } else {
                // upper block
                int photon_index;
                photon_index = (left_qubit - (l1 * l2)) / 2;
                int row_index, column_index;
                row_index = photon_index / l2;
                column_index =  photon_index % l2;
                if (column_index < (l2 / 2)){
                    // 左側なので右側にずらす
                    photon_index =  photon_index + (l2/2);
                } else {
                    // 右側なので左側にずらす
                    photon_index =  photon_index - (l2/2);
                }
                photon_of_qubits.push_back(photon_index);
            }
        }    
    }
    std::vector<std::vector<int> > photons = {};

    for (int i = 0; i < num_photons; i++){
        std::pair<int, int> photon;
        bool is_first_qubit;
        is_first_qubit = true;
        for (int j = 0; j < num_qubits; j++){
            if (photon_of_qubits[j] == i){
                if (is_first_qubit){
                    photon.first = j;
                    is_first_qubit = false;
                } else {
                    photon.second = j;
                }
            } else {
                //
            }
        }
        std::vector<int> photon_vec = {photon.first, photon.second};
        photons.push_back(photon_vec);
    }
    return photons;
}

std::vector<std::vector<int> > assign_deterministic_shrink(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // deterministic max dist
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    for (int ph = 0; ph  < num_photons; ph++){
        std::vector<int> photon;
        for (int qubit = multiplexing * ph; qubit < multiplexing * (ph + 1); qubit++){
            photon.push_back(left_qubits[qubit]);
        }
        photons.push_back(photon);
    }
    return photons;
}

std::vector<float> weights_from_degrees(std::vector<int> degrees, int force){
    // 次数rのベクトルを受け取って、AF^rに直す
    std::vector<int> vec_fr;
    
    for (int degree: degrees){
        int fr;
        fr = pow(force, degree);
        vec_fr.push_back(fr);
    }
    // 規格化するために和を取る
    int sum;
    sum = 0;
    for (int fr: vec_fr){
        sum = sum  + fr;
    }

    // 規格化する
    std::vector<float> vec_afr;
    for (int fr: vec_fr){
        float afr;
        afr = (float) fr / (float) sum;
        vec_afr.push_back(afr);
    }
    return vec_afr;
}

std::vector<std::vector<int> > assign_random_with_occupation_enhancement_for_each_photon(int l1, int l2, int multiplexing, int num_photons, int num_qubits, int force, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // Inspired by L. J. Duckers, and R. G. Ross. "Percolation with non-random site occupation." Physics Letters A 49.5 (1974): 361-362. 
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }

    for (int ph = 0; ph < num_photons; ph++){
        // initialize degree vector
        // std::cout << "\nnew photon!";
        std::vector<int> degrees;
        for (int left_qubit: left_qubits){
            int deg = 0;
            degrees.push_back(deg);
        }
        // convert degree vector into weight vector (AF^r)
        std::vector<float> weights;
        weights = weights_from_degrees(degrees, force);

        std::vector<int> photon;
        for (int qb = 0; qb < multiplexing; qb++){
            // pick weighted random
            std::pair <int, int> selected_item_and_index;
            int selected_qubit, selected_index;
            selected_item_and_index = weighted_random(left_qubits, weights, engine, dist);
            selected_qubit = selected_item_and_index.first;
            selected_index = selected_item_and_index.second;
            // add selected_qubit to photon
            photon.push_back(selected_qubit);
            // update degrees
            std::vector<int> vertices;// vertices of the selected qubit
            vertices =  edge_to_vertices(l1, l2, selected_qubit);
            std::vector<int> adjacent_qubits; 
            for (int vertex: vertices){
                std::vector<int> edges;
                edges = vertex_to_edges(l1, l2, vertex);
                for (int edge: edges){
                    if (edge != selected_qubit){
                        adjacent_qubits.push_back(edge);  
                    }
                }
            }
            for (int adjacent_qubit: adjacent_qubits){
                size_t adj_count = std::count(left_qubits.begin(), left_qubits.end(), adjacent_qubit);
                if (adj_count > 0){
                    // update degree
                    int adjacent_qubits_index;
                    adjacent_qubits_index = 0;
                    for (int left_q: left_qubits){
                        if (left_q == adjacent_qubit){
                            break;
                        }
                        adjacent_qubits_index = adjacent_qubits_index + 1;
                    }
                    degrees[adjacent_qubits_index] = degrees[adjacent_qubits_index] + 1;
                }
            }

            // remove degree of selected_qubit
            degrees.erase(degrees.begin() + selected_index);
            // remove selected_qubit from left_qubits
            left_qubits.erase(remove(left_qubits.begin(), left_qubits.end(), selected_qubit), left_qubits.end());
            // update weights
            weights = weights_from_degrees(degrees, force);
        }
        photons.push_back(photon);
    }
    return photons;
}



std::vector<std::vector<int> > assign_random_with_occupation_enhancement(int l1, int l2, int multiplexing, int num_photons, int num_qubits, int force, std::mt19937& engine, std::uniform_real_distribution<double>& dist){
    // Inspired by L. J. Duckers, and R. G. Ross. "Percolation with non-random site occupation." Physics Letters A 49.5 (1974): 361-362. 
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    // initialize degree vector
    std::vector<int> degrees;
    for (int left_qubit: left_qubits){
        int deg = 0;
        degrees.push_back(deg);
    }

    for (int ph = 0; ph < num_photons; ph++){
        // convert degree vector into weight vector (AF^r)
        std::vector<float> weights;
        weights = weights_from_degrees(degrees, force);

        std::vector<int> photon;
        for (int qb = 0; qb < multiplexing; qb++){
            // pick weighted random
            std::pair <int, int> selected_item_and_index;
            int selected_qubit, selected_index;
            selected_item_and_index = weighted_random(left_qubits, weights, engine, dist);
            selected_qubit = selected_item_and_index.first;
            selected_index = selected_item_and_index.second;
            // add selected_qubit to photon
            photon.push_back(selected_qubit);
            // update degrees
            std::vector<int> vertices;// vertices of the selected qubit
            vertices =  edge_to_vertices(l1, l2, selected_qubit);
            std::vector<int> adjacent_qubits; 
            for (int vertex: vertices){
                std::vector<int> edges;
                edges = vertex_to_edges(l1, l2, vertex);
                for (int edge: edges){
                    if (edge != selected_qubit){
                        adjacent_qubits.push_back(edge);  
                    }
                }
            }
            for (int adjacent_qubit: adjacent_qubits){
                size_t adj_count = std::count(left_qubits.begin(), left_qubits.end(), adjacent_qubit);
                if (adj_count > 0){
                    // update degree
                    int adjacent_qubits_index;
                    adjacent_qubits_index = 0;
                    for (int left_q: left_qubits){
                        if (left_q == adjacent_qubit){
                            break;
                        }
                        adjacent_qubits_index = adjacent_qubits_index + 1;
                    }
                    degrees[adjacent_qubits_index] = degrees[adjacent_qubits_index] + 1;
                }
            }

            // remove degree of selected_qubit
            degrees.erase(degrees.begin() + selected_index);
            // remove selected_qubit from left_qubits
            left_qubits.erase(remove(left_qubits.begin(), left_qubits.end(), selected_qubit), left_qubits.end());
            // update weights
            weights = weights_from_degrees(degrees, force);
        }
        photons.push_back(photon);
    }
    return photons;
}

std::vector<std::vector<int> > assign_by_star_stabilizer_even_coordinate(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // check whether l1 and l2 are even or not
    if (l1 % 2 != 0 || l2 % 2 != 0){
        std::cout << "l1 and l2 must be even for assign_by_star_stabilizer strategy" << std::flush;
        return {};
    }
    // check whether multiplexing is 4 or not
    if (multiplexing != 4){
        std::cout << "multiplexing must be 4 for assign_by_star_stabilizer strategy" << std::flush;
        return {};
    }
    // assign qubits by stabilizer
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    // make a list of star stabilizers 
    std::vector<int> star_stabilizers;
    int star_index = 0;
    for (int i = 0; i < l1; i++){
        for (int j = 0; j < l2; j++){
            int star_stabilizer;
            star_stabilizer = star_index;
            star_index = star_index + 1;
            star_stabilizers.push_back(star_stabilizer);
        }
    }
    std::cout << "\nstar stabilizers: "<< std::flush;
    print_vec(star_stabilizers);
    for (int star_stabilizer: star_stabilizers){
        // add supports of stabilizer to a photon if the coordinate is even for both x and y
        std::vector<int> photon;
        //given a star stabilizer, get the coordinate
        std::vector<int> coordinate;
        coordinate = vertex_to_coordinate(l2, star_stabilizer);
        // std::cout << "\ncoordinate: " << coordinate[0] << "," << coordinate[1] << std::flush;

        // if (coordinate[0] % 2 == 0 && coordinate[1] % 2 == 0){ 
        if ((coordinate[0] + coordinate[1]) % 2 == 0){
            //  std::cout << "\ncoordinate is satisfying: " << coordinate[0] + coordinate[1]  << std::flush;
            std::vector<int> supports;
            supports = vertex_to_edges(l1, l2, star_stabilizer);
            // add supports of the stabilizer to the photon
            for (int support: supports){
                photon.push_back(support);
            }
            photons.push_back(photon);
        }
    }
    return photons;
}

std::vector<std::vector<int> > assign_by_star_stabilizer_odd_coordinate(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // check whether l1 and l2 are even or not
    if (l1 % 2 != 0 || l2 % 2 != 0){
        std::cout << "l1 and l2 must be even for assign_by_star_stabilizer strategy" << std::flush;
        return {};
    }
    // check whether multiplexing is 4 or not
    if (multiplexing != 4){
        std::cout << "multiplexing must be 4 for assign_by_star_stabilizer strategy" << std::flush;
        return {};
    }
    // assign qubits by stabilizer
    std::vector<std::vector<int> > photons;
    std::vector<int> left_qubits;
    for (int q = 0; q < num_qubits; q++){
        left_qubits.push_back(q);
    }
    // make a list of star stabilizers 
    std::vector<int> star_stabilizers;
    int star_index = 0;
    for (int i = 0; i < l1; i++){
        for (int j = 0; j < l2; j++){
            int star_stabilizer;
            star_stabilizer = star_index;
            star_index = star_index + 1;
            star_stabilizers.push_back(star_stabilizer);
        }
    }
    std::cout << "\nstar stabilizers: "<< std::flush;
    print_vec(star_stabilizers);
    for (int star_stabilizer: star_stabilizers){
        // add supports of stabilizer to a photon if the coordinate is even for both x and y
        std::vector<int> photon;
        //given a star stabilizer, get the coordinate
        std::vector<int> coordinate;
        coordinate = vertex_to_coordinate(l2, star_stabilizer);
        // std::cout << "\ncoordinate: " << coordinate[0] << "," << coordinate[1] << std::flush;

        // if (coordinate[0] % 2 == 0 && coordinate[1] % 2 == 0){ 
        if ((coordinate[0] + coordinate[1]) % 2 == 1){
            //  std::cout << "\ncoordinate is satisfying: " << coordinate[0] + coordinate[1]  << std::flush;
            std::vector<int> supports;
            supports = vertex_to_edges(l1, l2, star_stabilizer);
            // add supports of the stabilizer to the photon
            for (int support: supports){
                photon.push_back(support);
            }
            photons.push_back(photon);
        }
    }
    return photons;
}

std::vector<std::vector<int> > assign_by_plaquette_stabilizer_even_coordinate(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // check whether l1 and l2 are even or not
    if (l1 % 2 != 0 || l2 % 2 != 0){
        std::cout << "l1 and l2 must be even for assign_by_plaquette_stabilizer_even_coordinate strategy" << std::flush;
        return {};
    }
    // check whether multiplexing is 4 or not
    if (multiplexing != 4){
        std::cout << "multiplexing must be 4 for assign_by_plaquette_stabilizer_even_coordinate strategy" << std::flush;
        return {};
    }
    
    // make plaquette stabilizers
    std::vector<int> plaquette_stabilizers;
    int plaquette_index = 0;
    for (int i = 0; i < l1; i++){
        for (int j = 0; j < l2; j++){
            int plaquette_stabilizer;
            plaquette_stabilizer = plaquette_index;
            plaquette_index = plaquette_index + 1;
            plaquette_stabilizers.push_back(plaquette_stabilizer);
        }
    }
    // std::cout << "\nplaquette stabilizers      :";
    // print_vec(plaquette_stabilizers);
    // assign qubits by stabilizer
    std::vector<std::vector<int> > photons;
    for (int plaquette_stabilizer: plaquette_stabilizers){
        // add supports of stabilizer to a photon if the coordinate is even for both x and y
        std::vector<int> photon;
        //given a star stabilizer, get the coordinate
        std::vector<std::vector<int>> coordinates;
        coordinates = face_to_coordinates(l2, plaquette_stabilizer);
        std::vector<int> coordinate_lower_left;
        coordinate_lower_left = coordinates[0];

        if ((coordinate_lower_left[0] + coordinate_lower_left[1]) % 2 == 0){
            std::vector<int> supports;
            supports = face_to_edges(l1, l2, plaquette_stabilizer);
            // add supports of the stabilizer to the photon
            for (int support: supports){
                photon.push_back(support);
            }
            photons.push_back(photon);
        }
    }
    return photons;
}

std::vector<std::vector<int> > assign_by_plaquette_stabilizer_odd_coordinate(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // check whether l1 and l2 are even or not
    if (l1 % 2 != 0 || l2 % 2 != 0){
        std::cout << "l1 and l2 must be even for assign_by_plaquette_stabilizer_odd_coordinate strategy" << std::flush;
        return {};
    }
    // check whether multiplexing is 4 or not
    if (multiplexing != 4){
        std::cout << "multiplexing must be 4 for assign_by_plaquette_stabilizer_odd_coordinate strategy" << std::flush;
        return {};
    }
    
    // make plaquette stabilizers
    std::vector<int> plaquette_stabilizers;
    int plaquette_index = 0;
    for (int i = 0; i < l1; i++){
        for (int j = 0; j < l2; j++){
            int plaquette_stabilizer;
            plaquette_stabilizer = plaquette_index;
            plaquette_index = plaquette_index + 1;
            plaquette_stabilizers.push_back(plaquette_stabilizer);
        }
    }

    // assign qubits by stabilizer
    std::vector<std::vector<int> > photons;
    for (int plaquette_stabilizer: plaquette_stabilizers){
        // add supports of stabilizer to a photon if the coordinate is even for both x and y
        std::vector<int> photon;
        //given a star stabilizer, get the coordinate
        std::vector<std::vector<int>> coordinates;
        coordinates = face_to_coordinates(l2, plaquette_stabilizer);
        std::vector<int> coordinate_lower_left;
        coordinate_lower_left = coordinates[0];

        if ((coordinate_lower_left[0] + coordinate_lower_left[1]) % 2 == 1){
            //  std::cout << "\ncoordinate is satisfying: " << coordinate[0] + coordinate[1]  << std::flush;
            std::vector<int> supports;
            supports = face_to_edges(l1, l2, plaquette_stabilizer);
            // add supports of the stabilizer to the photon
            for (int support: supports){
                photon.push_back(support);
            }
            photons.push_back(photon);
        }
    }
    return photons;
}

std::vector<std::vector<int> > assign_by_mixed_stabilizer(int l1, int l2, int multiplexing, int num_photons, int num_qubits){
    // this strategy only works for 4n x 4n lattice
    if (l1 % 4 != 0 || l2 % 4 != 0){
        std::cout << "l1 and l2 must be multiples of 4 for assign_by_star_and_plaquette_stabilizer strategy" << std::flush;
        return {};
    }
    // this strategy only works for l1 = l2
    if (l1 != l2){
        std::cout << "l1 and l2 must be equal for assign_by_star_and_plaquette_stabilizer strategy" << std::flush;
        return {};
    }
    // this strategy only works for multiplexing = 4
    if (multiplexing != 4){
        std::cout << "multiplexing must be 4 for assign_by_star_and_plaquette_stabilizer strategy" << std::flush;
        return {};
    }
    // make diagonal bands
    std::vector<int> bands;
    for (int i = 0; i < l2/2; i++){
        bands.push_back(i);
    }
    // make photons
    std::vector<std::vector<int> > photons;

    for (int band: bands){
        if (band % 2 == 0){
            // plaquette stabilizer based assignment 
            for (int plaquette = 0; plaquette <= l1 - 1; plaquette++){
                int x0 = ((band * 2) + plaquette) % l2;
                int y0 = 0 + plaquette;
                int x1 = x0 + 1;
                int y1 = y0;
                int x2 = x0;
                int y2 = y0 + 1;
                int x3 = x0 + 1;
                int y3 = y0 + 1;
                int face;
                face = coordinates_to_face(l2, x0, y0, x1, y1, x2, y2, x3, y3);
                std::vector<int> edges;
                edges = face_to_edges(l1, l2, face);
                photons.push_back(edges);
            }
        } else if (band % 2 == 1){ 
            // star stabilizer based assignment
            for (int star = 0; star <= l1 - 1; star++){
                int x = (2 * band + star) % l2;
                int y = 0 + star;
                int vertex;
                vertex = coordinate_to_vertex(l2, x, y);
                std::vector<int> edges;
                edges = vertex_to_edges(l1, l2, vertex);
                photons.push_back(edges);
            }
        }
    }    
    return photons;
}

