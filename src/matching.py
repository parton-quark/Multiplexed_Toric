import sys
import numpy as np
from scipy import sparse
from scipy.sparse import csc_matrix
import pymatching
import json

args = sys.argv
l1 = int(args[1])
print("l1:" + str(l1))
l2 = int(args[2])
print("l2:" + str(l2))
jsonfilename = args[3]
# jsonfilename = "result.json"
# json_open = open(jsonfilename, 'r')
# json_load = json.load(json_open)
with open(jsonfilename, 'r') as f:
    json_load = json.load(f)

x_pcm = json_load['X parity check matrix']
x_pcm_arr = np.array(x_pcm)
x_pcm_csc = sparse.csc_matrix(x_pcm_arr)
weights = np.ones(((l1*l2) * 2), dtype=int)
matching = pymatching.Matching(x_pcm_csc,  weights=weights)
x_synd_aft_crc = json_load['X syndromes after decoding']
x_synd_aft_crc_arr = np.empty(l1*l2, dtype = int)
for i in range(l1*l2):
    if x_synd_aft_crc[i] == True:
        x_synd_aft_crc_arr[i] = 1
    elif x_synd_aft_crc[i] == False:
        x_synd_aft_crc_arr[i] = 0

prediction = matching.decode(x_synd_aft_crc_arr)
matching_qubits = []
qubit_index = 0
for ele in prediction:
    if ele == 1:
        matching_qubits.append(qubit_index)
    qubit_index  = qubit_index + 1

json_load['matching qubits'] = matching_qubits

with open(jsonfilename, 'w') as f:
    json.dump(json_load, f)