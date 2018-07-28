#include <cstdio>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include <queue>
#include <stack>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <random>
#include <map>
#include <time.h>
#include <chrono>

using namespace std;

struct myclass {
	bool operator() (int i, int j) { return (i<j); }
} myobject;

class Node {
public:
	int row, col, n, lizard;
	Node* parent;
	int sub_index;

	Node(int r, int c, int max, int put)
		:row(r), col(c), n(max), lizard(put)
	{
		parent = NULL;
	}
};

bool isItOnTree(int cur_row, int cur_col, vector<int> tree_pos)
{
	for (int i = 0; i < tree_pos.size(); i += 2)
	{
		if (cur_row == tree_pos[i] && cur_col == -1)
		{// chcek only row
			return true;
		}
		if (cur_row == tree_pos[i] && cur_col == tree_pos[i + 1])
		{
			return true;
		}
	}
	return false;
}

bool checkChild(Node* cur, int c, int n, vector<int> tree_pos, int numOfTree)
{
	int tree_index = 0;
	int dia = 1, t_dia;
	bool left = true, straight = true, right = true;
	bool left_down = true, straight_down = true, right_down = true;
	int child_row = cur->row + 1;
	int t_r, t_c;
	int p_r, p_c;

	// 1. NO LIZARD node
	if (c == n)
	{
		return true;
	}

	// 2. it is ON the tree
	if (isItOnTree(child_row, c, tree_pos))
	{
		return false;
	}

	// 3. follow up its parents
	Node* climb = cur;
	while (climb != NULL && climb->row >= 0)
	{
		p_r = climb->row, p_c = climb->col;
		dia = child_row - p_r;

		if (p_c == n)
		{
			climb = climb->parent;
			continue;
		}

		if (p_c - dia == c)
		{
			left = false;

			if (numOfTree != 0)
			{
				for (int i = 0; i < tree_pos.size(); i += 2)
				{
					t_r = tree_pos[i]; // row
					t_c = tree_pos[i + 1]; // col

					if (p_r < t_r && t_r < child_row)
					{
						t_dia = t_r - p_r;
						if (t_c == p_c - t_dia && left_down)
						{
							left = true;
						}
					}
				}// tree travelser
			}//if
			left_down = false; // to fix it
		}
		else if (p_c == c)
		{
			straight = false;

			if (numOfTree != 0)
			{
				for (int i = 0; i < tree_pos.size(); i += 2)
				{
					t_r = tree_pos[i]; // row
					t_c = tree_pos[i + 1]; // col

					if (p_r < t_r && t_r < child_row)
					{
						if (t_c == p_c && straight_down)
						{
							straight = true;
						}
					}
				}// tree travelser
			}// if
			straight_down = false; 
		}
		else if (p_c + dia == c)
		{
			right = false;

			if (numOfTree != 0)
			{
				for (int i = 0; i < tree_pos.size(); i += 2)
				{
					t_r = tree_pos[i]; // row
					t_c = tree_pos[i + 1]; // col

					if (p_r < t_r && t_r < child_row)
					{
						t_dia = t_r - p_r;
						if (t_c == p_c + t_dia && right_down)
						{
							right = true;
						}
					}
				}// tree travelser
			}//if
			right_down = false; // to fix it

		}

		climb = climb->parent;
	}

	if (left && straight && right)
	{
		return true;
	}
	else
	{
		return false;
	}
}

vector<int> checkResult(Node* cur, int p, int n)
{// count lizard & print result board
	vector<int> path;
	int lizard = 0;

	while (cur->parent != NULL)
	{
		if (cur->col != n)
		{
			lizard++;
			path.push_back(cur->row); // even: row
			path.push_back(cur->col); // odd: col
		}
		cur = cur->parent;
	}

	if (lizard != p)
	{// fail: send empty path
		path.clear();
	}
	return path;
}

