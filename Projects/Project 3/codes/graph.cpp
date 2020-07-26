#include "graph.h"


Edge::Edge() {
	capacity = 0;
	flow = 0;
}
Edge::Edge(int in_capacity, int in_flow) {
	capacity = in_capacity;
	flow = in_flow;
}

Edge::Edge(const Edge& in_obj) {

	this->capacity = in_obj.capacity;
	this->flow = in_obj.flow;

	Node *temp = new Node(*in_obj.connected_node);
	this->connected_node = temp;

}

Edge::~Edge() {
	//std::cout << "Destructor is called!" << std::endl;W
}

Node::Node() {
	active = false;
	visited = false;

}

Node::Node(const Node& in_obj) {

	this->visited = in_obj.visited;
	this->active = in_obj.active;
	this->node_name = in_obj.node_name;

	for (int i = 0; i < in_obj.child_edges.size(); i++) {
		Edge* temp = new Edge(*in_obj.child_edges[i]);
		this->child_edges.push_back(temp);
	}
}

Node::~Node() {
	//std::cout << "Destructor is called!" << std::endl;

	for (int i = 0; i < child_edges.size(); i++) {
		delete child_edges[i];
	}
}

Graph::Graph(int N_in) {

	team_number = N_in;
	s_node = new Node();
	t_node = new Node();

	// creating team matrix
	for (int i = 0; i <= team_number; i++) {
		each_team_match_number.push_back(0);
		team_match_matrix.push_back(std::vector<int>());
		for (int j = 0; j < team_number; j++) {
			team_match_matrix[i].push_back(0);
		}
	}
}
Graph::Graph(const Graph &in_obj) {

	this->team_number = in_obj.team_number;
	this->s_node = new Node(*in_obj.s_node);
	this->t_node = new Node(*in_obj.t_node);
	this->team_match_matrix = in_obj.team_match_matrix;
	this->each_team_match_number = in_obj.each_team_match_number;

	for (int i = 0; i < in_obj.team_list_right_nodes.size(); i++) {
		Node *temp = new Node(*in_obj.team_list_right_nodes[i]);
		this->team_list_right_nodes.push_back(temp);
	}

	for (int i = 0; i < in_obj.match_list_left_nodes.size(); i++) {
		Node *temp = new Node(*in_obj.match_list_left_nodes[i]);
		this->match_list_left_nodes.push_back(temp);
	}

	for (int i = 0; i < in_obj.dfs_path.size(); i++) {
		Edge *temp = new Edge(*in_obj.dfs_path[i]);
		this->dfs_path.push_back(temp);
	}

}
Graph::~Graph() {
	//std::cout << "Destructor is called!" << std::endl;

	while (!dfs_stack.empty()) {
		dfs_stack.pop();
	}

	for (int i = 0; i < team_list_right_nodes.size(); i++) {
		delete team_list_right_nodes[i];
	}

	for (int i = 0; i < match_list_left_nodes.size(); i++) {
		delete match_list_left_nodes[i];
	}

	delete s_node;
	delete t_node;
}

