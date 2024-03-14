#ifndef EvaluationBase
#define EvaluationBase

#include"abalone.h"

struct Evaluation {
    virtual float evaluate(Abalone abalone){return 0;}
};

#endif