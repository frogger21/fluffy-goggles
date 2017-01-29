#include <iostream>
#include <string>
#include <list>
#include <fstream>
using namespace std;
/*
Breadth First Search Shortest Path
*/

/*
	---------------------------------------------------------------
	STRUCTS
	---------------------------------------------------------------
*/

//Edge has A->B
struct Edge {
	int src;
	int dest;
};

//The Graph network
struct Graph
{
	int Nvertices; //# of vertices
	int Medges; //# of edges
	std::list<Edge> Edges; //list of the edges
	std::list<int> Vertices; //list of vertices;
};

struct BFSshortest {
	int vertex;
	int dist;
};

struct vertexExplored
{
	int vertex;
	bool explored;
	//constructor
	vertexExplored()
	{
		vertex = 0;
		explored = false;
	}
};

/*
	---------------------------------------------------------------
	FUNCTIONS
	---------------------------------------------------------------
*/
bool aNumber(char *a)
{
	//[48,57] is ascii for #s 0 to 9
	int temp = static_cast<int>(*a);
	if (temp <= 57 && temp >= 48)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool check(Graph &X, Edge &Y)
{
	//checks if a -> b already exists as b -> a
	list<Edge>::iterator i;
	for (i = X.Edges.begin(); i != X.Edges.end(); ++i)
	{
		if (Y.src == i->dest && Y.dest == i->src)
		{
			return true; //we have a match
		}
	}
	return false;
}

//parses string line and looks for numbers to put into a list
//1 2 3 4 5 6 7 would be read as [1,2] [1,3] [1,4] [1,5] etc...
//ignores all non number ascii
void split(string line, Graph &X)
{
	string::size_type sz;
	Edge TempEdge;
	int NumCount = 0;
	int startPos, numLen, tracker, StringSize;
	int theNumber;
	int firstVertex;
	StringSize = line.size();
	bool Num = false;
	bool temp = false;
	for (int i = 0; i < StringSize; i++)
	{
		//is it a number?
		temp = aNumber(&line[i]);
		//if it's not a number then 
		if (temp == false)
		{
			Num = false;
		}
		//previous char was not a number but current is: we got a new number
		if (Num == false && temp == true)
		{
			Num = true;
			startPos = i;
			numLen = 1; //we don't know yet fully
			tracker = i + 1;
			while (tracker < StringSize)
			{
				if (aNumber(&line[tracker]) == true)
				{
					numLen += 1;
					tracker++;
				}
				else
				{
					// don't need to check these anymore cuz we already know 
					//it's # -1 b/c we i++ in for loop
					i += numLen - 1;
					break;
				}
			}
			string test2 = line.substr(startPos, numLen);
			theNumber = stoi(test2, &sz); //convert string to integer
			NumCount += 1;

			if (NumCount == 1)
			{
				firstVertex = theNumber;
				X.Vertices.push_back(firstVertex); //add to vertex list
				X.Nvertices += 1;
			}
			else
			{
				TempEdge.src = firstVertex;
				TempEdge.dest = theNumber;
				if (check(X, TempEdge) == false)
				{
					//we've never seen this before so add it.
					X.Edges.push_back(TempEdge);
					X.Medges += 1;
				}
			}
		}
	}
}

void printGraph(Graph &X)
{
	list<Edge>::iterator i;
	cout << "Graph Network\n";
	for (i = X.Edges.begin(); i != X.Edges.end(); ++i)
	{
		std::cout << "[" << i->src << "," << i->dest << "]" << endl;
	}
}

//does this vertex even exist in this graph?
bool VertexExistence(Graph &G, int vertexS)
{
	list<Edge>::iterator i;
	for (i = G.Edges.begin(); i != G.Edges.end(); ++i)
	{
		if (i->src == vertexS || i->dest == vertexS)
		{
			//this vertex exists
			return true;
		}
	}
	return false;
}

void MarkIt(vertexExplored *a, int b, int max)
{
	//find b in a of length max via linear 
	for (int i = 0; i < max; i++)
	{
		if (a[i].vertex == b)
		{
			a[i].explored = true;
			break;
		}
	}
}

void TransferVertices(vertexExplored *a, Graph &X, int max)
{
	list<int>::iterator z;
	z = X.Vertices.begin();
	for (int i = 0; i < max; i++)
	{
		a[i].vertex = *z;
		++z;
	}
}

void printQ(list<int> &Q)
{
	list<int>::iterator i;
	cout << "Q = [ ";
	for (i = Q.begin(); i != Q.end(); ++i)
	{
		cout << *i << " ";
	}
	cout << "]\n";
}

void printS(vertexExplored *p, int max)
{
	for (int i = 0; i < max; i++)
	{
		if (p[i].explored == true)
		{
			cout << "vertex: " << p[i].vertex << " has been explored!\n";
		}
	}
}

bool wasItMarked(vertexExplored *p, int max, int vertex)
{
	for (int i = 0; i < max; i++)
	{
		if (p[i].vertex == vertex)
		{
			if (p[i].explored == true)
			{
				return true;
			}
		}
	}
	return false;
}

void BFS(Graph &G, int vertexS)
{
	if (VertexExistence(G, vertexS) == true)
	{
		list<int> Q; //always push back and always pop front FIFO
		list<int>::iterator i; // for Q
		list<Edge>::iterator i2; //for Graph.Vertices
		bool Explored = false;
		vertexExplored *exploration = new vertexExplored[G.Nvertices]; //vertex and whether explored
		TransferVertices(exploration, G, G.Nvertices);
		int v, w;
		Edge temp;
		Q.push_back(vertexS);
		while (Q.empty() == false)
		{
			cout << "First!\n";
			printQ(Q);
			i = Q.begin();
			v = *i; //this is the vertex being popped out
			Q.pop_front(); //First out now
						   //find it in the vertexExplored list and mark it as explored
			MarkIt(exploration, v, G.Nvertices);
			printS(exploration, G.Nvertices);
			//loop through edges and find things connected to V
			for (i2 = G.Edges.begin(); i2 != G.Edges.end(); )
			{
				i2->dest;//B
				i2->src;//A
				if (i2->src == v || i2->dest == v)
				{
					//we found edge with v
					if (i2->src != v)
					{
						//src is W
						w = i2->src;
						Explored = wasItMarked(exploration, G.Nvertices, w);
						if (Explored == false)
						{
							MarkIt(exploration, i2->src, G.Nvertices);
						}
					}
					else {
						//dest is W
						w = i2->dest;
						Explored = wasItMarked(exploration, G.Nvertices, w);
						if (Explored == false)
						{
							MarkIt(exploration, i2->dest, G.Nvertices);
						}

					}
					if (Explored == false)
					{
						cout << "Found vertex " << w << " via [v,w] : [" << v << ", " << w << "]\n";
						Q.push_back(w); //add w to Q at the end
						i2 = G.Edges.erase(i2); //delete this edge we saw it
												// G.Nvertices--; //count of vertex down by 1
						G.Medges--;//count of edge down by 1
					}
					else
					{
						++i2;
					}
				}
				else
				{
					++i2;
				}
			}
			//cout << "Second!\n";
			//printQ(Q);
			//printS(exploration, G.Nvertices);
		}
		cout << "Final!" << endl;
		printS(exploration, G.Nvertices);
		delete[] exploration;
	}
	else
	{
		cout << "This vertex: " << vertexS << " does not exist!\n";
	}
}


void transferDist(Graph &G, list<BFSshortest> &l, int s)
{
	list<int>::iterator i;
	BFSshortest temp;
	for (i = G.Vertices.begin(); i != G.Vertices.end(); ++i)
	{
		temp.vertex = *i;
		if (temp.vertex == s)
		{
			temp.dist = 0;
		}
		else
		{
			temp.dist = -1; //initialize to -1
		}
		l.push_back(temp);
	}
}

int searchDist(list<BFSshortest> &X, int v)
{
	//returns distance of V
	list<BFSshortest>::iterator i;
	for (i = X.begin(); i != X.end(); ++i)
	{
		if (i->vertex == v)
		{
			return (i->dist);
		}
	}
	return (-99); //couldn't find it
}

void setDist(list<BFSshortest> &X, int v, int w)
{
	//given an unexplored W, set dist(w) = dist(v) + 1;
	list<BFSshortest>::iterator i;
	for (i = X.begin(); i != X.end(); ++i)
	{
		if (i->vertex == w)
		{
			i->dist = searchDist(X, v) + 1;
			break; //stop looping through
		}
	}
}

//compute shortest path from s to v
void BFSshortestPath(Graph &G, int s)
{
	if (VertexExistence(G, s) == true)
	{
		cout << "picked vertex: s=" << s << endl;
		list<BFSshortest> dist;
		transferDist(G, dist, s); //initialize  list of vertices and  distance
		list<int> Q;
		list<int>::iterator i; // for Q
		list<Edge>::iterator i2; //for Graph.Vertices
		bool Explored = false;
		vertexExplored *exploration = new vertexExplored[G.Nvertices]; //vertex and whether explored
		TransferVertices(exploration, G, G.Nvertices);
		int v, w;
		Edge temp;
		Q.push_back(s);
		while (Q.empty() == false)
		{
			//cout << "First!\n";
			//printQ(Q);
			i = Q.begin();
			v = *i;
			Q.pop_front();
			//find it in the vertexExplored list and mark it as explored
			MarkIt(exploration, v, G.Nvertices);

			//loop through edges and find things connected to v
			for (i2 = G.Edges.begin(); i2 != G.Edges.end();)
			{
				if (i2->src == v || i2->dest == v)
				{
					//we found edge with v, if w is unexplored then do stuff
					if (i2->src != v)
					{
						//src is W
						w = i2->src;
					}else {
						//dest is W
						w = i2->dest;
					}
					Explored = wasItMarked(exploration, G.Nvertices, w);
					if (Explored == false)
					{
						//W was unexplored then we need to set dist(w) = dist(v)+1
						MarkIt(exploration, w, G.Nvertices); //mark w as explored
						setDist(dist, v, w);
						Q.push_back(w);
						i2 = G.Edges.erase(i2);
						G.Medges--;

						//printing stuff
						//cout << "Found " << w << " via [v,w] : [" << v << ", " << w << "]\n";
						cout << "Shortest Distance of [s,w]=[" << s <<","<< w << "] is " << searchDist(dist, w) << endl;
					}
					else
					{
						++i2;
					}
				} 
				else
				{
					++i2;
				}
			} //end of for
		}
		//cout << "Final!" << endl;
		//printS(exploration, G.Nvertices);
		delete[] exploration;
	}
	else
	{
		cout << "This vertex: " << s << " does not exist!\n";
	}

}
/*
--------------------------------------------
	
	M A I N ()

--------------------------------------------
*/
int main()
{
	string fileNom;
	fileNom = "D:/coursera/stanford data struc and algos/4/test.txt"; //file path and file name
	Graph X;
	X.Nvertices = 0;
	X.Medges = 0;
	//Open file
	string line;
	ifstream myfile(fileNom);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			split(line, X);
		}
		myfile.close();
	}
	printGraph(X);
	BFSshortestPath(X, 4);
	//to temp pause cmd 
	string userResponse;
	cout << "Exit?\n";
	cin >> userResponse;
	return 0;
}