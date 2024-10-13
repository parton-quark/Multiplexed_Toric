#include <iostream>
#include <vector>
#include <algorithm>
#include "lattice.hpp"
#include "graph.hpp"
#include "spanning_forest.hpp"
#include "peeling_decoder.hpp"
#include "print.hpp"

// Union Find Decoder for toric codes which works for both of erasure error and Z errors
// This implementation is based on ``Almost-linear time decoding algorithm for topological codes'' by Nicolas Delfosse and Naomi Nicherson. 
// The original paper is available at https://quantum-journal.org/papers/q-2021-12-02-595/

bool is_even_cluster(graph cluster, std::vector<bool> syndrome){
    // check if the cluster has even number of syndrome or not
    bool result;
    int num_syndrome_in_cluster = 0;
    for (int vertex: cluster.vertices){
        if (syndrome[vertex] == true){
            num_syndrome_in_cluster+=1;
        }
    }
    // std::cout << "\nchecking if the cluster is even or not...";
    // cluster.print_graph();
    // std::cout << "\nnum_syndrome_in_cluster: " << num_syndrome_in_cluster;
    if (num_syndrome_in_cluster % 2 == 0){
        result = true;
    } else {
        result = false;
    }
    return result;
}

bool is_included_in_any_cluster(int index_for_a_syndrome, std::vector<graph> forest){
    // check if the -1 syndrome is included in any cluster or not.
    bool result = false;
    for (graph cluster: forest){
        for (int vertex: cluster.vertices){
            if (vertex == index_for_a_syndrome){
                result = true;
                break;
            }
        }
        if (result){
            break;
        }
    }
    return result;
}

graph grow_cluster(graph original, int l1, int l2){
    graph grown_cluster;
    for (int ed: original.edges){
        grown_cluster.add_edge(ed);
    }
    for (int v: original.vertices){
        grown_cluster.add_vertex(v);
        // add adjacent vertices
        std::vector<int> adjacent_edges;
        adjacent_edges = vertex_to_edges(l1, l2, v);
        for (int ae: adjacent_edges){
            std::vector<int> adjacent_vertices;
            grown_cluster.add_edge(ae);
            adjacent_vertices = edge_to_vertices(l1, l2, ae);
            for (int av: adjacent_vertices){
                grown_cluster.add_vertex(av);
            }
        }
    }
    if (original.vertices == grown_cluster.vertices){
    // std::cout << "\ncluster is not grown! cluster may reach all region or an error." << std::flush;
    }
    return grown_cluster;
}

// graph grow_cluster(graph cluster, int l1, int l2){
//     // for all vertices, add the adjacent vertices and edges to the cluster
//     // std::cout << "\nstart growing cluster: ";
//     std::vector<int> exiting_vertices;
//     exiting_vertices = cluster.vertices;
//     graph grown_cluster;

//     for(int ev: exiting_vertices){
//         // add adjacent vertices
//         grown_cluster.add_vertex(ev);
//         std::vector<int> adjacent_edges;
//         adjacent_edges = vertex_to_edges(l1, l2, ev);
//         for (int ae: adjacent_edges){
//             std::vector<int> adjacent_vertices;
//             adjacent_vertices = edge_to_vertices(l1, l2, ae);
//             for (int av: adjacent_vertices){
//                 grown_cluster.add_vertex(av);
//                 // if (std::count(cluster.vertices.begin(), cluster.vertices.end(), av) == 0){
//                     // cluster.add_vertex(av);
//                 // }
//             }
//         }

//         // add adjacent edges
//         for (int ae: adjacent_edges){
//             grown_cluster.add_edge(ae);
//             // if (std::count(cluster.edges.begin(), cluster.edges.end(), ae) == 0){
//                 // cluster.add_edge(ae);
//             // }
//         }
//     }
//     // std::cout << "\ngrown cluster: ";
//     // cluster.print_graph();
//     if (cluster.vertices == grown_cluster.vertices){
//     std::cout << "\ncluster is not grown! cluster may reach all region or an error." << std::flush;
//     }


