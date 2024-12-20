# Multiplexed Toric Codes on Erasure Channel
This is a C++ implementation of multiplexed toric codes simulator.
  
## Simulation Flow
You can simulate the multiplexed quantum communication with surface code by `g++ -O2 -std=c++20 *.cpp -o main && "your_path/multiplexed_toric/src/"main;` and then you get `result.json`.

Simulation flows are:
- Multiplexing (assign qubits to photons)
- Erasure error on a photon -> erasure errrors on multiple qubits
- Replace erased qubits with mixed state -> erasure can be regarded as a random Pauli error.
- X stabilizer measurement
- Run peeling decoder
- Determines if any Z logical errors remain after decoding.
![flow](sim_flow.png)

The default error model is erasure with `main_with_loss_error();`. Combined error model (random Z + Erasure + random Z) is also available in `main_with_combined_error()`.

You can visualize the result of the simulation as .png file with python `python draw_toric.py LATTICE_SIZE_V LATTICE_SIZE_H "result.json";` with lattice sizes (`python draw_toric.py 10 10 "result.json";` works for the default case).

You will get something like this:
![vis_toric](ten_ten_toric.png)
python draw_toric.py LATTICE_SIZE_V LATTICE_SIZE_H "result.json";


## Requirements
- [nlohmann/json](https://github.com/nlohmann/json)
  - JSON library for C++


## Citation 
ArXiv submission is available at [HERE](https://arxiv.org/abs/2406.08832). For the citation of this work, please use this bibtex file.
```
@misc{nishio2024multiplexed,
      title={Multiplexed Quantum Communication with Surface and Hypergraph Product Codes}, 
      author={Shin Nishio and Nicholas Connolly and Nicolò Lo Piparo and William John Munro and Thomas Rowan Scruby and Kae Nemoto},
      year={2024},
      eprint={2406.08832},
      archivePrefix={arXiv},
      primaryClass={id='quant-ph' full_name='Quantum Physics' is_active=True alt_name=None in_archive='quant-ph' is_general=False description=None}
}
```