#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

const int square = 100;

// Global arrays to track captured pieces
char whiteCaptured[16];
int whiteCapturedCount = 0;
char blackCaptured[16];
int blackCapturedCount = 0;


void drawboard(RenderWindow& window, int squaresize) {
	RectangleShape square(Vector2f(squaresize, squaresize));
	for (int rows = 0; rows < 8; rows++) {
		for (int col = 0; col < 8; col++) {
			if ((rows + col) % 2 == 0) {
				square.setFillColor(Color::White);
			}
			else {
				square.setFillColor(Color(80, 80, 80));
			}
			square.setPosition(col * squaresize, rows * squaresize);
			window.draw(square);
		}
	}
}

void drawSidebar(RenderWindow& window) {
	RectangleShape sidebar(Vector2f(100, 800));
	sidebar.setPosition(800, 0);
	sidebar.setFillColor(Color(40, 40, 40));
	window.draw(sidebar);

	RectangleShape divider(Vector2f(2, 800));
	divider.setPosition(799, 0);
	divider.setFillColor(Color::White);
	window.draw(divider);
}

void drawCapturedPieces(RenderWindow& window,
	Texture& wpawn, Texture& wrook, Texture& wknight,
	Texture& wbishop, Texture& wqueen, Texture& wking,
	Texture& bpawn, Texture& brook, Texture& bknight,
	Texture& bbishop, Texture& bqueen, Texture& bking) {

	int pieceSize = 35;
	int startX = 820;
	float spacing = 40;

	// Draw white captured pieces (captured by black) at top
	float topY = 20;
	for (int i = 0; i < whiteCapturedCount; i++) {
		Sprite capturedSprite;

		if (whiteCaptured[i] == 'P') capturedSprite.setTexture(wpawn);
		else if (whiteCaptured[i] == 'R') capturedSprite.setTexture(wrook);
		else if (whiteCaptured[i] == 'N') capturedSprite.setTexture(wknight);
		else if (whiteCaptured[i] == 'B') capturedSprite.setTexture(wbishop);
		else if (whiteCaptured[i] == 'Q') capturedSprite.setTexture(wqueen);
		else if (whiteCaptured[i] == 'K') capturedSprite.setTexture(wking);

		capturedSprite.setScale((float)pieceSize / wpawn.getSize().x,
			(float)pieceSize / wpawn.getSize().y);
		capturedSprite.setPosition(startX, topY);
		window.draw(capturedSprite);

		topY += spacing;
	}

	// Draw black captured pieces (captured by white) at bottom
	float bottomY = 420;
	for (int i = 0; i < blackCapturedCount; i++) {
		Sprite capturedSprite;

		if (blackCaptured[i] == 'p') capturedSprite.setTexture(bpawn);
		else if (blackCaptured[i] == 'r') capturedSprite.setTexture(brook);
		else if (blackCaptured[i] == 'n') capturedSprite.setTexture(bknight);
		else if (blackCaptured[i] == 'b') capturedSprite.setTexture(bbishop);
		else if (blackCaptured[i] == 'q') capturedSprite.setTexture(bqueen);
		else if (blackCaptured[i] == 'k') capturedSprite.setTexture(bking);

		capturedSprite.setScale((float)pieceSize / bpawn.getSize().x,
			(float)pieceSize / bpawn.getSize().y);
		capturedSprite.setPosition(startX, bottomY);
		window.draw(capturedSprite);

		bottomY += spacing;
	}
}

void drawLabels(RenderWindow& window, Font& font) {
	// Draw column labels (a-h) at bottom
	char colLabels[] = "abcdefgh";
	for (int i = 0; i < 8; i++) {
		Text label;
		label.setFont(font);
		label.setString(colLabels[i]);
		label.setCharacterSize(20);
		label.setFillColor(Color::Black);
		label.setPosition(i * square + 2, 2);
		window.draw(label);
	}

	// Draw row labels (8-1) on right side
	for (int i = 0; i < 8; i++) {
		Text label;
		label.setFont(font);
		label.setString(to_string(8 - i));
		label.setCharacterSize(20);
		label.setFillColor(Color::Black);
		label.setPosition(800 - 25, i * square + square / 2 - 12);
		window.draw(label);
	}
}

void initializerboard(char board[8][8]) {
	board[0][0] = 'r';
	board[0][1] = 'n';
	board[0][2] = 'b';
	board[0][3] = 'q';
	board[0][4] = 'k';
	board[0][5] = 'b';
	board[0][6] = 'n';
	board[0][7] = 'r';
	for (int i = 1; i <= 1; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = 'p';
		}
	}
	for (int i = 2; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = ' ';
		}
	}
	for (int i = 6; i <= 6; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = 'P';
		}
	}
	board[7][0] = 'R';
	board[7][1] = 'N';
	board[7][2] = 'B';
	board[7][3] = 'Q';
	board[7][4] = 'K';
	board[7][5] = 'B';
	board[7][6] = 'N';
	board[7][7] = 'R';
}

