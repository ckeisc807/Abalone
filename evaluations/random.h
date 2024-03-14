#ifndef RANDOM
#define RANDOM
#include"../EvaluationBase.h"
#include<random>
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-1.0, 1.0);
struct Random: Evaluation {
    float evaluate (Abalone abalone) override {
        return dis(gen);
    }
};

#endif