#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

void print_environment(int sheriffnum ,int maxCops, int[sheriffnum][maxCops][2], int[]);
int rand_row();
int rand_column();
int isDuplicate(int maxCops,int[][maxCops][2], int, int);
int isDuplicateRobber(int sheriff,int maxCops, int[][maxCops][2], int, int);
void RobberMove(int[], int , int);

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
    int copsPos[sheriffNum][maxCops][2];

    for(int i=0;i<sheriffNum;i++){
        for(int j=0;j<sheriff[i];j++){
            copsPos[i][j][0]=rand_row();
            copsPos[i][j][1]=rand_column();
            if (!isDuplicate(maxCops, copsPos[sheriffNum][maxCops][2], i, j))continue;
            j--;
        }
    }
    int robber[2];
    do{
        robber[0]=rand_row();
        robber[1]=rand_column();
    }while(isDuplicateRobber(sheriffNum, maxCops, copsPos[sheriffNum][maxCops][2], robber[0], robber[1]));

    system("cls");
    print_environment(sheriffNum, maxCops, copsPos[sheriffNum][maxCops][2], sheriff[sheriffNum]);

    
    return 0;
}

void print_environment(int sheriffN, int maxCops, int copsPos[sheriffNum][maxCops][2], int sheriff[]){
    static int round=1;
    printf("Round %d\n", round);
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            bool flag = false;
            for(int k=0;k<sheriffN;k++){
                for(int z=0; sheriff[k] > z; z++){
                    if(copsPos[k][z][0]==j && copsPos[k][z][1]==i){
                        flag= true;
                        if(j==column-1){
                            printf("D%d\n", k+1);
                        }else{
                            printf("D%d\t", k+1);
                        }
                        break;
                    }else{
                        flag=false;
                    }
                }
                if(flag){
                    break;
                }
            }
            if(!flag){
                if(j==column-1){
                    printf(" *\n");
                }else{
                    printf(" *\t");
                }
            }
        }
    }
    round++;
}

int rand_row(){
    srand(time(NULL));
    return ((float)rand()/RAND_MAX)*(row-1);
}

int rand_column(){
    srand(time(NULL));
    return ((float)rand()/RAND_MAX)*(column-1);
}

int isDuplicate(int maxCops, int arr[][maxCops][2], int lastIndexi,int lastIndexj){
    for(int i=0;i<=lastIndexi;i++){
        for(int j=0;j<maxCops;j++){
            if(i==lastIndexi && j==lastIndexj){
                break;
            }
            if(arr[i][j][0]==arr[lastIndexi][lastIndexj][0] && arr[i][j][1]==arr[lastIndexi][lastIndexj][1]){
                return 1;
            }
        }
    }
    return 0;
}

int isDuplicateRobber(int sheriff, int maxCops, int arr[sheriff][maxCops][2], int x, int y){
    for(int i=0;i<sheriff;i++){
        for(int j=0;j<maxCops;j++){
            if(arr[i][j][0]==x, arr[i][j][1]==y){
                return 1;
            }
        }
    }
    return 0;
}

void RobberMove(int RobPos[], int row, int column){
    here:
    srand(time(NULL));
    int move;
    move=(int)(((float)rand()/RAND_MAX)*8)+1;
    switch (move){
        case 1:
            if(RobPos[1]==0){
                goto here;
            }
            RobPos[1]++;
            break;
        case 2:
            if(RobPos[0]==column-1 || RobPos[1]==0){
                goto here;
            }
            RobPos[0]++;
            RobPos[1]++;
            break;
        case 3:
            if(RobPos[0]==column-1){
                goto here;
            }
            RobPos[0]++;
            break;
        case 4:
            if(RobPos[0]==column-1 || RobPos[1]==row-1){
                goto here;
            }
            RobPos[0]++;
            RobPos[1]--;
            break;
        case 5:
            if(RobPos[1]==row-1){
                goto here;
            }
            RobPos[1]--;
            break;
        case 6:
            if(RobPos[0]==0 || RobPos[1]==row-1){
                goto here;
            }
            RobPos[0]--;
            RobPos[1]--;
            break;
        case 7:
            if(RobPos[0]==0){
                goto here;
            }
            RobPos[0]--;
            break;
        case 8:
            if(RobPos[0]==0 || RobPos[1]==0){
                goto here;
            }
            RobPos[0]--;
            RobPos[1]++;
            break;
    }
}