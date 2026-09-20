#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>   //For _getch()


#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

//using namespace std;

#define MAX 100

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37;1m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"
#define BGCYAN  "\033[46m"   // cyan background for highlights

#define BGBLUE   "\033[44m"
#define BGGREEN  "\033[42m"
#define BGYELLOW "\033[43m"
#define BGWHITE  "\033[47m"
#define BLACK    "\033[30m"

#define BOLD    "\033[1m"
#define ITALIC  "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK   "\033[5m"
#define REVERSE "\033[7m"
#define MAGENTA "\033[35m"

// Cell status codes
#define EMPTY       0   // user-fillable but still empty
#define GIVEN       1   // predefined puzzle number
#define CORRECT     2   // user placed correctly
#define BACKTRACKED 3   // solver failed here, backtracked

class stk;

class Sudoku
{
    private:
        int score = 0;
        int maxHintsAllowed;
        void updateScore(int);
        void setMaxHintsAllowed(int);
        
    public :
        bool playGame(int arr[][9],int,int,int,int [][9]);
        bool isSafe(const int arr[][9],int,int,int);
        bool isGridFull(const int arr[][9]);
        void printBoard(const int [9][9],const int [][9],int,int);
        void displayWinMessage();
        bool gridChecking(const int arr[][9],int,int,int);
        void startDisplay();
        bool giveHint(const int board[9][9], int&, int&, int&);
        void loadGame();
        bool loadPuzzle(int[9][9]);
        void getFirstInput(int&,int&,int&);
        int getScore();
        bool isHintAllowed();
        void initCellStatus(const int board[9][9],int cellStatus[9][9]);
        void loadGameRules();
};

class stk
{
    public:
        int i,j,value;
        
        friend void push(stk*,int,int,int);
};

stk undo[MAX];  int u1 = -1;
stk redo[MAX];  int r1 = -1;

void push(stk* s,int i,int j,int value)
{
    s->i = i;
    s->j = j;
    s->value = value;
}

void unDo(int board[9][9],int cellStatus[9][9])
{
    if(u1>=0)
    {
        push(&(redo[++r1]),undo[u1].i,undo[u1].j,undo[u1].value);
        board[undo[u1].i][undo[u1].j] = 0;
        cellStatus[undo[u1].i][undo[u1].j] = EMPTY;
        u1--;
    }
    else
    {
        std::cout<<YELLOW<<"\n\t\t\t\t No more moves to undo!"<<RESET<<std::endl;
    }
}

void reDo(int board[9][9],int cellStatus[9][9])
{
    if(r1>=0)
    {
        push(&(undo[++u1]),redo[r1].i,redo[r1].j,redo[r1].value);
        board[redo[r1].i][redo[r1].j] = redo[r1].value;
        cellStatus[redo[r1].i][redo[r1].j] = CORRECT;
        r1--;
    }
    else
    {
        std::cout<<YELLOW<<"\n\t\t\t\t No moves to redo!"<<RESET<<std::endl;
    }
}

void Sudoku::setMaxHintsAllowed(int maxHints)
{
    this->maxHintsAllowed = maxHints;
}

bool Sudoku::isHintAllowed()
{
    if(maxHintsAllowed == 0)
        return false;
    
    maxHintsAllowed--;
    return true;
}

void Sudoku::updateScore(int sc)
{
    score += sc;
}

int Sudoku::getScore()
{
    return score;
}

