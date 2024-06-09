#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define EARTH_HEIGHT 32
#define EARTH_WIDTH 32


int matrixPosUp2Down(int i, int j);
int matrixPosDown2Up(int i, int j);
int matrixPosRight2Left(int i, int j);
int matrixPosLeft2Right(int i, int j);
int **boundaryPoint(int earth[], int (*matrixPosition)(int, int), int width, int height, int *length);
int whereIsSpace(int earth[], int width, int height);
void write2file(int **matrix, int length, int flag);


int main (){
    
    int lengthUp2Down = 0;
    int lengthDown2Up = 0;
    int lengthRight2Left = 0;
    int lengthLeft2Right = 0;
    int spaceIs = 0;
    
    int **boundaryPointUp2Down = malloc(EARTH_WIDTH*sizeof(int*));
    for (int i = 0; i < EARTH_WIDTH; i++) {
        boundaryPointUp2Down[i] = malloc(2*sizeof(int));
    }

    int **boundaryPointDown2Up = malloc(EARTH_WIDTH*sizeof(int*));
    for (int i = 0; i < EARTH_WIDTH; i++) {
        boundaryPointDown2Up[i] = malloc(2*sizeof(int));
    }

    int **boundaryPointRight2Left = malloc(EARTH_HEIGHT*sizeof(int*));
    for (int i = 0; i < EARTH_HEIGHT; i++) {
        boundaryPointRight2Left[i] = malloc(2*sizeof(int));
    }

    int **boundaryPointLeft2Right = malloc(EARTH_HEIGHT*sizeof(int*));
    for (int i = 0; i < EARTH_HEIGHT; i++) {
        boundaryPointLeft2Right[i] = malloc(2*sizeof(int));
    }

    
    int earth[1024];

    while(1){
        FILE *fp;
        char *filename = "imageData.txt";
        char line[1024];
        int data[1024];
        int scan = 0;

        fp = fopen(filename, "r");

        while (fgets(line, 1024, fp) != NULL) {
            sscanf(line, "%d", &data[scan]);
            scan++;
        }

        fclose(fp);
        int k = 0;
        
        for(int j = EARTH_WIDTH-1; j>-1; j--){
            for(int i = 0; i < EARTH_HEIGHT; i++){
                earth[matrixPosLeft2Right(i ,j)] = data[k];
                k++;
            }
        }


        // Direction of space in the photo
        spaceIs = whereIsSpace(earth, EARTH_WIDTH, EARTH_HEIGHT);
        printf("Burada %d\n", spaceIs);
        
        if(spaceIs == 0){       // Up to Down
            boundaryPointUp2Down = boundaryPoint(earth, matrixPosUp2Down, EARTH_WIDTH, EARTH_HEIGHT, &lengthUp2Down);
            printf("Up to Down\n");
            for (int i = 0; i<lengthUp2Down; i++) {
                printf("%d", boundaryPointUp2Down[i][0]);
                printf(", %d;\n", boundaryPointUp2Down[i][1]);
                write2file(boundaryPointUp2Down, lengthUp2Down, spaceIs);
            }
        }
        else if(spaceIs == 1){  // Down to Up
            boundaryPointDown2Up = boundaryPoint(earth, matrixPosDown2Up, EARTH_WIDTH, EARTH_HEIGHT, &lengthDown2Up);
            printf("Down to Up\n");
            for (int i = 0; i<lengthDown2Up; i++) {
                printf("%d", boundaryPointDown2Up[i][0]);
                printf(", %d;\n", boundaryPointDown2Up[i][1]);
                write2file(boundaryPointDown2Up, lengthDown2Up, spaceIs);
            }
        }
        else if(spaceIs == 2){  // Right to Left
            boundaryPointRight2Left = boundaryPoint(earth, matrixPosRight2Left, EARTH_HEIGHT, EARTH_WIDTH, &lengthRight2Left);
            printf("Right to Left\n");
            for (int i = 0; i<lengthRight2Left; i++) {
                printf("%d", boundaryPointRight2Left[i][0]);
                printf(", %d;\n", boundaryPointRight2Left[i][1]);
                write2file(boundaryPointRight2Left, lengthRight2Left, spaceIs);
            }
        }
        else if(spaceIs == 3){  // Left to Right
            boundaryPointLeft2Right = boundaryPoint(earth, matrixPosLeft2Right, EARTH_HEIGHT, EARTH_WIDTH, &lengthLeft2Right);
            printf("Left to Right\n");
            for (int i = 0; i<lengthLeft2Right; i++) {
                printf("%d", boundaryPointLeft2Right[i][0]);
                printf(", %d;\n", boundaryPointLeft2Right[i][1]);
                write2file(boundaryPointLeft2Right, lengthLeft2Right, spaceIs);
            }
        }
    Sleep(1000);
    }
    
    

    
    return 0;
 }




