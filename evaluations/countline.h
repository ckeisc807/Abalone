#ifndef COUNT_LINE
#define COUNT_LINE
#include"../EvaluationBase.h"
#include<random>

#ifndef RNADOM_DEVICE
#define RNADOM_DEVICE
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-1.0, 1.0);
#endif
struct CountLine: Evaluation {
    const EvaluationType weight[10] = {0,1,2,3,4,5,6,7,8,9};
    EvaluationType evaluate (Abalone abalone) override {
        EvaluationType score=0;
        // count x
        int cnt=0;
        for(int x=0;x<9;x++) {
            cnt=0;
            for(int y=0;y<9;y++){
                if(abalone.board[x][y]==abalone.current_player) cnt++;
            }
            score+=weight[cnt];
        }
        // count y
        for(int y=0;y<9;y++) {
            cnt=0;
            for(int x=0;x<9;x++){
                if(abalone.board[x][y]==abalone.current_player) cnt++;
            }
            score+=weight[cnt];
        }
        // count l
        for(int l=0;l<5;l++){
            for(int x=l,y=0;x<9&&y<9;x++,y++){
                if(abalone.board[x][y]==abalone.current_player) cnt++;
            }
            score+=weight[cnt];
        }
        for(int l=1;l<5;l++){
            for(int x=0,y=l;x<9&&y<9;x++,y++){
                if(abalone.board[x][y]==abalone.current_player) cnt++;
            }
            score+=weight[cnt];
        }
        return score;
    }
};

#endif