void displayboard(char board[8][8]) {
	cout << "   a b c d e f g h" << endl;
	cout << "------------------" << endl;
	for (int i = 0; i < 8; i++) {
		cout << "|" << 8 - i << "|";
		for (int j = 0; j < 8; j++) {
			cout << board[i][j] << " ";
		}
		cout << "|";
		cout << endl;
	}
	cout << "-------------------" << endl;
}

bool isvalidpawnmove(int sx, int sy, int dx, int dy, char board[8][8]) {
	if (board[sx][sy] == 'P') {
		if (dx == sx - 1 && dy == sy && board[sx - 1][sy] == ' ') {
			return true;
		}
		if (sx == 6 && dx == sx - 2 && dy == sy && board[sx - 2][sy] == ' ' && board[sx - 1][sy] == ' ') {
			return true;
		}
		if (dx == sx - 1 && (dy == sy + 1 || dy == sy - 1)) {
			if (board[dx][dy] >= 'a' && board[dx][dy] <= 'z') {
				return true;
			}
		}
	}
	if (board[sx][sy] == 'p') {
		if (dx == sx + 1 && dy == sy && board[sx + 1][sy] == ' ') {
			return true;
		}
		if (dx == sx + 2 && sx == 1 && dy == sy && board[sx + 1][sy] == ' ' && board[sx + 2][sy] == ' ') {
			return true;
		}
		if (dx == sx + 1 && (dy == sy + 1 || dy == sy - 1) && (board[dx][dy] >= 'A' && board[dx][dy] <= 'Z')) {
			return true;
		}
	}
	return false;
}

bool validbishopmove(int sx, int sy, int dx, int dy, char board[8][8]) {
	if (abs(dx - sx) != abs(dy - sy)) {
		return false;
	}
	int x;
	int y;
	if (dx > sx) {
		x = 1;
	}
	else {
		x = -1;
	}
	if (dy > sy) {
		y = 1;
	}
	else {
		y = -1;
	}
	int xmove = sx + x;
	int ymove = sy + y;
	while (xmove != dx && ymove != dy) {
		if (board[xmove][ymove] != ' ') {
			return false;
		}
		xmove += x;
		ymove += y;
	}
	char selected = board[sx][sy];
	char destination = board[dx][dy];
	if (destination != ' ') {
		if ((selected >= 'A' && selected <= 'Z') && (destination >= 'A' && destination <= 'Z') || (selected >= 'a' && selected <= 'z') && (destination >= 'a' && destination <= 'z')) {
			return false;
		}
	}
	return true;
}

bool isvalidrookmove(int sx, int sy, int dx, int dy, char board[8][8]) {
	if (sx != dx && sy != dy) {
		return false;
	}
	int x = 0;
	int y = 0;
	if (dx > sx) {
		x = 1;
	}
	else if (dx < sx) {
		x = -1;
	}
	if (dy > sy) {
		y = 1;
	}
	else if (dy < sy) {
		y = -1;
	}
	int xmove = sx + x;
	int ymove = sy + y;
	while (xmove != dx || ymove != dy) {
		if (board[xmove][ymove] != ' ') {
			return false;
		}
		xmove += x;
		ymove += y;
	}
	char selected = board[sx][sy];
	char destination = board[dx][dy];
	if (destination != ' ') {
		if ((selected >= 'A' && selected <= 'Z') && (destination >= 'A' && destination <= 'Z') || (selected >= 'a' && selected <= 'z') && (destination >= 'a' && destination <= 'z')) {
			return false;
		}
	}
	return true;
}

bool isvalidqueenmove(int sx, int sy, int dx, int dy, char board[8][8]) {
	if (!((sx == dx || sy == dy) || (abs(dx - sx) == abs(dy - sy)))) {
		return false;
	}
	int x = 0;
	int y = 0;
	if (dx > sx) {
		x = 1;
	}
	else if (dx < sx) {
		x = -1;
	}
	if (dy > sy) {
		y = 1;
	}
	else if (dy < sy) {
		y = -1;
	}
	int xmove = sx + x;
	int ymove = sy + y;
	while (xmove != dx || ymove != dy) {
		if (board[xmove][ymove] != ' ') {
			return false;
		}
		xmove += x;
		ymove += y;
	}
	char selected = board[sx][sy];
	char destination = board[dx][dy];
	if (destination != ' ') {
		if ((selected >= 'A' && selected <= 'Z') && (destination >= 'A' && destination <= 'Z') || (selected >= 'a' && selected <= 'z') && (destination >= 'a' && destination <= 'z')) {
			return false;
		}
	}
	return true;
}

