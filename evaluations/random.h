#ifndef RANDOM
#define RANDOM
#include"../EvaluationBase.h"
#include<random>
#ifndef RNADOM_DEVICE
#define RNADOM_DEVICE
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-1.0, 1.0);
#endif
struct Random: Evaluation {
    EvaluationType evaluate (Abalone abalone) override {
        return dis(gen);
    }
};

#endif