#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <fstream>
#include <vector>
#include <chrono>


class Multiplier {
	friend class myInteger;
private:
	const int bit_size;
	std::string binary_string_1;
	std::string binary_string_2;
	std::string algorithm1_result;
	std::string classical_method_result;
	std::string ex_time_classic;
	std::string ex_time_algo1;
public:
	std::string output_filename;

	Multiplier(int);
	~Multiplier();

	std::string algorithm1(std::string, std::string);
	void algorithms_execute();
	int max_size(std::string, std::string);

	std::string single_bit_multiplication(std::string,std::string);
	std::string add_strings(std::string, std::string);
	std::string subtract_strings(std::string, std::string);
	std::string left_shift_string(std::string, int);

	std::string multiply_by_two_decimal_string(std::string);
	std::string addition_decimal_string(std::string, std::string);
	int make_string_length_equal(std::string*, std::string*);

	std::string classical_method();
	std::string convert_to_decimal(std::string);

	void write_to_file();
};


Multiplier::Multiplier(int bit_size_in):bit_size(bit_size_in) {

	srand(static_cast<unsigned int>(time(NULL)));
	for (int i = 0; i < bit_size; i++) {
		binary_string_1 += std::to_string(rand() % 2);
		binary_string_2 += std::to_string(rand() % 2);
	}
}
Multiplier::~Multiplier() {
	//std::cout << "Destructor is called!" << std::endl;
}
void Multiplier::algorithms_execute() {

	std::string temp_binary_string_1 = binary_string_1;
	std::string temp_binary_string_2 = binary_string_2;

	std::cout << "Bit size: " << bit_size << std::endl;
	std::cout << "Random binary string 1: " << binary_string_1 << std::endl <<"Random binary string 2: " << binary_string_2 << std::endl;

	std::cout << "Algorithm1 method has been started!" << std::endl;
	auto start1 = std::chrono::high_resolution_clock::now();
	algorithm1_result = algorithm1(binary_string_1, binary_string_2);
	auto end1 = std::chrono::high_resolution_clock::now();
	auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
	std::cout << "Time taken by Algorithm1 method: "<< duration1.count() << " microseconds" << std::endl;
	ex_time_algo1 = std::to_string(duration1.count());
	std::cout << "Algorithm1 method has been finished!" << std::endl;

	binary_string_1 = temp_binary_string_1;
	binary_string_2 = temp_binary_string_2;

	std::cout << "Classical method has been started!" << std::endl;
	auto start2 = std::chrono::high_resolution_clock::now();
	classical_method_result = classical_method();
	auto end2 = std::chrono::high_resolution_clock::now();
	auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
	std::cout << "Time taken by Classical method: " << duration2.count() << " microseconds" << std::endl;
	ex_time_classic = std::to_string(duration2.count());
	std::cout << "Classical method has been finished!" << std::endl;

	std::cout << "Converting binary string to decimal string has been started!" << std::endl;
	write_to_file();
	std::cout << "Converting binary string to decimal string has been completed!" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
}
std::string Multiplier::multiply_by_two_decimal_string(std::string str) {
	
	//str = "234234525656789352345676";

	int size = str.size();

	int cary = 0;
	std::string result = "";
	for (int i = size - 1; i >= 0; i--) {
		int digit = (str[i] - '0') * 2 + cary;

		if (digit >= 10) {
			cary = digit / 10;
			digit %= 10;
		}
		else
			cary = 0;

		result = std::to_string(digit) + result;
	}

	if (cary != 0)
		result = std::to_string(cary) + result;

	return result;
}
std::string Multiplier::addition_decimal_string(std::string str1, std::string str2) {
	
	//str1 = "99923232355786567567234523456452";
	//str2 = "99234234180967523459";
	int size = make_string_length_equal(&str1, &str2);
	int cary = 0;
	std::string result = "";
	for (int i = size - 1; i >= 0; i--) {
		int digit = (str1[i] - '0') + (str2[i] - '0') + cary;

		if (digit >= 10) {
			cary = digit / 10;
			digit %= 10;
		}
		else
			cary = 0;

		result = std::to_string(digit) + result;
	}
	if(cary!=0)
		result = std::to_string(cary) + result;

	return result;
}
std::string Multiplier::convert_to_decimal(std::string str1) {

	int size = str1.size();
	std::string result = "0";
	std::string twos_power("1");
	for (int i = size - 1; i >= 0; i--) {
		if (str1[i] == '1')
			result = addition_decimal_string(result, twos_power);
		twos_power = multiply_by_two_decimal_string(twos_power);
	}
	return result;
}

