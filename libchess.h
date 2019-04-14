/*IMPROVE: NEL MAIN DEL PROGRAMMA PRINCIPALE implementare il ciclo di gioco*/
/*OK: controllare che sia possibile mangiare i pezzi dell'avversario*/
/*TODO: controllare movimenti in condizione di scacco al re*/
/*TODO: REINCARNAZIONE PEDONI CHE ARRIVANO IN FONDO ALLA SCACCHIERA*/
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
int iDoTheMovement(piece, piece, int);

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

int verticalMovement(piece p, piece d){
	int i;
	if(d.color == p.color) /*cant move on your own pieces*/
		return 0;
	if(p.y == d.y){ /*Same column is a requirement*/
		if(p.x > d.x){ /*moves down*/
			for(i=d.x+1; i<p.x; i++)
				if(d.name != ' ')
					return 0;
		}else if(p.x < d.x){ /*moves up*/
			for(i = p.x+1; i<d.x; i++)
				if(d.name != ' ')
					return 0;
		}else /*You cant move to the same position you are in!*/
			return 0;
	}else
		return 0;
	return 1;
}

int horizontalMovement(piece p, piece d){
	int j;
	if(d.color == p.color)
		return 0;
	if(p.x == d.x){ /*same row is a requirement*/
		if(p.y > d.y){
			for(j=p.y+1; j<d.y; j++)
				if(d.name != ' ')
					return 0;
			return 1;
		}else if(p.y < d.y){
			for(j=p.y+1; j<d.y; j++)
				if(d.name != ' ')
					return 0;
			return 1;
		}
	}
	return 0;
}

int diagonalMovement(piece p, piece d){ /*Il bishop può saltare i pezzi adiacenti*/
	int i, j;
	if(d.color == p.color)
		return 0;
	if(abs(p.y-d.y) != abs(p.x-d.x))
		return 0;
	if(p.y > d.y && p.x > d.x){ /*From bottom right to upper left*/
		for(i=d.x+1, j=d.y+1; i<p.x && j<p.y; i++, j++)
			if(d.name != ' ')
				return 0;
	}else if(p.y > d.y && p.x < d.x){ /*From upper right to bottom left*/
		for(i=p.x+1, j=d.y+1; i<d.x && j<p.y; i++, j++)
			if(d.name != ' ')
				return 0;

	}else if(p.y < d.y && p.x > d.x){ /*From upper left to bottom right*/
		for(i=d.x+1, j=p.y+1; i<p.x && j<d.y; i++, j++)
			if(d.name != ' ')
				return 0;
	}else if(p.y < d.y && p.x < d.x){ /*From bottom left to upper right*/
		for(i=p.x+1, j=p.y+1; i<d.x && j<d.y; i++, j++)
			if(d.name != ' ')
				return 0;
	}else /*Not possible ton move on the same square you were*/
		return 0;
	return 1;
}

/*Special pieces movement*/
int knightMovement(piece p, piece d){/*MAX 8 movimenti totali concessi*/
	if(d.color == p.color)
		return 0;
	
	if(d.x == p.x - 2)
		if(d.y == p.y - 1 || d.y == p.y + 1)
			return 1;
	if(d.x == p.x + 2)
		if(d.y == p.y - 1 || d.y == p.y + 1)
			return 1;
	if(d.y == p.y - 2)
		if(d.x == p.x - 1 || d.x == p.x + 1)
			return 1;
	if(d.y == p.y + 2)
		if(d.x == p.x - 1 || d.x == p.x + 1)
			return 1;
	return 0;
}

int pawnMovement(piece p, piece d){
	if(p.y == d.y){ /*Movimento normale*/
		if(d.name != ' ')
			return 0;
		if(p.stx == 1){ /*Can only move downwards (forward from the player's perspective)*/
			if(p.stx == p.x){	/*Is at starting point*/
				if((d.x - p.x == 1 || d.x - p.x == 2) && verticalMovement(p, d)) /*Can only move 1 or 2 places away*/
					return 1;
			}else
				if(((d.x - p.x) == 1) && verticalMovement(p, d))
					return 1;
		}else if(p.stx == 6){ /*Can only move upwards */
			if(p.stx == p.x){ /*starting point*/
				if((p.x - d.x == 1 || p.x - d.x == 2) && verticalMovement(p, d))
					return 1;
			}else
				if(((p.x - d.x) == 1) && verticalMovement(p, d))
					return 1;
		}else
			printf("Unexpected pawn here!\n");
	}else{ /*Mangiata*/
		if(d.color == p.color || d.name == ' ')
			return 0;
		if(p.stx == 1){ /*Down*/
			if(d.x == p.x + 1 && (d.y == p.y - 1 || d.y == p.y + 1))
				return 1;
		}else if(p.stx == 6){ /*Up*/
			if(d.x == p.x -1 && (d.y == p.y - 1 || d.y == p.y + 1))
				return 1;
		}else
			printf("Unexpected pawn here!\n");
	}
	return 0;
}
/*Takes 4 parameters as they're inserted by the user*/
int move(int sx, int sy, int x, int y, char color){
	int ret;
	piece p, a;
	
	/*Sanitize Input*/
	if(color != 'B' && color != 'W')
		return 0;
	if(sx < CBL && sx >= 0 && sy < CBL && sy >= 0 && x < CBL && x >= 0 && y < CBL && y >= 0){
		if(cboard[sx][sy].name == ' ' || cboard[sx][sy].color != color)
			return 0;
	}else /*Out of bounds*/
		return 0;

	p = cboard[sx][sy];
	a = cboard[x][y];
	ret = 0;
	
	ret = iDoTheMovement(p, a, 1);
	return ret;
}
/*Piece p moves towards destination d and ask if you want to replace or just check i possible (ret.1 if is fine, 0 if not)*/
int iDoTheMovement(piece p, piece d, int really){
	int r; /*return value: 1=ok, 0=ko*/

	r = 0;
	if(p.name == 'P'){
		r = pawnMovement(p, d);
	}else if(p.name == 'B'){
		r = diagonalMovement(p, d);
	}else if(p.name == 'H'){
		r = knightMovement(p, d);
	}else if(p.name == 'R'){
		if(p.x == d.x)
			r = horizontalMovement(p, d);
		else
			r = verticalMovement(p, d);
	}else if(p.name == 'Q'){
		if(p.x == d.x)
			r = horizontalMovement(p, d);
		else if(p.y == d.y)
			r = verticalMovement(p, d);
		else
			r = diagonalMovement(p, d);
	}else if(p.name == 'K'){
		if(abs(p.x - d.x) == 1 || abs(p.y - d.y) == 1){
			if(p.x == d.x)
				r = horizontalMovement(p, d);
			else if(p.y == d.y)
				r = horizontalMovement(p, d);
		}
	}
	if(r && really)
		replace(p, d);
	if(r)
		return 1;
	return 0;
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
/*Checks if the king of the specified color [B/W] is under scacco*/
int isScacco(char color){
	int i, j, flag;
	piece king;

	if(color != 'B' && color != 'W')
		return 0;
	flag = 1;
	for(i=0; i<CBL && flag; i++)
		for(j=0; j<CBL && flag; j++)
			if(cboard[i][j].name == 'K' && cboard[i][j].color == color){
				king = cboard[i][j];
				flag = 0;
			}
	for(i=0; i<CBL; i++){
		for(j=0; j<CBL; j++){
			if(iDoTheMovement(cboard[i][j], king, 0))
				return 1;
		}
	}
	return 0;
}
