#!/bin/bash
#SBATCH -p compute
#SBATCH --time=19:00:00
#SBATCH --mem=20G
#SBATCH --job-name=QM_Toric_10_7_4_100      # Job name\
#SBATCH -c 1                    # Run all processes on a single node\
#SBATCH --ntasks=1                   # Run a single task\
#SBATCH --output=toric_10_7_4_100_output.txt  # Standard output and error log\

gcc -xc++ -lstdc++ -shared-libgcc -O2 -std=c++20 *.cpp -o main && "/flash/NemotoU/shin/multiplexed_toric/src/"main 10 7 4 100