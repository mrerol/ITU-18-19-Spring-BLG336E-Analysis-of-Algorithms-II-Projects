#include <iostream>
#include <string>
#include <vector> 
#include <stack>
#include <fstream>
#include <climits>

class Edge;

class Node {
	friend class Graph;

private:
	std::vector<Edge*> child_edges;
	std::string node_name;
	bool active;
	bool visited;

public:
	Node();
	Node(const Node&);
	~Node();
};

class Edge {
public:
	int capacity;
	int flow;
	Node *connected_node;

	Edge();
	Edge(int, int);
	Edge(const Edge&);
	~Edge();

};


class Graph {

private:
	int team_number;
	Node* s_node;
	Node* t_node;
	std::stack<Node*> dfs_stack;
	std::vector<Node*> team_list_right_nodes;
	std::vector<Node*> match_list_left_nodes;
	std::vector<Edge*> dfs_path;
	std::vector<int> result;
	std::vector<int> each_team_match_number;

public:
	std::vector<std::vector<int>> team_match_matrix;

	Graph(int);
	Graph(const Graph&);
	~Graph();

	void create_graph();
	void create_residual_graph(int);
	int dfs();
	void augmenting_path(int);
	int ford_fulkerson(Graph *);
	void play_matches();

	void print_childs(Node*);
	void print_path();
	void print_active_nodes();
	void print_result(std::string);
};