int matrixPosUp2Down(int i, int j){

    int c = EARTH_WIDTH*i + j;
    return c;
}

int matrixPosDown2Up(int i, int j){

    int c = EARTH_WIDTH*(EARTH_HEIGHT-1-i) + j;
    return c;
}


int matrixPosRight2Left(int i, int j){

    int c = EARTH_HEIGHT*j + (EARTH_WIDTH-1-i);
    return c;
}

int matrixPosLeft2Right(int i, int j){

    int c = EARTH_HEIGHT*j + i;
    return c;
}



int **boundaryPoint(int earth[], int (*matrixPosition)(int, int), int width, int height, int *length){
    
    int E = 0;
    int S = 0;
    int beta = 0;
    int i = 0;
    int k = 1;
    int boundaryCount=0;
    int **points = malloc(width*sizeof(int *));
    for (int i = 0; i < width; i++) {
        points[i] = malloc(2*sizeof(int));
    }

    for (int j = 0; j<width; j++){
        k=1;
        i=0;
        S = (earth[(*matrixPosition)(0,j)] + earth[(*matrixPosition)(1,j)] + earth[(*matrixPosition)(2,j)])/3;
        E = (earth[(*matrixPosition)(28,j)] + earth[(*matrixPosition)(29,j)] + earth[(*matrixPosition)(30,j)])/3;
        beta = 1.3*((0.3*(E-S))+S);

        if(beta < 5){
            beta = 5;
        }

        while (k==1) {

            if ((earth[(*matrixPosition)(i,j)] > beta) && (earth[(*matrixPosition)(i+1,j)] > beta) && (earth[(*matrixPosition)(i+2,j)] > beta) && (earth[(*matrixPosition)(i+3,j)] > beta)){
                
                if ((earth[(*matrixPosition)(i-1,j)]) > (0.1*earth[(*matrixPosition)(i,j)])){
                    points[boundaryCount][0] = i-1;
                    points[boundaryCount][1] = j;
                    k=0;
                    boundaryCount++;
                }
                else{
                    points[boundaryCount][0] = i;
                    points[boundaryCount][1] = j;
                    k=0;
                    boundaryCount++;
                }
            }
            else{
                i++;
                if (i > (height-2)){
                    k=0;
                }
            }
        }
    }

    *length = boundaryCount;
    
    return points;
}

int whereIsSpace(int earth[], int width, int height){

    /*
    0 = space at the top
    1 = space at the bottom
    2 = space at the right
    3 = space at the left
    */

    int upperRow = 0;
    int lowerRow = 0;
    int rightColumn = 0;
    int leftColumn = 0;
    int numbers[4];
    int smallest = 255;
    int result = 4;

    for (int i = 0; i<width; i++){

        upperRow = upperRow + earth[matrixPosRight2Left(i, 0)];
        upperRow = upperRow + earth[matrixPosRight2Left(i, 1)];

        lowerRow = lowerRow + earth[matrixPosRight2Left(i, height-2)];
        lowerRow = lowerRow + earth[matrixPosRight2Left(i, height-1)];
    }
    upperRow = upperRow/(2*width);
    lowerRow = lowerRow/(2*width);

    for (int i = 0; i<height; i++){

        leftColumn = leftColumn + earth[matrixPosUp2Down(i, 0)];
        leftColumn = leftColumn + earth[matrixPosUp2Down(i, 1)];

        rightColumn = rightColumn + earth[matrixPosUp2Down(i, width-2)];
        rightColumn = rightColumn + earth[matrixPosUp2Down(i, width-1)];
    }
    rightColumn = rightColumn/(2*height);
    leftColumn = leftColumn/(2*height);

    numbers[0] = upperRow;
    numbers[1] = lowerRow;
    numbers[2] = rightColumn;
    numbers[3] = leftColumn;

    for (int i = 0; i < 4; i++) {
      if (numbers[i] <= smallest) {
         smallest = numbers[i];
         result = i;
      }
   }

    printf("0. upperRow = %d\n", upperRow);
    printf("1. lowerRow = %d\n", lowerRow);
    printf("2. rightColumn = %d\n", rightColumn);
    printf("3. leftColumn = %d\n", leftColumn);

    return result;

}


void write2file(int **matrix, int length, int flag){

    FILE *fptr = fopen("C:\\Users\\Osmno\\Desktop\\Lotusat\\boundaryPoints.txt", "w");
    
    fprintf(fptr,"%d, %d\n",flag, flag);
    
    for (int i=0; i<length; i++){

        fprintf(fptr,"%d, ",matrix[i][0]);
        fprintf(fptr,"%d\n",matrix[i][1]);
    }

    fclose(fptr);
}