bool isvalidkingmove(int sx, int sy, int dx, int dy, char board[8][8]) {
	if (!((dx == sx + 1 && dy == sy) || (dx == sx - 1 && dy == sy) || (dy == sy + 1 && dx == sx) || (dy == sy - 1 && dx == sx)
		|| (dx == sx + 1 && dy == sy + 1) || (dx == sx + 1 && dy == sy - 1) || (dy == sy + 1 && dx == sx - 1) || (dy == sy - 1 && dx == sx - 1))) {
		return false;
	}
	char destination = board[dx][dy];
	char selected = board[sx][sy];
	if (destination != ' ') {
		if (((selected >= 'A' && selected <= 'Z') && (destination >= 'A' && destination <= 'Z')) || ((selected >= 'a' && selected <= 'z') && (destination >= 'a' && destination <= 'z'))) {
			return false;
		}
	}
	return true;
}

bool isvalidknightmove(int sx, int sy, int dx, int dy, char board[8][8]) {
	if (!((dx == sx + 2 && dy == sy + 1) || (dx == sx + 2 && dy == sy - 1) || (dx == sx + 1 && dy == sy + 2) || (dx == sx + 1 && dy == sy - 2) || (dx == sx - 2 && dy == sy + 1) ||
		(dx == sx - 2 && dy == sy - 1) || (dx == sx - 1 && dy == sy + 2) || (dx == sx - 1 && dy == sy - 2))) {
		return false;
	}
	char destination = board[dx][dy];
	char selected = board[sx][sy];
	if (destination != ' ') {
		if (((selected >= 'A' && selected <= 'Z') && (destination >= 'A' && destination <= 'Z')) || ((selected >= 'a' && selected <= 'z') && (destination >= 'a' && destination <= 'z'))) {
			return false;
		}
	}
	return true;
}

bool incheck(char board[8][8], bool& whiteturn) {
	int xking = -1;
	int yking = -1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (whiteturn && board[i][j] == 'K') {
				xking = i;
				yking = j;
			}
			if (!whiteturn && board[i][j] == 'k') {
				xking = i;
				yking = j;
			}
		}
	}
	if (xking == -1) {
		return false;
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			char piece = board[i][j];
			if (!whiteturn && (piece >= 'A' && piece <= 'Z')) {
				if ((piece == 'P' && xking == i - 1 && (yking == j - 1 || yking == j + 1)) ||
					(piece == 'R' && isvalidrookmove(i, j, xking, yking, board)) ||
					(piece == 'B' && validbishopmove(i, j, xking, yking, board)) ||
					(piece == 'Q' && isvalidqueenmove(i, j, xking, yking, board)) ||
					(piece == 'N' && isvalidknightmove(i, j, xking, yking, board)) ||
					(piece == 'K' && isvalidkingmove(i, j, xking, yking, board))) {
					return true;
				}
			}
			if (whiteturn && (piece >= 'a' && piece <= 'z')) {
				if ((piece == 'p' && xking == i + 1 && (yking == j - 1 || yking == j + 1)) ||
					(piece == 'r' && isvalidrookmove(i, j, xking, yking, board)) ||
					(piece == 'b' && validbishopmove(i, j, xking, yking, board)) ||
					(piece == 'q' && isvalidqueenmove(i, j, xking, yking, board)) ||
					(piece == 'n' && isvalidknightmove(i, j, xking, yking, board)) ||
					(piece == 'k' && isvalidkingmove(i, j, xking, yking, board))) {
					return true;
				}
			}
		}
	}
	return false;
}

bool stillincheck(int sx, int sy, int dx, int dy, char board[8][8], bool& whiteturn) {
	char destination = board[dx][dy];
	char selected = board[sx][sy];
	board[dx][dy] = board[sx][sy];
	board[sx][sy] = ' ';
	bool stillincheck = incheck(board, whiteturn);
	board[dx][dy] = destination;
	board[sx][sy] = selected;
	return stillincheck;
}

