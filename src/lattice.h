std::vector<int> vertex_to_coordinate(int l2, int v);
int coordinate_to_vertex(int l2, int x, int y);
std::vector<std::vector<int>> edge_to_coordinate(int l2, int v);
int coordinates_to_edge(int l2, int x1, int y1, int x2, int y2);
std::vector<std::vector<int>> face_to_coordinates(int l2,int v);
int coordinates_to_face(int l2, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);