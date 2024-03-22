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

namespace NN {
	map<string, function<double(double)>>
		act_functions = {{"Sigmoid", NN::Sigmoid}, {"ReLU", NN::ReLU}},
		dact_functions = {{"dSigmoid", NN::dSigmoid}, {"dReLU", NN::dReLU}};

	bool write(const NN::NeuroNetwork &target, ostream &out, const string &act_type, const string &dact_type) {
		out << act_type << " " << dact_type << "\n";
		out << target.layer_size << "\n";
		for (int i = 1; i < target.layer_size; i++) {
			// Value
			const NN::Matrix &weight = target.Weight[i];
			auto [vR, vC] = weight.size();
			out << vR << " " << vC << "\n";
			for (int _r = 0; _r < vR; _r++) {
				for (int _c = 0; _c < vC; _c++) {
					out << weight(_r, _c) << " ";
				}
				out << "\n";
			}
			out << "\n";
			// Biase

			const NN::Matrix &bias = target.Biases[i];
			auto [bR, bC] = bias.size();
			out << bR << "\n";
			for (int _r = 0; _r < bR; _r++)
				out << bias(_r, 0) << "\n";
			out << "\n";

			out << "\n";
		}
		return true;
	}

	void register_act_func(const string &func_name, function<double(double)> func) {
		act_functions[func_name] = func;
	}

	void register_dact_func(const string &func_name, function<double(double)> func) {
		dact_functions[func_name] = func;
	}

	bool read(NN::NeuroNetwork &target, istream &in) {
		string act_func_name, dact_func_name;
		in >> act_func_name >> dact_func_name;
		function<double(double)> act_func = act_functions[act_func_name], dact_func = dact_functions[dact_func_name];
		target.act_func = act_func, target.dact_func = dact_func;

		int size;
		in >> size;
		target.layer_size = size;
		target.Value.resize(size), target.Biases.resize(size), target.Weight.resize(size), target.Zeta.resize(size);

		for (int i = 1; i < size; i++) {
			int vR, vC;
			in >> vR >> vC;

			Matrix &weight = target.Weight[i];
			weight = Matrix(vR, vC);
			for (int _r = 0; _r < vR; _r++)
				for (int _c = 0; _c < vC; _c++)
					in >> weight(_r, _c);

			int bR;
			in >> bR;
			Matrix &bias = target.Biases[i];
			bias = Matrix(bR, 1);
			for (int _r = 0; _r < bR; _r++)
				in >> bias(_r, 0);
		}

		target.input_size = target.Weight[1].size().second;
		target.output_size = target.Weight[size - 1].size().first;
		return true;
	}
}  // namespace NN

#endif