bool makemove(int sx, int sy, int dx, int dy, char board[8][8], bool& whiteturn) {
	char piece = board[sx][sy];
	char captured = board[dx][dy];
	bool valid = false;

	if (whiteturn && piece != ' ') {
		if (piece >= 'A' && piece <= 'Z') {
			if (piece == 'R') {
				valid = isvalidrookmove(sx, sy, dx, dy, board);
			}
			else if (piece == 'B') {
				valid = validbishopmove(sx, sy, dx, dy, board);
			}
			else if (piece == 'N') {
				valid = isvalidknightmove(sx, sy, dx, dy, board);
			}
			else if (piece == 'P') {
				valid = isvalidpawnmove(sx, sy, dx, dy, board);
			}
			else if (piece == 'K') {
				valid = isvalidkingmove(sx, sy, dx, dy, board);
			}
			else if (piece == 'Q') {
				valid = isvalidqueenmove(sx, sy, dx, dy, board);
			}
			if (valid && !stillincheck(sx, sy, dx, dy, board, whiteturn)) {
				// Track captured piece
				if (captured >= 'a' && captured <= 'z' && blackCapturedCount < 16) {
					blackCaptured[blackCapturedCount] = captured;
					blackCapturedCount++;
				}
				board[dx][dy] = piece;
				board[sx][sy] = ' ';
				whiteturn = false;
				return true;
			}
		}
	}
	else if (!whiteturn && piece != ' ') {
		if (piece >= 'a' && piece <= 'z') {
			if (piece == 'r') {
				valid = isvalidrookmove(sx, sy, dx, dy, board);
			}
			else if (piece == 'b') {
				valid = validbishopmove(sx, sy, dx, dy, board);
			}
			else if (piece == 'n') {
				valid = isvalidknightmove(sx, sy, dx, dy, board);
			}
			else if (piece == 'p') {
				valid = isvalidpawnmove(sx, sy, dx, dy, board);
			}
			else if (piece == 'k') {
				valid = isvalidkingmove(sx, sy, dx, dy, board);
			}
			else if (piece == 'q') {
				valid = isvalidqueenmove(sx, sy, dx, dy, board);
			}
			if (valid && !stillincheck(sx, sy, dx, dy, board, whiteturn)) {
				// Track captured piece
				if (captured >= 'A' && captured <= 'Z' && whiteCapturedCount < 16) {
					whiteCaptured[whiteCapturedCount] = captured;
					whiteCapturedCount++;
				}
				board[dx][dy] = piece;
				board[sx][sy] = ' ';
				whiteturn = true;
				return true;
			}
		}
	}
	return false;
}

