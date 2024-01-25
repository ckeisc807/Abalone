#ifndef _NN_STDIO_HPP_
#define _NN_STDIO_HPP_

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#include "NN_act_functions.hpp"
#include "NN_network.hpp"

using std::function;
using std::ifstream;
using std::map;
using std::ofstream;
using std::string;

const string dir = "./NN_example/";

namespace NN {
	map<string, function<double(double)>>
		act_functions = {{"Sigmoid", NN::Sigmoid}, {"ReLU", NN::ReLU}},
		dact_functions = {{"dSigmoid", NN::dSigmoid}, {"dReLU", NN::dReLU}};

	bool write(NN::NeuroNetwork &target, const string &path, const string &name, const string &act_type, const string &dact_type) {
		string output_path = dir + path + "/" + name + ".nn";
		ofstream file(output_path);
		if (file.fail()) {
			std::cout << "open file failed\n";
			file.close();
			return false;
		}

		file << act_type << " " << dact_type << "\n";
		file << target.layer_size << "\n";
		for (int i = 1; i < target.layer_size; i++) {
			// Value
			NN::Matrix &weight = target.Weight[i];
			auto [vR, vC] = weight.size();
			file << vR << " " << vC << "\n";
			for (int _r = 0; _r < vR; _r++) {
				for (int _c = 0; _c < vC; _c++) {
					file << weight(_r, _c) << " ";
				}
				file << "\n";
			}
			file << "\n";
			// Biase

			NN::Matrix &bias = target.Biases[i];
			auto [bR, bC] = bias.size();
			file << bR << "\n";
			for (int _r = 0; _r < bR; _r++)
				file << bias(_r, 0) << "\n";
			file << "\n";

			file << "\n";
		}

		file.close();
		return true;
	}

	void register_act_func(const string &func_name, function<double(double)> func) {
		act_functions[func_name] = func;
	}

	void register_dact_func(const string &func_name, function<double(double)> func) {
		dact_functions[func_name] = func;
	}

	bool read(NN::NeuroNetwork &target, const string &path, const string &name) {
		string input_path = dir + path + "/" + name + ".nn";
		ifstream file(input_path);
		if (file.fail()) {
			std::cout << "open file failed\n";
			file.close();
			return false;
		}

		string act_func_name, dact_func_name;
		file >> act_func_name >> dact_func_name;
		function<double(double)> act_func = act_functions[act_func_name], dact_func = dact_functions[dact_func_name];
		target.act_func = act_func, target.dact_func = dact_func;

		int size;
		file >> size;
		target.layer_size = size;
		target.Value.resize(size), target.Biases.resize(size), target.Weight.resize(size), target.Zeta.resize(size);

		for (int i = 1; i < size; i++) {
			int vR, vC;
			file >> vR >> vC;

			Matrix &weight = target.Weight[i];
			weight = Matrix(vR, vC);
			for (int _r = 0; _r < vR; _r++)
				for (int _c = 0; _c < vC; _c++)
					file >> weight(_r, _c);

			int bR;
			file >> bR;
			Matrix &bias = target.Biases[i];
			bias = Matrix(bR, 1);
			for (int _r = 0; _r < bR; _r++)
				file >> bias(_r, 0);
		}

		target.input_size = target.Weight[1].size().second;
		target.output_size = target.Weight[size - 1].size().first;
		return true;
	}
}  // namespace NN

#endif