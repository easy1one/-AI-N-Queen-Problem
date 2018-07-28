# [AI] N-Queen-Problem
Lizard arrangement problem like upgrade N-queen problem by using 3 different search algorithms; Breadth-first search (BFS), Depth-first search (DFS), Simulated annealing(SA)

# Highlevel Description
For each baby lizard, you can place them in one spot on a grid. From there, they can shoot out their tongue up, down, left, right and diagonally as well. Their tongues are very long and can reach to the edge of the nursery from any location.

# Code :
## 1. input.txt
- "input.txt" file will be formatted as follws:
> __First line:__ instruction of which algorithm to use: BFS, DFS or SA<br />
> __Second line:__ strictly positive 32-bit integer n, the width and height of the square nursery<br />
> __Third line:__ strictly positive 32-bit integer p, the number of baby lizards<br />
> __Next n lines:__ the n x n nursery, one file line per nursery row (to show you where the trees are). It will have a 0 where there is nothing, and a 2 where there is a tree.<br />
- __For example,__<br />
```
DFS
8
8
00000000
00000000
00000000
00002000
00000000
00000200
00000000
00000000
```
## 2. lizard.cpp
- "lizard.cpp" will take an input file that has an arrangement of trees and will output a possible arrangement of lizards such that no baby lizard can eat another one. My program will find the solution for lizard position using 3 different algorithms
  * Breadth-first search (BFS) 
  * Depth-first search (DFS)
  * Simulated annealing (SA)

## 3. output.txt
- "output.txt" will be formatted as follows:<br />
> __First line:__ OK or FAIL, indicating whether a solution was found or not. If FAIL, any following lines are ignored.<br />
> __Next n lines:__ the n x n nursery, one line in the file per nursery row, including the baby lizards and trees. It will have a 0 where there is nothing, a 1 where you placed a baby lizard, and a 2 where there is a tree.<br />
- __For example,__<br />
```
OK
00000100
10000000
00001000
01002001
00000000
00100200
00000010
00010000
```
