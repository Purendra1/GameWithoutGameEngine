#include <stdio.h>
#include <conio.h> // _getch() / _kbhit()
#include <stdlib.h>

struct Paddle {
    int x; //0 or 119
    int ys; //0-25
}p1, p2;

struct AngleIR {
    int angle;
    int incx;
    int incy;
}zero, thirty, fourtyfive, sixty;

struct AngleIR* angles[] = {&zero,&thirty,&fourtyfive,&sixty};

struct Ball {
  int x;
  int y;
  int ccx;
  int ccy;
  //left = 0, right = 1
  int chd;
  //up = 1, down = 0
  int cvd;
  struct AngleIR* angle;
}b;

void initPaddles() {
    p1.x=1;
    p2.x=118;

    p1.ys=12;
    p2.ys=12;
}

void initAngles() {
    zero.incx = -1;
    zero.incy = 0;
    zero.angle = 0;

    thirty.incx = 3;
    thirty.incy = 1;
    thirty.angle = 30;

    fourtyfive.incx = 1;
    fourtyfive.incy = 1;
    fourtyfive.angle = 45;

    sixty.incx = 1;
    sixty.incy = 3;
    sixty.angle = 60;
}

void initBall() {
    b.x=59;
    b.y=15;
    b.ccx=0;
    b.ccy=0;
    b.chd=0;
    b.cvd=1;
    b.angle=&zero;
}

void draw();
int isPaddleHere(struct Paddle*,int,int);
char getPixelToPrint(int,int);
void processInput(int c);
int isBoundary(int,int);
int isBall(int,int);
void updateBall();

int main() {

    initPaddles();
    initAngles();
    initBall();
    int k=0;

    while(1) {
        if (_kbhit()) {
            int c = _getch();
            processInput(c);
        }
        draw();
        updateBall();
        printf("\033[%d;%dH",0,0);
    }
    return 0;
}

/**
W - Player 1 Paddle Up
S - Player 1 Paddle Down
Numpad 8 - Player 2 Paddle Up
Numpad 5 - Player 2 Paddle Down
*/
void processInput(int c) {
    if (c=='w')if(p1.ys>1)p1.ys--;
    if (c=='s')if(p1.ys<24)p1.ys++;
    if (c=='8')if(p2.ys>1)p2.ys--;
    if (c=='5')if(p2.ys<24)p2.ys++;
}


void draw() {
    for(int y=0;y<30;y++) {
        for(int x=0;x<120;x++) {
            printf("%c",getPixelToPrint(x,y));
        }
    }
}

char getPixelToPrint(int x, int y) {
    if (isPaddleHere(&p1,x,y)||isPaddleHere(&p2,x,y)) return '*';
    if (isBall(x,y))return 'O';
    if (isBoundary(x,y)) return '#';
    return ' ';
}

int isPaddleHere(struct Paddle* p, int x, int y) {
    if ((p->x)!=x)return 0;
    if ((p->ys)>y)return 0;
    if ((p->ys)+5<=y)return 0;
    return 1;
}

int isBoundary(int x, int y) {
    if (y==0||y==29)return 1;
    if (y>=1&&y<=7||y<=29&&y>=22) {
        if(x==0||x==119)return 1;
    }
    return 0;
}

int isBall(int x,int y) {
    if (x==b.x&&y==b.y)return 1;
    return 0;
}

