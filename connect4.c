/* connect4.c
 *
 * Kyle Schlanger and Connor Hanify
 * cs313 hw4 - Professor Scharstein
 * Used c4rand.c as a basis for our code
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ROWS 6
#define COLS 7

// global variable for the board
// uses 0 for empty cells, 1 and 2 for player 1 and 2
int board[ROWS][COLS] = {{0}};

// maps board contents 0, 1, 2 to characters
char sym[] = {' ', 'X', 'O'};

// simple assert function, quits program with error message if assertion fails
void assert(int cond, char *msg) {
    if (!cond) {
        fprintf(stderr, "failed assertion %s\n", msg);
        exit(1);
    }
}

  // seed random number generator based on on current time in microseconds and board state
void randseed() {
    struct timeval t1;
    gettimeofday(&t1, NULL);
    unsigned int s = t1.tv_usec * t1.tv_sec;
    int row, col;
    for (col = 0; col < COLS; col++) {
        for (row = 0; row < ROWS; row++) {
            int b = board[row][col];
            s += (b+1) * (13*row + 37*col);
        }
    }
    srand(s);
}

// read encoded board from stdin:
// 7 strings of 'X' and 'O', representing columns from bottom up
// followed by single X/O representing player to move
// returns player num (1=X, 2=O)
int read_board() {
    int row = 0, col = 0;
    char c;
    while (col < COLS) {
        c = getchar();
        if (c == 'X' || c == 'O') {
            assert(row < ROWS, "row < ROWS");
            board[row++][col] = (c == 'X' ? 1 : 2);
        } else if (c == '\n') {
            row = 0;
            col++;
        } else {
            assert(0, "input symbols must be 'X', 'O', or '\\n'");
        }
    }
    // read one more symbol indicating whose move it is
    c = getchar();
    assert(c == 'X' || c == 'O', "last input symbol must 'X' or 'O'");
    randseed(); // seed random number generator based on board state and current time
    return (c == 'X' ? 1 : 2);
}

  // prints board (for debugging)
void print_board() {
    int row, col;
    for (row = ROWS-1; row >= 0; row--) {
        printf("| ");
        for (col = 0; col < COLS; col++)
        printf("%c ", sym[board[row][col]]);
        printf("|\n");
    }
    printf("+---------------+\n");
    printf("  0 1 2 3 4 5 6  \n");
}

int make_move() {
    // controls initial board play. Plays in the middle if it is open,
    // if middle is taken, plays in next left column. Also, this prevents
    // and immediate 3 of a kind win.
    if (board[0][3] == 0){
        return 4;
    } else if (board[0][3] != 0) {
        if (board[0][2] == 0){
            return 3;
        }
    }

    //checking for vertical 3 in a row
    for (int row = ROWS-4; row >= 0; row--) {
        for (int col = 0; col < COLS; col++){
            if (board[row][col] != 0){
                if ((board[row][col] == board[row+1][col]) &&  (board[row][col] == board[row+2][col])){
                    if (board[row+3][col] == 0){
                        return col+1;
                    }
                }
            }
         }
     }

    //checking for horizontal 3 in a row
    for (int row = 0; row < ROWS; row++) {
        for (int col = COLS-4; col >= 0; col--){
            if (board[row][col] != 0){
                if ((board[row][col] == board[row][col+1]) &&  (board[row][col] == board[row][col+2])){
                    if ((board[row][col+3] == 0) && (board[row-1][col+3] != 0) && (col != 6)) { //might give an error
                        return col+4;
                    } else if (col != 0){
                        if ((board[row][col-1] == 0) && (board[row-1][col-1] != 0)){
                        return col;
                        }
                    }
                }
            }
        }
    }

    // Checking for a situation "xx_x" horizontally.
    for (int row = 0; row < ROWS; row++) {
        for (int col = COLS-4; col >= 0; col--){
            if (board[row][col] != 0){
                if ((board[row][col] == board[row][col+1]) &&  (board[row][col] == board[row][col+3])){
                    if ((board[row][col+2] == 0) && (board[row-1][col+2] != 0)) { //might give an error
                        return col+3; // had to add +1
                    }
                }
            }
        }
    }

    // Checking for a situation "x_xx" horizontally.
    for (int row = 0; row < ROWS; row++) {
        for (int col = COLS-4; col >= 0; col--){
            if (board[row][col] != 0){
                if ((board[row][col] == board[row][col+2]) &&  (board[row][col] == board[row][col+3])){
                    if ((board[row][col+1] == 0) && (board[row-1][col+1] != 0)) {
                        return col+2;
                    }
                }
            }
        }
    }

    //checking for forward diagonal 3 in a row
    int count = 1;
    int indr = -1;
    int indc = -1;
    for (int row = ROWS-4; row >= 0; row--) {
        for (int col = COLS-4; col >=0; col--){
            if (board[row][col] != 0){
                for (int i=1; i<4; i++){
                    if (board[row][col] == board[row+i][col+i]){
                        count +=1;
                    } else if ((board[row][col] != board[row+i][col+i]) && (board[row+i][col+i] != 0)){
                        count =1;
                    } else if ((board[row][col] != board[row+i][col+i]) && (board[row+i][col+i] == 0)){
                        indr = row+i;
                        indc = col+i;
                    } if (count == 3) {
                        if (board[indr-1][indc] != 0){
                            return indc+1; //+1 for indexing shift
                        } else if ((row != 0) && (col != 0)){
                            if (board[row-1][col-1] == 0){
                                return col;
                            }
                        }
                    }
                }
            }
        }
    }

    //checking for backward diagonal 3 in a row
    count = 1;
    indr = -1;
    indc = -1;
    for (int row = ROWS-4; row >=0; row--) {
        for (int col = COLS-4; col < COLS; col++){
            if (board[row][col] != 0){
                for (int i=1; i<4; i++){
                    if (board[row][col] == board[row+i][col-i]){
                        count +=1;
                    } else if ((board[row][col] != board[row+i][col-i]) && (board[row+i][col-i] != 0)){
                        count =1;
                    } else if ((board[row][col] != board[row+i][col-i]) && (board[row+i][col-i] == 0)){
                        indr = row+i;
                        indc = col-i;
                    } if (count == 3) {
                        if (board[indr-1][indc] != 0){
                            return indc+1; //+1 for indexing shift
                        } else if ((row != 0) && (col != 6)){
                            if (board[row-1][col+1] == 0) {
                                return col+2; //same as col-1 above with indexing shift
                            }
                        }
                    }
                }
            }
        }
    }

    // make a random move by selecting a non-full column
    int r = rand();
    int c;
    for (c = 0; c < COLS; c++) {
    int a = (c + r) % COLS;
        if (board[ROWS-1][a] == 0){
            return a+1;
        }
    }
    assert(0, "should find at least one non-full column");
    return 0;
}

// main function. Calls read_board and prints make_move.
int main() {
    read_board();
    //int p = read_board();
    //printf("player %d's turn\n", p);
    //print_board();
    printf("%d\n", make_move());
    return 0;
}