bool Sudoku :: playGame(int board[9][9],int i,int j,int value,int cellStatus[9][9])
{
    int visited[10] = {0,0,0,0,0,0,0,0,0,0};
    visited[value] = 1;

    int newI = 0,newJ = 0,newValue = 0,count = 0;

    while(board[i][j]!=0)
    {
        std::cout<<RED<<"\n\t\t\t\t You are accessing invalid position.Please try again!!"<<RESET<<std::endl;
        std::cout<<"\n\t\t\t\t Enter position : ";
        std::cout<<"\n\t\t\t\t Enter row number : ";
        std::cin>>i;
        std::cout<<"\t\t\t\t Enter column number : ";
        std::cin>>j;
        std::cout<<"\t\t\t\t Enter value : ";
        std::cin>>value;
    }

    if(board[i][j] == 0)
    {
        while(1)
        {
            if(isSafe(board,i,j,value))
            {
                board[i][j] = value;
                cellStatus[i][j] = CORRECT;

                updateScore(10);    //10 marks for valid input
                std::cout<<GREEN<<"\n\t\t\t\t Correct move! +10 points"<<RESET<<std::endl;
                visited[value] = 1;
                
                push(&(undo[++u1]),i,j,value);
                printBoard(board,cellStatus,i,j);

                bool stay = true;

                while(stay)
                {
                    int choice;
                    std::cout << "\n\t\t\t\t What would you like to do?"<<std::endl;
                    std::cout << "\t\t\t\t "<<YELLOW<<"1.CONTINUE  2.UNDO  3.REDO  4.HINT  5.EXIT.\n"<<RESET<<std::endl;
                    std::cout << "\t\t\t\t Enter your choice : ";
                    std::cin >> choice;

                    bool isHintAvailable;

                    switch(choice)
                    {
                        case 1:
                            //continue
                            stay = false;
                            break;

                        case 2:
                            if (u1 >= MAX-1) 
                            {
                                std::cout << RED << "Undo stack full!" << RESET << std::endl;
                            }
                            else
                            {
                                unDo(board,cellStatus);
                                updateScore(-2);    //-2 marks for undo
                                std::cout << YELLOW << "\n\t\t\t\t -2 points (UNDO)" << RESET << std::endl;
                                printBoard(board,cellStatus,i,j);
                            }
                        
                            break;

                        case 3:
                            if (r1 >= MAX-1) 
                            {
                                std::cout << RED << "Redo stack full!" << RESET << std::endl;
                            }
                            else
                            {
                                reDo(board,cellStatus);
                                updateScore(-1);    //-1 marks for redo
                                std::cout << YELLOW << "\n\t\t\t\t -1 points (REDO)" << RESET << std::endl;
                                printBoard(board,cellStatus,i,j);
                            }

                            break;
                        
                        case 4:
                            isHintAvailable = isHintAllowed();

                            if(isHintAvailable)
                            {
                                int hr, hc, hv;
                                if (giveHint(board, hr, hc, hv))
                                {
                                    std::cout << GREEN<< "\n\t\t\t\t Hint: Try placing "<< hv << " at position ["<< hr << "][" << hc << "]"<< RESET << std::endl;

                                    updateScore(-3);    //-3 for using hint
                                    std::cout << YELLOW<< "\n\t\t\t\t -3 points for using a hint"<< RESET << std::endl;

                                    printBoard(board, cellStatus, hr, hc);
                                }
                                else
                                {
                                    std::cout << YELLOW<< "\n\t\t\t\t No hints available!"<< RESET << std::endl;
                                }
                            }
                            else
                            {
                                std::cout<<RED<<"\n\t\t\t\t You cannot access more hints!!"<<RESET<<std::endl;
                            }
                            break;

                        case 5:
                            std::cout<<GREEN<<"Thankyou for playing."<<RESET<<std::endl;
                            exit(0);

                        default:
                            std::cout<<RED<<"\n\t\t\t\t Invalid choice!Please try again"<<RESET<<std::endl;
                    }
                }

                if(isGridFull(board))
                {
                    return true;
                }
    
                std::cout<<"\n\t\t\t\t Enter new position : "<<std::endl;
                std::cout<<"\t\t\t\t Enter row number : ";
                std::cin>>newI;
                std::cout<<"\t\t\t\t Enter column number : ";
                std::cin>>newJ;
                std::cout<<"\t\t\t\t Enter value : ";
                std::cin>>newValue;

                bool a = playGame(board,newI,newJ,newValue,cellStatus);

                if(a == true)
                {
                    return true;
                }
                else
                {
                    board[i][j] = 0;
                    std::cout<<YELLOW<<"\n\t\t\t\t Backtracked to position arr["<<i<<"]["<<j<<"]."<<RESET<<std::endl;
                    cellStatus[i][j] = BACKTRACKED;
                    printBoard(board,cellStatus,i,j);      // yellow highlight for backtracking
                    
                    //std::this_thread::sleep_for(std::chrono::milliseconds(400));
                    std::cout<<"\n\t\t\t\t Enter new value at arr["<<i<<"]["<<j<<"] : ";
                    std::cin>>value;

                    while(visited[value])
                    {
                        std::cout<<RED<<"\n\t\t\t\t You have already entered this value earlier.Try another input."<<RESET<<std::endl;
                        std::cout<<"\n\t\t\t\t Enter new value : ";
                        std::cin>>value;
                    }
                    visited[value] = 1;
                }
            }
            else
            {
                bool a = true;
                for(int i=1;i<10;i++)
                {
                    if(visited[i] == 0)
                        a = false;
                }

                if(a)  //if( all visited={1})
                {
                    std::cout<<YELLOW<<"\n\t\t\t\t You have tried all possibilities from(1-9) here.Backtracking to your previous position."<<RESET<<std::endl;
                    board[i][j] = 0;
                    return false;
                }

                std::cout << RED << "\n\t\t\t\t Invalid move!Try again." << RESET << std::endl;
                std::cout<<"\n\t\t\t\t Enter new value : ";
                std::cin>>value;

                while(visited[value])
                {
                    std::cout<<RED<<"\n\t\t\t\t You have already entered this value earlier.Try another input."<<RESET<<std::endl;
                    std::cout<<"\n\t\t\t\t Enter new value : ";
                    std::cin>>value;
                } 

                visited[value] = 1;
            }
        }
    }
}