vector<int> BFS(queue<Node*> myque, int n, int p, vector<int>tree_pos, int numOfTree)
{
	Node* temp;
	vector<int> result;
	vector<Node*>::iterator it;
	int end_col;
	queue<Node*> subque;
	Node *cur;
	int cur_row, cur_col;
	int child_row;
	int start, end, end_index;
	bool first = false;

	/* 1. check whether the node is empty*/
	while (!myque.empty())
	{
		/* 2. Get a node from the queue */
		cur = myque.front();
		cur_row = cur->row;
		cur_col = cur->col;
		child_row = cur_row + 1;
		myque.pop();

		/* 3. Check the node postion*/

		/* 4. Check the goal with lizard numbers */
		if (cur->lizard == p)
		{
			result = checkResult(cur, p, n);
			if (result.size() == 0)
			{// this path is fail
				;
				// keep going
				// go to end of the queue
			}
			else
			{// sucess: Print the result board
				cout << "OK" << endl;
				return result;
			}
		}

		/* 5. leaf node: update& make a child to queue */
		if (cur->row == n - 1)
		{
			continue;
		}

		/* 6-1. There is no tree on the board: push current node's child to the queue */
		if (numOfTree == 0)
		{
			for (int c = 0; c < n; c++)
			{
				if (checkChild(cur, c, n, tree_pos, numOfTree))
				{
					// Create Children
					Node *child = new Node(cur_row + 1, c, n, cur->lizard + 1);
					child->parent = cur;
					myque.push(child);

					/* Check the goal with lizard numbers */
					if (child->lizard == p)
					{
						result = checkResult(child, p, n);
						if (result.size() == 0)
						{// this path is fail
							;
							// keep going
							// go to end of the queue
						}
						else
						{// sucess: Print the result board
							cout << "OK" << endl;
							return result;
						}
					}
				}
			}
		}
		/* 6-2. There is tree on the board */
		else
		{
			/* 7-1. cur's child row doesn't have a tree */
			if (!isItOnTree(child_row, -1, tree_pos))
			{
				for (int c = 0; c < n + 1; c++)
				{
					if (c == n)
					{// create nothing node
						Node *child = new Node(cur_row + 1, c, n, cur->lizard);
						child->parent = cur;
						myque.push(child);
					}
					else if (checkChild(cur, c, n, tree_pos, numOfTree))
					{
						// Create Children
						Node *child = new Node(cur_row + 1, c, n, cur->lizard + 1);
						child->parent = cur;
						myque.push(child);

						/* Check the goal with lizard numbers */
						if (child->lizard == p)
						{
							result = checkResult(child, p, n);
							if (result.size() == 0)
							{// this path is fail
								;
								// keep going
								// go to end of the queue
							}
							else
							{// sucess: Print the result board
								cout << "OK" << endl;
								return result;
							}
						}

					}
				}
			}
			/* 7-2.  cur's child row has a tree */
			else
			{
				start = 0;
				first = true;

				// 7-2-1.Find end value
				for (int i = 0; i < tree_pos.size(); i += 2) // col 
				{
					if (tree_pos[i] == child_row && first)
					{
						first = false;
						end_index = i + 1;
						end = tree_pos[end_index]; // first tree col on this row
					}
				}

				// 7-2-2 while loop on the row
				int sub_stamp = 0;
				while (start <= n)
				{
					// (1)
					if (subque.empty())
					{
						temp = cur;
						temp->sub_index = 1;
					}
					else
					{
						temp = subque.front();
						subque.pop();
					}

					// (2)
					for (int c = start; c <= end; c++)
					{
						if (c == end)
						{// create nothing node
							Node *child = new Node(cur_row + 1, n, n, temp->lizard);
							child->parent = temp;
							child->sub_index = temp->sub_index + 1;
							subque.push(child);
						}
						else if (checkChild(cur, c, n, tree_pos, numOfTree))
						{
							// Create Children
							Node *child = new Node(cur_row + 1, c, n, temp->lizard + 1);
							child->parent = temp;
							child->sub_index = temp->sub_index + 1;
							subque.push(child);

							/* Check the goal with lizard numbers */
							if (child->lizard == p)
							{
								result = checkResult(child, p, n);
								if (result.size() == 0)
								{// this path is fail
									;
									// keep going
									// go to end of the queue
								}
								else
								{// sucess: Print the result board
									cout << "OK" << endl;
									return result;
								}
							}

						}
					}

					// (3) update index

					if (subque.front()->sub_index != temp->sub_index) // Let's update
					{
						start = end + 1;
						if ((end_index + 1) < tree_pos.size())
						{
							if (tree_pos[end_index + 1] == child_row)
							{// Same Row
								end_index += 2;
								end = tree_pos[end_index];
							}
							else
							{
								end = n;
							}
						}
						else
						{
							end = n;
						}
					}

				}// while(start <= n)

				 //7-2-3. move nodes to myque
				while (!subque.empty())
				{
					temp = subque.front();
					subque.pop();
					myque.push(temp);
				}

			}
		}

	}

	// FAIL: NOTHING TO SEARCH IN THE QUEUE
	cout << "End of queue" << endl;
	result.clear();
	return result;
}

