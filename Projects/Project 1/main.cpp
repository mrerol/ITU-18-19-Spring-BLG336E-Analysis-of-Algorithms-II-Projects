#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <sstream>
#include <ctime>
#include <iterator>
#include <vector> 
#include <iostream>
#include <queue>
#include <stack> 
#include <map> 
#include <string>
#include <cstring>
#include <exception>

class Node {
public:
	std::vector<Node *> childs_ptr;
	std::string** input_matrix;
	std::string matrix_string;
	bool valid;

	int total_miner;
	int row_number;
	int column_number;

	Node(std::string** , int , int );
	Node(std::string**, int, int, int, int, int);
	~Node();

	void print_matrix();
};

class Graph {
private:
	Node *root_ptr;
	std::queue<Node*> bfs_queque;
	std::stack<Node*> dfs_stack;
	int row_number;
	int column_number;
	int number_visited_nodes;
	int number_nodes_in_memory;
	bool finish_flag;
	char algo_type;
	std::map<std::string, int> all_nodes;

public:
	std::string** result_matrix;
	std::string** miner_info;
	double running_time;

	Graph(Node*, int, int, char);
	~Graph();

	void bfs();
	void dfs();
	void bfs_create_childs(Node*);
	void dfs_create_childs(Node*);
	bool check_node(Node*, int, int);
	void print_results();
};


Node::Node(std::string** matrix_in, int row_number, int column_number, int i_index, int j_index, int total_miner_in) {

	total_miner = 0;

	this->row_number = row_number;
	this->column_number = column_number;

	try {
		input_matrix = new std::string*[row_number + 1];
		for (int i = 0; i <= row_number; i++)
			input_matrix[i] = new std::string[column_number + 1];
	}
	catch (std::exception& e) {
		std::cout << e.what() << '\n';
	}
	catch (...) {
		std::cout << "Exception is occured!" << std::endl;
	}

	matrix_string = "";
	for (int i = 0; i <= row_number; i++) {
		for (int j = 0; j <= column_number; j++) {
			if (i > 0 && j > 0) {
				if (i == i_index && j == j_index) {
					matrix_string += "m";
					continue;
				}

				matrix_string += matrix_in[i][j];
			}
			input_matrix[i][j] = matrix_in[i][j];
		}
	}
	input_matrix[i_index][j_index] = "m";
	input_matrix[0][j_index] = std::to_string(stoi(input_matrix[0][j_index]) - 1);
	input_matrix[i_index][0] = std::to_string(stoi(input_matrix[i_index][0]) - 1);
	total_miner = total_miner_in - 2;
};
Node::Node(std::string** matrix_in, int row_number, int column_number) {

	this->row_number = row_number;
	this->column_number = column_number;

	try {
		input_matrix = new std::string*[row_number + 1];
		for (int i = 0; i <= row_number; i++)
			input_matrix[i] = new std::string[column_number + 1];
	}
	catch (std::exception& e) {
		std::cout << e.what() << '\n';
	}
	catch (...) {
		std::cout << "Exception is occured!" << std::endl;
	}

	matrix_string = "";
	for (int i = 0; i <= row_number; i++) {
		for (int j = 0; j <= column_number; j++) {
			if ((i == 0  || j == 0) && matrix_in[i][j] != " ") {
				total_miner += std::stoi(matrix_in[i][j]);
			}
			if (i > 0 && j > 0) {
				matrix_string += matrix_in[i][j];
			}
			
			input_matrix[i][j] = matrix_in[i][j];
		}
	}
	//std::cout << total_miner << std::endl;
};

Node::~Node() {

	for (int i = 0; i <= row_number; i++)
		delete[] input_matrix[i];
	delete[] input_matrix;

	childs_ptr.clear();
	//std::cout << "Destruction is done!" << std::endl;
};

