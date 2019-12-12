#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int rand_row();
int rand_column();
int isDuplicate(int, int *, int);

int row, column;
int sheriffNum, copsNum=0, maxCops=0;

int main() {
    printf("Please enter dimensions of game environment as follow:\nrow column\n");
    scanf("%d %d", &row, &column);
    printf("Please enter number of Sheriff:\n");
    scanf("%d", &sheriffNum);
    int sheriff[sheriffNum];
    for(int i=0;i<sheriffNum;i++){
        printf("Please enter number of Cops in s sheriff:\n");
        scanf("%d", &sheriff[i]);
        copsNum+= sheriff[i];
        maxCops= fmax(maxCops, sheriff[i]);
    }
    //define a array for duplicate check
    int poses[copsNum];

    int sheriffStation[sheriffNum][maxCops];
    static int count=0;
    for(int i=0;i<sheriffNum;i++){
        for(int j=0;j<sheriff[i];j++){
            sheriffStation[i][j]=rand_row()*1000+ rand_column();
            //printf("%d\n", sheriffStation[i][j]);
            poses[count]=sheriffStation[i][j];
            count++;
            if (isDuplicate(count, poses, sheriffStation[i][j])){
                //printf("her\n");
                j--;
            }
        }
    }
    /*printf("\n");
    for(int i=0;i<sheriffNum;i++){
        for(int j=0;j<sheriff[i];j++){
            printf("%d\n", sheriffStation[i][j]);
        }
    }*/
    int robberPos;
    do{
        robberPos= rand_row()*1000+ rand_column();
    }while(isDuplicate(count+1, poses, robberPos));
    //printf("\n%d\n", robberPos);
    return 0;
}

int rand_row(){
    //srand(time(NULL));
    return ((float)rand()/RAND_MAX)*(row-1)+1;
}

int rand_column(){
    //srand(time(NULL));
    return ((float)rand()/RAND_MAX)*(column-1)+1;
}

int isDuplicate(int count, int *ptr, int value){
    for(int i=0;i<count-1;i++){
        //printf("%d, %d\n", *ptr, value);
        if(*(ptr+i)==value){
//            printf("ter\n");
            return 1;
        }
    }
    return 0;
}