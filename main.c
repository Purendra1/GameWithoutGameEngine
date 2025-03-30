#include <stdio.h>
void draw();
int sp(int,int);
int main() {
    while(1) {
        draw();
        printf("\033[%d;%dH",0,0);
    }
    return 0;
}

void draw() {

    for(int y=0;y<30;y++) {
        for(int x=0;x<120;x++) {
            if(sp(x,y))printf("*");
            else printf(" ");
        }
    }
}

int sp(int x, int y) {
    if(x==0||x==119) {
        if(y==0||y==1||y==2||y==3||y==4)return 1;
    }
    if (x==59&&y==2)return 1;
    return 0;
}

