# [AI] N-Queen-Problem
Lizard arrangement problem like upgrade N-queen problem by using 3 different search algorithms; Breadth-first search (BFS), Depth-first search (DFS), Simulated annealing(SA)

# Highlevel Description
For each baby lizard, you can place them in one spot on a grid. From there, they can shoot out their tongue up, down, left, right and diagonally as well. Their tongues are very long and can reach to the edge of the nursery from any location.

# Code
- "lizard.cpp" reads a text file called "input.txt" that contains a problem definition
- "input.txt" file will be formatted as follws:
> First line: instruction of which algorithm to use: BFS, DFS or SA<br />
> Second line: strictly positive 32-bit integer n, the width and height of the square nursery<br />
> Third line: strictly positive 32-bit integer p, the number of baby lizards<br />
> Next n lines: the n x n nursery, one file line per nursery row (to show you where the trees are). It will have a 0 where there is nothing, and a 2 where there is a tree.<br />
- __For example,__<br />
```
DFS<br />
8<br />
8 <br />
00000000<br />
00000000<br />
00000000<br />
00002000<br />
00000000<br />
00000200<br />
00000000<br />
00000000<br />
```
<br />
- Then, it would write a file "output.txt" as a possible lizard arrangements
- "output.txt" will be formatted as follows:<br />
> First line: OK or FAIL, indicating whether a solution was found or not. If FAIL, any following lines are ignored.<br />
> Next n lines: the n x n nursery, one line in the file per nursery row, including the baby lizards and trees. It will have a 0 where there is nothing, a 1 where you placed a baby lizard, and a 2 where there is a tree.<br />
- __For example,__<br />
```
OK<br />
00000100<br />
10000000<br />
00001000<br />
01002001<br />
00000000<br />
00100200<br />
00000010<br />
00010000<br />
```