vector<int> BFS(Node *root, int n, int p, vector<int> tree_pos, int numOfTree)
{
	queue<Node*> myque;
	vector<Node*>::iterator it;

	if (!root)
	{
		vector<int> empty;
		return empty;
	}
	else
	{
		myque.push(root);
	}

	return BFS(myque, n, p, tree_pos, numOfTree);
}

vector<int> DFS(stack<Node*> mystack, int n, int p, vector<int>tree_pos, int numOfTree)
{
	Node* temp;
	vector<int> result;
	vector<Node*>::iterator it;
	int end_col;
	stack<Node*> substack;
	queue<Node*> subque;
	Node *cur;
	int cur_row, cur_col;
	int child_row;
	int start, end, end_index;
	bool first = false;

	/* 1. check whether the node is empty*/
	while (!mystack.empty())
	{
		/* 2. Get a node from the queue */
		cur = mystack.top();
		cur_row = cur->row;
		cur_col = cur->col;
		child_row = cur_row + 1;
		mystack.pop();

		/* 4. Check the goal with lizard numbers */
		if (cur->lizard == p)
		{
			result = checkResult(cur, p, n);
			if (result.size() == 0)
			{// this path is fail
				;
				// keep going
				// go to end of the queue
			}
			else
			{// sucess: Print the result board
				cout << "OK" << endl;
				return result;
			}
		}

		/* 5. leaf node: update& make a child to queue */
		if (cur->row == n - 1)
		{
			continue;
		}

		/* 6-1. There is no tree on the board: push current node's child to the queue */
		if (numOfTree == 0)
		{
			for (int c = 0; c < n; c++)
			{
				if (checkChild(cur, c, n, tree_pos, numOfTree))
				{
					// Create Children
					Node *child = new Node(cur_row + 1, c, n, cur->lizard + 1);
					child->parent = cur;
					mystack.push(child);

					/* Check the goal with lizard numbers */
					if (child->lizard == p)
					{
						result = checkResult(child, p, n);
						if (result.size() == 0)
						{// this path is fail
							;
							// keep going
							// go to end of the queue
						}
						else
						{// sucess: Print the result board
							cout << "OK" << endl;
							return result;
						}
					}
				}
			}
		}
		/* 6-2. There is tree on the board */
		else
		{
			/* 7-1. cur's child row doesn't have a tree */
			if (!isItOnTree(child_row, -1, tree_pos))
			{
				for (int c = 0; c < n + 1; c++)
				{
					if (c == n)
					{// create nothing node
						Node *child = new Node(cur_row + 1, c, n, cur->lizard);
						child->parent = cur;
						mystack.push(child);
					}
					else if (checkChild(cur, c, n, tree_pos, numOfTree))
					{
						// Create Children
						Node *child = new Node(cur_row + 1, c, n, cur->lizard + 1);
						child->parent = cur;
						mystack.push(child);

						/* Check the goal with lizard numbers */
						if (child->lizard == p)
						{
							result = checkResult(child, p, n);
							if (result.size() == 0)
							{// this path is fail
								;
								// keep going
								// go to end of the queue
							}
							else
							{// sucess: Print the result board
								cout << "OK" << endl;
								return result;
							}
						}

					}
				}
			}
			/* 7-2.  cur's child row has a tree */
			else
			{
				start = 0;
				first = true;

				// 7-2-1.Find end value
				for (int i = 0; i < tree_pos.size(); i += 2) // col 
				{
					if (tree_pos[i] == child_row && first)
					{
						first = false;
						end_index = i + 1;
						end = tree_pos[end_index]; // first tree col on this row
					}
				}
				// 7-2-2 while loop on the row
				int sub_stamp = 0;
				while (start <= n)
				{
					// (1)
					if (subque.empty())
					{
						temp = cur;
						temp->sub_index = 1;
					}
					else
					{
						temp = subque.front();
						subque.pop();
					}

					// (2)
					for (int c = start; c <= end; c++)
					{
						if (c == end)
						{// create nothing node
							Node *child = new Node(cur_row + 1, n, n, temp->lizard);
							child->parent = temp;
							child->sub_index = temp->sub_index + 1;
							subque.push(child);
						}
						else if (checkChild(cur, c, n, tree_pos, numOfTree))
						{
							// Create Children
							Node *child = new Node(cur_row + 1, c, n, temp->lizard + 1);
							child->parent = temp;
							child->sub_index = temp->sub_index + 1;
							subque.push(child);

							/* Check the goal with lizard numbers */
							if (child->lizard == p)
							{
								result = checkResult(child, p, n);
								if (result.size() == 0)
								{// this path is fail
									;
									// keep going
									// go to end of the queue
								}
								else
								{// sucess: Print the result board
									cout << "OK" << endl;
									return result;
								}
							}

						}
					}

					// (3) update index
					if (subque.front()->sub_index != temp->sub_index) // Let's update
					{
						start = end + 1;
						if ((end_index + 1) < tree_pos.size())
						{
							if (tree_pos[end_index + 1] == child_row)
							{// Same Row
								end_index += 2;
								end = tree_pos[end_index];
							}
							else
							{
								end = n;
							}
						}
						else
						{
							end = n;
						}
					}

				}// while(start <= n)

				 //7-2-3. move nodes to myque
				while (!subque.empty())
				{
					temp = subque.front();
					subque.pop();
					mystack.push(temp);
				}

			}
		}

	}

	// FAIL: NOTHING TO SEARCH IN THE QUEUE
	cout << "End of queue" << endl;
	result.clear();
	return result;
}