//     return cluster;
// }

graph fuse_cluster(graph clusterA, graph clusterB){
    // fuse two graphs
    // std::cout << "\nclusterA: ";
    // clusterA.print_graph();
    // std::cout << "\nclusterB: ";
    // clusterB.print_graph();
    graph fused_cluster;
    for (int vertex: clusterA.vertices){
        fused_cluster.add_vertex(vertex);
    }
    for (int vertex: clusterB.vertices){
        fused_cluster.add_vertex(vertex);
    }
    for (int edge: clusterA.edges){
        fused_cluster.add_edge(edge);
    }
    for (int edge: clusterB.edges){
        fused_cluster.add_edge(edge);
    }
    // std::cout << "\nfused_cluster: ";
    // fused_cluster.print_graph();
    return fused_cluster;
}

bool detect_collision(graph clusterA, graph clusterB){
    bool result = false;
    // check if two clusters have common vertices or not
    for(int vertexA: clusterA.vertices){
        for (int vertexB: clusterB.vertices){
            if (vertexA == vertexB){
                result = true;
                break;
            }
        }
    }
    // check if two clusters have common edges or not
    for (int edgeA: clusterA.edges){
        for (int edgeB: clusterB.edges){
            if (edgeA == edgeB){
                result = true;
                break;
            }
        }
    }
    return result;
}

std::vector<graph> erasure_to_non_empty_divided_epsilon(int l1, int l2, int num_qubits, std::vector<bool> qubit_loss){
    graph epsilon; /* graph which has erased edges */
    // convert erasure qubits into edges
    for (int i = 0; i < num_qubits; i++){
        if (qubit_loss[i] == true){
            epsilon.add_edge(i);
            std::vector<int> v0_v1;
            int v0, v1;
            v0_v1 = edge_to_vertices(l1, l2, i);
            v0 = v0_v1[0];
            v1 = v0_v1[1];
            epsilon.add_vertex(v0);
            epsilon.add_vertex(v1);
        }
    }
    // Divide epsilon to multiple connected graphs
    std::vector<graph> divided_epsilon;
    divided_epsilon = devide_graph(epsilon, l1, l2);
    // remove empty graph
    std::vector<graph> non_empty_divided_epsilon;
    for (graph cluster: divided_epsilon){
        if (cluster.num_vertices() != 0){
            non_empty_divided_epsilon.push_back(cluster);
        }
    }
    return non_empty_divided_epsilon;
}

std::pair<std::vector<graph>, std::vector<graph>> classify_odd_even_clusters(std::vector<graph> non_empty_divided_epsilon, std::vector<bool> x_syndrome){
    // classify cluster (even or odd)
    std::vector<graph> even_clusters;
    std::vector<graph> odd_clusters;
    for (graph cluster: non_empty_divided_epsilon){
        if (is_even_cluster(cluster, x_syndrome)){
            even_clusters.push_back(cluster);
        } else {
            odd_clusters.push_back(cluster);
        }
    }    
    std::pair<std::vector<graph>, std::vector<graph>> even_odd_clusters;
    even_odd_clusters = std::make_pair(even_clusters, odd_clusters);
    return even_odd_clusters;
}


