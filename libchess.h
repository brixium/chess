/*TODO: NEL MAIN DEL PROGRAMMA PRINCIPALE implementare il ciclo di gioco*/
/*TODO: controllare che sia possibile mangiare i pezzi dell'avversario*/
/*TODO: condizione di scacco al re*/
/*TODO: fix pedone OverPowered (mangia anche per dritto)*/
/*Remember how to set up a chessboard on chess.com*/
/*FUTURE: EN PASSANT for pawns, arrocco (rook and king)*/
#include <stdio.h>
#include <stdlib.h>

#define CBL 8
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define RESET "\x1B[0m"

typedef struct piece_s{
	int x;		/*X position*/
	int y;		/*Y position*/
	/*positions may be between 0 and 7*/
	char name;	/*K, Q, R, B, H, P*/
	char color;	/*[B]lack or [W]hite*/
	int moves;	/*Nr of movements done*/
	int stx;	/*Start point x*/
	int sty;	/*Start point y*/
	/*
		K = king	re
		Q = queen	regina
		R = rook	torre
		B = bishop	alfiere
		H = knight	cavallo
		P = pawn	pedone
	*/
}piece;

piece cboard[CBL][CBL];	/*chessboard = scacchiera*/
int scacco, checkmate;

/*Functions*/
void initCB();	/*Initializes the chessboard with sorted pieces*/
void printCB();	/*Prints the chessboard with all pieces on it*/
void replace(piece, piece); /*Replaces the piece on a square with the second one*/

void initCB(){
	int i, j;

	scacco = 0;
	checkmate = 0;
	for(i=0; i<CBL; i++){
		for(j=0; j<CBL; j++){
			/*Every square has a position and a name*/
			cboard[i][j].x = i;
			cboard[i][j].y = j;
			cboard[i][j].name = ' ';
		}
	}
	/*only the pieces have a "proper" name != ' '*/
	cboard[0][0].name = 'R';
	cboard[0][1].name = 'H';
	cboard[0][2].name = 'B';
	cboard[0][3].name = 'K';
	cboard[0][4].name = 'Q';
	cboard[0][5].name = 'B';
	cboard[0][6].name = 'H';
	cboard[0][7].name = 'R';
	
	cboard[7][0].name = 'R';
	cboard[7][1].name = 'H';
	cboard[7][2].name = 'B';
	cboard[7][3].name = 'Q';
	cboard[7][4].name = 'K';
	cboard[7][5].name = 'B';
	cboard[7][6].name = 'H';
	cboard[7][7].name = 'R';

	for(i=0; i<CBL; i++){
		cboard[0][i].color = 'B';
		cboard[1][i].color = 'B';
		cboard[1][i].name  = 'P';
		cboard[6][i].name  = 'P';
		cboard[6][i].color = 'W';
		cboard[7][i].color = 'W';
	}
	for(i=0; i<2; i++)
		for(j=0; j<CBL; j++){
			cboard[i][j].moves = 0;
			cboard[i][j].stx = i;
			cboard[i][j].sty = j;
		}
	for(i=6; i<CBL; i++)
		for(j=0; j<CBL; j++){
			cboard[i][j].moves = 0;
			cboard[i][j].stx = i;
			cboard[i][j].sty = j;
		}
	return;
}

void printCB(){
	int i, j;

	printf("| ");
	for(i=0; i<CBL; i++)
		printf("|%c", 'a'+i);
	printf("| |\n");

	for(i=0; i<CBL; i++){
		printf("|%d", i+1);
		for(j=0; j<CBL; j++){
			if(cboard[i][j].color == 'W')
				printf("|" MAG "%c" RESET, cboard[i][j].name);
			else if(cboard[i][j].color == 'B')
				printf("|" CYN "%c" RESET, cboard[i][j].name);
			else
				printf("|%c", cboard[i][j].name);
		}
		printf("|%d|\n", i+1);
	}
	printf("| ");
	for(i=0; i<CBL; i++)
		printf("|%c", 'a'+i);
	printf("| |\n");

	return;
}

int verticalMovement(piece p, int x, int y){
	int i;
	if(cboard[x][y].color == p.color) /*cant move on your own pieces*/
		return 0;
	if(p.y == y){ /*Same column is a requirement*/
		if(p.x > x){ /*moves down*/
			for(i=x+1; i<p.x; i++)
				if(cboard[i][y].name != ' ')
					return 0;
		}else if(p.x < x){ /*moves up*/
			for(i = p.x+1; i<x; i++)
				if(cboard[i][y].name != ' ')
					return 0;
		}else /*You cant move to the same position you are in!*/
			return 0;
	}else
		return 0;
	return 1;
}

int horizontalMovement(piece p, int x, int y){
	int j;
	if(cboard[x][y].color == p.color)
		return 0;
	if(p.x == x){ /*same row is a requirement*/
		if(p.y > y){
			for(j=p.y+1; j<y; j++)
				if(cboard[x][j].name != ' ')
					return 0;
			return 1;
		}else if(p.y < y){
			for(j=p.y+1; j<y; j++)
				if(cboard[x][j].name != ' ')
					return 0;
			return 1;
		}
	}
	return 0;
}

