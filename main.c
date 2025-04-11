#include <stdio.h>
#include <conio.h> // _getch() / _kbhit()

struct Paddle {
    int x; //0 or 119
    int ys; //0-25
}p1, p2;

struct AngleIR {
    int incx;
    int incy;
}zero, thirty, fourtyfive, sixty, ninety;

struct Ball {
  int x;
  int y;
  int chd;
  int cvd;
  AngleIR angle;
}b;

/**
angle  incx  incy
0       -1    0
90       0    -1
30       3    1
45       1    1
60       1    3
*/

void initPaddles() {
    p1.x=1;
    p2.x=118;

    p1.ys=12;
    p2.ys=12;
}

void draw();
int isPaddleHere(struct Paddle*,int,int);
char getPixelToPrint(int,int);
void processInput(int c);
int isBoundary(int,int);
int isBall(int,int);

int main() {
    initPaddles();
    while(1) {
        if (_kbhit()) {
            int c = _getch();
            processInput(c);
        }
        draw();
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
    if (x==59&&y==15)return 1;
    return 0;
}
