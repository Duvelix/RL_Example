#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ROW 4
#define COLUMN 5
#define ACTION 4 // 0 : UP, 1 : DOWN, 2 : RIGHT, 3 : LEFT
#define ALPHA 0.5
#define GAMMA 0.5
#define CONSTANT 1

int destination = 0;
int boundary = 0;

int main(int argc, const char * argv[]) {
    
    static double Q[ROW][COLUMN][ACTION];
    double Epsilon[ROW][COLUMN];
    int Visit[ROW][COLUMN];
    
    double Max_Q, REWARD;
    int Agent_row, Agent_col, Episode = 0;
    int Next_row, Next_col;
    int Direction;
    int i, j, k;
    //int count = 0;
    
    srand((int)time(NULL));
    
    // ----- Initialization ----- //
    
    for(i=0;i<ROW;i++){
        for(j=0;j<COLUMN;j++){
            for(k = 0; k < ACTION; k++)
                Q[i][j][k] = 0.0;
            Visit[i][j] = 0;
        }
    }
    
    Agent_row = 2;
    Agent_col = 1;
    
    // ----- Q Learning ----- //
    
    while(1){
        
        // ----- Epsilon Greedy Policy ----- //
        Epsilon[Agent_row][Agent_col] = (double)CONSTANT / (double)Visit[Agent_row][Agent_col]++;
        if(Epsilon[Agent_row][Agent_col] > rand()/RAND_MAX){
            // ----- Select Random Action with Prob 1/|A| ----- //
            Direction = rand()%4;
        }
        else{
            // ----- Select Maximum Q value Action ----- //
            Max_Q = Q[Agent_row][Agent_col][0];
            Direction = 0;
            if(Max_Q < Q[Agent_row][Agent_col][1]){
                Max_Q = Q[Agent_row][Agent_col][1];
                Direction = 1;
            }
            if(Max_Q < Q[Agent_row][Agent_col][2]){
                Max_Q = Q[Agent_row][Agent_col][2];
                Direction = 2;
            }
            if(Max_Q < Q[Agent_row][Agent_col][3]){
                Max_Q = Q[Agent_row][Agent_col][3];
                Direction = 3;
            }
        }
        
        // ----- Find Next State ----- //
        switch (Direction) {
            case 0: // UP
                Next_row = Agent_row - 1;
                Next_col = Agent_col;
                break;
            case 1: // DOWN
                Next_row = Agent_row + 1;
                Next_col = Agent_col;
                break;
            case 2: // RIGHT
                Next_row = Agent_row;
                Next_col = Agent_col + 1;
                break;
            case 3: // LEFT
                Next_row = Agent_row;
                Next_col = Agent_col - 1;
                break;
            default: // Meaningless
                Next_row = Agent_row;
                Next_col = Agent_col;
                break;
        }
        
        // ----- Calculate Reward ----- //
        if((Next_row == 1) && (Next_col == 4))
            REWARD = 8;
        else if((Next_row == 0) || (Next_row == ROW-1) || (Next_col == 0) || (Next_col == COLUMN-1))
            REWARD = -8;
        else
            REWARD = 0;
        
        // ----- Find Maximum Q value of Next state ----- //
        Max_Q = Q[Next_row][Next_col][0];
        if(Max_Q < Q[Next_row][Next_col][1])
            Max_Q = Q[Next_row][Next_col][1];
        if(Max_Q < Q[Next_row][Next_col][2])
            Max_Q = Q[Next_row][Next_col][2];
        if(Max_Q < Q[Next_row][Next_col][3])
            Max_Q = Q[Next_row][Next_col][3];
        
        
        // ----- Update Q-value using Q(s, a) ----- //
        Q[Agent_row][Agent_col][Direction] = (1-ALPHA) * Q[Agent_row][Agent_col][Direction] + ALPHA * (REWARD + GAMMA * Max_Q);
        //printf("REWARD = %.2f\n", REWARD);
        //printf("Q[%d][%d][%d] = %.2f\n", Agent_row, Agent_col, Direction, Q[Agent_row][Agent_col][Direction]);
        
        // ----- Move to the Next state ----- //
        Agent_row = Next_row;
        Agent_col = Next_col;
        
        // ----- Destination Checking ----- //
        if((Agent_row == 1) && (Agent_col == 4)){
            Agent_row = 2;
            Agent_col = 1;
            Episode++;
            destination++;
        }

        // ----- Boundary Checking ----- //
        if((Agent_row == 0) || (Agent_row == ROW-1) || (Agent_col == 0) || (Agent_col == COLUMN-1)){
            Agent_row = 2;
            Agent_col = 1;
            Episode++;
            boundary++;
        }
        
        // ----- Break Condition ----- //
        if(Episode == 10000)
            break;
        
    }
    
    
    
    printf("Number of Episode : %d\n", Episode);
    printf("# of Destination : %d\n", destination);
    printf("# of Boundary : %d\n", boundary);
    
    
    Agent_row = 2;
    Agent_col = 1;
    printf("(%d, %d)", Agent_row, Agent_col);
    
    while((Agent_row != 1) || (Agent_col != 4)){
        
        printf(" -> ");
        
        Max_Q = Q[Agent_row][Agent_col][0];
        Direction = 0;
        if(Max_Q < Q[Agent_row][Agent_col][1]){
            Max_Q = Q[Agent_row][Agent_col][1];
            Direction = 1;
        }
        if(Max_Q < Q[Agent_row][Agent_col][2]){
            Max_Q = Q[Agent_row][Agent_col][2];
            Direction = 2;
        }
        if(Max_Q < Q[Agent_row][Agent_col][3]){
            Max_Q = Q[Agent_row][Agent_col][3];
            Direction = 3;
        }
        
        switch (Direction) {
            case 0: // UP
                Agent_row--;
                break;
            case 1: // DOWN
                Agent_row++;
                break;
            case 2: // RIGHT
                Agent_col++;
                break;
            case 3: // LEFT
                Agent_col--;
                break;
            default:
                break;
        }
        
        if((Agent_row < 0) || (Agent_col < 0))
            break;
        
        printf("(%d, %d)", Agent_row, Agent_col);
        
    }
    
    printf("\n");
    
    /*
    
    for(i=0;i<ROW;i++){
        for(j=0;j<COLUMN;j++){
            printf("%d   ", ++count);
        }
    }
    
    printf("\n");
    
    for(k = 0; k < ACTION; k++){
        for(i = 0; i < ROW; i++){
            for(j = 0; j < COLUMN; j++){
                printf("%.2f ", Q[i][j][k]);
            }
        }
        printf("\n");
    }
     
     */
    
    return 0;
}