void Graph::create_graph() {

	s_node->node_name = "s_node";
	s_node->active = true;
	t_node->node_name = "t_node";
	t_node->active = true;


	for (int i = 0; i < team_number; i++) {

		Node* teams = new Node();
		teams->node_name = std::to_string(i);

		Edge* edges = new Edge(INT_MAX, INT_MAX);
		edges->connected_node = t_node;

		teams->child_edges.push_back(edges);
		team_list_right_nodes.push_back(teams);

		Edge* reverse_edge = new Edge(0, 0);
		t_node->child_edges.push_back(reverse_edge);
		reverse_edge->connected_node = teams;
	}


	// all matches
	for (int i = 2; i <= team_number; i++) {
		for (int j = 0; j < i - 1; j++) {
			//std::cout << i << " " << j << std::endl;
			if (team_match_matrix[i][j] == 0)
				continue;

			// creating match nodes
			Node* s_node_childs = new Node;
			s_node_childs->node_name = std::to_string(j) + "-" + std::to_string(i - 1);
			match_list_left_nodes.push_back(s_node_childs);

			Edge *reverse_matches_edge_temp = new Edge(0, 0);
			reverse_matches_edge_temp->connected_node = s_node;
			s_node_childs->child_edges.push_back(reverse_matches_edge_temp);

			//creating related edges for match nodes from s_node
			Edge *s_node_edge = new Edge(team_match_matrix[i][j], 0);
			s_node_edge->connected_node = s_node_childs;

			// total team matches
			each_team_match_number[j] += team_match_matrix[i][j];
			each_team_match_number[i - 1] += team_match_matrix[i][j];

			// teams have two match, these edges represent that matches
			Edge *edge_of_match_nodes_1 = new Edge(team_match_matrix[i][j], 0);
			edge_of_match_nodes_1->connected_node = team_list_right_nodes[j];

			Edge *reverse_team1_edge_temp = new Edge(0, 0);
			reverse_team1_edge_temp->connected_node = s_node_childs;
			edge_of_match_nodes_1->connected_node->child_edges.push_back(reverse_team1_edge_temp);

			Edge *edge_of_match_nodes_2 = new Edge(team_match_matrix[i][j], 0);
			edge_of_match_nodes_2->connected_node = team_list_right_nodes[i - 1];

			Edge *reverse_team2_edge_temp = new Edge(0, 0);
			reverse_team2_edge_temp->connected_node = s_node_childs;
			edge_of_match_nodes_2->connected_node->child_edges.push_back(reverse_team2_edge_temp);


			//middle edges connected 
			s_node_childs->child_edges.push_back(edge_of_match_nodes_1);
			s_node_childs->child_edges.push_back(edge_of_match_nodes_2);

			// adding match edges to s_node, left edges connected
			s_node->child_edges.push_back(s_node_edge);
			//print_childs(s_node_childs);
		}
	}
	//print_childs(&s_node);


}

void Graph::create_residual_graph(int which_team) {


	for (int i = 0; i < s_node->child_edges.size(); i++) {

		size_t found = match_list_left_nodes[i]->node_name.find(std::to_string(which_team));
		if (found == std::string::npos) { // not found
			match_list_left_nodes[i]->active = true;
			match_list_left_nodes[i]->child_edges[1]->connected_node->active = true;
			match_list_left_nodes[i]->child_edges[2]->connected_node->active = true;
		}
	}

	// setting t_nodes coming edge values
	for (int i = 0; i < team_list_right_nodes.size(); i++) {
		if (team_list_right_nodes[i]->active) {
			team_list_right_nodes[i]->child_edges[0]->capacity = team_match_matrix[0][which_team] + each_team_match_number[which_team] - team_match_matrix[0][std::stoi(team_list_right_nodes[i]->node_name)];
			team_list_right_nodes[i]->child_edges[0]->flow = 0;
		}
	}
}

int Graph::dfs() {
	
	// clearing dfs path
	dfs_path.clear();

	// reseting visited values of graph
	for (int i = 0; i < match_list_left_nodes.size(); i++) {
		match_list_left_nodes[i]->visited = false;
	}
	for (int i = 0; i < team_list_right_nodes.size(); i++) {
		team_list_right_nodes[i]->visited = false;
	}
	s_node->visited = false;
	t_node->visited = false;

	//std::string path = "";
	bool path_found = false;

	int max_flow = INT_MAX;

	Edge* temp_path_edge;
	Node* temp;
	dfs_stack.push(s_node);
	s_node->visited = true;

	while (!dfs_stack.empty()) {

		temp = dfs_stack.top();

		//path += "/" + temp->node_name;
		dfs_stack.pop();

		int i;
		for (i = 0; i < temp->child_edges.size(); i++) {

			if (temp->child_edges[i]->capacity != 0 && temp->child_edges[i]->connected_node->node_name == "t_node") {
				dfs_path.push_back(temp->child_edges[i]);
				path_found = true;
				t_node->visited = true;
				break;
			}

			if (temp->child_edges[i]->connected_node->active && !(temp->child_edges[i]->connected_node->visited) && temp->child_edges[i]->capacity != 0) {
				temp_path_edge = temp->child_edges[i];
				dfs_stack.push(temp->child_edges[i]->connected_node);
				temp->child_edges[i]->connected_node->visited = true;
			}
		}

		if (path_found)
			break;

		dfs_path.push_back(temp_path_edge);


	}

	// clearing stack	
	while (!dfs_stack.empty()) {
		dfs_stack.pop();
	}


	if (path_found) {

		for (int i = 0; i < dfs_path.size(); i++) {
			if (dfs_path[i]->capacity < max_flow)
				max_flow = dfs_path[i]->capacity;
		}

		return max_flow;
	}
	return -1;
	//return path;
}

