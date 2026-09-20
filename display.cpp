#include <stdio.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37;1m"

void printBoard(int board[9][9]) 
{
    for (int i = 0; i < 9; i++) 
    {
        if (i % 3 == 0) 
            printf("\t+-------+-------+-------+\n");
        
        for (int j = 0; j < 9; j++) 
        {
            if (j % 3 == 0) 
            {
                if(j==0)  printf("\t");
                printf("| ");
            }
            
            if (board[i][j] == 0) 
            {
                printf(CYAN ". " RESET); // Empty cell
            } 
            else 
            {
                printf(WHITE "%d " RESET, board[i][j]); // Pre-filled/user
            }
        }
        printf("|\n");
    }
    printf("\t+-------+-------+-------+\n");
}

int main() {
    int sudoku[9][9] = {
        {5,3,0, 0,7,0, 0,0,0},
        {6,0,0, 1,9,5, 0,0,0},
        {0,9,8, 0,0,0, 0,6,0},
        {8,0,0, 0,6,0, 0,0,3},
        {4,0,0, 8,0,3, 0,0,1},
        {7,0,0, 0,2,0, 0,0,6},
        {0,6,0, 0,0,0, 2,8,0},
        {0,0,0, 4,1,9, 0,0,5},
        {0,0,0, 0,8,0, 0,7,9}
    };

    printBoard(sudoku);
    return 0;
}