import sys
import json
from matplotlib import pyplot
import matplotlib.pyplot as plt
import matplotlib.patches as pat
from matplotlib.collections import LineCollection

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



print("draw_toric.py")
args = sys.argv
l1 = int(args[1])
l2 = int(args[2])
jsonfilename = args[3]
# jsonfilename = "result.json"
json_open = open(jsonfilename, 'r')
json_load = json.load(json_open)


code_lattice = make_lattice(l1,l2)

vertices = code_lattice[0]
faces = code_lattice[1]

erasure_vector_for_qubits_vec  = json_load['erasure vector for qubits']
erasure_vector_for_qubits = []
er_qb_index = 0
for er_qb in erasure_vector_for_qubits_vec :
    if er_qb == True:
        erasure_vector_for_qubits.append(er_qb_index)
    er_qb_index = er_qb_index + 1

z_errors_on_qubits_vec  = json_load['Z errors']
z_errors_on_qubits = []
z_qb_index = 0
for er_qb in z_errors_on_qubits_vec :
    if er_qb == True:
        z_errors_on_qubits.append(z_qb_index)
    z_qb_index  = z_qb_index  + 1

x_syndromes_vec = json_load['X syndromes']
x_syndromes = []
x_synd_index  = 0
for x_synd in x_syndromes_vec:
    if x_synd == True:
        x_syndromes.append(x_synd_index)
    x_synd_index = x_synd_index + 1

z_correction_vec = json_load['Z correction']
z_correction = []
z_correction_index  = 0
for z_cor in z_correction_vec:
    if z_cor == True:
        z_correction.append(z_correction_index)
    z_correction_index = z_correction_index + 1


x_syndromes_aft_vec = json_load['X syndromes after decoding']
x_syndromes_aft = []
x_synd_aft_index  = 0
for x_synd_aft in x_syndromes_aft_vec:
    if x_synd_aft == True:
        x_syndromes_aft.append(x_synd_aft_index)
    x_synd_aft_index= x_synd_aft_index + 1

z_errors_after_correction_vec = json_load['Z errors after correction']
z_errors_after_correction = []
z_errors_aft_index  = 0
for z_error_aft in z_errors_after_correction_vec:
    if z_error_aft  == True:
        z_errors_after_correction .append(z_errors_aft_index)
    z_errors_aft_index = z_errors_aft_index + 1

z_errors_matching = json_load['matching qubits']

X_1_Logical_operator = json_load['X_1 Logical operator']
X_2_Logical_operator = json_load['X_2 Logical operator']

# draw codes
fig, ax = plt.subplots(figsize=(l1, l2))

# vertices, Z stab
for i in vertices:
    plt.plot(i[0], i[1],'.',markersize=10, color = "gray")

# faces
for i in faces:
    p = pat.Polygon(xy = [i[0], i[2], i[3], i[1]], ec = "gray", color = "lightgray")
    ax.add_patch(p)
    
# edges, qubit
lines = []
for e in erasure_vector_for_qubits:
    cdn = edge_to_coordinate(l1, l2, e)
    lines.append(cdn)
lc = LineCollection(lines, linewidth = 10,color = "gray", label="Erasure error")
ax.add_collection(lc)

zlines = []
for z in z_errors_on_qubits:
    cdn = edge_to_coordinate(l1, l2, z)
    zlines.append(cdn)
lc = LineCollection(zlines, linewidth = 6,color = "blue", label="Z error")
ax.add_collection(lc)

# xlines = []
# for z in x_errors_on_qubits:
#     cdn = edge_to_coordinate(l1, l2, z)
#     xlines.append(cdn)
# lc = LineCollection(xlines, linewidth = 2,color = "red", label="X error")
# ax.add_collection(lc)

# X stab
first_stab = True
for x_synd in x_syndromes:
    x_xy = vertex_to_coordinate(l2, x_synd)
    if first_stab:
        plt.plot(x_xy[0], x_xy[1],'.',markersize=25, color = "red", label="X stabilizer")
        first_stab = False
    else:
        plt.plot(x_xy[0], x_xy[1],'.',markersize=25, color = "red")


first_time_to_show = True

plt.legend(loc='upper left')
plt.xlim(-1,l2 + 2)
plt.ylim(-1,l1 + 3)
[ax.spines[side].set_visible(False) for side in ['right','top']]
plt.savefig(str(l1) + "_" + str(l2) + "_toric.pdf")
fig, ax = plt.subplots(figsize=(l1, l2))

############################################################################
# vertices, Z stab
for i in vertices:
    plt.plot(i[0], i[1],'.',markersize=10, color = "gray")

# faces
for i in faces:
    p = pat.Polygon(xy = [i[0], i[2], i[3], i[1]], ec = "gray", color = "lightgray")
    ax.add_patch(p)
    
# edges, qubit
lines = []
for e in erasure_vector_for_qubits:
    cdn = edge_to_coordinate(l1, l2, e)
    lines.append(cdn)
lc = LineCollection(lines, linewidth = 10,color = "gray", label="Erasure error")
ax.add_collection(lc)

# zlines = []
# for z in z_errors_on_qubits:
#     cdn = edge_to_coordinate(l1, l2, z)
#     zlines.append(cdn)
# lc = LineCollection(zlines, linewidth = 6,color = "blue", label="Z error")
# ax.add_collection(lc)

# X stab
first_stab = True
for x_synd_aft in x_syndromes_aft:
    x_xy = vertex_to_coordinate(l2, x_synd_aft)
    if first_stab:
        plt.plot(x_xy[0], x_xy[1],'.',markersize=25, color = "lightcoral", label="X stabilizer after decoding")
        first_stab = False
    else:
        plt.plot(x_xy[0], x_xy[1],'.',markersize=25, color = "lightcoral") # lightcoral

# Z correction
zlines = []
for z in z_correction:
    cdn = edge_to_coordinate(l1, l2, z)
    zlines.append(cdn)
lc = LineCollection(zlines, linewidth = 6,color = "yellow", label="Z correction")
ax.add_collection(lc)

# Z error after corretion
zlines = []
for z in z_errors_after_correction:
    cdn = edge_to_coordinate(l1, l2, z)
    zlines.append(cdn)
lc = LineCollection(zlines, linewidth = 4,color = "blue", label="Z error after correction")
ax.add_collection(lc)

print(X_1_Logical_operator)
print(X_2_Logical_operator)

# logicalxlines = []
# for qubit in X_1_Logical_operator:
#     cdn = edge_to_coordinate(l1, l2, qubit)
#     logicalxlines.append(cdn)
# lc = LineCollection(logicalxlines, linewidth = 4,color = "turquoise", label="X_1 Logical Operator")
# ax.add_collection(lc)

# logicalxlines = []
# for qubit in X_2_Logical_operator:
#     cdn = edge_to_coordinate(l1, l2, qubit)
#     logicalxlines.append(cdn)
# lc = LineCollection(logicalxlines, linewidth = 4,color = "orange", label="X_2 Logical Operator")
# ax.add_collection(lc)

# matching
# zlines = []
# for z in z_errors_matching:
#     cdn = edge_to_coordinate(l1, l2, z)
#     zlines.append(cdn)
# lc = LineCollection(zlines, linewidth = 4,color = "purple", label="Matching")
# ax.add_collection(lc)

    
first_time_to_show = True

plt.legend(loc='upper left')
plt.xlim(-1,l2 + 2)
plt.ylim(-1,l1 + 3)
[ax.spines[side].set_visible(False) for side in ['right','top']]
plt.savefig(str(l1) + "_" + str(l2) + "_toric_matching.pdf")