void updateBall() {
    int dir = b.chd*1000 + b.cvd*100 + b.angle->angle;
    switch(dir){
        case 0:
        case 100:
            b.x--;
            break;
        case 45:
            b.x--;
            b.y--;
            break;
        case 145:
            b.x--;
            b.y++;
            break;
        case 1000:
            b.x++;
            break;
        case 1045:
            b.x++;
            b.y--;
            break;
        case 1100:
            b.x++;
            break;
        case 1145:
            b.x++;
            b.y++;
            break;
        case 30:
            b.x--;
            b.ccx++;
            b.ccx=b.ccx%3;
            countCh(&b.ccx,0,&b.y);
            break;
        case 60:
            b.y--;
            b.ccy++;
            b.ccy=b.ccy%3;
            countCh(&b.ccy,0,&b.x);
            break;
        case 130:
            b.x--;
            b.ccx++;
            b.ccx=b.ccx%3;
            countCh(&b.ccx,1,&b.y);
            break;
        case 160:
            b.y++;
            b.ccy++;
            b.ccy=b.ccy%3;
            countCh(&b.ccy,0,&b.x);
            break;
        case 1030:
            b.x++;
            b.ccx++;
            b.ccx=b.ccx%3;
            countCh(&b.ccx,0,&b.y);
            break;
        case 1060:
            b.y--;
            b.ccy++;
            b.ccy=b.ccy%3;
            countCh(&b.ccy,1,&b.x);
            break;
        case 1130:
            b.x++;
            b.ccx++;
            b.ccx=b.ccx%3;
            countCh(&b.ccx,1,&b.y);
            break;
        case 1160:
            b.y++;
            b.ccy++;
            b.ccy=b.ccy%3;
            countCh(&b.ccy,1,&b.x);
            break;
    }

    if(isBoundary(b.x, b.y) || isPaddleHere(&p1, b.x, b.y) || isPaddleHere(&p2, b.x, b.y)) {
        b.ccx=0;
        b.ccy=0;

        //takes random angle, if we are at a boundary angle doesn't change until its 0 else it changes into a random angle
        int ridx = (rand()%4);
        if(isBoundary(b.x,b.y)) {
            if(ridx==0)ridx++;
            if(b.angle->angle==0)b.angle=angles[ridx];
        } else {
            b.angle=angles[ridx];
        }

        //necessarily changes the vertical direction of the ball
        if(!b.cvd) {
            b.cvd=1;
            if(isBoundary(b.x,b.y)&&(b.y==0||b.y==29))b.y++;
        } else if (b.cvd) {
            b.cvd=0;
            if(isBoundary(b.x,b.y)&&(b.y==0||b.y==29))b.y--;
        }

        //Changes horizontal direction of the ball, only on boundaries of the vertical edges
        if(isBoundary(b.x,b.y)&&(b.x==0||b.x==119)) {
            if(!b.chd) {
                b.x++;
                b.chd=1;
            }
            else if (b.chd) {
                b.x--;
                b.chd=0;
            }
        }

        //change horizontal direction if its a paddle
        if(isPaddleHere(&p1,b.x,b.y) || isPaddleHere(&p2,b.x,b.y)){
            if(!b.chd) {
                b.chd=1;
                b.x++;
            } else if (b.chd) {
                b.chd=0;
                b.x--;
            }
        }

        updateBall();
    }
}

void countCh(int *ccv, int pm, int *u) {
    if ((*ccv)==2) {
        (*ccv)=0;
        if(pm==1)(*u)=(*u)+1;
        else (*u)=(*u)-1;
    }
}
/**
countCh(ccv,2,+/-);
hdc(chd,cc); //1/0 change if * else no
chd cvd angle   x                   y                   nextChd     nextCvd     nextAngle
0   0   00      -1                  0                   hdc(chd,cc) 1           rand()
0   0   30      -1                  countCh(ccx,2,-)    hdc(chd,cc) 1           rand()
0   0   45      -1                  -1                  hdc(chd,cc) 1           rand()
0   0   60      countCh(ccy,2,-)    -1                  hdc(chd,cc) 1           rand()
0   1   00      -1                  0                   hdc(chd,cc) 0           rand()
0   1   30      -1                  countCh(ccx,2,+)    hdc(chd,cc) 0           rand()
0   1   45      -1                  1                   hdc(chd,cc) 0           rand()
0   1   60      countCh(ccy,2,-)    1                   hdc(chd,cc) 0           rand()
1   0   00      1                   0                   hdc(chd,cc) 1           rand()
1   0   30      1                   countCh(ccx,2,-)    hdc(chd,cc) 1           rand()
1   0   45      1                   -1                  hdc(chd,cc) 1           rand()
1   0   60      countCh(ccy,2,+)    -1                  hdc(chd,cc) 1           rand()
1   1   00      1                   0                   hdc(chd,cc) 0           rand()
1   1   30      1                   countCh(ccx,2,+)    hdc(chd,cc) 0           rand()
1   1   45      1                   1                   hdc(chd,cc) 0           rand()
1   1   60      countCh(ccy,2,+)    1                   hdc(chd,cc) 0           rand()




angle  incx  incy
0       -1    0
30       3    1
45       1    1
60       1    3
*/
