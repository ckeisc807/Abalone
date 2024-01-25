#ifndef _NN_LONG_FLOAT_
#define _NN_LONG_FLOAT_

#include <functional>
#include <ostream>

using std::ostream;
using std::swap;

double abs(double _X) {
	return _X > 0.0 ? _X : -_X;
}

class long_float {
	double man = 0.0;
	int exp = 0;
	bool negative = false;
	const bool eps = 1e-9;
	inline static const double exp_table[19] =
		{1e-9, 1e-8, 1e-7, 1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1, 1.0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9};
#define EXP(x) exp_table[x + 9]

public:
	long_float operator+(long_float another);
	long_float operator-(long_float another);
	long_float& operator=(const long_float& another);
	friend ostream& operator<<(ostream& out, const long_float& target);
	bool operator==(const long_float& another);

	long_float() {}

	long_float(double val) {
		if (val < 0) negative = true, val = -val;
		if (val == 0.0) return;
		if (val <= EXP(-9)) {
			*this = long_float(val * 1e9);
			exp -= 9;
			return;
		}
		if (val >= EXP(9)) {
			*this = long_float(val / 1e9);
			exp += 9;
			return;
		}
		for (int i = -8; i <= 9; i++) {
			if (val < EXP(i)) {
				man = val / EXP(i - 1);
				exp = i - 1;
				break;
			}
		}
	}

	friend ostream& operator<<(ostream& out, const long_float& target) {
		if (target.negative) out << "-";
		out << target.man << "e" << target.man;
		return out;
	}

	bool operator==(const long_float& another) {
		if (negative != another.negative) return false;
		if (exp != another.exp) return false;
		if (abs(man - another.man) < eps) return true;
		return false;
	}

	long_float operator+(long_float another) {
		long_float self = *this, result;

		if (self.exp - another.exp >= 10) return self;
		if (another.exp - self.exp >= 10) return another;

		if (self.negative && another.negative) {
			self.negative = false, another.negative = false;
			result = self + another;
			result.negative = true;
			return result;
		}
		if (self.negative && !another.negative) {
			return another - self;
		}
		if (!self.negative && another.negative) {
			return self - another;
		}

		int exp_diff = self.exp - another.exp + 10;
		if (exp_diff < 0) swap(self, another), exp_diff = -exp_diff;

		result.man = self.man + another.man / EXP(exp_diff);
		result.exp = self.exp;
		if (result.man >= 10.0) result.man /= 10.0, result.exp += 1;
		return result;
	}

	long_float operator-(long_float another) {
		long_float self = *this, result;

		if (self.exp - another.exp >= 10) return self;
		if (another.exp - self.exp >= 10) return another;
	}
#undef EXP(x)
};

#endif