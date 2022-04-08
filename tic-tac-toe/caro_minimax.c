#include <stdio.h>
#include <unistd.h>
#include <conio.h>

#define ROW 3
#define COL 3
#define AI_TURN 0
#define PLAYER_TURN 1
#define AI 'X'
#define PLAYER 'O'

typedef struct position {
	int row, col;
} pos;

void print_winner(int turn) {
	printf("%s", turn == PLAYER_TURN ? "PLAYER WIN!" : "AI WIN!");
}

int is_win(char board[ROW][COL]) {
	int row, col;
	for(row = 0; row < ROW; row++) {
    if(board[row][0] == board[row][1] && board[row][1] == board[row][2] && board[row][0] != ' ') 
    	return 1; 
  }
  for(col = 0; col < COL; col++) {
    if(board[0][col] == board[1][col] && board[1][col] == board[2][col] && board[0][col] != ' ') {
    	return 1; 
    }
  }
  if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] != ' ') 
  	return 1; 
  if(board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] != ' ') 
  	return 1; 
  return 0;
}

int minimax(char board[ROW][COL], int depth, int is_ai) {
	int score, best_score;
	int row, col;  
	if (is_win(board) == 1) return is_ai == 1 ? -1 : 1; 
	else {
		if(depth < ROW * COL) {
			if(is_ai == 1) {
				best_score = -1000;
				for(row = 0; row < ROW; row++) {
					for(col = 0; col < COL; col++) {
						if(board[row][col] == ' ') {
							board[row][col] = AI;
							score = minimax(board, depth + 1, 0);
							board[row][col] = ' ';
							if(score > best_score) best_score = score; 
						}
					}
				}
				return best_score;
			}
			else {
				best_score = 1000;
				for(row = 0; row < ROW; row++) {
					for(col = 0; col < COL; col++) {
						if(board[row][col] == ' ') {
							board[row][col] = PLAYER;
							score = minimax(board, depth + 1, 1);
							board[row][col] = ' ';
							if(score < best_score) best_score = score; 
						}
					}
				}
				return best_score;
			}
		}
		else return 0;
	}
}

pos get_best_pos(char board[ROW][COL], int move_index) {
	pos best_pos;
	best_pos.row = -1; 
	best_pos.col = -1;
	int i, j; 
	int score = 0, best_score = -1000;
	for(i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			if (board[i][j] == ' ') {
				board[i][j] = AI;
				score = minimax(board, move_index+1, 0);
				board[i][j] = ' ';
				if(score > best_score) {
					best_score = score;
					best_pos.row = i; 
					best_pos.col = j;
				}
			}
		}
	}
	return best_pos;
}

void render_board(char board[ROW][COL]) {
	short i, j;
	printf("+-----------+\n");
	for(i = 0; i < ROW; i++) {
		for(j = 0; j < COL; j++) {
			printf("| %c ", board[i][j]);
		}
		printf("|\n");
		printf("+-----------+\n");  
	}
	printf("\n");
}

void start_game(char board[ROW][COL], int turn) {
	if(turn == -1) return;
	int move_index = 0, i = 0, j = 0;
	pos move; 
	while(is_win(board) == 0 && move_index != (ROW * COL)) { 
		if(turn == AI_TURN) {
			move = get_best_pos(board, move_index); 
			board[move.row][move.col] = AI;
			printf("AI danh: \n");
			render_board(board);
			move_index++;
			turn = PLAYER_TURN;
			sleep(1);
		}
		else if(turn == PLAYER_TURN) {
			printf("Den luot ban\n");
			printf("row: "); 
			scanf("%d", &i);
			printf("col: "); 
			scanf("%d", &j);
			if(board[i][j] == ' ' && i < ROW && i >= 0 && j < COL && j >= 0) {
				board[i][j] = PLAYER; 
				render_board(board); 
				move_index++; 
				turn = AI_TURN;
			}
			else printf("\nVi tri khong hop le");
			sleep(1);
		}
	}
	if(is_win(board) == 0 && move_index == ROW * COL) 
		printf("\nGame Hoa\n"); 
	else { 
		turn = turn == AI_TURN ? PLAYER_TURN : AI_TURN; 
		print_winner(turn);
	} 
}

int main(void) {
	char keep_playing;
	char board[ROW][COL] = {
	  {' ', ' ', ' '},
	  {' ', ' ', ' '},
	  {' ', ' ', ' '}
	};
	printf("Bat dau tro choi\n\n");
	render_board(board);
	printf("\nBan la [O] - AI la [X]\n");
	char opt;
	do {
	 	printf("\nBan muon di truoc khong?(y/n) : ");
	 	scanf("%c%*c", &opt);
		start_game(board, opt == 'y' ? PLAYER_TURN : (opt == 'n' ? AI_TURN : -1));
		printf("\nBan muon thoat game khong(y/n): ");
   	keep_playing = getchar();
   	fflush(stdin);
	} while(keep_playing == 'n');
}