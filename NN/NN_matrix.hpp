#ifndef _NN_MATRIX_HPP_
#define _NN_MATRIX_HPP_

#include <cassert>
#include <ostream>
#include <random>
#include <utility>
#include <vector>

using std::ostream;
using std::pair;
using std::swap;
using std::vector;

namespace NN {
	class Matrix {
		inline static const double matrix_upper = 4.0;
		inline static const double matrix_lower = -4.0;
		vector<vector<double>> data;
		int R, C;
		bool transpos = false;

	public:
		void T() {
			swap(R, C);
			transpos = !transpos;
		}

		double& operator()(int _r, int _c) {
			if (!transpos) return data[_r][_c];
			else return data[_c][_r];
		}

		Matrix() {}

		Matrix(int r, int c)
			: data(r, vector<double>(c)), R(r), C(c) {
#ifdef RANDOM_MODE
			static std::default_random_engine rand(std::random_device{}());
			static std::uniform_real_distribution<double> distributor((double)matrix_lower, (double)matrix_upper);
#endif

			for (int i = 0; i < R; i++) {
				for (int j = 0; j < C; j++) {
#ifdef RANDOM_MODE
					(*this)(i, j) = distributor(rand);
#else
					(*this)(i, j) = 0.0;
#endif
				}
			}
		}
		Matrix(int r, int c, double x)
			: data(r, vector<double>(c, x)), R(r), C(c) {}

		Matrix(const vector<double>& _data)
			: R(_data.size()), C(1), data(_data.size(), vector<double>(1, (double)0.0)) {
			for (int i = 0; i < R; i++) (*this)(i, 0) = _data[i];
		}

		pair<int, int> size() {
			return {R, C};
		}

		Matrix operator*(Matrix& another) {
			assert(C == another.R);

			Matrix result(R, another.C);
			for (int i = 0; i < R; i++)
				for (int j = 0; j < another.C; j++)
					for (int k = 0; k < C; k++)
						result(i, j) += (*this)(i, k) * another(k, j);

			return result;
		}

		Matrix operator+(Matrix& another) {
			Matrix result(*this);
			for (int i = 0; i < R; i++)
				for (int j = 0; j < C; j++)
					result(i, j) += another(i, j);

			return result;
		}

		Matrix operator-(Matrix& another) {
			Matrix result(*this);
			for (int i = 0; i < R; i++)
				for (int j = 0; j < C; j++)
					result(i, j) -= another(i, j);

			return result;
		}

		friend std::ostream& operator<<(std::ostream& out, Matrix& target) {
			out << "       ";
			for (int i = 0; i < target.C; i++) out << i << "   ";
			out << "\n";
			for (int i = 0; i < target.R; i++) {
				out << i << "  ";
				for (int j = 0; j < target.C; j++) out << target(i, j) << " ";
				out << "\n";
			}
			return out;
		}
	};
};	// namespace NN

#endif