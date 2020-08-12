#include "ConsoleHelper.h"
#include <string>// for reading string line
#include "ChessRules.h"
#include "GameTypes.h"

#define TOTAL_SQUARE 8
#define GAME_COLOR 161

#define TOTAL_PLAYER_PIECES 16
#define TOTAL_PIECES (TOTAL_PLAYER_PIECES*2)




#define COLOR_BLACK 15
#define COLOR_WHITE 240

#define WINDOW_TITLE "Chess"


Piece g_PlayerBlack[TOTAL_PLAYER_PIECES];//Player NUmber 0
Piece g_PlayerWhite[TOTAL_PLAYER_PIECES];//Player Number 1

int nTurnBlack;
int nTurnWhite;


void DrawBoard()
{
	WORD def_clr = 0;
	console::GetConsoleColor(def_clr);
	console::SetConsoleColor(GAME_COLOR);


	
	
	std::cout << "   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|" << std::endl;
	std::cout << "[8]|     |     |     |     |     |     |     |     |" << std::endl;
	std::cout << "   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|" << std::endl;
	std::cout << "[7]|     |     |     |     |     |     |     |     |" << std::endl;
	std::cout << "   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|" << std::endl;
	std::cout << "[6]|     |     |     |     |     |     |     |     |" << std::endl;
	std::cout << "   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|" << std::endl;
	std::cout << "[5]|     |     |     |     |     |     |     |     |" << std::endl;
	std::cout << "   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|" << std::endl;
	std::cout << "[4]|     |     |     |     |     |     |     |     |" << std::endl;
	std::cout << "   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|" << std::endl;
	std::cout << "[3]|     |     |     |     |     |     |     |     |" << std::endl;
	std::cout << "   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|" << std::endl;
	std::cout << "[2]|     |     |     |     |     |     |     |     |" << std::endl;
	std::cout << "   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|" << std::endl;
	std::cout << "[1]|     |     |     |     |     |     |     |     |" << std::endl;
	std::cout << "   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|" << std::endl;
	std::cout << "     [A]   [B]   [C]   [D]   [E]   [F]   [G]   [H]  " << std::endl;
	console::SetConsoleColor(def_clr);
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
}
void TransformCoords(PCOORD pBoardCoords)
{
	if (pBoardCoords->Y == 1) pBoardCoords->Y = 8;
	else if (pBoardCoords->Y == 2) pBoardCoords->Y = 7;
	else if (pBoardCoords->Y == 3) pBoardCoords->Y = 6;
	else if (pBoardCoords->Y == 4) pBoardCoords->Y = 5;
	else if (pBoardCoords->Y == 5) pBoardCoords->Y = 4;
	else if (pBoardCoords->Y == 6) pBoardCoords->Y = 3;
	else if (pBoardCoords->Y == 7) pBoardCoords->Y = 2;
	else if (pBoardCoords->Y == 8) pBoardCoords->Y = 1;

	pBoardCoords->Y = (2 * pBoardCoords->Y)-1;

	SHORT x = pBoardCoords->X;

	pBoardCoords->X = (3* x)-1;
	pBoardCoords->X += (3 * x);
}
void DrawPiece(LPCSTR lpPieceStr, WORD wColor, SHORT wBoardX, SHORT wBoardY)
{
	console::StoreState();

	COORD c = { wBoardX ,wBoardY };

	TransformCoords(&c);

	console::SetConsolePosition(c);

	console::SetConsoleColor(wColor);

	std::cout << lpPieceStr;
	

	console::RestoreState();
}
void DrawPieces()
{
	for (size_t i = 0; i < TOTAL_PIECES; i++)
	{
		PPiece pPiece = i >= TOTAL_PLAYER_PIECES ? &g_PlayerWhite[(i - TOTAL_PLAYER_PIECES)] : &g_PlayerBlack[i];
		WORD wPieceColor = i >= TOTAL_PLAYER_PIECES ? COLOR_WHITE : COLOR_BLACK;

		char strPiece[PIECE_STR_SIZE] = {0};
		GetPieceString(pPiece->ePiece, strPiece);
		DrawPiece(strPiece, wPieceColor, pPiece->X, pPiece->Y);
	}
}
void ClearConsole(char c, size_t nCharInLine, size_t nTotalLines)
{
	char * lpszLine = new char[(nCharInLine+1)];
	//ZeroMemory(lpszLine, (nCharInLine + 1));
	memset(lpszLine, c, nCharInLine);
	lpszLine[nCharInLine] = '\0';
	for (size_t i = 0; i < nTotalLines; i++)
	{
		std::cout << lpszLine << std::endl;
	}
	delete[] lpszLine;
}
PPiece FindPieceByCoord(PCOORD pCoord, BOOL bPlayer)
{
	for (size_t i = 0; i < TOTAL_PLAYER_PIECES; i++)
	{
		PPiece p = bPlayer? &g_PlayerWhite[i] : &g_PlayerBlack[i];

		if (p->X == pCoord->X &&
			p->Y == pCoord->Y)
		{
			return p;
		}
	}
	return nullptr;
}
bool MovePiece(PCOORD pCoord, PCOORD pCoordDest, BOOL bPlayer)
{
	PPiece p = FindPieceByCoord(pCoord, bPlayer);

	if (p == nullptr)
	{
		std::cout << "No piece found at coord (" << pCoord->X << "," << pCoord->Y << ")" << std::endl;
		return false;
	}

	if (CheckMove(p, pCoordDest, bPlayer) == false)
	{
		std::cout << "Invalid move..." << std::endl;
		return false;
	}

	char strPiece[PIECE_STR_SIZE] = { 0 };
	GetPieceString(p->ePiece, strPiece);
	

	p->bPlayed = true;
	p->X = pCoordDest->X;
	p->Y = pCoordDest->Y;

	DrawPiece(PIECE_EMPTY, GAME_COLOR, pCoord->X, pCoord->Y);
	DrawPiece(strPiece, bPlayer ? COLOR_WHITE : COLOR_BLACK, p->X, p->Y);


	std::cout << "Piece Moved..." << std::endl;

	return true;
}
bool ParseCoords(std::string & Str, PCOORD pCoords)
{
	if (Str.length() < 2)
		return false;


	for (size_t i = 0; i < (Str.length()-1); i++)// parse signature sample 'E4', 'A5'
	{
		char cX = tolower(Str[i]);
		char cY = Str[(i+1)];

		if (cX == 'a') pCoords->X = 1;
		else if (cX == 'b') pCoords->X = 2;
		else if (cX == 'c') pCoords->X = 3;
		else if (cX == 'd') pCoords->X = 4;
		else if (cX == 'e') pCoords->X = 5;
		else if (cX == 'f') pCoords->X = 6;
		else if (cX == 'g') pCoords->X = 7;
		else if (cX == 'h') pCoords->X = 8;
		

		if (pCoords->X >= 1 && pCoords->X <= 8)
		{
			if (cY == '1') pCoords->Y = 1;
			else if (cY == '2') pCoords->Y = 2;
			else if (cY == '3') pCoords->Y = 3;
			else if (cY == '4') pCoords->Y = 4;
			else if (cY == '5') pCoords->Y = 5;
			else if (cY == '6') pCoords->Y = 6;
			else if (cY == '7') pCoords->Y = 7;
			else if (cY == '8') pCoords->Y = 8;

			return ((pCoords->X >= 1 && pCoords->X <= 8) && (pCoords->Y >= 1 && pCoords->Y <= 8));
		}
	}

	
	
	return false;
}
void Begin()
{
	COORD start_pos = {0};
	COORD end_pos = { 0 };
	console::GetConsolePosition(&start_pos);

	nTurnBlack = 1;
	nTurnWhite = 1;


	std::cout << "Pieces: PWN=Pawn, KNG=King, QEN=Queen, ROK=Rook, BHP=Bishop, KNT=Knight" << std::endl;
	std::cout << "To move a piece, use command: 'VertPos_HoriPos->newVertPos_newHoriPos'" << std::endl;
	std::cout << "Example: 'A2->A3'" << std::endl;


	std::string readbuffer;
	while (true)
	{
		const int nTotalTurn = (nTurnBlack + nTurnWhite);

		bool bPlayer = !(nTotalTurn % 2);
		std::cout << nTotalTurn << "]"<< (bPlayer ? "White" : "Black") << "Turn(VPos_HPos->newVPos_newHPos): ";
		std::getline(std::cin, readbuffer);
		
		//clear___ console area.....
		console::GetConsolePosition(&end_pos);
		console::SetConsolePosition(start_pos);
		ClearConsole(' ', 100, (end_pos.Y - start_pos.Y - 1));
		console::SetConsolePosition(start_pos);

		std::cout << std::endl;

		bool turn_sucessful = false;

		size_t mid = readbuffer.find_first_of("->");

		if (mid != std::string::npos)
		{
			std::string fst = readbuffer.substr(0, mid);
			std::string lst = readbuffer.substr(mid+2, (readbuffer.length() - mid));


			COORD cfst = { 0 };
			COORD clst = { 0 };

			turn_sucessful = ParseCoords(fst, &cfst);
			turn_sucessful = ParseCoords(lst, &clst);

			if (turn_sucessful)
			{
				turn_sucessful = MovePiece(&cfst, &clst, 0);
			}
			else
			{
				std::cout << "[1]Failed to parse line...." << readbuffer << std::endl;
			}
		}
		else
		{
			std::cout << "[2]Failed to parse line...." << readbuffer << std::endl;
		}



		if (turn_sucessful == false)
			continue;
		
		if (bPlayer)
			nTurnWhite++;
		else
			nTurnBlack++;
	}
}
void StartGame()
{
	g_PlayerBlack[0]  = { Piece_t::Pawn, 1,2, FALSE, TRUE  };
	g_PlayerBlack[1]  = { Piece_t::Pawn, 2,2, FALSE, TRUE };
	g_PlayerBlack[2]  = { Piece_t::Pawn, 3,2, FALSE, TRUE };
	g_PlayerBlack[3]  = { Piece_t::Pawn, 4,2, FALSE, TRUE };
	g_PlayerBlack[4]  = { Piece_t::Pawn, 5,2, FALSE, TRUE };
	g_PlayerBlack[5]  = { Piece_t::Pawn, 6,2, FALSE, TRUE };
	g_PlayerBlack[6]  = { Piece_t::Pawn, 7,2, FALSE, TRUE };
	g_PlayerBlack[7]  = { Piece_t::Pawn, 8,2, FALSE, TRUE };

	g_PlayerBlack[8]  = { Piece_t::Rook, 1, 1, FALSE, TRUE };
	g_PlayerBlack[9]  = { Piece_t::Knight, 2, 1, FALSE, TRUE };
	g_PlayerBlack[10] = { Piece_t::Bishop, 3, 1, FALSE, TRUE };
	g_PlayerBlack[11] = { Piece_t::Queen, 4, 1, FALSE, TRUE };
	g_PlayerBlack[12] = { Piece_t::King, 5, 1, FALSE, TRUE };
	g_PlayerBlack[13] = { Piece_t::Bishop, 6, 1, FALSE, TRUE };
	g_PlayerBlack[14] = { Piece_t::Knight, 7, 1, FALSE, TRUE };
	g_PlayerBlack[15] = { Piece_t::Rook, 8, 1, FALSE, TRUE };

	//player two
	g_PlayerWhite[0] = { Piece_t::Pawn, 1, 7, FALSE, TRUE };
	g_PlayerWhite[1] = { Piece_t::Pawn, 2, 7, FALSE, TRUE };
	g_PlayerWhite[2] = { Piece_t::Pawn, 3, 7, FALSE, TRUE };
	g_PlayerWhite[3] = { Piece_t::Pawn, 4, 7, FALSE, TRUE };
	g_PlayerWhite[4] = { Piece_t::Pawn, 5, 7, FALSE, TRUE };
	g_PlayerWhite[5] = { Piece_t::Pawn, 6, 7, FALSE, TRUE };
	g_PlayerWhite[6] = { Piece_t::Pawn, 7, 7, FALSE, TRUE };
	g_PlayerWhite[7] = { Piece_t::Pawn, 8, 7, FALSE, TRUE };

	g_PlayerWhite[8]  = { Piece_t::Rook, 1, 8, FALSE, TRUE };
	g_PlayerWhite[9]  = { Piece_t::Knight, 2, 8, FALSE, TRUE };
	g_PlayerWhite[10] = { Piece_t::Bishop, 3, 8, FALSE, TRUE };
	g_PlayerWhite[11] = { Piece_t::Queen, 4, 8, FALSE, TRUE };
	g_PlayerWhite[12] = { Piece_t::King, 5, 8, FALSE, TRUE };
	g_PlayerWhite[13] = { Piece_t::Bishop, 6, 8, FALSE, TRUE };
	g_PlayerWhite[14] = { Piece_t::Knight, 7, 8, FALSE, TRUE };
	g_PlayerWhite[15] = { Piece_t::Rook, 8, 8, FALSE, TRUE };

	DrawBoard();
	DrawPieces();

	Begin();

}
int main()
{
	console::UpdateConsoleHandle();
	console::SetTitle(WINDOW_TITLE);

	HWND hWnd = GetForegroundWindow();
	SetWindowPos(hWnd, NULL, NULL, NULL, 460, 800, SWP_NOMOVE);

	
	StartGame();


	std::cout << "End of game.... press any key to exit" << std::endl;
	std::cin.ignore();
	return S_OK;
}