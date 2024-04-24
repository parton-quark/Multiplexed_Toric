# lattice_size, strategy, multiplexing, num_shots, force, error_type, p_burst
lattice_size = [12]
strategy = [4,5]
multiplexing = [2,4]
num_shots = [100000]
force = [0,1,2,4,8,16,32]
p_burst = [0.1, 0.2, 0.4, 0.8, 1.0]

texts = ""
file = open('parameters.txt', 'w')
for l in lattice_size:
    for s in strategy:
        for m in multiplexing:
            for shots in num_shots:
                for f in force:
                    for p in p_burst:
                        text = str(l) + " " + str(s) + " " + str(m) + " " + str(shots) + " " + str(f) + " " + str(p) + '\n'
                        texts = texts + text

texts = texts.rstrip('\r\n')
print(texts)
file.write(texts)
file.close()