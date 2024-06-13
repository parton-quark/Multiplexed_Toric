#!/bin/bash
   
#SBATCH -p compute
#SBATCH --job-name=QM_T%j
#SBATCH -t 10:00:00
#SBATCH --mem-per-cpu=10G
#SBATCH --ntasks=1
#SBATCH --array=1-20
#SBATCH --output=output_%j.txt  # Standard output and error log\

taskno=${SLURM_ARRAY_TASK_ID}
paramfile=/flash/NemotoU/shin/0611/Multiplexed_Toric/src/params.txt

lattice_size="`sed -n ${taskno}p $paramfile | awk '{print $1}'`"
strategy="`sed -n ${taskno}p $paramfile | awk '{print $2}'`"
multiplexing="`sed -n ${taskno}p $paramfile | awk '{print $3}'`"
num_shots="`sed -n ${taskno}p $paramfile | awk '{print $4}'`"
force="`sed -n ${taskno}p $paramfile | awk '{print $5}'`"
error_type="`sed -n ${taskno}p $paramfile | awk '{print $6}'`"
p_burst="`sed -n ${taskno}p $paramfile | awk '{print $7}'`"

g++ -O2 -lm -std=c++20 *.cpp -o main && "/flash/NemotoU/shin/0611/Multiplexed_Toric/src/"main "$lattice_size" "$strategy" "$multiplexing" "$num_shots" "$force" "$error_type" "$burst"

<< COMMENTOUT
lattice_size, strategy, multiplexing, num_shots, force, error_type, p_burst
#SBATCH --job-name=QM_T${SLURM_ARRAY_TASK_ID}
#SBATCH --output=output_QM_Toric.txt  # Standard output and error log\
COMMENTOUT
