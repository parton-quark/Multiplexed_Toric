#!/bin/bash
#SBATCH -p compute
#SBATCH --time=19:00:00
#SBATCH --mem=20G
#SBATCH --job-name=12_withoutQM    # Job name\
#SBATCH -c 1                    # Run all processes on a single node\
#SBATCH --ntasks=1                   # Run a single task\
#SBATCH --output=output_toric_12_0_1_100000.txt  # Standard output and error log\

g++ -O2 -lm -std=c++20 *.cpp -o main && "/flash/NemotoU/shin/multiplexed_toric/src/"main 12 0 1 100000