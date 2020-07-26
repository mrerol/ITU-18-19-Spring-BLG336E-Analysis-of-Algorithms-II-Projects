#include "graph.h"

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#include <string>
#include <fstream>
#endif



Graph* reading_file(std::string);

int main(int argc, char *argv[]) {

	std::string input_filename;
	std::string output_filename;

	if (argc == 2) {
		input_filename = std::string(argv[1]);
		output_filename = "std::out";
	}
	else if (argc == 3) {
		input_filename = std::string(argv[1]);
		output_filename = std::string(argv[2]);
	}
	else {
		std::cout << "Not defined argument is read!" << std::endl;
		getchar();
		return 1;
	}
	
	Graph* main_graph = reading_file(input_filename);
	main_graph->play_matches();
	main_graph->print_result(output_filename);

	delete main_graph;

	getchar();
	return 0;
}

Graph* reading_file(std::string input_filename) {

	std::ifstream readfile;
	readfile.open(input_filename);

	if (!readfile.is_open()) {
		std::cout << "Reading file cannot be opened!" << std::endl;
		exit(1);
	}

	int N;
	readfile >> N;

	Graph *graph = new Graph(N);

	for (int i = 0; i <= N; i++) {
		for (int j = 0; j < N; j++) {
			readfile >> graph->team_match_matrix[i][j];
		}
	}

	return graph;
}