void Graph::play_matches() {


	for (int i = 0; i < team_number; i++) {

		Graph* main_graph = new Graph(team_number);
		main_graph->team_match_matrix = this->team_match_matrix;
		main_graph->create_graph();

		main_graph->create_residual_graph(i);
		//main_graph->print_active_nodes();

		int win_status = ford_fulkerson(main_graph);
		result.push_back(win_status);


		delete main_graph;
	}

}

int Graph::ford_fulkerson(Graph* graph) {

	int max_flow;
	bool win_status = true;
	while ((max_flow = graph->dfs()) != -1) {
		graph->augmenting_path(max_flow);
	}

	for (int i = 0; i < graph->s_node->child_edges.size(); i++) {
		if (graph->s_node->child_edges[i]->connected_node->active && graph->s_node->child_edges[i]->capacity != 0)
			win_status = false;
	}

	if(win_status)
		return 1;
	return 0;
}

void Graph::print_childs(Node *node) {
	
	for (int i = 0; i < node->child_edges.size(); i++) {
		std::cout << "Node name: ";
		std::cout << node->child_edges[i]->connected_node->node_name << std::endl;
		std::cout << "Capacity of that node(coming): ";
		std::cout << node->child_edges[i]->capacity << std::endl;
		std::cout << "Flow of that node(coming): ";
		std::cout << node->child_edges[i]->flow << std::endl;
	}
	
	/*
	std::cout << "Each team match number" << std::endl;
	for (int i = 0; i < team_number; i++) {
		std::cout << each_team_match_number[i] << std::endl;
	}
	*/
}

void Graph::print_path() {

	for (int i = 0; i < dfs_path.size(); i++) {
		std::cout << dfs_path[i]->connected_node->node_name << std::endl;
	}
}

void Graph::print_active_nodes() {


	std::cout << "Active nodes left:" << std::endl;
	for (int i = 0; i < match_list_left_nodes.size(); i++) {
		if (match_list_left_nodes[i]->active)
			std::cout << match_list_left_nodes[i]->node_name << std::endl;
	}

	std::cout << "Active nodes right:" << std::endl;
	for (int i = 0; i < team_list_right_nodes.size(); i++) {
		if (team_list_right_nodes[i]->active)
			std::cout << team_list_right_nodes[i]->node_name << std::endl;
	}



}

void Graph::print_result(std::string filename) {

	if (filename == "std::out") {
		for (int i = 0; i < result.size(); i++) {
			std::cout << result[i] << " ";
		}
		std::cout << std::endl;
		return;
	}

	std::ofstream writefile;
	writefile.open(filename, std::ios::out | std::ios::trunc);

	if (!writefile.is_open()) {
		std::cout << "Writing file cannot be opened!" << std::endl;
		return;
	}

	std::cout << "Result is written to file: " << filename << std::endl;
	for (int i = 0; i < result.size(); i++) {
		writefile << result[i] << " ";
	}
	writefile << std::endl;
}

void Graph::augmenting_path(int max_flow) {

	std::string previous_node_name = "s_node";
	for (int i = 0; i < dfs_path.size(); i++) {

		// reverse edges
		for (int j = 0; j < dfs_path[i]->connected_node->child_edges.size(); j++) {

			if (dfs_path[i]->connected_node->active && dfs_path[i]->connected_node->child_edges[j]->connected_node->node_name == previous_node_name) {
				dfs_path[i]->connected_node->child_edges[j]->capacity +=  max_flow;
				dfs_path[i]->connected_node->child_edges[j]->flow = 0;
			}
		}
		// forward edges
		dfs_path[i]->capacity -= max_flow;
		dfs_path[i]->flow += max_flow;

		previous_node_name = dfs_path[i]->connected_node->node_name;
	}

}
