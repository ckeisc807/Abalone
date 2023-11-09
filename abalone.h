#include<iostream>
#include<assert.h>
using std::ostream;
using std::cout;//
using std::max;
using std::swap;
const int dx[6]={1,1,0,-1,-1,0};
const int dy[6]={0,1,1,0,-1,-1};
struct Abalone{
    int winner=0;
    int score[3]={5,0,0};
    int cnt_turn=0;
    int current_player=1;
    int board[9][9];
    Abalone():winner(0),cnt_turn(0),current_player(1){
        score[0]=5;
        score[1]=score[2]=0;
        for(int y=0;y<9;y++){
            for(int x=0;x<9;x++){
                if(abs(y-x)>4) board[x][y]=-1;
                else if(y<=1||y==2&&2<=x&&x<=4) board[x][y]=1;
                else if(y>=7||y==6&&4<=x&&x<=6) board[x][y]=2;
                else board[x][y]=0;
            }
        }
    };
    inline void step(int x1,int y1,int x2,int y2,int direction){
        int xm=x1+x2>>1,ym=y1+y2>>1; // x_middle, y_middle
        assert(0<=x1&&x1<9&&0<=y1&&y1<9&&0<=x2&&x2<9&&0<=y2&&y2<9&&0<=direction&&direction<6&&abs(x1-y1)<5&&abs(x2-y2)<5); //check: over limit
        assert(x1==x2||y1==y2||x1-y1==x2-y2); //check: same line
        assert(board[x1][y1]==current_player&&board[xm][ym]==current_player&&board[x2][y2]==current_player);
        if(x1==x2&&y1==y2){// one
            int nextx=x1+dx[direction],nexty=y1+dy[direction];
            assert(0<=nextx&&nextx<9&&0<=nexty&&nexty<9);
            assert(board[nextx][nexty]==0);
            board[nextx][nexty]=current_player;
            board[x1][y1]=0;
            current_player=(current_player==1)?2:1;
            return ;
        }
        int len=max(abs(x1-x2),abs(y1-y2))+1;
        assert(len==2||len==3);
        if(x1>x2||x1==x2&&y1>y2) swap(x1,x2),swap(y1,y2); //p2->p1 direction in [0,1,2]
        int line_direction=direction%3;
        if(x2-x1==(dx[line_direction])<<(len&1)&&y2-y1==dy[line_direction]<<(len&1)){
            if(x2-x1!=(dx[direction])<<(len&1)||y2-y1!=dy[direction]<<(len&1)) swap(x1,x2),swap(y1,y2);
            int nextx=x2+dx[direction],nexty=y2+dy[direction],opponent_player=(current_player==1)?2:1,opponent_len=0;
            for(;0<=nextx&&nextx<9&&0<=nexty&&nexty<9&&board[nextx][nexty]==opponent_player;nextx+=dx[direction],nexty+=dy[direction]) opponent_len++;
            assert(len>opponent_len);
            if(opponent_len==0){
                assert(board[nextx][nexty]==0&&0<=nextx&&nextx<9&&0<=nexty&&nexty<9); //ouob
                board[nextx][nexty]=current_player;
                board[x1][y1]=0;
                current_player=(current_player==1)?2:1;
                return ;
            }
            else if(0<=nextx&&nextx<9&&0<=nexty&&nexty<9&&board[nextx][nexty]==0){
                board[nextx][nexty]=opponent_player;
                board[x2+dx[direction]][y2+dy[direction]]=current_player;
                board[x1][y1]=0;
                current_player=(current_player==1)?2:1;
                return ;
            }
            assert(board[nextx][nexty]==-1||nextx<0||nextx>=9||nexty<0||nexty>=9);
            score[current_player]++;
            board[x2+dx[direction]][y2+dy[direction]]=current_player;
            board[x1][y1]=0;
            current_player=(current_player==1)?2:1;
            return ;
        }
        assert(board[x1+dx[direction]][y1+dy[direction]]==0);
        assert(board[xm+dx[direction]][ym+dy[direction]]==0);
        assert(board[x2+dx[direction]][y2+dy[direction]]==0);
        board[x1+dx[direction]][y1+dy[direction]]=board[xm+dx[direction]][ym+dy[direction]]=board[x2+dx[direction]][y2+dy[direction]]=current_player;
        board[x1][y1]=board[xm][ym]=board[x2][y2]=0;
        current_player=(current_player==1)?2:1;
        return ;
    }
};
ostream &operator<<(ostream &out,Abalone abalone){
    for(int y=0;y<9;y++){
        for(int s=abs(4-y);s--;) out<<' ';
        for(int x=max(0,y-4);x<9&&abalone.board[x][y]!=-1;x++) out<<abalone.board[x][y]<<' ';
        out<<'\n';
    }
    return out;
}
