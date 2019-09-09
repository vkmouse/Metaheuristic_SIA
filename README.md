## Metaheuristic algorithms for the service-to-interface assignment for IoT system management
This code implements three metaheuristic algorithms, simulated annealing (SA), genetic algorithm (GA), and search economics (SE) respectively, and four rule-based algorithms, hill climbing (HC), first come first served (FCFS), Rand-Init-Allocation (RIA), and Average-Cost-Allocation (ACA) to solve the service-to-interface assignment (SIA) problem for IoT system management.

- [Introduction](#Introduction)
- [Compile](#Compile)
- [Usage](#Usage)
  - [Hill climbing](#Hill-climbing)
  - [Simulated annealing](#Simulated-annealing)
  - [Genetic algorithm](#Genetic-algorithm)
  - [Search economics](#Search-economics)
  - [Rule based algorithm](#Rule-based-algorithm)

### Introduction
[Search economics (SE)](https://doi.org/10.1109/SMC.2015.447) is a novel metaheuristic algorithm proposed in 2015. The two main differences between SE and traditional metaheuristic are dividing the search space and investing computation resource depending on the potential of each subspace. The simulated results show that the [SESIA](https://doi.org/10.1109/JIOT.2018.2796310) is able to find a better solution than the HC, FCFS, [RIA](https://doi.org/10.1109/JIOT.2016.2535163), [ACA](https://doi.org/10.1109/JIOT.2016.2535163), SA, and GA compared in this paper.

### Compile
Install GCC 4.7 or newer version in the [link](https://sourceforge.net/projects/mingw-w64/files/). Execute the command ```make``` in the path of the Makefile to compile the code.

    make

### Usage
Four parameters are required for all algorithms to execute the program. Each algorithm requires specific parameters for itself.
1. Dataset path
2. Number of evaluations
3. Number of evaluations per output
4. Number of runs

#### Hill climbing
No other parameter is required to execute the HC. For example, dataset path is **dataset/DS1.txt**, number of evaluations is 200000, number of evaluations per output is 4000, and number of runs is 2.

    main.exe hc dataset/DS1.txt 200000 4000 2
    
#### Simulated annealing
Four parameters are required to execute the SA.
1. Number of neighbors
2. Initial temperature
3. Minimum temperature
4. Cooling rate

For example, number of neighbors is 7, initial temperature is 10, minimum temperature is 0.01, and cooling rate is 0.99.
 
    main.exe sa dataset/DS1.txt 200000 4000 2 7 10 0.01 0.99

#### Genetic algorithm
Three parameters are required to execute the GA.
1. Number of chromsomes
2. Crossover rate
3. Mutation rate

For example, number of chromsomes is 20, crossover rate is 0.6, and mutation rate is 0.1.
 
    main.exe ga dataset/DS1.txt 200000 4000 2 20 0.6 0.1

#### Search economics
Four parameters are required to execute the SE.
1. Number of searchers
2. Number of regions
3. Number of samples
4. Number of players

For example, number of searchers is 8, number of regions is 8, number of samples is 2, and number of players is 3.
 
    main.exe se dataset/DS1.txt 200000 4000 2 8 8 2 3

#### Rule based algorithm
First come first served (FCFS), Rand-Init-Allocation (RIA), and Average-Cost-Allocation (ACA) do not require the number of evaluations, the number of evaluations per output, and other parameters because they do not have any random factor to cause differences cost. The number of evaluations and the number of evaluations per output set greater than one, and the number of runs set to one.

Example for **Rand-Init-Allocation (RIA)**,

    main.exe RIA dataset/DS1.txt 10 10 1
    
Example for **Average-Cost-Allocation (ACA)**,

    main.exe ACA dataset/DS1.txt 10 10 1
    
Example for **First come first served (FCFS)**,

    main.exe FCFS dataset/DS1.txt 10 10 1
    