void Node::print_matrix() {

	std::cout << "Total miner: " << total_miner << std::endl;
	if (valid)
		std::cout << "valid" << std::endl;
	else
		std::cout << "not valid" << std::endl;
	std::cout << "Matrix: " << std::endl;
	for (int i = 0; i <= row_number; i++) {
		for (int j = 0; j <= column_number; j++)
			std::cout << input_matrix[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << "MAP String: " << matrix_string << std::endl;
	std::cout << "----------------------"<< std::endl;
}

Graph::Graph(Node* root_in, int row_number_in, int column_number_in, char algo_type_in) {

	root_ptr = root_in;

	if(algo_type_in=='b')
		bfs_queque.push(root_ptr);
	else
		dfs_stack.push(root_ptr);

	root_ptr->valid = true;
	row_number = row_number_in;
	column_number = column_number_in;
	finish_flag = false;
	number_visited_nodes = 0;
	number_nodes_in_memory = 0;
	algo_type = algo_type_in;

	result_matrix = new std::string*[row_number + 1];
	for (int i = 0; i <= row_number; i++)
		result_matrix[i] = new std::string[column_number + 1];

	miner_info = new std::string*[row_number + 1];
	for (int i = 0; i <= row_number; i++)
		miner_info[i] = new std::string[column_number + 1];

	for (int i = 0; i <= row_number; i++) {
		for (int j = 0; j <= column_number; j++) {
			miner_info[i][j] = root_ptr->input_matrix[i][j];
		}
	}

};
Graph::~Graph() {

	for (int i = 0; i <= row_number; i++)
		delete[] result_matrix[i];
	delete[] result_matrix;

	for (int i = 0; i <= row_number; i++)
		delete[] miner_info[i];
	delete[] miner_info;

	//std::cout << "Destruction is done!" << std::endl;
};
void Graph::bfs() {

	Node *temp = NULL;

	while (!bfs_queque.empty()) {

		temp = bfs_queque.front();

		bfs_create_childs(temp);

		if (finish_flag) {
			//std::cout << all_nodes.size() << std::endl;
			return;
		}

		bfs_queque.pop();
	}	
}
void Graph::dfs() {

	Node *temp = NULL;

	while (!dfs_stack.empty()) {

		temp = dfs_stack.top();

		dfs_create_childs(temp);

		if (finish_flag) {
			return;
		}

		dfs_stack.pop();

	}


}
void Graph::bfs_create_childs(Node* node_ptr) {

	for (int i = 0;i < row_number; i++) {
		for (int j = 0; j < column_number; j++) {
			if (node_ptr->input_matrix[i + 1][j + 1] == ".") {

				// creating new node
				Node *temp = new Node(node_ptr->input_matrix, row_number, column_number, i + 1, j + 1, node_ptr->total_miner);

				//add the node to graph
				node_ptr->childs_ptr.push_back(temp); // add the childs to the current node

				number_nodes_in_memory++;
				if (all_nodes.find(temp->matrix_string) == all_nodes.end()) { // not found
					all_nodes[temp->matrix_string] = 1;
					//temp->print_matrix();
					number_visited_nodes++;
					
					if (check_node(temp, i + 1, j + 1)){
						finish_flag = true;
						result_matrix = temp->input_matrix;
						return;
					}
					if (temp->valid) {
						bfs_queque.push(temp);
						
					}
				}
			}

		}
	}
}
void Graph::dfs_create_childs(Node* node_ptr) {

	for (int i = 0; i < row_number; i++) {
		for (int j = 0; j < column_number; j++) {
			if (node_ptr->input_matrix[i + 1][j + 1] == ".") {

				// creating new node
				Node *temp = new Node(node_ptr->input_matrix, row_number, column_number, i + 1, j + 1, node_ptr->total_miner);

				//add the node to graph
				node_ptr->childs_ptr.push_back(temp); // add the childs to the current node

				number_nodes_in_memory++;
				if (all_nodes.find(temp->matrix_string) == all_nodes.end()) { // not found
					all_nodes[temp->matrix_string] = 1;
					//temp->print_matrix();
					number_visited_nodes++;
					if (check_node(temp, i + 1, j + 1)) {
						finish_flag = true;
						result_matrix = temp->input_matrix;
						return;
					}
					if (temp->valid) {
						dfs_stack.push(temp);
						
					}
				}
			}

		}
	}
}
bool Graph::check_node(Node* node_ptr, int i_index, int j_index) {

	bool temp_valid = false;

	// i-1 j
	if (i_index - 1 > 0 && node_ptr->input_matrix[i_index - 1][j_index] == "s") {
		temp_valid = true;
	}// i j-1
	if (j_index - 1 > 0 && node_ptr->input_matrix[i_index][j_index - 1] == "s") {
		temp_valid = true;
	}// i j+1
	if (j_index < column_number && node_ptr->input_matrix[i_index][j_index + 1] == "s") {
		temp_valid = true;
	}
	if (i_index < row_number && node_ptr->input_matrix[i_index + 1][j_index] == "s") {
		temp_valid = true;
	}


	if (stoi(node_ptr->input_matrix[0][j_index]) < 0) {
		temp_valid = false;
	}
	else if (stoi(node_ptr->input_matrix[i_index][0]) < 0) {
		temp_valid = false;
	}

	// top
	if (i_index == 1 ) {
		// top left
		if (j_index == 1) {
			if (node_ptr->input_matrix[i_index][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index ] == "m")
				temp_valid = false;
		} //top right
		else if (j_index == column_number) {
			if (node_ptr->input_matrix[i_index][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index] == "m")
				temp_valid = false;
		}// middle
		else {
			if (node_ptr->input_matrix[i_index][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index][j_index + 1] == "m")
				temp_valid = false;
		}
	}
	else if (i_index == row_number) {
		// bottom left
		if (j_index == 1) {
			if (node_ptr->input_matrix[i_index][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index] == "m")
				temp_valid = false;
		} //bottom right
		else if (j_index == column_number) {
			if (node_ptr->input_matrix[i_index][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index] == "m")
				temp_valid = false;
		}// middle
		else {
			if (node_ptr->input_matrix[i_index][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index][j_index + 1] == "m")
				temp_valid = false;

		}
	}
	else {
		// middle left
		if (j_index == 1) {
			if (node_ptr->input_matrix[i_index - 1][j_index] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index] == "m")
				temp_valid = false;
	
		} // middle right
		else if (j_index == column_number) {
			if (node_ptr->input_matrix[i_index - 1][j_index] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index] == "m")
				temp_valid = false;
		}// middle
		else {
			if (node_ptr->input_matrix[i_index - 1][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index - 1][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index][j_index + 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index - 1] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index] == "m")
				temp_valid = false;
			if (node_ptr->input_matrix[i_index + 1][j_index + 1] == "m")
				temp_valid = false;

		}
	}

	node_ptr->valid = temp_valid;
	if (node_ptr->total_miner == 0 && temp_valid)
		return true;
	return false;

}
void Graph::print_results() {
	if (algo_type == 'b')
		std::cout << "Algorithm: BFS" << std::endl;
	else
		std::cout << "Algorithm: DFS" << std::endl;

	std::cout << "Number of the visited nodes: " << number_visited_nodes + 1 << std::endl; // one more for root and for last node(because not checked)
	std::cout << "Maximum number of nodes kept in the memory: " << number_nodes_in_memory + 1 << std::endl; // one more node for root
	std::cout << "Running time: " << running_time << std::endl;
	std::cout << "Solution is written to the file." << std::endl;

}