std::vector<bool> union_find_decoder_for_z_errors(int l1, int l2, int num_qubits, std::vector<bool> qubit_loss, std::vector<bool> x_syndrome){
    // inputs:
    // lattice size (int l1, int l2)
    // number of qubits (int num_qubits)
    // erasure vector(std::vector<bool> qubit_loss)
    // x stabilizer syndrome (z error syndrome, std::vector<bool> x_syndrome)
    // outputs:
    // Z correction (std::vector<bool> uf_z)
    // std::cout << "\nStart union find decoder for Z errors..." << std::flush;
    // convert erasure qubits into edges
    std::vector<graph> non_empty_divided_epsilon;
    non_empty_divided_epsilon = erasure_to_non_empty_divided_epsilon(l1, l2, num_qubits, qubit_loss);
    // std::cout << "\ndivied epsilon            :";
    // for (graph cluster: non_empty_divided_epsilon){
        // cluster.print_graph();
    // }

    // classify cluster (even or odd)
    std::vector<graph> even_clusters, odd_clusters;
    std::pair<std::vector<graph>, std::vector<graph>> even_odd_clusters;
    even_odd_clusters = classify_odd_even_clusters(non_empty_divided_epsilon, x_syndrome);
    even_clusters = even_odd_clusters.first;
    odd_clusters = even_odd_clusters.second;

    // make a vector of left syndromes (which is not included in any erasure cluster)
    std::vector<int> left_syndromes; 
    int index_for_a_syndrome = 0;
    for (bool syndrome: x_syndrome){
        // is the syndrome is inclueded in any cluster?
        if (syndrome){
            if (!is_included_in_any_cluster(index_for_a_syndrome, non_empty_divided_epsilon)){
                left_syndromes.push_back(index_for_a_syndrome); 
            }
        }
        index_for_a_syndrome++;
    }
    // std::cout << "\nleft syndromes: ";
    // print_vec(left_syndromes);
    // Construct a single node cluster from left -1 syndrome and add to odd clusters
    for (int left_syndrome: left_syndromes){
        graph new_cluster;
        new_cluster.add_vertex(left_syndrome);
        odd_clusters.push_back(new_cluster);
    }
    // print clusters before growing
    // std::cout << "\nnumber of even clusters: " << even_clusters.size();
    // for (graph cluster: even_clusters){
    //     std::cout << "\ngraph: ";
    //     cluster.print_graph();
    // }
    // std::cout << "\nnumber of odd clusters: " << odd_clusters.size();
    // for (graph cluster: odd_clusters){
    //     cluster.print_graph();
    // }
    // std::cout << "\nodd_clusters.size(): " << odd_clusters.size() << std::flush;
    bool is_odd_clusters_empty;
    if (odd_clusters.size() == 0){
        is_odd_clusters_empty = true;
        // std::cout << "\nodd clusters is empty." << std::flush;
    } else {
        is_odd_clusters_empty = false;
        // std::cout << "\nodd clusters is not empty." << std::flush;
    }
    for (graph oc: odd_clusters){
        // oc.print_graph();
        if (oc.num_vertices() == 0){
            std::cout << "\nThis cluster is empty!" << std::flush;
        }
    }


    // ここまで大丈夫
    bool is_first_time = true;
    while (!is_odd_clusters_empty){ // while there is at least one odd cluster
        for (int odd_i = 0; odd_i <= odd_clusters.size() - 1; odd_i++){// このループが終わらない
            graph original_cluster;
            original_cluster = odd_clusters[odd_i];
            // if (original_cluster.num_vertices() == 0){
            //     std::cout << "\nThis cluster is empty!" << std::flush;
            //     // I wonder why we have empty cluster...
            //     // remove empty cluster
            //     std::cout << "\nerase empty cluster!" << std::flush;
            //     std::cout << "\nnumber of odd clusters before: " << odd_clusters.size() << std::flush;
            //     odd_clusters.erase(odd_clusters.begin() + odd_i);
            //     std::cout << "\nnumber of odd clusters after: " << odd_clusters.size() << std::flush;
            //     continue;
            // }
            // grow cluster
            graph grown_cluster;
            grown_cluster = grow_cluster(original_cluster, l1, l2);
            // if it meets other clusters, fuse cluster and update parity
            std::vector<graph> collided_odd_clusters;
            std::vector<int> collided_odd_cluster_indices;
            // check if the grown cluster collides with other odd clusters
            for (int col_odd_i = 0; col_odd_i <= odd_clusters.size() - 1; col_odd_i++){
                if (col_odd_i == odd_i){ // skip the same cluster
                    continue;
                } else {
                    if (detect_collision(grown_cluster, odd_clusters[col_odd_i])){
                        // std::cout << "\ncluster has been collided with an odd cluster!" << std::flush;
                        collided_odd_clusters.push_back(odd_clusters[col_odd_i]);
                        collided_odd_cluster_indices.push_back(col_odd_i);
                    }
                }
            }
            // if (is_first_time){
                // std::cout << "\nodd cluster index: ";
                // std::cout << odd_i << std::flush;
                // std::cout << "\ncollided odd clusters indices: ";
                // print_vec(collided_odd_cluster_indices);
            // }
            std::vector<graph> collided_even_clusters;
            std::vector<int> collided_even_cluster_indices;
            // check if the grown cluster collides with other even clusters
            if (even_clusters.size() == 0){
                // std::cout << "\nno even cluster!" << std::flush;
            } else {
                for (int even_i = 0; even_i <= even_clusters.size()-1; even_i++){
                    // even_clusters[even_i].print_graph();
                    if (detect_collision(grown_cluster, even_clusters[even_i])){
                    collided_even_clusters.push_back(even_clusters[even_i]);
                    collided_even_cluster_indices.push_back(even_i);
                    }
                }
            }
            // fuse grown cluster and collided clusters
            for (graph collided_odd_cluster: collided_odd_clusters){
                // std::cout << "\ncluster has been collided with an odd cluster!" << std::flush;
                grown_cluster = fuse_cluster(grown_cluster, collided_odd_cluster);
            }
            for (graph collided_even_cluster: collided_even_clusters){
                // std::cout << "\ncluster has been collided with an even cluster!" << std::flush;
                grown_cluster = fuse_cluster(grown_cluster, collided_even_cluster);
            }
            if (is_first_time){
                // std::cout << "\ngrown cluster: ";
                // grown_cluster.print_graph();
                // std::cout << std::flush;
            }
            is_first_time = false;
            bool is_grown_even = is_even_cluster(grown_cluster, x_syndrome);
            // std::cout << "\nnumber of odd/even before: " << odd_clusters.size() << "/" << even_clusters.size() << std::flush;
            if (is_grown_even){ // remove original and add grown to even_clusters
                // remove colided odds and the original odd from odd_clusters
                std::vector<int> erased_odd_cluster_indices;
                for (int i: collided_odd_cluster_indices){
                    erased_odd_cluster_indices.push_back(i); // collided odd cluster
                }
                erased_odd_cluster_indices.push_back(odd_i); //original cluster
                std::sort(erased_odd_cluster_indices.begin(), erased_odd_cluster_indices.end());
                int num_removed_odd = 0;
                for (int i: erased_odd_cluster_indices){
                    int erase_index = i - num_removed_odd;
                    odd_clusters.erase(odd_clusters.begin() + erase_index);
                    num_removed_odd++;
                }
                std::vector<int> erased_even_cluster_indices;
                for (int i: collided_even_cluster_indices){
                    erased_even_cluster_indices.push_back(i); // collided even cluster
                }
                std::sort(erased_even_cluster_indices.begin(), erased_even_cluster_indices.end());
                int num_removed_even = 0;
                for (int i: erased_even_cluster_indices){
                    int erase_index = i - num_removed_even;
                    even_clusters.erase(even_clusters.begin() + erase_index);
                    num_removed_even++;
                }
                // add grown to even_clusters
                even_clusters.push_back(grown_cluster);
            } else { // update original and remove collided odds/evens
                // update original odd cluster to grown
                odd_clusters[odd_i] = grown_cluster;
                // remove collided odd clusters from odd_clusters
                std::vector<int> erased_odd_cluster_indices;
                for (int i: collided_odd_cluster_indices){
                    erased_odd_cluster_indices.push_back(i);
                }
                std::sort(erased_odd_cluster_indices.begin(), erased_odd_cluster_indices.end());
                int num_removed_odd = 0;
                for (int i: erased_odd_cluster_indices){
                    int erase_index = i - num_removed_odd;
                    odd_clusters.erase(odd_clusters.begin() + erase_index);
                    num_removed_odd++;
                }
                // remove collided even clusters from even_clusters
                std::vector<int> erased_even_cluster_indices;
                for (int i: collided_even_cluster_indices){
                    erased_even_cluster_indices.push_back(i);
                }
                std::sort(erased_even_cluster_indices.begin(), erased_even_cluster_indices.end());
                int num_removed_even = 0;
                for (int i: erased_even_cluster_indices){
                    int erase_index = i - num_removed_even;
                    even_clusters.erase(even_clusters.begin() + erase_index);
                    num_removed_even++;
                }
                if (odd_clusters.size() == 1){
                    std::cout << "\nodd cluster has only one! Weird!" << std::flush;
                    odd_clusters[0].print_graph();
                }
            }
            // std::cout << "\nnumber of odd/even after: " << odd_clusters.size() << "/" << even_clusters.size() << std::flush;
            if (odd_clusters.size() == 0){
                is_odd_clusters_empty = true;
                break;
            }
        }
        if (odd_clusters.size() == 0){
            // std::cout << "\nodd clusters is empty." << std::flush;
            is_odd_clusters_empty = true;
        } else {
            // std::cout << "\nodd cluster has " << odd_clusters.size() << " clusters." << std::flush;
            is_odd_clusters_empty = false;
        }
    }

    // print the cluster before the peeling decoder
    // std::cout << "\nclustering finished!" << std::flush;
    // std::cout << "\nodd clusters : ";
    // for (graph cluster: odd_clusters){
    //     cluster.print_graph();
    // }
    // std::cout << "\neven clusters : ";
    // for (graph cluster: even_clusters){
    //     cluster.print_graph();
    // }

    // Apply peeling decoder to even clusters and get the correction
    // std::cout << "\napplying peeling decoder..." << std::flush;
    std::vector<bool> p_z(num_qubits);
    std::vector<graph> spanning_forest;
    for (graph cluster: even_clusters){
        if (cluster.num_vertices() == 0){
            std::cout << "\nsomething went wrong. cluster is empty!" << std::flush;
        }
        // convert graph to spanning tree before        
        graph st;
        st = maximal_spanning_tree(cluster, l1, l2);
        spanning_forest.push_back(st);
    }
    // apply peeling decoder to get the correction
    graph A;
    for (graph f_eps: spanning_forest){
        // 3. While f_eps = ∅, pick a leaf edge e={u,v} with pendant vertex u,remove e from f_eps and apply the two rules: 
        while (f_eps.num_edges() != 0){
            std::pair<int, std::pair<int, int>> leaf_and_pendant; /* leaf edge */
            // pick a leaf edge e={u,v} with pendant vertex u
            leaf_and_pendant = pick_leaf_edge(f_eps, l1, l2);
            int leaf, pendant, connected;
            leaf = leaf_and_pendant.first;
            pendant = leaf_and_pendant.second.first;
            connected = leaf_and_pendant.second.second;
            // remove e from f_eps
            f_eps.remove_edge(leaf);
            // std::cout << "\nx_syndrome: ";
            int x_syndrome_index;
            if (x_syndrome[pendant]){
                // 4. (R1)If u∈σ, add e to A,remove u from σ and flip v in σ.
                A.add_edge(leaf);
                x_syndrome[pendant] = false;
                x_syndrome[connected] = !x_syndrome[connected];
            } else {
                // 5. (R2)If u/∈σ do nothing. 
                // std::cout << "\nu is not in sigma";
            }
        }
    }
    // Return P = Prod_{e∈A} Ze.
    std::vector<bool> uf_z(num_qubits); /* Z correction*/
    if (!A.edges.empty()){
        for (int edge: A.edges){
        uf_z[edge] = true;
        }
    } else {
        // No correction
        // std::cout << "\n No correction!";
    }
    return uf_z;
}

