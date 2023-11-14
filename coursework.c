#include "graphics.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

const int width = 600;
const int height = 600;
const int scale = 50;
const int number_marker = 4;
const int number_block = 5;

typedef enum {
    north = 0,
    east = 1,
    south = 2,
    west = 3
} direction;

void drawGrid(void){
    background();
    setColour(black);
    for (int x = 1; x < 12; x++){
        drawLine(50, x * 50, 550, x * 50); // row
        drawLine(x * 50, 50, x * 50, 550); // column
    }
}

void homeSquare(int x, int y){
    background();
    setColour(blue);
    fillRect(x * 50, y * 50, 50, 50);
}

int in(int *a,int n,int num)
{
    for(int i=1;i < number_marker;++i){
        if(a[i*2-num] == n){
            return true;
        }
    } 
    return false;
}

// Ensure that markers, blocks and homesquare not overlap
int compareArray(int array1[], int array2[], int size1, int size2) {
    for (int i = 0; i < size1; i += 2) {
        for (int j = 0; j < size2; j += 2) {
            if (array1[i] == array2[j] && array1[i + 1] == array2[j + 1]) {
                return 1; // Overlap found
            }
        }
    }
    return 0; // No overlap
}

void randomCommon(int coords_array[],int number,int initialX,int initialY){
    for (int i = 1;i<=number;++i){
        coords_array[i*2-2] = 0;
        coords_array[i*2-1] = 0;
        while (coords_array[i*2-2]<=1||coords_array[i*2-2]>=10||coords_array[i*2-1]<=1||coords_array[i*2-1]>=10){
            int x = rand()%10+1;
            int y = rand()%10+1;
            if((in(coords_array,x,2)||in(coords_array,y,1))||(x==initialX&&y==initialY)){
                continue;
            }
            coords_array[i*2-2]=x;
            coords_array[i*2-1]=y;
        }
    }
}

void markerRandom(int marker_coords[],int initialX,int initialY){
    randomCommon(marker_coords,number_marker,initialX,initialY);
}

void blockRandom(int marker_coords[], int block_coords[], int initialX, int initialY) {
    randomCommon(block_coords, number_block, initialX, initialY);

    while (compareArray(marker_coords, block_coords, number_marker * 2, number_block * 2)) {
        randomCommon(block_coords, number_block, initialX, initialY);
    }
}



void drawAllMarkers(int marker_coords[]){
    setRGBColour(192, 192, 192);
    for (int i = 1;i<=number_marker;i++){
        fillArc(marker_coords[i*2-2]*scale, marker_coords[i*2-1]*scale, 50, 50,0,360);
    }
}

void drawAllBlocks(int block_coords[]){
    setColour(black);
    for (int i = 1;i<=number_block;i++){
        fillRect(block_coords[i*2-2]*scale,block_coords[i*2-1]*scale,50,50);
    }
}

void makeAnimation(int initialX,int initialY,int robot_x,int robot_y,direction robot_direction,int marker_coords[],int block_coords[]){
    clear();
    drawGrid();
    homeSquare(initialX, initialY);
    drawAllMarkers(marker_coords);
    drawAllBlocks(block_coords);
}

void MarkerMove(int x, int y,int n,int marker_coords[]){
    setRGBColour(192, 192, 192);
    fillArc(x*scale, y*scale, 50, 50,0,360);
    marker_coords[n*2-2]=marker_coords[99]; // delete the position of the collected marker
    marker_coords[n*2-1]=marker_coords[99];
}

void robotDisplay(int x, int y, direction robot_direction){
    setColour(green);
    switch (robot_direction){
    case north: // 0
        fillPolygon(3, (int[]){x * scale, (x + 1) * scale, x * scale + scale / 2}, (int[]){(y + 1) * scale, (y + 1) * scale, y * scale});
        break;
    case east: // 1
        fillPolygon(3, (int[]){x * scale, x * scale, (x + 1) * scale}, (int[]){y * scale, (y + 1) * scale, y * scale + scale / 2});
        break;
    case south: // 2
        fillPolygon(3, (int[]){x * scale, (x + 1) * scale, x * scale + scale / 2}, (int[]){y * scale, y * scale, (y + 1) * scale});
        break;
    case west: // 3
        fillPolygon(3, (int[]){x * scale, (x + 1) * scale, (x + 1) * scale}, (int[]){y * scale + scale / 2, y * scale, (y + 1) * scale});
        break;
    }
}