std::string Multiplier::algorithm1(std::string str1, std::string str2) {
	

	int n = max_size(str1, str2);

	if (n == 0)
		std::cout << "zero length;" << std::endl;

	if (n == 1)
		return single_bit_multiplication(str1, str2);

	int half_size = n / 2;
	int remain_size = n - half_size; 

	make_string_length_equal(&str1, &str2);

	std::string str1L = str1.substr(0, n / 2);
	std::string str1R = str1.substr(n / 2);

	std::string str2L = str2.substr(0, n / 2);
	std::string str2R = str2.substr(n / 2);

	std::string P1 = algorithm1(str1L, str2L);
	std::string P2 = algorithm1(str1R, str2R);
	std::string P3 = algorithm1(add_strings(str1L, str1R), add_strings(str2L, str2R));

	return add_strings(add_strings(left_shift_string(P1,2*remain_size),left_shift_string(subtract_strings(subtract_strings(P3, P1), P2),remain_size)),P2);

}
std::string Multiplier::classical_method() {

	//binary_string_1 = "1010";
	//binary_string_2 = "0111";

	int str1_size = binary_string_1.size();
	int str2_size = binary_string_2.size();

	std::string temp_str1 = binary_string_1;
	std::string result = "";
	for (int i = 0; i < str1_size; i++)
		result += "0";

	if (binary_string_2[str2_size - 1] == '1') {
		result = add_strings(result, binary_string_1);
	}

	for (int i = str2_size- 2; i >=0 ; i--) {
		temp_str1 += "0";
		if (binary_string_2[i] == '1') {
			result = add_strings(temp_str1, result);
		}
	
	}

	return result;
}
std::string Multiplier::single_bit_multiplication(std::string str1, std::string str2) {
	return std::to_string((str1[0] - '0')*(str2[0] - '0'));
}
int Multiplier::make_string_length_equal(std::string *str1, std::string *str2) {

	int str1_size = (*str1).size();
	int str2_size = (*str2).size();
	int new_size = 0;

	std::string temp_str = "";
	if (str1_size >= str2_size) {
		new_size = str1_size;
		for (int i = 0; i < str1_size - str2_size; i++) {
			temp_str += "0";
		}
		*str2 = temp_str + *str2;
	}
	else {
		new_size = str2_size;
		for (int i = 0; i < str2_size - str1_size; i++) {
			temp_str += "0";
		}
		*str1 = temp_str + *str1;
	}

	return new_size;
}
std::string Multiplier::add_strings(std::string str1, std::string str2) {

	int new_size = make_string_length_equal(&str1, &str2);

	std::string result = "";
	int carry = 0;  
	for (int i = new_size - 1; i >= 0; i--)
	{
		int first_bit = str1[i] - '0';
		int second_bit = str2[i] - '0';

		int temp_sum = (first_bit ^ second_bit ^ carry);
		result = (char)(temp_sum + '0') + result;
		carry = (first_bit & second_bit) | (second_bit & carry) | (first_bit & carry);
	}

	if (carry)  
		result = '1' + result;
	return result;
}
std::string Multiplier::subtract_strings(std::string str1, std::string str2) {

	int new_size = make_string_length_equal(&str1, &str2);
	std::string result;
	for (int i = 0; i < new_size; i++) {
		if (str2[i] == '1')
			str2[i] = '0';
		else
			str2[i] = '1';
	}

	str2 = add_strings(str2, "1");
	result = add_strings(str1, str2);
	if (result.size() > new_size)
		result = result.substr(1);
	return result;
}
int Multiplier::max_size(std::string str1, std::string str2) {
	int str1_size = str1.size();
	int str2_size = str2.size();
	if (str1_size < str2_size)
		return str2_size;
	else return str1_size;
}
std::string Multiplier::left_shift_string(std::string str1, int n) {

	for (int i = 0; i < n; i++) {
		str1 += "0";
	}

	return str1;
}
void Multiplier::write_to_file() {

	std::ofstream writefile;
	writefile.open(output_filename, std::ios::out | std::ios::app);

	if (!writefile.is_open()) {
		std::cout << "Writing file cannot be opened!" << std::endl;
		return;
	}

	writefile << "Bit size: " << bit_size << std::endl;

	writefile << "The first binary string:\t";;
	for (int i = 0; i < bit_size; i++) {
		writefile << binary_string_1[i];
	}
	writefile << std::endl;
	writefile << "The second binary string:\t";;
	for (int i = 0; i < bit_size; i++) {
		writefile << binary_string_2[i];
	}
	writefile << std::endl;

	writefile << "The binary result for classical method:  " << classical_method_result << std::endl;
	writefile << "The binary result for algorithm1 method: " << algorithm1_result << std::endl;

	std::string classical_method_result_decimal = convert_to_decimal(classical_method_result);
	std::string algorithm1_result_decimal = convert_to_decimal(algorithm1_result);

	writefile << "The decimal result for classical method:  " << classical_method_result_decimal << std::endl;
	writefile << "The decimal result for algorithm1 method: " << algorithm1_result_decimal << std::endl;

	std::cout << "The decimal result for classical method:  " << classical_method_result_decimal << std::endl;
	std::cout << "The decimal result for algorithm1 method: " << algorithm1_result_decimal << std::endl;

	writefile << "The execution time for classical method:  " << ex_time_classic << " miliseconds" << std::endl;
	writefile << "The execution time for algorithm1 method: " << ex_time_algo1 << " miliseconds" << std::endl;

	writefile << "-----------------------------------------------------------------" << std::endl;

	writefile.close();
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
		std::cout << "Missing Arguments!" << std::endl;
		return 1;
	}

	std::string output_filename(argv[1]);
	//std::cout << output_filename << std::endl;
	std::ofstream writefile;
	writefile.open(output_filename, std::ios::out | std::ios::trunc);
	writefile.close();

	int bit_size = 0;
	std::cout << "Enter the bit size (for all sizes press 1): ";
	std::cin >> bit_size;
	
	if(!(bit_size==1 || bit_size == 32 || bit_size == 64 || bit_size == 128 || bit_size == 256 || bit_size == 512 || bit_size == 1024)){
		std::cout << "Size is not defined! The program will be terminated! (Only 32, 64, 128, 256, 512, 1024 bit)" << std::endl;
		getchar();
		getchar();
		return 1;
	}

	if (bit_size == 1) {
		while (true) {

			bit_size *= 2;

			if (bit_size == 2048)
				break;

			Multiplier my_multiplier(bit_size);
			my_multiplier.output_filename = output_filename;
			my_multiplier.algorithms_execute();
		}
	}
	else {
		Multiplier my_multiplier(bit_size);
		my_multiplier.output_filename = output_filename;
		my_multiplier.algorithms_execute();
	}
	std::cout << "Press any key to exit!" << std::endl;
	getchar();
	getchar();
	return 0;
}