vector<int> DFS(Node *root, int n, int p, vector<int> tree_pos, int numOfTree)
{
	stack<Node*> mystack;
	vector<Node*>::iterator it;

	if (!root)
	{
		vector<int> empty;
		return empty;
	}
	else
	{
		mystack.push(root);
	}

	return DFS(mystack, n, p, tree_pos, numOfTree);
}

map<int, vector<int> > updateLizard(vector<vector<int> > board, int n, int p, map<int, vector<int> > myLizard, map<int, vector<int> > myTrees, bool treeFree, bool* treeFree_row)
{
	int direction;
	int lizard_row, lizard_col, lizard_col_index; // exist positions
	int new_lizard_row, new_lizard_col; // new 2 index needed
	bool complete = false;
	vector<int> chosen_row;
	// uniformed random generator
	random_device rand_dev;
	mt19937 gen(rand_dev());
	uniform_int_distribution<int> i_dir(1, 4); // 1:L, 2:R, 3:U, 4:D
	uniform_int_distribution<int> t_dir(1, 2); // 1:L, 2:R
	uniform_int_distribution<int> r_liz(0, n - 1); // row: 0 ~ n-1
	int movement = (n - 1);
	uniform_int_distribution<int> m_dis(1, movement); //추가
	int *treeFree_lock = new int[n];
	for (int i = 0; i < n; i++)
	{
		treeFree_lock[i] = 0; // max 2
	}

	while (!complete)
	{
		if (treeFree)
		{
			direction = t_dir(gen);
		}
		else
		{
			direction = i_dir(gen);
		}

		// 1. pick a lizard, direction
		complete = true;
		// choose a row
		while (1)
		{
			lizard_row = r_liz(gen);
			if (myLizard.count(lizard_row)) // choose exist row
			{
				if (myLizard[lizard_row].size() != 0)
				{
					chosen_row = myLizard[lizard_row];
					break;
				}
			}
		}
		// choose a col
		uniform_int_distribution<int> c_liz(0, chosen_row.size() - 1); // Nth index in the row (NOT col value)
		lizard_col_index = c_liz(gen);
		lizard_col = myLizard[lizard_row][lizard_col_index];

		// 2. Get a new poistion
		int move = m_dis(gen);
		new_lizard_row = lizard_row;
		new_lizard_col = lizard_col;

		switch (direction)
		{
		case 1:
			// Left
			if (new_lizard_col - move < 0)
			{
				new_lizard_col = n - move;
			}
			else
			{
				new_lizard_col -= move;
			}

			break;
		case 2:
			// Right
			new_lizard_row = lizard_row;

			if (n - 1 < new_lizard_col + move)
			{
				new_lizard_col = new_lizard_col + move - n;
			}
			else
			{
				new_lizard_col += move;
			}
			break;
		case 3:
			// Up
			if (new_lizard_row - move < 0)
			{
				new_lizard_row = new_lizard_row - move + n;
			}
			else
			{
				new_lizard_row -= move;
			}
			break;
		case 4:
			// Down
			new_lizard_col = myLizard[lizard_row][lizard_col_index];

			if (n - 1 < new_lizard_row + move)
			{
				new_lizard_row = new_lizard_row + move - n;

			}
			else
			{
				new_lizard_row += move;
			}
			break;
		}

		// 2. check there is conflict with trees 
		if (myTrees.count(new_lizard_row))
		{// there is tree on the row
			for (int i = 0; i < myTrees[new_lizard_row].size(); i++)
			{
				if (myTrees[new_lizard_row][i] == new_lizard_col)
				{// exactely on the tree
				 // pick other random positions
					complete = false;
					break;
				}
			}
		}//if

		 // 3. check there is conflict with other lizards
		if (myLizard.count(new_lizard_row))
		{
			for (int i = 0; i < myLizard[new_lizard_row].size(); i++)
			{
				if (myLizard[new_lizard_row][i] == new_lizard_col)
				{// exactely on the other lizar
				 // pick other random positions
					complete = false;
					break;
				}
			}
		}
		// check treeFree: Max 2
		if (treeFree_row[new_lizard_row])
		{
			if (treeFree_lock[new_lizard_row] >= 2)
			{
				complete = false;
				break;
			}
		}


	}//while

	 // 4. update lizard's posiiton
	 // remove previous lizard
	for (vector<int>::iterator iter = myLizard[lizard_row].begin(); iter != myLizard[lizard_row].end(); ++iter)
	{
		if (*iter == lizard_col)
		{
			myLizard[lizard_row].erase(iter);
			break;
		}
	}
	if (myLizard[lizard_row].size() == 0)
	{// if it was the last col in the row, delete the key
		myLizard[lizard_row].clear();
		//myLizard.erase(lizard_row);
	}

	// add lizard 
	if (myLizard.count(new_lizard_row))
	{// on the row it already has lizard
		myLizard[new_lizard_row].push_back(new_lizard_col);
		//sort할까?
	}
	else
	{// there is no lizard on the row
		vector<int> cols;
		cols.push_back(new_lizard_col);
		myLizard.insert(pair<int, vector<int> >(new_lizard_row, cols));
	}
	
	return myLizard;
}

