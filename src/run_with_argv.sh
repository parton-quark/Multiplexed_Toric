#!/bin/bash
#SBATCH -p compute
#SBATCH --job-name=${SLURM_ARRAY_TASK_ID}_%j
#SBATCH -t 20:00:00
#SBATCH --mem-per-cpu=15G
#SBATCH --ntasks=1
#SBATCH --array=1-8
#SBATCH --output=output_QM_%j.txt 
paramfile=/flash/NemotoU/shin/Multiplexed_Toric/src/params.txt
lattice_size="`sed -n ${taskno}p $paramfile | awk '{print $1}'`"
strategy="`sed -n ${taskno}p $paramfile | awk '{print $2}'`"
multiplexing="`sed -n ${taskno}p $paramfile | awk '{print $3}'`"
num_shots="`sed -n ${taskno}p $paramfile | awk '{print $4}'`"
force="`sed -n ${taskno}p $paramfile | awk '{print $5}'`"
error_type="`sed -n ${taskno}p $paramfile | awk '{print $6}'`"
p_burst="`sed -n ${taskno}p $paramfile | awk '{print $7}'`"
g++ -O2 -lm -std=c++20 *.cpp -o main && "/flash/NemotoU/shin/Multiplexed_Toric/src/"main "$lattice_size" "$strategy" "$multiplexing" "$num_shots" "$force" "$error_type" "$burst"
sleep 20