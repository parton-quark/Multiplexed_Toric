#!/bin/bash
paramfile=/Users/parton/OIST/Multiplexed_Toric/src/params_local.txt
for i in {1..2}
do
  echo "${i}"
  lattice_size="`sed -n ${i}p $paramfile | awk '{print $1}'`"
  strategy="`sed -n ${i}p $paramfile | awk '{print $2}'`"
  multiplexing="`sed -n ${i}p $paramfile | awk '{print $3}'`"
  num_shots="`sed -n ${i}p $paramfile | awk '{print $4}'`"
  force="`sed -n ${i}p $paramfile | awk '{print $5}'`"
  error_type="`sed -n ${i}p $paramfile | awk '{print $6}'`"
  p_burst="`sed -n ${i}p $paramfile | awk '{print $7}'`"
  g++ -O2 -std=c++20 *.cpp -o main && "/Users/parton/OIST/Multiplexed_Toric/src/"main "$lattice_size" "$strategy" "$multiplexing" "$num_shots" "$force" "$error_type" "$burst"
done

