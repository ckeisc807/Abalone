#ifndef EvaluationBase
#define EvaluationBase

#include"abalone.h"
#include"alphaBeta.h"

struct Evaluation {
    virtual EvaluationType evaluate(Abalone abalone){return 0;}
};

#endif