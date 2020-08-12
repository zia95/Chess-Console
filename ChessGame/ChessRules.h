#pragma once
#include "GameTypes.h"

#define CheckCoords(X) (X >= 0 && X <= 8)

bool PawnMove(PPiece pPiece, PCOORD pMovCoord, bool bPlayerDirection)
{

	bool x = 
		(pPiece->X) == pMovCoord->X || 
		(pPiece->X + 1) == pMovCoord->X || 
		(pPiece->X - 1) == pMovCoord->X;

	
	bool y = bPlayerDirection ? (pPiece->Y - 1) == pMovCoord->Y : (pPiece->Y + 1) == pMovCoord->Y;


	if (y == false && pPiece->bPlayed == false)
	{
		bool y = bPlayerDirection ? (pPiece->Y - 2) == pMovCoord->Y : (pPiece->Y + 2) == pMovCoord->Y;
	}

	return (x&&y);
}
bool KingMove(PPiece pPiece, PCOORD pMovCoord)
{
	return (
		(pPiece->X + 1) == pMovCoord->X || 
		(pPiece->X - 1) == pMovCoord->X ||
		(pPiece->Y + 1) == pMovCoord->Y ||
		(pPiece->Y - 1) == pMovCoord->Y
			);
}
bool RookMove(PPiece pPiece, PCOORD pMovCoord)
{
	return (pPiece->X == pMovCoord->X) || (pPiece->Y == pMovCoord->Y);
}
bool BishopMove(PPiece pPiece, PCOORD pMovCoord)
{
	SHORT _orgX = pPiece->X;
	SHORT _orgY = pPiece->Y;

	while (CheckCoords(_orgX) && CheckCoords(_orgY))
	{
		if (Coord_IsEqual(pMovCoord, _orgX, _orgY))
			return true;

		_orgX++;
		_orgY++;
	}
	//------------X---trans
	_orgX = pPiece->X;
	_orgY = pPiece->Y;

	while (CheckCoords(_orgX) && CheckCoords(_orgY))
	{
		if (Coord_IsEqual(pMovCoord, _orgX, _orgY))
			return true;

		_orgX--;
		_orgY--;
	}

	//------------Y---trans
	_orgX = pPiece->X;
	_orgY = pPiece->Y;

	while (CheckCoords(_orgX) && CheckCoords(_orgY))
	{
		if (Coord_IsEqual(pMovCoord, _orgX, _orgY))
			return true;

		_orgX++;
		_orgY--;
	}

	//------------Y---trans
	_orgX = pPiece->X;
	_orgY = pPiece->Y;

	while (CheckCoords(_orgX) && CheckCoords(_orgY))
	{
		if (Coord_IsEqual(pMovCoord, _orgX, _orgY))
			return true;

		_orgX--;
		_orgY++;
	}




	return false;
}
bool QueenMove(PPiece pPiece, PCOORD pMovCoord)
{
	return (RookMove(pPiece, pMovCoord) || BishopMove(pPiece, pMovCoord));
}
bool KnightMove(PPiece pPiece, PCOORD pMovCoord)
{
	bool conX1 = ((pPiece->X + 2) == pMovCoord->X) || ((pPiece->X - 2) == pMovCoord->X);
	bool conY1 = ((pPiece->Y - 1) == pMovCoord->Y || (pPiece->Y + 1) == pMovCoord->Y);
	bool con1 = conX1 && conY1;

	bool conX2 = ((pPiece->X - 1) == pMovCoord->X || (pPiece->X + 1) == pMovCoord->X);
	bool conY2 = ((pPiece->Y - 2) == pMovCoord->Y || (pPiece->Y + 2) == pMovCoord->Y);
	bool con2 = conX2 && conY2;


	return (con1 || con2);
}




bool CheckMove(PPiece pPiece, PCOORD pMovCoord, bool bPlayerDirection)//bPlayerDir -> up = true , down = false
{
	switch (pPiece->ePiece)
	{
	case King:
		return KingMove(pPiece, pMovCoord);
		break;
	case Queen:
		return QueenMove(pPiece, pMovCoord);
		break;
	case Rook:
		return RookMove(pPiece, pMovCoord);
		break;
	case Bishop:
		return BishopMove(pPiece, pMovCoord);
		break;
	case Knight:
		return KnightMove(pPiece, pMovCoord);
		break;
	case Pawn:
		return PawnMove(pPiece, pMovCoord, bPlayerDirection);
		break;
	default:
		throw std::exception("piece type error....");
		break;
	}

	return false;
}