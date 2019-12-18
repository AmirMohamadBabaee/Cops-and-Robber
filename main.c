#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

int row, column;
int sheriffNum, copsNum=0, maxCops=0;
int turn1=0;
//sheriffNum and copsNum and maxCops are global variable

//additional code to clear console screen
//stack over flow
//start
/* Standard error macro for reporting API errors */
#define PERR(bSuccess, api){if(!(bSuccess)) printf("%s:Error %d from %s \
    on line %d\n", __FILE__, GetLastError(), api, __LINE__);}

void cls( HANDLE hConsole )
{
    COORD coordScreen = { 0, 0 };    /* here's where we'll home the
                                        cursor */
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
    DWORD dwConSize;                 /* number of character cells in
                                        the current buffer */

    /* get the number of character cells in the current buffer */

    bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
    PERR( bSuccess, "GetConsoleScreenBufferInfo" );
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    /* fill the entire screen with blanks */

    bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
                                           dwConSize, coordScreen, &cCharsWritten );
    PERR( bSuccess, "FillConsoleOutputCharacter" );

    /* get the current text attribute */

    bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
    PERR( bSuccess, "ConsoleScreenBufferInfo" );

    /* now set the buffer's attributes accordingly */

    bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
                                           dwConSize, coordScreen, &cCharsWritten );
    PERR( bSuccess, "FillConsoleOutputAttribute" );

    /* put the cursor at (0, 0) */

    bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
    PERR( bSuccess, "SetConsoleCursorPosition" );
    return;
}
//end

int isDuplicate(int, int *, int);
int RandMove(int);
int firstEndCondition(int *, int);
int robberSeen(int , int);
int smartMove(int *,int, int);
void visualMap(int *,int *, int);
//copied from StackOverFlow
void SetColor(int);

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
        printf("Please enter number of Cops in a sheriff:\n");
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
            sheriffStation[i][j]=(rand()%(row-1)+1)*1000+ (rand()%(column-1)+1);
            //printf("%d\n", sheriffStation[i][j]);
            poses[count]=sheriffStation[i][j];
            count++;
            if (isDuplicate(count, poses, sheriffStation[i][j])){
                //printf("her\n");
                count--;
                j--;
            }
        }
    }

    do{
        robberPos= (rand()%(row-1)+1)*1000+ (rand()%(column-1)+1);
    }while(isDuplicate(count+1, poses, robberPos));

    int round=1;
    while(1){
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
        if(firstEndCondition(poses, robberPos)){
            round++;
            printf("time %d:", round);
            printf("Arrested the robber\n");
            visualMap(poses, sheriff, robberPos);
            return 0;
        }

        cls( GetStdHandle( STD_OUTPUT_HANDLE ));
        printf("time %d:\n", round);
        round++;
        visualMap(poses, sheriff, robberPos);
        usleep(500000);

        int counter=0;
        for(int i=0;i<sheriffNum;i++){
            turn1=0;
            for(int j=0;j<sheriff[i];j++){
                if(sheriffState[i]==1){
                    do{
                        sheriffStation[i][j]= smartMove(poses, sheriffStation[i][j], formerRobberPos);
                        poses[counter]= sheriffStation[i][j];
                        turn1++;
                    }while(isDuplicate(counter, poses, poses[counter]));
                }else{
                    do{
                        sheriffStation[i][j]= RandMove(sheriffStation[i][j]);
                        poses[counter]= sheriffStation[i][j];
                    }while(isDuplicate(counter+1, poses, poses[counter]));
                }
                counter++;
            }
        }
        printf("\n");
        if(firstEndCondition(poses, robberPos)){
            round++;
            printf("time %d:", round);
            printf("Arrested the robber\n");
            visualMap(poses, sheriff, robberPos);
            return 0;
        }
    }
    return 0;
}

int isDuplicate(int count, int arr[], int value){
    for(int i=0;i<count-1;i++){
        if(arr[i]==value){
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
}

int smartMove(int *poses, int currentPos, int robberPos){
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
    while(isDuplicate(copsNum, poses, res)){
        res = RandMove(currentPos);
    }

    return res;
}

//copied from StackOverFlow
void SetColor(int ForgC){
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
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
                SetColor(14);
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
            SetColor(15);
        }
    }
}