void updatePosition(int *robot_x, int *robot_y, int direction){
    switch (direction){
    case north:
        *robot_y -= 1;
        break;
    case east:
        *robot_x += 1;
        break;
    case south:
        *robot_y += 1;
        break;
    case west:
        *robot_x -= 1;
        break;
    }
}

int canMoveForward(int robot_x, int robot_y, direction robot_direction){
    if ((robot_x == 1 && robot_direction == west) || (robot_x == 10 && robot_direction == east) || (robot_y == 1 && robot_direction == north) || (robot_y == 10 && robot_direction == south))
    {
        return false;
    }
    return true;
}

void forward(int *robot_x, int *robot_y, direction robot_direction, int initialX, int initialY,int marker_coords[],int block_coords[]){
    updatePosition(robot_x, robot_y, robot_direction);
    makeAnimation(initialX,initialY,*robot_x,*robot_y,robot_direction,marker_coords,block_coords);
    robotDisplay(*robot_x, *robot_y, robot_direction);
}

void right(int robot_x, int robot_y, direction *robot_direction, int initialX, int initialY,int marker_coords[],int block_coords[]){
    makeAnimation(initialX,initialY,robot_x,robot_y,*robot_direction,marker_coords,block_coords);
    switch (*robot_direction)
    {
    case north:
        robotDisplay(robot_x, robot_y, *robot_direction = east);
        break;
    case east:
        robotDisplay(robot_x, robot_y, *robot_direction = south);
        break;
    case south:
        robotDisplay(robot_x, robot_y, *robot_direction = west);
        break;
    case west:
        robotDisplay(robot_x, robot_y, *robot_direction = north);
        break;
    }
}

void left(int robot_x, int robot_y, direction *robot_direction, int initialX, int initialY,int marker_coords[],int block_coords[]){
    makeAnimation(initialX,initialY,robot_x,robot_y,*robot_direction,marker_coords,block_coords);
    right(robot_x, robot_y, robot_direction, initialX, initialY,marker_coords,block_coords);
    right(robot_x, robot_y, robot_direction, initialX, initialY,marker_coords,block_coords);
    right(robot_x, robot_y, robot_direction, initialX, initialY,marker_coords,block_coords);
    sleep(200);
}

int isAtMarker(int robot_x, int robot_y, int *n, int *marker_x, int *marker_y, int marker_coords[]) {
    for (int i = 0; i < number_marker; i++) {
        if (robot_x == marker_coords[i * 2] && robot_y == marker_coords[i * 2 + 1]) {
            if (i == *n - 1) {
                continue;
            }
            *n = i + 1;
            return false;
        }
    }
    return true;
}


// store the route of robot when finding marker
int push(int* robot_route_x,int* robot_route_y,direction* robot_route_direction,int top,int robot_x,int robot_y,int robot_direction){
    robot_route_x[++top]=robot_x;
    robot_route_y[top]=robot_y;
    robot_route_direction[top]=robot_direction;
    return top;
}

// pop out the route of robot to let it reverse motion and back to homesquare
int pop(int *robot_route_x,int *robot_route_y,direction *robot_route_direction,int top,int *robot_x,int *robot_y,direction *robot_direction){
    *robot_x = robot_route_x[top];
    *robot_y = robot_route_y[top];
    switch (robot_route_direction[top--]){
    case north:
        *robot_direction = south;
        break;
    case east:
        *robot_direction = west;
        break;
    case south:
        *robot_direction = north;
        break;
    case west:
        *robot_direction = east;
        break;
    }
    return top;
}

int isEmpty(int top){
    if (top==-1) {
        return false;
    } return true;
}

void isNearMarker(int *robot_x,int *robot_y,direction *robot_direction,int marker_coords[],int initialX, int initialY,int block_coords[],int *robot_route_x,int *robot_route_y,direction *robot_route_direction,int top){
    if (*robot_x==1||*robot_x==10){
        for(int i =1;i<=number_marker;i++){
            if (marker_coords[i*2-1]==*robot_y){
                right(*robot_x, *robot_y, robot_direction, initialX, initialY,marker_coords,block_coords);
                top = push(robot_route_x,robot_route_y,robot_route_direction,top,*robot_x,*robot_y,*robot_direction);
                sleep(200);
            }
        }
    } else if (*robot_y==1||*robot_y==10){
        for(int i =1;i<=number_marker;i++){
            if (marker_coords[i*2-2]==*robot_x){
                right(*robot_x, *robot_y, robot_direction, initialX, initialY,marker_coords,block_coords);
                top = push(robot_route_x,robot_route_y,robot_route_direction,top,*robot_x,*robot_y,*robot_direction);
                sleep(200);
            }
        }
    }
}