std::string** read_file(std::string, int *, int *);
void write_file(std::string, std::string**, std::string**, int *, int *);

int main(int argc, char *argv[]) {
	
	if (argc != 4) {
		std::cout << "Missing Arguments!" << std::endl;
		return 1;
	}
	
	std::string algorithm_type = "";
	if (strcmp(argv[1], "bfs") == 0) {
		algorithm_type = "bfs";
	}
	else if (strcmp(argv[1], "dfs") == 0) {
		algorithm_type = "dfs";
	}
	else {
		std::cout << "Invalid algorithm type! Please, use just 'dfs' or 'bfs' keywords!" << std::endl;
		getchar();
		return 1;
	}

	std::string input_filename(argv[2]);
	std::string output_filename(argv[3]);
	//std::cout <<" " <<algorithm_type << " " << input_filename << " " << output_filename << std::endl;
	
	int *row_number = new int(0);
	int *column_number = new int(0);
	std::string** input_matrix = read_file(input_filename, row_number, column_number);

	/*
	std::cout << "Input matrix: " << std::endl;
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 3; j++)
			std::cout << input_matrix[i][j] << " ";
		std::cout << std::endl;
	}
	*/

	if (algorithm_type == "bfs") {
		clock_t t_start, t_end;

		Node* root= new Node(input_matrix, *row_number, *column_number);
		Graph BFS_Graph(root, *row_number, *column_number, 'b');

		t_start = clock();
		BFS_Graph.bfs();
		t_end = clock();

		BFS_Graph.running_time = double(t_end - t_start) / (CLOCKS_PER_SEC);

		BFS_Graph.print_results();

		write_file(output_filename, BFS_Graph.result_matrix,BFS_Graph.miner_info, row_number, column_number);

		delete root;

	}
	else {
			clock_t t_start, t_end;

			Node* root = new Node(input_matrix, *row_number, *column_number);
			Graph DFS_Graph(root, *row_number, *column_number, 'd');

			t_start = clock();
			DFS_Graph.dfs();
			t_end = clock();

			DFS_Graph.running_time = double(t_end - t_start) / (CLOCKS_PER_SEC);

			DFS_Graph.print_results();

			write_file(output_filename, DFS_Graph.result_matrix, DFS_Graph.miner_info, row_number, column_number);

			delete root;

	}


	for (int i = 0; i < *row_number + 1; i++)
		delete [] input_matrix[i];
	delete [] input_matrix;

	delete row_number;
	delete column_number;

	getchar();
	return 0;
}