int countConflict(int n, int p, map<int, vector<int> > myLizard, map<int, vector<int> > myTrees)
{
	int sumOfConflict = 0;
	vector<int> temp_row_1, temp_row_2, temp_tree;
	int cur_liz_row, cur_liz_col;
	int other_liz_row, other_liz_col;
	int dia, t_dia;
	int t_r, t_c;
	bool left_trueLock = true, straight_trueLock = true, right_trueLock = true, same_trueLock = true;

	for (int i = 0; i < n; i++) // itr all rows
	{
		if (myLizard.count(i))
		{// i th row has a lizard
			cur_liz_row = i;
			temp_row_1 = myLizard[cur_liz_row];

			// 1. check on the same row
			if (temp_row_1.size() >= 2)
			{

				sumOfConflict += (temp_row_1.size()-1);

				// compare with trees
				for (int j = 0; j < temp_row_1.size() - 1; j++)
				{
					same_trueLock = true;

					if (myTrees.count(cur_liz_row))
					{
						temp_tree = myTrees[cur_liz_row];
						for (int x = 0; x < temp_tree.size(); x++)
						{
							if (temp_row_1[j] < temp_tree[x]
								&& temp_tree[x] < temp_row_1[j + 1] && same_trueLock)
							{
								same_trueLock = false;
								sumOfConflict --;
								break;
							}

						}//for
					}//if
				}// for
			}

			// 2. check lizards with Trees
			for (int j = 0; j < temp_row_1.size(); j++)
			{
				cur_liz_col = temp_row_1[j];

				for (int k = cur_liz_row + 1; k < n; k++)
				{
					if (myLizard.count(k))
					{// k th row has a lizard
						other_liz_row = k;
						temp_row_2 = myLizard[other_liz_row];

						for (int m = 0; m < temp_row_2.size(); m++) // lizard traveler
						{
							other_liz_col = temp_row_2[m];
							dia = other_liz_row - cur_liz_row;

							// left
							if (other_liz_col == cur_liz_col - dia)
							{
								sumOfConflict ++;
								left_trueLock = true;
								//tree travelser
								for (int t_r = cur_liz_row + 1; t_r < other_liz_row; t_r++) // tree traveler
								{// t= tree's row
									t_dia = t_r - cur_liz_row;

									if (myTrees.count(t_r))
									{// there is tree btw 2 lizs
										for (int t_c = 0; t_c < myTrees[t_r].size(); t_c++)
										{
											if (myTrees[t_r][t_c] == cur_liz_col - t_dia && left_trueLock)
											{
												left_trueLock = false;
												sumOfConflict--;
												break;
											}
										}
									}
								}// for: t
							}

							// straight
							if (other_liz_col == cur_liz_col)
							{
								left_trueLock = true, straight_trueLock = true, right_trueLock = true;
								sumOfConflict ++;

								straight_trueLock = true;
								//tree travelser
								for (int t_r = cur_liz_row + 1; t_r < other_liz_row; t_r++)
								{// t= tree's row

									if (myTrees.count(t_r))
									{// there is tree btw 2 lizs
										for (int t_c = 0; t_c < myTrees[t_r].size(); t_c++)
										{
											if (myTrees[t_r][t_c] == cur_liz_col && straight_trueLock)
											{
												straight_trueLock = false;
												sumOfConflict--;
												break;
											}
										}
									}
								}// for: t
							}

							// right
							if (other_liz_col == cur_liz_col + dia)
							{
								left_trueLock = true, straight_trueLock = true, right_trueLock = true;
								sumOfConflict ++;
								right_trueLock = true;
								//tree travelser
								for (int t_r = cur_liz_row + 1; t_r < other_liz_row; t_r++)
								{// t= tree's row
									t_dia = t_r - cur_liz_row;

									if (myTrees.count(t_r))
									{// there is tree btw 2 lizs
										for (int t_c = 0; t_c < myTrees[t_r].size(); t_c++)
										{
											if (myTrees[t_r][t_c] = cur_liz_col + t_dia && right_trueLock)
											{
												right_trueLock = false;
												sumOfConflict --;
												break;
											}
										}

									}
									if (!right_trueLock)
									{
										break;
									}
								}// for: t
							}
						}

					}//if
				}//for: k

			}// for: j
		}// if: count(i)
	}// for: i 
	return sumOfConflict;
}