void nextIsBlock(int *robot_x,int *robot_y,direction *robot_direction,int marker_coords[],int initialX, int initialY,int block_coords[],int *robot_route_x,int *robot_route_y,direction *robot_route_direction,int top){
    for (int i = 1;i<=number_block;i++){
        switch (*robot_direction)
        {
        case north:
            if (block_coords[i*2-2]==*robot_x&&block_coords[i*2-1]==*robot_y-1){
                left(*robot_x, *robot_y, robot_direction, initialX, initialY,marker_coords,block_coords);
            } break;
        case east:
            if (block_coords[i*2-2]==*robot_x+1&&block_coords[i*2-1]==*robot_y){
                left(*robot_x, *robot_y, robot_direction, initialX, initialY,marker_coords,block_coords);
            } break;
        case south:
            if (block_coords[i*2-2]==*robot_x&&block_coords[i*2-1]==*robot_y+1){
                left(*robot_x, *robot_y, robot_direction, initialX, initialY,marker_coords,block_coords);
            } break;
        case west:
            if (block_coords[i*2-2]==*robot_x-1&&block_coords[i*2-1]==*robot_y){
                left(*robot_x, *robot_y, robot_direction, initialX, initialY,marker_coords,block_coords);
            } break;
        }
    }
    top = push(robot_route_x,robot_route_y,robot_route_direction,top,*robot_x,*robot_y,*robot_direction);
}

int main(int argc, char **argv){
    setWindowSize(width, height);

    // The default values if the command line arguments are not given.
    int initialX = 6;
    int initialY = 5;
    int initialDirection = 0;
    if (argc == 4) // Four arguments were typed
    {
        initialX = atoi(argv[1]);
        initialY = atoi(argv[2]);
        initialDirection = atoi(argv[3]);
    }

    int counter = 4;
    int marker_coords[100];
    int block_coords[100];
    srand((unsigned)time(NULL));
    markerRandom(marker_coords,initialX,initialY);
    blockRandom(marker_coords,block_coords,initialX,initialY);

    int robot_x = initialX;
    int robot_y = initialY;
    direction robot_direction = initialDirection;

    int robot_route_x[1000];
    int robot_route_y[1000];
    direction robot_route_direction[1000];
    int top = -1;

    int n;
    int marker_x;
    int marker_y;

    while (counter!=0){
        makeAnimation(initialX,initialY,robot_x,robot_y,robot_direction,marker_coords,block_coords);
        robotDisplay(initialX, initialY, initialDirection);
        sleep(200);
        
        while (isAtMarker(robot_x, robot_y,&n,&marker_x,&marker_y,marker_coords)){
            top = push(robot_route_x,robot_route_y,robot_route_direction,top,robot_x,robot_y,robot_direction);
            isNearMarker(&robot_x,&robot_y,&robot_direction,marker_coords,initialX,initialY,block_coords,robot_route_x,robot_route_y,robot_route_direction,top);
            nextIsBlock(&robot_x,&robot_y,&robot_direction,marker_coords,initialX,initialY,block_coords,robot_route_x,robot_route_y,robot_route_direction,top);
            if (canMoveForward(robot_x, robot_y, robot_direction)){
                forward(&robot_x, &robot_y, robot_direction, initialX, initialY,marker_coords,block_coords);
            }else{
                right(robot_x, robot_y, &robot_direction, initialX, initialY,marker_coords,block_coords);
            }
            sleep(200);
        }

        right(robot_x, robot_y, &robot_direction, initialX, initialY,marker_coords,block_coords);
        sleep(200);
        right(robot_x, robot_y, &robot_direction, initialX, initialY,marker_coords,block_coords);
        sleep(200);

        while(isEmpty(top)){
            top = pop(robot_route_x,robot_route_y,robot_route_direction,top,&robot_x,&robot_y,&robot_direction);
            MarkerMove(robot_x,robot_y,n,marker_coords);
            makeAnimation(initialX,initialY,robot_x,robot_y,robot_direction,marker_coords,block_coords);
            MarkerMove(robot_x,robot_y,n,marker_coords);
            robotDisplay(robot_x, robot_y, robot_direction);
            sleep(200);
        }
        counter--;
    }
    return 0;
}