bool candefend(char board[8][8], bool& whiteturn) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			char piece = board[i][j];
			if (piece == ' ') {
				continue;
			}
			if (whiteturn && !(piece >= 'A' && piece <= 'Z')) {
				continue;
			}
			if (!whiteturn && !(piece >= 'a' && piece <= 'z')) {
				continue;
			}
			for (int x = 0; x < 8; x++) {
				for (int y = 0; y < 8; y++) {
					bool validMove = false;
					if (piece == 'Q' || piece == 'q') {
						validMove = isvalidqueenmove(i, j, x, y, board);
					}
					else if (piece == 'N' || piece == 'n') {
						validMove = isvalidknightmove(i, j, x, y, board);
					}
					else if (piece == 'B' || piece == 'b') {
						validMove = validbishopmove(i, j, x, y, board);
					}
					else if (piece == 'R' || piece == 'r') {
						validMove = isvalidrookmove(i, j, x, y, board);
					}
					else if (piece == 'P' || piece == 'p') {
						validMove = isvalidpawnmove(i, j, x, y, board);
					}
					else if (piece == 'K' || piece == 'k') {
						validMove = isvalidkingmove(i, j, x, y, board);
					}
					if (validMove) {
						if (!stillincheck(i, j, x, y, board, whiteturn)) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool checkmate(char board[8][8], bool& whiteturn) {
	if (!incheck(board, whiteturn)) {
		return false;
	}
	if (!candefend(board, whiteturn)) {
		return true;
	}
	return false;
}

bool stalemate(char board[8][8], bool& whiteturn) {
	if (incheck(board, whiteturn)) {
		return false;
	}
	if (!candefend(board, whiteturn)) {
		return true;
	}
	return false;
}

bool pawnpromotion(int dx, int dy, char board[8][8]) {
	char piece = board[dx][dy];
	if (piece == 'P' && dx == 0) {
		cout << "\n*** WHITE PAWN PROMOTION ***" << endl;
		cout << "Enter piece (Q/R/N/B): ";
		cout.flush();
		char promoted;
		cin >> promoted;
		if (promoted == 'Q' || promoted == 'R' || promoted == 'N' || promoted == 'B') {
			board[dx][dy] = promoted;
			cout << "Promoted to " << promoted << "!\n" << endl;
			return true;
		}
		else {
			cout << "Invalid! Defaulting to Queen.\n";
			board[dx][dy] = 'Q';
			return true;
		}
	}
	if (piece == 'p' && dx == 7) {
		cout << "\n---------- BLACK PAWN PROMOTION -------" << endl;
		cout << "Enter piece (q/r/n/b): ";
		cout.flush();
		char promoted;
		cin >> promoted;
		if (promoted == 'q' || promoted == 'r' || promoted == 'n' || promoted == 'b') {
			board[dx][dy] = promoted;
			cout << "Promoted to " << promoted << "!\n" << endl;
			return true;
		}
		else {
			cout << "Invalid! Defaulting to Queen.\n";
			board[dx][dy] = 'q';
			return true;
		}
	}
	return false;
}

void spritesafterpromotion(int dx, int dy, char promotedPiece,
	Sprite& whiteQueen, Sprite& whiteRook, Sprite& whiteBishop, Sprite& whiteKnight,
	Sprite& blackQueen, Sprite& blackRook, Sprite& blackBishop, Sprite& blackKnight,
	int square) {
	if (promotedPiece == 'Q') {
		whiteQueen.setPosition(dy * square, dx * square);
	}
	else if (promotedPiece == 'R') {
		whiteRook.setPosition(dy * square, dx * square);
	}
	else if (promotedPiece == 'B') {
		whiteBishop.setPosition(dy * square, dx * square);
	}
	else if (promotedPiece == 'N') {
		whiteKnight.setPosition(dy * square, dx * square);
	}
	else if (promotedPiece == 'q') {
		blackQueen.setPosition(dy * square, dx * square);
	}
	else if (promotedPiece == 'r') {
		blackRook.setPosition(dy * square, dx * square);
	}
	else if (promotedPiece == 'b') {
		blackBishop.setPosition(dy * square, dx * square);
	}
	else if (promotedPiece == 'n') {
		blackKnight.setPosition(dy * square, dx * square);
	}
}


void hidecapturepieces(Sprite pieces[], int size, char pieceChar, char board[8][8]) {
	for (int i = 0; i < size; i++) {
		Vector2f pos = pieces[i].getPosition();
		int col = int(pos.x / square);
		int row = int(pos.y / square);
		if (col >= 0 && col < 8 && row >= 0 && row < 8) {
			if (board[row][col] != pieceChar) {
				pieces[i].setPosition(-1000, -1000);
			}
		}
	}
}

bool removedaftercapture(Sprite& sprite, char piece, char board[8][8], bool isdragging) {
	if (isdragging) {
		return true;
	}
	Vector2f spritepos = sprite.getPosition();
	if (spritepos.x < -100 || spritepos.y < -100) {
		return false;
	}
	int col = int(spritepos.x / square);
	int row = int(spritepos.y / square);
	if (row > 7 || row < 0 || col < 0 || col>7) {
		return false;
	}
	if (board[row][col] == piece) {
		return true;
	}
	return false;
}

bool isvalidmovehighlight(int sx, int sy, int dx, int dy, char board[8][8], bool whiteturn) {
	char piece = board[sx][sy];
	if (piece == ' ') return false;
	bool valid = false;
	if (whiteturn && piece >= 'A' && piece <= 'Z') {
		if (piece == 'P') {
			valid = isvalidpawnmove(sx, sy, dx, dy, board);
		}
		else if (piece == 'R') {
			valid = isvalidrookmove(sx, sy, dx, dy, board);
		}
		else if (piece == 'N') {
			valid = isvalidknightmove(sx, sy, dx, dy, board);
		}
		else if (piece == 'B') {
			valid = validbishopmove(sx, sy, dx, dy, board);
		}
		else if (piece == 'Q') {
			valid = isvalidqueenmove(sx, sy, dx, dy, board);
		}
		else if (piece == 'K') {
			valid = isvalidkingmove(sx, sy, dx, dy, board);
		}
	}
	else if (!whiteturn && piece >= 'a' && piece <= 'z') {
		if (piece == 'p') {
			valid = isvalidpawnmove(sx, sy, dx, dy, board);
		}
		else if (piece == 'r') {
			valid = isvalidrookmove(sx, sy, dx, dy, board);
		}
		else if (piece == 'n') {
			valid = isvalidknightmove(sx, sy, dx, dy, board);
		}
		else if (piece == 'b') {
			valid = validbishopmove(sx, sy, dx, dy, board);
		}
		else if (piece == 'q') {
			valid = isvalidqueenmove(sx, sy, dx, dy, board);
		}
		else if (piece == 'k') {
			valid = isvalidkingmove(sx, sy, dx, dy, board);
		}
	}
	if (valid) {
		valid = !stillincheck(sx, sy, dx, dy, board, whiteturn);
	}
	return valid;
}

void highlightDropSquare(RenderWindow& window, int sx, int sy, int dx, int dy, char board[8][8], bool whiteturn) {
	int square = 100;
	RectangleShape box(Vector2f(square, square));
	bool valid = false;
	if (dx >= 0 && dx < 8 && dy >= 0 && dy < 8) {
		valid = isvalidmovehighlight(sx, sy, dx, dy, board, whiteturn);
	}
	box.setPosition(dy * square, dx * square);
	box.setFillColor(valid ? Color(0, 255, 0, 120) : Color(255, 0, 0, 120));
	window.draw(box);
}

void handleDrag(Sprite pieces[], int size, RenderWindow& window, bool& dragging,
	int& draggingIndex, Vector2f& grabbed, int& inuse, int arrayid,
	char board[8][8], bool& whiteturn, int& origRow, int& origCol, bool& gameover,
	Sprite& whiteQueen, Sprite& whiteRook, Sprite& whiteBishop, Sprite& whiteKnight,
	Sprite& blackQueen, Sprite& blackRook, Sprite& blackBishop, Sprite& blackKnight) {

	Vector2i pixelPos = Mouse::getPosition(window);
	Vector2f boardPos = window.mapPixelToCoords(pixelPos);

	if (Mouse::isButtonPressed(Mouse::Left) && !dragging) {
		for (int i = 0; i < size; i++) {
			if (pieces[i].getGlobalBounds().contains(boardPos)) {
				Vector2f pos = pieces[i].getPosition();
				int col = int(pos.x / square);
				int row = int(pos.y / square);
				char piece = board[row][col];
				bool isWhitePiece = (piece >= 'A' && piece <= 'Z');
				bool isBlackPiece = (piece >= 'a' && piece <= 'z');
				if ((whiteturn && isWhitePiece) || (!whiteturn && isBlackPiece)) {
					dragging = true;
					draggingIndex = i;
					inuse = arrayid;
					grabbed = boardPos - pieces[i].getPosition();
					origRow = row;
					origCol = col;
				}
				break;
			}
		}
	}

	if (inuse != arrayid) {
		return;
	}

	if (!Mouse::isButtonPressed(Mouse::Left)) {
		if (dragging && draggingIndex != -1) {
			Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
			int newCol = int(mousePos.x / square);
			int newRow = int(mousePos.y / square);
			if (newCol < 0) newCol = 0;
			if (newCol > 7) newCol = 7;
			if (newRow < 0) newRow = 0;
			if (newRow > 7) newRow = 7;

			bool moveSuccess = makemove(origRow, origCol, newRow, newCol, board, whiteturn);
			if (moveSuccess) {
				pieces[draggingIndex].setPosition(newCol * square, newRow * square);
				if (pawnpromotion(newRow, newCol, board)) {
					char promotedPiece = board[newRow][newCol];
					spritesafterpromotion(newRow, newCol, promotedPiece,whiteQueen,whiteRook,whiteBishop,whiteKnight,blackQueen,blackRook,blackBishop,blackKnight, square);
				
					cout << "pawn promoted\n";
				}
				displayboard(board);
				if (!gameover) {
					if (checkmate(board, whiteturn)) {
						gameover = true;
						if (whiteturn) {
							cout << "Checkmate! Black wins" << endl;
						}
						else {
							cout << "Checkmate! White wins" << endl;
						}
					}
					else if (stalemate(board, whiteturn)) {
						gameover = true;
						cout << "Stalemate! Game is a draw!" << endl;
					}
					else if (incheck(board, whiteturn)) {
						if (whiteturn) {
							cout << "White is in check!" << endl;
						}
						else {
							cout << "Black is in check!" << endl;
						}
					}
				}
			}
			else {
				pieces[draggingIndex].setPosition(origCol * square, origRow * square);
				cout << "Invalid move!" << endl;
			}
		}
		dragging = false;
		draggingIndex = -1;
		inuse = -1;
	}

	if (dragging && draggingIndex != -1) {
		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		pieces[draggingIndex].setPosition(mousePos - grabbed);
	}
}

int main() {
	RenderWindow window(VideoMode(900, 800), "Chess Board");
	int square = 100;
	char board[8][8];
	bool dragging = false;
	int draggingindex = -1;
	int inuse = -1;
	bool whiteturn = true;
	int origRow = 0;
	int origCol = 0;
	bool gameover = false;

	initializerboard(board);
	displayboard(board);

	Font Font;
	if (!Font.loadFromFile("Textures/arial.ttf")) {
		cout << "Font error" << endl;
	}

	Vector2f grabbed;

	Texture pawntex, blackpawn, king, queen, bishop, knight, rook;
	Texture bking, bqueen, bbishop, bknight, brook;


	if (!pawntex.loadFromFile("Textures/white-pawn.png")) {
		cout << "error" << endl;
	}
	if (!blackpawn.loadFromFile("Textures/black-pawn.png")) {
		cout << "error" << endl;
	}
	if (!king.loadFromFile("Textures/white-king.png")) {
		cout << "error" << endl;
	}
	if (!queen.loadFromFile("Textures/white-queen.png")) {
		cout << "error" << endl;
	}
	if (!bishop.loadFromFile("Textures/white-bishop.png")) {
		cout << "error" << endl;
	}
	if (!knight.loadFromFile("Textures/white-knight.png")) {
		cout << "error" << endl;
	}
	if (!rook.loadFromFile("Textures/white-rook.png")) {
		cout << "error" << endl;
	}
	if (!bking.loadFromFile("Textures/black-king.png")) {
		cout << "error" << endl;
	}
	if (!bqueen.loadFromFile("Textures/black-queen.png")) {
		cout << "error" << endl;
	}
	if (!bbishop.loadFromFile("Textures/black-bishop.png")) {
		cout << "error" << endl;
	}
	if (!bknight.loadFromFile("Textures/black-knight.png")) {
		cout << "error" << endl;
	}
	if (!brook.loadFromFile("Textures/black-rook.png")) {
		cout << "error";
	}

	Sprite pawn[8], blackp[8];
	Sprite whiteking[1], whitequeen[1], whitebishop[2], whiteknight[2], whiterook[2];
	Sprite blackking[1], blackqueen[1], blackbishop[2], blackknight[2], blackrook[2];


	for (int i = 0; i < 8; i++) {
		pawn[i].setTexture(pawntex);
		pawn[i].setScale((float)square / pawntex.getSize().x, (float)square / pawntex.getSize().y);
		pawn[i].setPosition(i * square, 6 * square);

		blackp[i].setTexture(blackpawn);
		blackp[i].setScale((float)square / blackpawn.getSize().x, (float)square / blackpawn.getSize().y);
		blackp[i].setPosition(i * square, 1 * square);
	}

	whiteking[0].setTexture(king);
	whiteking[0].setScale((float)square / king.getSize().x, (float)square / king.getSize().y);
	whiteking[0].setPosition(4 * square, 7 * square);

	whitequeen[0].setTexture(queen);
	whitequeen[0].setScale((float)square / queen.getSize().x, (float)square / queen.getSize().y);
	whitequeen[0].setPosition(3 * square, 7 * square);

	whitebishop[0].setTexture(bishop);
	whitebishop[0].setScale((float)square / bishop.getSize().x, (float)square / bishop.getSize().y);
	whitebishop[0].setPosition(2 * square, 7 * square);
	whitebishop[1].setTexture(bishop);
	whitebishop[1].setScale((float)square / bishop.getSize().x, (float)square / bishop.getSize().y);
	whitebishop[1].setPosition(5 * square, 7 * square);

	whiteknight[0].setTexture(knight);
	whiteknight[0].setScale((float)square / knight.getSize().x, (float)square / knight.getSize().y);
	whiteknight[0].setPosition(1 * square, 7 * square);
	whiteknight[1].setTexture(knight);
	whiteknight[1].setScale((float)square / knight.getSize().x, (float)square / knight.getSize().y);
	whiteknight[1].setPosition(6 * square, 7 * square);

	whiterook[0].setTexture(rook);
	whiterook[0].setScale((float)square / rook.getSize().x, (float)square / rook.getSize().y);
	whiterook[0].setPosition(0 * square, 7 * square);
	whiterook[1].setTexture(rook);
	whiterook[1].setScale((float)square / rook.getSize().x, (float)square / rook.getSize().y);
	whiterook[1].setPosition(7 * square, 7 * square);

	blackking[0].setTexture(bking);
	blackking[0].setScale((float)square / bking.getSize().x, (float)square / bking.getSize().y);
	blackking[0].setPosition(4 * square, 0 * square);

	blackqueen[0].setTexture(bqueen);
	blackqueen[0].setScale((float)square / bqueen.getSize().x, (float)square / bqueen.getSize().y);
	blackqueen[0].setPosition(3 * square, 0 * square);

	blackbishop[0].setTexture(bbishop);
	blackbishop[0].setScale((float)square / bbishop.getSize().x, (float)square / bbishop.getSize().y);
	blackbishop[0].setPosition(2 * square, 0 * square);
	blackbishop[1].setTexture(bbishop);
	blackbishop[1].setScale((float)square / bbishop.getSize().x, (float)square / bbishop.getSize().y);
	blackbishop[1].setPosition(5 * square, 0 * square);

	blackknight[0].setTexture(bknight);
	blackknight[0].setScale((float)square / bknight.getSize().x, (float)square / bknight.getSize().y);
	blackknight[0].setPosition(1 * square, 0 * square);
	blackknight[1].setTexture(bknight);
	blackknight[1].setScale((float)square / bknight.getSize().x, (float)square / bknight.getSize().y);
	blackknight[1].setPosition(6 * square, 0 * square);

	blackrook[0].setTexture(brook);
	blackrook[0].setScale((float)square / brook.getSize().x, (float)square / brook.getSize().y);
	blackrook[0].setPosition(0 * square, 0 * square);
	blackrook[1].setTexture(brook);
	blackrook[1].setScale((float)square / brook.getSize().x, (float)square / brook.getSize().y);
	blackrook[1].setPosition(7 * square, 0 * square);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}

		if (!gameover) {
			handleDrag(pawn, 8, window, dragging, draggingindex, grabbed, inuse, 0, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(blackp, 8, window, dragging, draggingindex, grabbed, inuse, 1, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(whiteking, 1, window, dragging, draggingindex, grabbed, inuse, 2, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(whitequeen, 1, window, dragging, draggingindex, grabbed, inuse, 3, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(whitebishop, 2, window, dragging, draggingindex, grabbed, inuse, 4, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(whiteknight, 2, window, dragging, draggingindex, grabbed, inuse, 5, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(whiterook, 2, window, dragging, draggingindex, grabbed, inuse, 6, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(blackking, 1, window, dragging, draggingindex, grabbed, inuse, 7, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(blackqueen, 1, window, dragging, draggingindex, grabbed, inuse, 8, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(blackbishop, 2, window, dragging, draggingindex, grabbed, inuse, 9, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(blackknight, 2, window, dragging, draggingindex, grabbed, inuse, 10, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
			handleDrag(blackrook, 2, window, dragging, draggingindex, grabbed, inuse, 11, board, whiteturn, origRow, origCol, gameover, whitequeen[0], whiterook[0], whitebishop[0], whiteknight[0], blackqueen[0], blackrook[0], blackbishop[0], blackknight[0]);
		}

		hidecapturepieces(pawn, 8, 'P', board);
		hidecapturepieces(blackp, 8, 'p', board);
		hidecapturepieces(whiteking, 1, 'K', board);
		hidecapturepieces(whitequeen, 1, 'Q', board);
		hidecapturepieces(whitebishop, 2, 'B', board);
		hidecapturepieces(whiteknight, 2, 'N', board);
		hidecapturepieces(whiterook, 2, 'R', board);
		hidecapturepieces(blackking, 1, 'k', board);
		hidecapturepieces(blackqueen, 1, 'q', board);
		hidecapturepieces(blackbishop, 2, 'b', board);
		hidecapturepieces(blackknight, 2, 'n', board);
		hidecapturepieces(blackrook, 2, 'r', board);

		window.clear();
		drawboard(window, square);
		drawLabels(window, Font);
		drawSidebar(window);
		drawCapturedPieces(window, pawntex, rook, knight, bishop, queen, king,
			blackpawn, brook, bknight, bbishop, bqueen, bking);

		if (dragging && draggingindex != -1) {
			Vector2i mousePixel = Mouse::getPosition(window);
			Vector2f mousePos = window.mapPixelToCoords(mousePixel);
			int dropCol = int(mousePos.x / square);
			int dropRow = int(mousePos.y / square);
			highlightDropSquare(window, origRow, origCol, dropRow, dropCol, board, whiteturn);
		}

		for (int i = 0; i < 8; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 0);
			if (removedaftercapture(pawn[i], 'P', board, isdragging)) {
				window.draw(pawn[i]);
			}
		}
		for (int i = 0; i < 8; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 1);
			if (removedaftercapture(blackp[i], 'p', board, isdragging)) {
				window.draw(blackp[i]);
			}
		}
		for (int i = 0; i < 1; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 2);
			if (removedaftercapture(whiteking[i], 'K', board, isdragging)) {
				window.draw(whiteking[i]);
			}
		}
		for (int i = 0; i < 1; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 3);
			if (removedaftercapture(whitequeen[i], 'Q', board, isdragging)) {
				window.draw(whitequeen[i]);
			}
		}
		for (int i = 0; i < 2; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 4);
			if (removedaftercapture(whitebishop[i], 'B', board, isdragging)) {
				window.draw(whitebishop[i]);
			}
		}
		for (int i = 0; i < 2; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 5);
			if (removedaftercapture(whiteknight[i], 'N', board, isdragging)) {
				window.draw(whiteknight[i]);
			}
		}
		for (int i = 0; i < 2; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 6);
			if (removedaftercapture(whiterook[i], 'R', board, isdragging)) {
				window.draw(whiterook[i]);
			}
		}
		for (int i = 0; i < 1; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 7);
			if (removedaftercapture(blackking[i], 'k', board, isdragging)) {
				window.draw(blackking[i]);
			}
		}
		for (int i = 0; i < 1; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 8);
			if (removedaftercapture(blackqueen[i], 'q', board, isdragging)) {
				window.draw(blackqueen[i]);
			}
		}
		for (int i = 0; i < 2; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 9);
			if (removedaftercapture(blackbishop[i], 'b', board, isdragging)) {
				window.draw(blackbishop[i]);
			}
		}
		for (int i = 0; i < 2; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 10);
			if (removedaftercapture(blackknight[i], 'n', board, isdragging)) {
				window.draw(blackknight[i]);
			}
		}
		for (int i = 0; i < 2; i++) {
			bool isdragging = (dragging && draggingindex == i && inuse == 11);
			if (removedaftercapture(blackrook[i], 'r', board, isdragging)) {
				window.draw(blackrook[i]);
			}
		}
		


		window.display();
	}
	return 0;
}