double schedule(int itr_t, int numOfempty)
{
	return (log(1 + 0.0001*itr_t));
}

vector<int> SA(int n, int p, vector<int> tree_pos, int numOfempty, Node* root)
{
	clock_t begin_SA = clock();
	vector<vector<int> > board(n, vector<int>(n, 0));
	vector<int> result;
	int itr_t = 0;
	int lizard = p;
	int cur_r, cur_c;
	int cur_conf, next_conf;
	int deltaE;
	double lotto, T;
	bool treeFree = false;
	map<int, vector<int> > cur_myLizard;
	map<int, vector<int> > next_myLizard;
	map<int, vector<int> > myTrees;
	int numOftreeFree = n;
	bool *treeFree_row = new bool[n];
	for (int i = 0; i < n; i++)
	{
		treeFree_row[i] = true;
	}
	// uniformed random generator
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> i_dis(0, n - 1);
	uniform_real_distribution<double> d_dis(0, 1);

	// Initialization 
	// 1. put tree on the board & make a tree map
	for (int i = 0; i < tree_pos.size(); i += 2)
	{
		board[tree_pos[i]][tree_pos[i + 1]] = 2;
		treeFree_row[tree_pos[i]] = false;
		numOftreeFree--;

		if (myTrees.count(tree_pos[i]))
		{// already trees on the row
			myTrees[tree_pos[i]].push_back(tree_pos[i + 1]);
		}
		else
		{// first tree on the row
			vector<int> t_cols;
			t_cols.push_back(tree_pos[i + 1]);
			myTrees.insert(pair<int, vector<int> >(tree_pos[i], t_cols));
		}
	}

	//2. If there is no trees
	if (tree_pos.size() == 0)
	{
		treeFree = true;
		for (int i = 0; i < n; i++)
		{
			cur_r = i;
			cur_c = i_dis(gen);
			// put the lizard on it
			board[cur_r][cur_c] = 1;
			lizard--;

			//update myLizard hashmap
			if (cur_myLizard.count(cur_r))
			{// on the row it already has lizard
				cur_myLizard[cur_r].push_back(cur_c);
			}
			else
			{// there is no lizard on the row
				vector<int> cols;
				cols.push_back(cur_c);
				cur_myLizard.insert(pair<int, vector<int> >(cur_r, cols));
			}
		}
	}
	else
	{
		// 2. get random (row,col) "p" times
		lizard = p;
		while (lizard > 0)
		{
			cur_r = i_dis(gen);
			cur_c = i_dis(gen);

			if (board[cur_r][cur_c] == 2		//tree
				|| board[cur_r][cur_c] == 1)	//other lizard
			{// get other rand values
				continue;
			}
			else
			{// put the lizard on it
				board[cur_r][cur_c] = 1;
				lizard--;

				//update myLizard hashmap
				if (cur_myLizard.count(cur_r))
				{// on the row it already has lizard
					cur_myLizard[cur_r].push_back(cur_c);
				}
				else
				{// there is no lizard on the row
					vector<int> cols;
					cols.push_back(cur_c);
					cur_myLizard.insert(pair<int, vector<int> >(cur_r, cols));
				}
			}
		}//while
	}

	// 3. calculate the conflicts @ cur_board
	cur_conf = countConflict(n, p, cur_myLizard, myTrees);

	// infinite loop 
	while (1) 
	{
		itr_t++;
		next_conf = 0;

		// 1. goal check
		if (cur_conf == 0)
		{
			cout << "OK" << endl;
			for (int i = 0; i < n; i++)
			{
				if (cur_myLizard.count(i))
				{
					for (int j = 0; j < cur_myLizard[i].size(); j++)
					{
						result.push_back(i);
						result.push_back(cur_myLizard[i][j]);
					}
				}
			}

			clock_t end = clock();
			double executeTime = ((end - begin_SA) / (double)CLOCKS_PER_SEC);
			if (executeTime > 1.0)
			{
				//cout << "Out of time: " << executeTime << endl;
			}

			return result;
		}

		// 2. make next lizard info.
		next_myLizard = updateLizard(board, n, p, cur_myLizard, myTrees, treeFree, treeFree_row); // 여기 updatedLizard

		// 3. calculate the conflicts @ next_board
		next_conf = countConflict(n, p, next_myLizard, myTrees);

		// 4. Comparision
		deltaE = next_conf - cur_conf; // Bigger, worse: [1:p]

		if (deltaE < 0)
		{// choose next_board
			cur_conf = next_conf;
			for (int i = 0; i < next_myLizard.size(); i++)
			{
				for (int j = 0; j < next_myLizard[i].size(); j++)
				{
					// update cur_myLizard
					cur_myLizard = next_myLizard;
				}
			}
		}// if
		else
		{// choose which one?
			lotto = 1;
			while (lotto == 1)
			{
				lotto = d_dis(gen);
			}

			T = schedule(itr_t, numOfempty);
			//prev_T = T;

			if (lotto < exp((-1) * (deltaE / T)))
			{// choose next_board
				cur_conf = next_conf;
				for (int i = 0; i < next_myLizard.size(); i++)
				{
					for (int j = 0; j < next_myLizard[i].size(); j++)
					{
						// update cur_myLizard
						cur_myLizard.clear();
						cur_myLizard = next_myLizard;
					}
				}
			}//if

		}//else

		clock_t end = clock();
		double executeTime = ((end - begin_SA) / (double)CLOCKS_PER_SEC);

		if (executeTime > 170.0) // 200으로바꿔!
		{
			//cout << "Out of time: " << executeTime << endl;
			break;

		}

	}//while

	vector<int> empty;
	return empty;
}


