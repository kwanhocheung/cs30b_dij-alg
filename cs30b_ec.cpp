//Name: Kwanho Cheung
//Class: CS30B

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <tuple>
#include <fstream>
using namespace std;

class graph
{
private:
	vector<pair<char, vector<char>>>ver;
	//<ver,ver>.....
	vector<pair<char, char>>class_edge_set;
	//<ver,ver,weight>.....
	vector<tuple<char,char,int>>edge_weight;
public:
	graph() {};
	graph(vector<char>ver_set, vector<pair<char, char>>edge_set)
	{
		for (int i = 0; i < ver_set.size(); i++)
		{
			vector<char>g = {};
			for (int j = 0; j < edge_set.size(); j++)
			{
				if (ver_set[i] == edge_set[j].first)
				{
					g.push_back(edge_set[j].second);
				}
			}
			ver.push_back(make_pair(ver_set[i], g));
		}
		class_edge_set = edge_set;
	}
	vector<vector<int>>make_adj_matrix()
	{
		//init a zero matrix
		vector<vector<int>>adj_matrix(ver.size(), vector<int>(ver.size(), 0));
		for (int i = 0; i < ver.size(); i++)
		{
			for (auto k:ver[i].second)
			{
				int j = 0;
				while (ver[j].first != k)
				{
					j++;
				}
				//fill in the zero matrix
				adj_matrix[i][j] = 1;
			}
		}
		return adj_matrix;
	}
	void print_adj_matrix()
	{
		//open file as write
		fstream outfile;
		outfile.open("algorithm_output.txt", ios::out);

		outfile << "problem 1)" << endl;
		outfile << "Here is the adj matrix:\n";
		vector<vector<int>> m = make_adj_matrix();
		for (int i = 0; i < m.size(); i++)
		{
			for (int j = 0; j < m[i].size(); j++)
			{
				outfile << m[i][j] << " ";
			}
			outfile << "\n";
		}
		//close file
		outfile.close();
	}
	void print_adj_list()
	{
		//open file as add write
		fstream outfile;
		outfile.open("algorithm_output.txt", ios::app);

		outfile << "Here is the adj list:\n";
		for (int i = 0; i < ver.size(); i++)
		{
			outfile << ver[i].first << ": ";
			for (int j = 0; j < ver[i].second.size(); j++)
			{
				outfile << ver[i].second[j] << " ";
			}
			outfile << "\n";
		}
		outfile << "//////////////////////////////////////////////////////////////" << endl;
		outfile.close();
	}

	void add_weight(vector<int>weight)
	{
		cout << "\n";
		//cout << "add weight to edge" << endl;
		for (int i = 0; i < weight.size(); i++)
		{
			edge_weight.push_back(make_tuple(class_edge_set[i].first, class_edge_set[i].second,weight[i]));
			//cout << "["<<class_edge_set[i].first << " , " << class_edge_set[i].second << "] " << weight[i] << "\n";
		}

	}
	int shortest_path(char start, char end)
	{
		unordered_map<char, bool>visited;
		//assign all as unvisited (false)
		for (int i = 0; i < ver.size(); i++)
		{
			visited[ver[i].first] = false;
		}
		//record the previous vertex
		unordered_map<char, char>previous;
		//record the distance from start vertex to other vertex
		unordered_map<char, int>distance;
		for (int i = 0; i < ver.size(); i++)
		{
			//since start vertex to start vertex is no distance, so assign 0
			if (ver[i].first == start)
			{
				distance[ver[i].first] = 0;
			}
			//assign start vertex to other vertex to 10000, some big number
			else
			{
				distance[ver[i].first] = 10000;
			}
		}
		queue<char>q;
		//push the start vertex to queue
		q.push(start);
		while (!q.empty())
		{
			vector<int>direction;
			char current = q.front();
			q.pop();
			//cout << "current is " << current << endl;
			for (int i = 0; i < edge_weight.size(); i++)
			{
				char o = get<0>(edge_weight[i]);
				char x = get<1>(edge_weight[i]);
				int j = get<2>(edge_weight[i]);
				if (o == current && visited[x] == false)
				{
					direction.push_back(j);
					previous[x] = o;
					//cout << "previous of " << x << " is " << o << endl;
					//if the distance is 10000, then assign the distance to it
					if (distance[x] == 10000)
					{
						distance[x] = distance[current] + j;
						//cout << "distance of " << x << " is " << distance[x] << endl;
					}
					else
					{
						//if the new distance smaller than old distance, assign the new distance to it
						if (distance[x] > distance[current] + j)
						{
							distance[x] = distance[current] + j;
							//cout << "distance of " << x << " is " << distance[x] << endl;
						}
					}
					//cout << "add " << j << endl;
				}
			}
			//if no more direction to go, then break the while loop
			if (direction.empty())
			{
				break;
			}

			//find the min direction
			int min = direction[0];
			for (int i = 0; i < direction.size(); i++)
			{
				if (direction[i] < min)
				{
					min = direction[i];
				}
			}
			//cout << "min is " << min << endl;
			
			for (int i = 0; i < edge_weight.size(); i++)
			{
				char o = get<0>(edge_weight[i]);
				char x = get<1>(edge_weight[i]);
				int j = get<2>(edge_weight[i]);
				if (o == current && j == min)
				{
					//cout << "push " << x << endl;
					//push the min direction to queue
					q.push(x);
				}
			}
			//assign current as visited
			visited[current] = true;
			/*if (visited[current] == true)
			{
				cout << current << " is visited" << endl << endl;
			}*/
			
			//if the end vertex already visited, we can break the while loop and get the distance from the unordered_map distance
			if (visited[end] == true)
			{
				break;
			}
		}
		//cout << "end of processing" << endl << endl;
		return distance[end];
		
	}
};

int main()
{
	//problem 1
	vector<char>vertices = { 'a','b','c','d','e','f' };
	vector<pair<char,char>>edges = { {'a','c'},{'b','c'},{'b','e'},{'c','a'},{'c','b'},{'c','d'},{'c','e'},{'d','c'},{'e','c'},{'e','b'}};
	graph mygraph(vertices, edges);
	mygraph.print_adj_matrix();
	mygraph.print_adj_list();

	//problem 3
	vector<char>vertices2= { 'a','b','c','d','e','z' };
	vector<pair<char, char>>edges2 = { {'a','b'},{'a','c'},{'b','a'},{'b','c'},{'b','d'},{'c','a'},{'c','b'},{'c','d'},{'c','e'},{'d','b'},{'d','c'},{'d','e'},{'d','z'},{'e','c'},{'e','d'},{'e','z'},{'z','d'},{'z','e'} };
	vector<int>weight2 = { 4,2,4,1,5,2,1,8,10,5,8,2,6,10,2,3,6,3 };
	graph mygraph2(vertices2, edges2);
	mygraph2.add_weight(weight2);


	fstream outfile;
	outfile.open("algorithm_output.txt", ios::app);

	outfile << endl;
	outfile << "Problem 3) Dijkstra Algorithm" << endl;

	for (int i = 0; i < vertices2.size(); i++)
	{
		int shortest_path_distance = mygraph2.shortest_path('a', vertices2[i]);

		if (shortest_path_distance == 10000)
		{
			outfile << "There is no shortest path" << endl;
		}
		else
		{
			outfile << "The shortest path from a to " << vertices2[i] <<" is " << shortest_path_distance << endl;
		}
	}

	outfile.close();

	return 0;
	//end
}
