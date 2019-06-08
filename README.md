# Solving TSP using a genetic algorithm
Implementation of genetic algorithm to solve TSP problem in C++

## Getting Started

These instructions will able you to compile and execute this program. 

### Prerequisites

You must use C++11 or higher version. Once the project is compiled, you can start it indicating the next parameters

```
<NAME_EXE>.exe <NAME>.tsp <sizePob> <Pcross> <Pmutation> <Iterations> <NAME>.opt.tour
```

Where the last parameter (<NAME>.opt.tour) is optional.

### Input

The input for this program will be a text document provided by TSPLIB (https://wwwproxy.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/). The file .opt will be mandatory and its optimal solution (opt.tour) can be optional.

### Output

Our program prints in console the best path of cities and the distance between them. Time execution for each instance of TSP will be too provided.

## Paper document

A paper for this project has been writed. It describes the algorithm, features, methodology and includes results and discussion. If you want to know more about it, you can request the paper by sending an email to contact.juanrodriguez@gmail.com