int main()
{
	/* Check the time */
	clock_t begin, end;
	begin = clock();

	/* Read input.txt file */
	ifstream in("input.txt");
	ofstream out("output.txt");
	string line;
	string type;
	int n, p, type_index = 0;
	int result_col, result_row;
	vector<int> result;

	in >> type;
	in >> n >> p;
	if (type == "BFS")
	{
		type_index = 1;
	}
	else if (type == "DFS")
	{
		type_index = 2;
	}
	else if (type == "SA")
	{
		type_index = 3;
	}

	/* Check input
	cout << "type: " << type << endl;
	cout << "n: " << n << endl;
	cout << "p: " << p << endl;
	*/

	vector<int> tree_pos;
	int numOfTree = 0;
	int numOfempty = 0;
	string s = "";
	char *input;

	// create the answer matrix to write to out.txt
	char **answer_matrix = new char*[n];
	for (int i = 0; i < n; i++)
	{
		answer_matrix[i] = new char[n];
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			answer_matrix[i][j] = 0;
		}
	}

	// write input matrix to answer matrix
	for (int i = 0; i < n; i++)
	{
		in >> s;

		input = new char[s.length() + 1];
		strcpy(input, s.c_str());
		for (int j = 0; j < n; j++)
		{
			answer_matrix[i][j] = input[j];

			if (input[j] == '2')
			{
				numOfTree++;
				tree_pos.push_back(i); // tree's row
				tree_pos.push_back(j); // tree's col
			}
		}
		input = "";

	}

	in.close();

	/* check N and P values */
	bool size_fail = false;
	numOfempty = n*n - numOfTree;

	if (n <= 0 || p <= 0)
	{
		size_fail = false;
	}
	else if (numOfempty < p)
	{
		size_fail = false;
	}
	else if (n == 1)
	{
		if (p > 1)
		{
			size_fail = true;
		}
		else if (numOfTree > 1)
		{
			size_fail = true;
		}
	}
	else if (n == 2)
	{
		if (p == 1)
		{
			out << "OK" << endl << "10" << endl << "00" << endl;
			in.close();
			return 0;
		}
		else if (p > 1)
		{
			size_fail = true;
		}
	}
	else if (numOfTree == 0)
	{
		if (n == 3 && p > 2)
		{
			size_fail = true;
		}
		else if (n > 3 && p > n)
		{
			size_fail = true;
		}
	}

	if (size_fail)
	{
		cout << "FAIL" << endl;
		out << "FAIL" << endl;
		out.close();
		return 0;
	}

	Node *root = new Node(-1, -1, n, 0);
	switch (type_index)
	{
	case 1: //BFS
		result = BFS(root, n, p, tree_pos, numOfTree);

		if (result.size() == 0)
		{
			cout << "FAIL" << endl;
			out << "FAIL" << endl;
			return 0;
		}

		/* save the result */
		out << "OK" << endl;

		for (int i = 0; i < result.size(); i += 2)
		{
			//cout << result[i] << "," << result[i + 1] << endl;
			answer_matrix[result[i]][result[i + 1]] = '1';
		}
		cout << endl;

		break;

	case 2:// DFS
		result = DFS(root, n, p, tree_pos, numOfTree);

		if (result.size() == 0)
		{
			cout << "FAIL" << endl;
			out << "FAIL" << endl;
			return 0;
		}


		/* save the result */
		out << "OK" << endl;

		for (int i = 0; i < result.size(); i += 2)
		{
			//cout << result[i] << "," << result[i + 1] << endl;
			answer_matrix[result[i]][result[i + 1]] = '1';
		}
		cout << endl;

		break;

	case 3: // SA
		result = SA(n, p, tree_pos, numOfempty, root);

		if (result.size() == 0)
		{
			cout << "FAIL" << endl;
			out << "FAIL" << endl;
			return 0;
		}

		// save the result 
		out << "OK" << endl;

		for (int i = 0; i < result.size(); i += 2)
		{
			answer_matrix[result[i]][result[i + 1]] = '1';
		}
		cout << endl;

		
		break;

	default:
		cout << "wrong type! " << endl;
		out << "wrong type! " << endl;
		return 0;
	}

	/* write out.text */
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			out << answer_matrix[i][j];
		}
		out << endl;
	}
	out.close();

	out.close();
	return 0;
}