bool Sudoku::isSafe(const int board[9][9],int i,int j,int value)
{
    //For row checking
    for(int m=0;m<9;m++)        
    {
        if(board[i][m] == value)
        {
            return false;
        }
    }

    //For column checking
    for(int n=0;n<9;n++)    
    {
        if(board[n][j] == value)
        {
            return false;
        }  
    }

    //Grid checking
    return gridChecking(board,i,j,value);
}

bool Sudoku::gridChecking(const int board[9][9],int i,int j,int value)
{
    int endRowIndex,endColIndex;
    
    if(i%3==0)  endRowIndex = i+2;
    else if(i%3==2) endRowIndex = i;
    else    endRowIndex = i%3+i;

    if(j%3==0)  endColIndex = j+2;
    else if(j%3==2) endColIndex = j;
    else endColIndex = j%3+j;

    for(int m = endRowIndex-2;m<=endRowIndex;m++)
    {
        for(int n = endColIndex-2;n<=endColIndex;n++)
        {
            if(!(m==i && n==j))
            {
                if(board[m][n] == value)    return false;
            }
        }
    }

    return true;
}

bool Sudoku::isGridFull(const int board[9][9])
{
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            if(board[i][j] == 0)
                return false;
        }
    }
    return true;
}

bool Sudoku::giveHint(const int board[9][9], int &row, int &col, int &value)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            // Find an empty cell
            if (board[i][j] == 0)
            {
                for (int v = 1; v <= 9; v++)
                {
                    if (isSafe(board, i, j, v))
                    {
                        row = i;
                        col = j;
                        value = v;
                        return true;   // Hint found
                    }
                }
            }
        }
    }
    return false;   // No hint available
}

void Sudoku::printBoard(const int board[9][9], const int cellStatus[9][9], int selRow, int selCol)
{
    bool hasSelection = (selRow >= 0 && selCol >= 0);
    int gridRowStart = hasSelection ? (selRow / 3) * 3 : -1;
    int gridColStart = hasSelection ? (selCol / 3) * 3 : -1;

    // Column indices
    printf("\n\t\t\t\t\t\t\t    ");
    for (int j = 0; j < 9; j++) 
    {
        printf("%d ", j);
        if ((j + 1) % 3 == 0 && j != 8) printf("  ");
    }
    printf("\n\t\t\t\t\t\t\t  +-------+-------+-------+\n");

    for (int i = 0; i < 9; i++) 
    {
        printf("\t\t\t\t\t\t\t%d | ", i);
        for (int j = 0; j < 9; j++) 
        {
            bool isInHighlight = hasSelection && (i == selRow || j == selCol || (i >= gridRowStart && i < gridRowStart + 3 && j >= gridColStart && j < gridColStart + 3));
            const char *bg = isInHighlight ? BGCYAN : "";

            if (board[i][j] == 0) 
            {
                if (cellStatus[i][j] == BACKTRACKED)
                    printf("%s" YELLOW ". " RESET, bg);
                else
                    printf("%s" CYAN ". " RESET, bg);
            } 
            else 
            {
                if (cellStatus[i][j] == GIVEN)
                    printf("%s%s%d " RESET, bg, WHITE, board[i][j]);
                else if (cellStatus[i][j] == CORRECT)
                    printf("%s%s%d " RESET, BGGREEN, BLACK, board[i][j]);
                else if (cellStatus[i][j] == BACKTRACKED)
                    printf("%s%s%d " RESET, BGYELLOW, BLACK, board[i][j]);
                else
                    printf("%s%s%d " RESET, bg, BLACK, board[i][j]);
            }

            if ((j + 1) % 3 == 0) printf("| ");
        }

        printf("\n");
        if ((i + 1) % 3 == 0)
            printf("\t\t\t\t\t\t\t  +-------+-------+-------+\n");
    }

    printf("\n\t\t\t\t        %sWhite=Given%s | %sGreen=Correct%s | %sYellow=Backtracked%s | %sCyan=Empty/Highlight%s\n\n",
           WHITE, RESET, GREEN, RESET, YELLOW, RESET, CYAN, RESET);

}