int diagonalMovement(piece p, int x, int y){ /*Il bishop può saltare i pezzi adiacenti*/
	int i, j;
	if(cboard[x][y].color == p.color)
		return 0;
	if(abs(p.y-y) != abs(p.x-x))
		return 0;
	if(p.y > y && p.x > x){ /*From bottom right to upper left*/
		for(i=x+1, j=y+1; i<p.x && j<p.y; i++, j++)
			if(cboard[i][j].name != ' ')
				return 0;
	}else if(p.y > y && p.x < x){ /*From upper right to bottom left*/
		for(i=p.x+1, j=y+1; i<x && j<p.y; i++, j++)
			if(cboard[i][j].name != ' ')
				return 0;

	}else if(p.y < y && p.x > x){ /*From upper left to bottom right*/
		for(i=x+1, j=p.y+1; i<p.x && j<y; i++, j++)
			if(cboard[i][j].name != ' ')
				return 0;
	}else if(p.y < y && p.x < x){ /*From bottom left to upper right*/
		for(i=p.x+1, j=p.y+1; i<x && j<y; i++, j++)
			if(cboard[i][j].name != ' ')
				return 0;
	}else /*Not possible ton move on the same square you were*/
		return 0;
	return 1;
}

/*Special pieces movement*/
int knightMovement(piece p, int x, int y){/*MAX 8 movimenti totali concessi*/
	if(cboard[x][y].color == p.color)
		return 0;
	
	if(x == p.x - 2)
		if(y == p.y - 1 || y == p.y + 1)
			return 1;
	if(x == p.x + 2)
		if(y == p.y - 1 || y == p.y + 1)
			return 1;
	if(y == p.y - 2)
		if(x == p.x - 1 || x == p.x + 1)
			return 1;
	if(y == p.y + 2)
		if(x == p.x - 1 || x == p.x + 1)
			return 1;
	return 0;
}

int pawnMovement(piece p, int x, int y){
	if(p.y == y){ /*Movimento normale*/
		if(cboard[x][y].name != ' ')
			return 0;
		if(p.stx == 1){ /*Can only move downwards (forward from the player's perspective)*/
			if(p.stx == p.x){	/*Is at starting point*/
				if((x - p.x == 1 || x - p.x == 2) && verticalMovement(p, x, y)) /*Can only move 1 or 2 places away*/
					return 1;
			}else
				if(((x - p.x) == 1) && verticalMovement(p, x, y))
					return 1;
		}else if(p.stx == 6){ /*Can only move upwards */
			if(p.stx == p.x){ /*starting point*/
				if((p.x - x == 1 || p.x - x == 2) && verticalMovement(p,x,y))
					return 1;
			}else
				if(((p.x - x) == 1) && verticalMovement(p, x, y))
					return 1;
		}else
			printf("Unexpected pawn here!\n");
	}else{ /*Mangiata*/
		if(cboard[x][y].color == p.color || cboard[x][y].name == ' ')
			return 0;
		if(p.stx == 1){ /*Down*/
			if(x == p.x + 1 && (y == p.y - 1 || y == p.y + 1))
				return 1;
		}else if(p.stx == 6){ /*Up*/
			if(x == p.x -1 && (y == p.y - 1 || y == p.y + 1))
				return 1;
		}else
			printf("Unexpected pawn here!\n");
	}
	return 0;
}

int move(int sx, int sy, int x, int y){
	int ret;
	piece p;

	/*Sanitize Input*/
	if(sx < CBL && sx >= 0 && sy < CBL && sy >= 0 && x < CBL && x >= 0 && y < CBL && y >= 0){
		if(cboard[sx][sy].name == ' ')
			return 0;
	}else /*Out of bounds*/
		return 0;

	p = cboard[sx][sy];
	ret = 0;

	if(p.name == 'P'){
		ret = pawnMovement(p, x, y);
	}else if(p.name == 'B'){
		ret = diagonalMovement(p, x, y);
	}else if(p.name == 'H'){
		ret = knightMovement(p, x, y);
	}else if(p.name == 'R'){
		if(p.x == x)
			ret = horizontalMovement(p, x, y);
		else
			ret = verticalMovement(p, x, y);
	}else if(p.name == 'Q'){
		if(p.x == x)
			ret = horizontalMovement(p, x, y);
		else if(p.y == y)
			ret = verticalMovement(p, x, y);
		else
			ret = diagonalMovement(p, x, y);
	}else if(p.name == 'K'){
		if(abs(p.x - x) == 1 || abs(p.y - y) == 1){
			if(p.x == x)
				ret = horizontalMovement(p, x, y);
			else if(p.y == y)
				ret = horizontalMovement(p, x, y);
		}
	}
	if(ret)
		replace(p, cboard[x][y]);
	return ret;
}

void replace(piece p1, piece p2){
	p2.name  = p1.name;
	p2.color = p1.color;
	p2.moves = p1.moves;
	p2.stx   = p1.stx;
	p2.sty   = p1.sty;

	p1.name = ' ';
	p1.color = ' ';
	p1.moves = -1;
	p1.stx = -1;
	p1.sty = -1;
		
	cboard[p1.x][p1.y] = p1;
	cboard[p2.x][p2.y] = p2;

	return ;
}

