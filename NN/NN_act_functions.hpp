#ifndef _NN_ACT_FUNCTIONS_HPP
#define _NN_ACT_FUNCTIONS_HPP

#include <cmath>
#include <functional>
using std::max;

namespace NN {
	double Sigmoid(double x) {
		return 1.0 / (1.0 + exp(-x));
	};
	double dSigmoid(double x) {
		return Sigmoid(x) * (1.0 - Sigmoid(x));
	};

	double ReLU(double x) {
		return max(0.0, x);
	}
	double dReLU(double x) {
		if (x > 0.0) return 1.0;
		else return 0.0;
	}
}  // namespace NN
#endif