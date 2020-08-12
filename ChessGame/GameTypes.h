#pragma once


#define PIECE_STR_SIZE 4

#define PIECE_EMPTY "   "
#define PIECE_PAWN "PWN"
#define PIECE_KING "KNG"
#define PIECE_QUEEN "QEN"
#define PIECE_ROOK "ROK"
#define PIECE_BISHOP "BHP"
#define PIECE_KNIGHT "KNT"


enum Piece_t
{
	None,
	King,
	Queen,
	Rook,
	Bishop,
	Knight,
	Pawn,
};
void GetPieceString(Piece_t ePiece, LPSTR lpPieceStr)
{
	switch (ePiece)
	{
	case King:
		strcpy_s(lpPieceStr, PIECE_STR_SIZE, PIECE_KING);
		break;
	case Queen:
		strcpy_s(lpPieceStr, PIECE_STR_SIZE, PIECE_QUEEN);
		break;
	case Rook:
		strcpy_s(lpPieceStr, PIECE_STR_SIZE, PIECE_ROOK);
		break;
	case Bishop:
		strcpy_s(lpPieceStr, PIECE_STR_SIZE, PIECE_BISHOP);
		break;
	case Knight:
		strcpy_s(lpPieceStr, PIECE_STR_SIZE, PIECE_KNIGHT);
		break;
	case Pawn:
		strcpy_s(lpPieceStr, PIECE_STR_SIZE, PIECE_PAWN);
		break;
	default:
		throw std::exception("piece type error....");
		break;
	}
}
struct _PIECE_
{
	Piece_t ePiece;

	SHORT X;//board coord
	SHORT Y;

	bool bPlayed;
	bool bActive;
}
typedef Piece, *PPiece;
bool Coord_IsEqual(PCOORD a, PCOORD b)
{
	return (a->X == b->X && a->Y == b->Y);
}
bool Coord_IsEqual(PCOORD a, SHORT bX, SHORT bY)
{
	return (a->X == bX && a->Y == bY);
}