std::string** read_file(std::string input_filename, int *row_number_ptr, int *column_number_ptr) {

	std::ifstream readfile;
	readfile.open(input_filename);

	if (!readfile.is_open()) {
		std::cout << "Reading file cannot be opened!" << std::endl;
		return NULL;
	}

	std::string** input_matrix = NULL;
	int row_number = 0, column_number = 0;

	readfile >> column_number >> row_number;

	input_matrix = new std::string*[row_number + 1];
	for (int i = 0; i < row_number + 1; i++)
		input_matrix[i] = new std::string[column_number + 1];

	int line_counter = -1;
	std::string line_string;

	while (!readfile.eof())
	{
		std::getline(readfile, line_string, '\n');
		
		if (line_string == "")
			continue;

		line_counter++;

		std::istringstream iss(line_string);
		std::vector<std::string> splitted_line_string(std::istream_iterator<std::string>{iss},
			std::istream_iterator<std::string>());
		
		if (line_counter == 0 && line_string[0] == '\t') {
			input_matrix[0][0] = " ";
			for (int i = 0; i < (int)splitted_line_string.size(); i++) {
				input_matrix[line_counter][i+1] = splitted_line_string[i];
				//std::cout << splitted_line_string[i] << " ";
			}

		}
		else if (line_counter == 0 && line_string[0] != ' ') {
			input_matrix[0][0] = " ";
			for (int i = 0; i < (int)splitted_line_string.size() - 1; i++) {
				input_matrix[line_counter][i + 1] = splitted_line_string[i+1];
				//std::cout << splitted_line_string[i] << " ";
			}
		}
		else {
			for (int i = 0; i < (int)splitted_line_string.size(); i++) {

				input_matrix[line_counter][i] = splitted_line_string[i];
				//std::cout << splitted_line_string[i] << " ";
			}
		}
		//std::cout << std::endl;
	

	}

	/*
	std::cout << "Input matrix: " << std::endl;
	for (int i = 0; i <= row_number; i++) {
		for (int j = 0; j <= column_number ; j++)
			std::cout << input_matrix[i][j] << " ";
		std::cout<<std::endl;
	}
	*/

	readfile.close();
	//std::cout << "Row number: " << row_number << std::endl << "Column number: " << column_number << std::endl;
	*row_number_ptr = row_number;
	*column_number_ptr = column_number;
	return input_matrix;
}
void write_file(std::string output_filename, std::string** output_matrix, std::string** miner_info, int *row_number, int *column_number) {

	std::ofstream writefile;
	writefile.open(output_filename, std::ios::out | std::ios::trunc);

	if (!writefile.is_open()) {
		std::cout << "Writing file cannot be opened!" << std::endl;
		return;

	}

	writefile << *column_number << "\t" << *row_number << std::endl;

	for (int i = 0; i <= *row_number; i++) {
		for (int j = 0; j <= *column_number; j++) {
			if (i == 0 || j == 0) {
				writefile << miner_info[i][j] << "\t";
				continue;
			}
			writefile << output_matrix[i][j] << "\t";
		}
		writefile << std::endl;
	}

	writefile.close();
}