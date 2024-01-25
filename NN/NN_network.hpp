#ifndef _NN_NETWORK_HPP_
#define _NN_NETWORK_HPP_

#include <functional>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "NN_matrix.hpp"

#pragma optimize(2, "Ofast")

using std::function;
using std::max;
using std::ostream;
using std::pair;
using std::string;
using std::swap;
using std::vector;

namespace NN {
	class NeuroNetwork {
	private:
		vector<Matrix> Weight, Biases;
		vector<Matrix> Zeta, Value;
		int layer_size;	 // 含輸入、輸出層
		double learning_rate;
		function<double(double)> act_func, dact_func;
		int input_size, output_size;

	public:
		NeuroNetwork() {}

		NeuroNetwork(const vector<int>& topo, function<double(double)> _act_func, function<double(double)> _dact_func, double _learning_rate)
			: layer_size(topo.size()), act_func(_act_func), dact_func(_dact_func), learning_rate(_learning_rate), input_size(topo[0]), output_size(topo[topo.size() - 1]) {
			Weight.resize(layer_size), Biases.resize(layer_size), Zeta.resize(layer_size), Value.resize(layer_size);
			for (int i = 1; i < layer_size; i++)
				Weight[i] = Matrix(topo[i], topo[i - 1]), Biases[i] = Matrix(topo[i], 1);
		}

	private:
		void forward(const vector<double>& input) {
			auto act = [&](Matrix& target) -> void {
				auto [R, C] = target.size();
				for (int i = 0; i < R; i++)
					for (int j = 0; j < C; j++)
						target(i, j) = act_func(target(i, j));
			};

			Value[0] = Matrix(input);
			for (int i = 1; i < layer_size; i++) {
				Matrix &x_pre = Value[i - 1], &W = Weight[i], &b = Biases[i], &z = Zeta[i], &x = Value[i];
				z = W * x_pre + b;
				x = z, act(x);
			}
		}

	private:
		void backward(const vector<double>& label) {
			Matrix y(label);
			Matrix dx = Value[layer_size - 1] - y;
			for (int l = layer_size - 1; l > 0; l--) {
				Matrix &x = Value[l], &b = Biases[l], &z = Zeta[l], &W = Weight[l], &x_pre = Value[l - 1];
				int x_size = x.size().first;

				// db = L * dact(z)
				Matrix db(dx);
				for (int j = 0; j < x_size; j++)
					db(j, 0) *= dact_func(z(j, 0));

				// dW = L * dact(z) * x_pre
				Matrix dW(W);
				auto [R, C] = dW.size();
				for (int i = 0; i < R; i++)
					for (int j = 0; j < C; j++)
						dW(i, j) = db(i, 0) * x_pre(j, 0);

				// dx = W.T * dx
				W.T();
				dx = W * dx;
				W.T();

				auto multiply_by_lr = [&](Matrix& target) -> void {
					auto [R, C] = target.size();
					for (int i = 0; i < R; i++)
						for (int j = 0; j < C; j++)
							target(i, j) = target(i, j) * learning_rate;
				};
				multiply_by_lr(db);
				multiply_by_lr(dW);
				b = b - db;
				W = W - dW;
			}
		}

	public:
		vector<double> predict(const vector<double>& input) {
			assert(input_size == input.size());
			forward(input);
			vector<double> result(output_size);
			for (int i = 0; i < output_size; i++) result[i] = Value[layer_size - 1](i, 0);
			return result;
		}

	public:
		double train(const vector<double>& input, const vector<double>& output) {
			assert(input_size == input.size());
			assert(output_size == output.size());
			forward(input);
			double loss = 0.0;
			Matrix label(output);
			Matrix L = Value[layer_size - 1] - label;
			for (int i = 0; i < output_size; i++)
				loss += L(i, 0) * L(i, 0);
			backward(output);
			return loss;
		}

	public:
		friend ostream& operator<<(ostream& out, NeuroNetwork& target) {
			for (int i = 1; i < target.layer_size; i++) {
				out << "layer" << i << ":\n";
				out << "  Weight:\n";
				out << target.Weight[i];
				out << "  Biases:\n";
				out << target.Biases[i];
				out << "\n\n";
			}
			return out;
		}

	public:
		friend bool write(NN::NeuroNetwork& target, const string& path, const string& name, const string& act_type, const string& dact_type);
		friend bool read(NN::NeuroNetwork& target, const string& path, const string& name);
	};
}  // namespace NN

#endif