void Sudoku::initCellStatus(const int board[9][9], int cellStatus[9][9]) 
{
    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++) 
        {
            if (board[i][j] != 0)
                cellStatus[i][j] = GIVEN;   // Predefined puzzle numbers
            else
                cellStatus[i][j] = EMPTY;   // Empty, player will fill
        }
    }
}


void Sudoku::displayWinMessage()
{
    std::cout<<GREEN<<"\n\tCongratulation!You have won the game."<<RESET<<std::endl;

    updateScore(50);
    std::cout << GREEN<< "\n\t\t\t\t +50 points for completing the puzzle!"<< RESET << std::endl;

    std::cout<<"\n\t\t\t\t SCORE : "<<getScore()<<std::endl;
}

void Sudoku::startDisplay()
{
    std::string lines[] = 
    {
        CYAN BOLD "\t\t\t\t\t\t******************************************\n\t\t\t\t\t\t\t\t" RESET,
        CYAN BOLD  RESET YELLOW BOLD "\t\t\t\t\t\t           $ SUDOKU MASTER $          " RESET CYAN BOLD  RESET,
        CYAN BOLD "\n\t\t\t\t\t\t******************************************" RESET,
        MAGENTA "\n\n        \t\t\t\t\tTips:" RESET " Use logic, not luck! Try to spot patterns.\n" RESET
    };

    for (const std::string &line : lines) 
    {
        std::cout<<line<<std::endl;
        //Sleep_ms(500);
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        Sleep(500);
    }
    std::cout<<std::endl;
}

bool Sudoku::loadPuzzle(int board[9][9])
{
    int level;
    std::cout<<BOLD<<"\n\n        \t\t\t\t\tCHOOSE YOUR DIFFICULTY\n"<<RESET;
    std::cout<<GREEN BOLD<<"\n        \t\t\t\t\t1. Easy\n"<<RESET;
    std::cout<<YELLOW BOLD<<"\n        \t\t\t\t\t2. Medium\n"<<RESET;
    std::cout<<RED BOLD<<"\n        \t\t\t\t\t3. Hard\n"<<RESET;
    std::cout<<BOLD<<"\n        \t\t\t\t\tEnter your choice: ";
    std::cin >> level;

    std::string filename;

    if (level == 1) filename = "puzzles/easy.txt";
    else if (level == 2) filename = "puzzles/medium.txt";
    else filename = "puzzles/hard.txt";

    std::ifstream file(filename);
    if (!file)
    {
        std::cout << RED << "\n\t\t\t\t Error loading puzzle!" << RESET << std::endl;
        return false;
    }

    std::vector<std::vector<int>> puzzles;
    std::vector<int> temp;

    int num;
    while (file >> num)
    {
        temp.push_back(num);
        if (temp.size() == 81)
        {
            puzzles.push_back(temp);
            temp.clear();
        }
    }

    if (puzzles.empty())
    {
        std::cout << RED << "\n\t\t\t\t Error loading puzzle!" << RESET << std::endl;
        return false;
    }
             
    std::cout<<CYAN BOLD<<"\n\n        \t\t\t\t\t\t  NEW GAME"<<RESET;

    if(level == 1)
    {
        setMaxHintsAllowed(7);
        std::cout<<BOLD<<"  (DIFFICULTY : "<<GREEN<<"EASY"<<RESET<<")"<<std::endl;
    }
    else if(level == 2)
    {
        setMaxHintsAllowed(5);
        std::cout<<BOLD<<"  (DIFFICULTY : "<<YELLOW<<"MEDIUM"<<RESET<<")"<<std::endl;
    }
    else
    {
        setMaxHintsAllowed(3);
        std::cout<<BOLD<<"  (DIFFICULTY : "<<RED<<"HARD"<<RESET<<")"<<std::endl;
    }
    
    std::cout<<std::endl;

    srand(time(0));
    int index = rand() % puzzles.size();

    for (int i = 0; i < 81; i++)
        board[i / 9][i % 9] = puzzles[index][i];

    file.clear();
    file.seekg(0,std::ios::beg);

    return true;
}

