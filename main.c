#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

int row, column;
int sheriffNum, copsNum=0, maxCops=0;
//sheriffNum and copsNum and maxCops are global variable

int rand_row();
int rand_column();
int isDuplicate(int, int *, int, int);
int RandMove(int);
int firstEndCondition(int *, int);
int robberSeen(int , int);
int smartMove(int, int);
void visualMap(int *,int *, int);

int main() {
    //for random generating!
    srand(time(NULL));

    printf("Please enter dimensions of game environment as follow:\nrow column\n");
    scanf("%d %d", &row, &column);
    printf("Please enter number of Sheriff:\n");
    scanf("%d", &sheriffNum);
    int sheriff[sheriffNum];
    int sheriffState[sheriffNum];
    for(int i=0;i<sheriffNum;i++){
        printf("Please enter number of Cops in s sheriff:\n");
        scanf("%d", &sheriff[i]);
        copsNum+= sheriff[i];
        maxCops= fmax(maxCops, sheriff[i]);
    }
    //define a array for duplicate check
    int poses[copsNum];
    int sheriffStation[sheriffNum][maxCops];

    int robberPos=0;
    static int count=0;
    for(int i=0;i<sheriffNum;i++){
        for(int j=0;j<sheriff[i];j++){
            sheriffStation[i][j]=rand_row()*1000+ rand_column();
            //printf("%d\n", sheriffStation[i][j]);
            poses[count]=sheriffStation[i][j];
            count++;
            if (isDuplicate(count, poses, sheriffStation[i][j], robberPos)){
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
    do{
        robberPos= rand_row()*1000+ rand_column();
    }while(isDuplicate(count+1, poses, robberPos, robberPos));
    //printf("\n%d\n", robberPos);

    int round=1;
    while(1){
        printf("round %d:\n", round);
        round++;
        for(int i=0;i<sheriffNum;i++){
            for(int j=0;j<sheriff[i];j++){
                if(robberSeen(sheriffStation[i][j], robberPos)){
                    sheriffState[i]=1;
                }else{
                    sheriffState[i]=0;
                }
            }
        }

        int formerRobberPos=robberPos;
        robberPos= RandMove(robberPos);
        //printf("%d\n",robberPos);
        if(firstEndCondition(poses, robberPos)){
            printf("Arrested the robber");
            return 0;
        }

        printf("\e[1;1H\e[2J");
        visualMap(poses, sheriff, robberPos);
        sleep(1);

        int counter=0;
        for(int i=0;i<sheriffNum;i++){
            for(int j=0;j<sheriff[i];j++){
                if(sheriffState[i]==1){
                    do{
                        sheriffStation[i][j]= smartMove(sheriffStation[i][j], formerRobberPos);
                        poses[counter]= sheriffStation[i][j];
                    }while(isDuplicate(counter, poses, poses[counter], robberPos));
                }else{
                    do{
                        sheriffStation[i][j]= RandMove(sheriffStation[i][j]);
                        poses[counter]= sheriffStation[i][j];
                    }while(isDuplicate(counter, poses, poses[counter], robberPos));
                }
                counter++;
            }
        }
        printf("\n");
        if(firstEndCondition(poses, robberPos)){
            printf("Arrested the robber");
            return 0;
        }

        /*printf("\n");
        //for loop for test
        for(int i=0;i<sheriffNum;i++){
            printf("%d\n", sheriffState[i]);
        }
        printf("\n");
        for(int i=0;i<counter;i++){
            printf("%d\n", poses[i]);
        }*/
    }
    return 0;
}

int rand_row(){
    return ((float)rand()/RAND_MAX)*(row-1)+1;
}

int rand_column(){
    return ((float)rand()/RAND_MAX)*(column-1)+1;
}

int isDuplicate(int count, int *ptr, int value, int robberPos){
    bool flag= true;
    for(int i=0;i<count-1;i++){
        //printf("%d, %d\n", *ptr, value);
        /*if((fabs(ptr[i]%1000 - robberPos%1000)<2 || fabs(ptr[i]/1000 - robberPos/1000)<2) && flag){
            flag= false;
            return 1;
        }*/
        if(*(ptr+i)==value){
//            printf("ter\n");
            return 1;
        }
    }
    return 0;
}

int RandMove(int Pos) {
    here:
    ;
    int move;
    move = (int) (((float) rand() / RAND_MAX) * 8) + 1;
    int colVar = Pos % 1000;
    int rowVar = Pos / 1000;
    switch (move) {
        case 1:
            if (rowVar == 1) {
                goto here;
            }
            rowVar--;
            break;
        case 2:
            if (colVar == column || rowVar == 1) {
                goto here;
            }
            colVar++;
            rowVar--;
            break;
        case 3:
            if (colVar == column) {
                goto here;
            }
            colVar++;
            break;
        case 4:
            if (colVar == column || rowVar == row) {
                goto here;
            }
            colVar++;
            rowVar++;
            break;
        case 5:
            if (rowVar == row) {
                goto here;
            }
            rowVar++;
            break;
        case 6:
            if (colVar == 1 || rowVar == row) {
                goto here;
            }
            colVar--;
            rowVar++;
            break;
        case 7:
            if (colVar == 1) {
                goto here;
            }
            colVar--;
            break;
        case 8:
            if (colVar == 1 || rowVar == 1) {
                goto here;
            }
            colVar--;
            rowVar--;
            break;
    }
    int res = rowVar * 1000 + colVar;
    return res;
}

int firstEndCondition(int poses[], int robberPos){
    for(int i=0;i<copsNum;i++){
        if(robberPos == poses[i]){
            return 1;
        }
    }
    return 0;
}

int robberSeen(int copsPos, int robPos){
    int colVarRob = robPos % 1000;
    int colVarCops= copsPos % 1000;

    int rowVarRob = robPos / 1000;
    int rowVarCops= copsPos / 1000;

    if((fabs(colVarCops-colVarRob)<3 && fabs(rowVarCops-rowVarRob)<3)){
        return 1;
    }else{
        return 0;
    }
    /*if(rowVarCops == rowVarRob){
        return 0;
    }else{
        if((colVarCops-colVarRob)/(rowVarCops-rowVarRob)==1){
            return 1;
        }else{
            return 0;
        }
    }*/
}

int smartMove(int currentPos, int robberPos){
    int colVarRob = robberPos % 1000;
    int colVarCops= currentPos % 1000;

    int rowVarRob = robberPos / 1000;
    int rowVarCops= currentPos / 1000;

    if(colVarRob > colVarCops){
        colVarCops++;
    }else if(colVarRob < colVarCops){
        colVarCops--;
    }

    if(rowVarRob > rowVarCops){
        rowVarCops++;
    }else if(rowVarRob < rowVarCops){
        rowVarCops--;
    }
    int res = rowVarCops*1000+colVarCops;
    return res;
}

void visualMap(int poses[],int sheriff[], int robberPos){
    int colVarRob = robberPos % 1000;
    int rowVarRob = robberPos / 1000;
    int colVarCops;
    int rowVarCops;
    int count =0;
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            bool flag= false;
            count=0;
            for(int k=0;k<copsNum;k++){
                colVarCops= poses[k] % 1000;
                rowVarCops= poses[k] / 1000;
                if(colVarCops==j+1 && rowVarCops==i+1){
                    while(k>=0){
                        k-=sheriff[count];
                        count++;
                    }
                    flag = true;
                    break;
                }
            }
            if(flag){
                if(j==column-1){
                    if(colVarRob==j+1 && rowVarRob==i+1){
                        printf("D%d:T\n", count);
                    }else{
                        printf("D%d\n", count);
                    }
                }else{
                    if(colVarRob==j+1 && rowVarRob==i+1){
                        printf("D%d:T\t", count);
                    }else{
                        printf("D%d\t", count);
                    }
                }
            }else if(colVarRob==j+1 && rowVarRob==i+1){
                if(j==column-1){
                    printf("T\n");
                }else{
                    printf("T\t");
                }
            }else{
                if(j==column-1){
                    printf("*\n");
                }else{
                    printf("*\t");
                }
            }
        }
    }
}