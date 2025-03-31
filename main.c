#include <stdio.h>
#include <conio.h> // _getch() / _kbhit()

struct Paddle {
    int x; //0 or 119
    int ys; //0-25
}p1, p2;

void initPaddles() {
    p1.x=0;
    p2.x=119;

    p1.ys=12;
    p2.ys=12;
}

void draw();
int isPaddleHere(struct Paddle*,int,int);
int shouldPrintPixel(int,int);
void processInput(int c);

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
    if (c=='w')if(p1.ys>0)p1.ys--;
    if (c=='s')if(p1.ys<25)p1.ys++;
    if (c=='8')if(p2.ys>0)p2.ys--;
    if (c=='5')if(p2.ys<25)p2.ys++;
}


void draw() {
    for(int y=0;y<30;y++) {
        for(int x=0;x<120;x++) {
            if(shouldPrintPixel(x,y))printf("*");
            else printf(" ");
        }
    }
}

int shouldPrintPixel(int x, int y) {
    if (isPaddleHere(&p1,x,y)||isPaddleHere(&p2,x,y)) return 1;
    if (x==59&&y==14)return 1;
    return 0;
}

int isPaddleHere(struct Paddle* p, int x, int y) {
    if ((p->x)!=x)return 0;
    if ((p->ys)>y)return 0;
    if ((p->ys)+5<=y)return 0;
    return 1;
}