void Sudoku::loadGameRules()
{
    std::string filename = "Game Rules/Rules.txt";

    std::ifstream fin(filename);
    if (!fin)
    {
        std::cout << RED << "        \t\t\t\t\t\tFailed to open puzzle file!" << RESET << std::endl;
        return;
    }

    std::string str;
    std::cout<<YELLOW BOLD<<"\n\n        \t\t\t\t\t\t        RULES"<<std::endl;
    while(getline(fin,str))
    {
        std::cout<<"\n        \t\t\t\t\t"<<RED BOLD<<str<<RESET<<std::endl;
    }

    fin.clear();    //Reset eof bit
    fin.seekg(0,std::ios::beg);  //move file ptr to beginning
}

void Sudoku::getFirstInput(int& i,int& j,int& value)
{
    std::cout<<"\n\t\t\t\t Enter position : ";
    std::cout<<"\n\t\t\t\t Enter row number : ";
    std::cin>>i;
    std::cout<<"\t\t\t\t Enter column number : ";
    std::cin>>j;
    std::cout<<"\t\t\t\t Enter value : ";
    std::cin>>value;
}

int main()
{
    
    Sudoku s;
    bool game;

    s.startDisplay();
    //sleep_ms(500);
    Sleep(500);

    std::cout<<"        \t\t\t\t\t\tPRESS ANY KEY TO CONTINUE"<<std::endl;
    _getch();   //Reads directly from console,doesn't wait for Enter.

    int choice;
    do{
        std::cout<<CYAN BOLD<<"\n\n        \t\t\t\t\t\t  ***** MAIN MENU ****"<<RESET<<std::endl;
        std::cout<<"\n        \t\t\t\t\t\t       "<<YELLOW BOLD<<"1.NEW GAME"<<RESET<<std::endl;
        std::cout<<"\n        \t\t\t\t\t\t       "<<YELLOW BOLD<<"2.RULES"<<RESET<<std::endl;
        std::cout<<"\n        \t\t\t\t\t\t       "<<YELLOW BOLD<<"3.QUIT."<<RESET<<std::endl;

        std::cout<<"\n\n        \t\t\t\t\t"<<"Enter your choice : ";
        std::cin>>choice;

        switch(choice)
        {
            case 1:
                int board[9][9];
                game = s.loadPuzzle(board);

                if(game)
                {
                    int cellStatus[9][9];
                    s.initCellStatus(board,cellStatus);
                    
                    int i,j,value;
                    s.printBoard(board,cellStatus,-1,-1);
                    s.getFirstInput(i,j,value);

                    bool a = s.playGame(board,i,j,value,cellStatus);  
                
                    if(a == true)
                    {
                        s.displayWinMessage();
                    }
                    else
                    {
                        std::cout<<"\n\t\t\t\t Not won,Try again."<<std::endl;
                    }
                }

                break;

            case 2:
                s.loadGameRules();
                break;

            case 3:
                //std::cout<<RED BOLD<<"\n\n        \t\t\t\t\tAre you sure you want to exit?"<<RESET<<std::endl;
                std::cout<<"        \t\t\t\t\t\tThankyou for playing the game. "<<std::endl;
                break;

            default:
                std::cout<<"        \t\t\t\t\t\tInvalid Choice.Please try again"<<std::endl;
        }
    }while(choice!=3);
         
    return 0;
}