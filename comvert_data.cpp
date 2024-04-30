#include <assert.h>
#include <time.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const int start = 1;
const int end = 4;
const int data_size[] = {10, 10, 10, 10};

using std::fstream;
using std::string, std::to_string;
using std::vector;

std::string readFileAsString(const string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Unable to open file: " << filePath << std::endl;
		return "";
	}

	string fileContent((std::istreambuf_iterator<char>(file)),
					   std::istreambuf_iterator<char>());

	return fileContent;
}

vector<int> get_time_cost(const string& data) {
	std::istringstream iss;
	iss.str(data);

	vector<string> lines;
	string tmp;
	for (; getline(iss, tmp);) {
		lines.push_back(tmp);
	}

	vector<int> time_cost(4);

	std::istringstream(lines[lines.size() - 2]).swap(iss);
	iss >> time_cost[0] >> time_cost[1];
	std::istringstream(lines[lines.size() - 1]).swap(iss);
	iss >> time_cost[2] >> time_cost[3];

	return time_cost;
}

int main() {
	fstream dout;
	dout.open("data/compareSpeed.csv", fstream::out | fstream::trunc);

	string file_name, data;
	long long cnt_step, total_time_ab = 0, total_time_mm = 0, total_step = 0;
	vector<int> time_cost;
	dout << "depth, total steps, AlphaBeta total time, MinMax total time, ratio,\n";
	for (int d = start; d <= end; d++) {
		total_time_ab = total_time_mm = total_step = 0;
		for (int i = 1; i <= data_size[d - start]; i++) {
			file_name = "datas/depth" + to_string(d) + "/" + to_string(i) + ".txt";
			data = readFileAsString(file_name);

			cnt_step = 0;
			for (const char& c : data) {
				if (c == '\n') cnt_step++;
			}
			cnt_step -= 6;
			total_step += cnt_step;

			time_cost = get_time_cost(data);
			total_time_ab += time_cost[0];
			total_time_mm += time_cost[1];
		}
		dout << d << ", " << total_step << ", " << total_time_ab << ", " << total_time_mm << ", " << (float(total_time_ab)) / CLOCKS_PER_SEC / total_step << ", " << (float(total_time_mm)) / CLOCKS_PER_SEC / total_step << ", " << (float(total_time_ab)) / total_time_mm << ", \n";
		std::cout << d << ", " << total_step << ", " << total_time_ab << ", " << total_time_mm << ", " << (float(total_time_ab)) / CLOCKS_PER_SEC / total_step << ", " << (float(total_time_mm)) / CLOCKS_PER_SEC / total_step << ", " << (float(total_time_ab)) / total_time_mm << ", \n";
	}

	dout.close();
}