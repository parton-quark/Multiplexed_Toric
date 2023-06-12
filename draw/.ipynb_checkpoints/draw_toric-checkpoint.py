def make_lattice(l1,l2):
    vertices = []
    faces = []
    # vertex
    for i in range(l1*l2):
        x = i%l1
        y = i//l1
        vertices.append([x,y])
    # face
    for i in range(l1):
        for j in range(l2):
            ld = [i,j]
            lu = [i,j+1]
            rd = [i+1,j]
            ru = [i+1,j+1]
            faces.append([ld,lu,rd,ru])
            
    return vertices, faces

def vertex_to_coordinate(l2, v):
    x = v % l2
    y = v // l2
    coordinate = [x,y]
    return coordinate

def edge_to_vertices(l1, l2, e):
    if e%2 == 0:
        # // vertical edge
        u = e // 2
        if (e >= 2 * (l1 - 1) * l2):# 上端
            v = (e % (2 * l2)) // 2
        else:
            v = u + l2
    else:
        # // horizontal edge
        u = (e - 1) // 2;
        if (e % (2 * l2) == (2 * l2) - 1):# 右端
            v = (e // (2 * l2)) * l2
        else:
            v = u + 1
    vertices = [u,v]
    return vertices

def edge_to_coordinate(l1, l2, e):
    uv = edge_to_vertices(l1, l2, e)
    u = vertex_to_coordinate(l2, uv[0])
    v = vertex_to_coordinate(l2, uv[1])
    # right most
    if u[0] > v[0]:
        v[0] = u[0] + 1
    # top most
    if u[1] > v[1]:
        v[1] = u[1] + 1
    coordinates = [u,v]
    return coordinates

def face_to_coordinate(l2, v):
    v_0_x = v % l2
    v_0_y = v // l2
    v_1_x = v % l2 + 1
    v_1_y = v // l2
    v_2_x = v % l2
    v_2_y = v // l2 + 1
    v_3_x = v % l2 + 1
    v_3_y = v // l2 + 1
    coordinates = [[v_0_x,v_0_y],[v_1_x,v_1_y],[v_2_x,v_2_y],[v_3_x,v_3_y]]
    return coordinates