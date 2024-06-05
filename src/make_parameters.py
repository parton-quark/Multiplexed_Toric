# lattice_size, strategy, multiplexing, num_shots, force, error_type, p_burst
lattice_size = [8,10,12,14,16,18,20]
strategy = [1]
multiplexing = [4]
num_shots = [100000]
force = [0]
error_model = ["uniform"]
p_burst = [0.1]

texts = ""
file = open('parameters.txt', 'w')
for l in lattice_size:
    for s in strategy:
        for m in multiplexing:
            for shots in num_shots:
                for f in force:
                    for e in error_model:
                        for p in p_burst:
                            text = str(l) + " " + str(s) + " " + str(m) + " " + str(shots) + " " + str(f) + " " + str(e) + " " + str(p) + '\n'
                            texts = texts + text

texts = texts.rstrip('\r\n')
print(texts)
file.write(texts)
file.close()

# lattice_size, strategy, multiplexing, num_shots, force, error_model, p_burst