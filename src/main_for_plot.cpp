
// graph prim(graph G, int l1, int l2, std::vector<int> edge_weights){
//     std::cout << "\nprim start! \nG: " << std::flush;
//     G.print_graph();
//     graph left_graph(G.vertices, G.edges);
//     graph mm_st; /* minimal spanning tree */

//     // bool is_connected;
//     // is_connected = is_connected_graph(left_graph);
//     // std::cout << "\nis_connected: " << is_connected << std::flush;
//     int first_vertex;
//     first_vertex = left_graph.vertices[0];
//     mm_st.add_vertex(first_vertex);
//     left_graph.remove_vertex(first_vertex);
//     std::cout << "\nfirst_vertex: " << first_vertex << std::flush;


//     const int INF = 1e9;
//     std::vector<int> candidates_edge;

//     while (left_graph.num_vertices() > 0){
//         for (int le: left_graph.edges){
//             std::vector<int> v0v1;
//             int v0, v1;
//             v0v1 = edge_to_vertices(l1, l2, le);
//             v0 = v0v1[0];
//             v1 = v0v1[1];

//             // if one of v0 and v1 is in the tree and the other is not in the tree
//             if ((v_is_in_g(v0, mm_st) && !v_is_in_g(v1, mm_st)) || (!v_is_in_g(v0, mm_st) && v_is_in_g(v1, mm_st))){
//                 candidates_edge.push_back(le);
//             }
//         }
//         if (candidates_edge.size() == 0){
//             std::cout << "\nno candidates_edge!" << std::flush;
//             break;
//         }

//         // find the edge with the minimum weight in the candidates_edge
//         // make the vector of weights of the candidates_edge
//          std::vector<int> candidate_edge_weights;
//         for (int ce: candidates_edge){
//             for (int ce: candidates_edge){
//                 candidate_edge_weights.push_back(edge_weights[ce]);
//             }
//         }
//         // find the minimum weight in the candidates_edge
//         int min_weight;
//         for (int cew: candidate_edge_weights){
//             if (min_weight > cew){
//                 min_weight = cew;
//             }
//         }
//         // find the edges with the minimum weight in the candidates_edge
//         std::vector<int> min_weight_edges;
//         for (int ce: candidates_edge){
//             if (edge_weights[ce] == min_weight){
//                 min_weight_edges.push_back(ce);
//             }
//         }

//         if (min_weight_edges.size() == 0){
//             std::cout << "\nno min_weight_edges!" << std::flush;
//             break;
//         } else {
//             // choose one of the edges with the minimum weight randomly
//             int random_index;
//             // std::cout << "\nmin_weight_edges.size(): " << min_weight_edges.size() << std::flush;
//             random_index = rand()%min_weight_edges.size();
//             if (random_index > min_weight_edges.size()){
//                 std::cout << "\nrandom_index is " << random_index <<  ". It is out of range!" << std::flush;
//                 exit(1);
//             }
//             int winner_edge;
//             winner_edge = min_weight_edges[random_index];
//             // add the winner edge to the minimal spanning tree
//             mm_st.add_edge(winner_edge);
//             left_graph.remove_edge(winner_edge);
//             // add the vertex of the winner edge to the minimal spanning tree
//             std::vector<int> w0w1;
//             int w0, w1;
//             w0w1 = edge_to_vertices(l1, l2, winner_edge);
//             w0 = w0w1[0];
//             w1 = w0w1[1];
//             if (std::find(mm_st.vertices.begin(), mm_st.vertices.end(), w0) !=mm_st.vertices.end()){
//                 // w0 is in the tree
//                 // continue;
//             } else {
//                 mm_st.add_vertex(w0);
//                 left_graph.remove_vertex(w0);
//             }
//             if (std::find(mm_st.vertices.begin(), mm_st.vertices.end(), w1) !=mm_st.vertices.end()){
//                 // w1 is in the tree
//                 // continue;
//             } else {
//                 mm_st.add_vertex(w1);
//                 left_graph.remove_vertex(w1);
//             }
//         }
//     }
//     std::cout << "\nprim finish!" << std::flush;
//     mm_st.print_graph();
//     return mm_st;
// }