#pragma once


#include <windows.h>
#include <iostream>

namespace console
{
	void SetTitle(LPCSTR lpTitle)
	{
		SetConsoleTitleA(lpTitle);
	}


	HANDLE hConsole = nullptr;
	bool UpdateConsoleHandle(DWORD nStdHandle = STD_OUTPUT_HANDLE)
	{
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		return hConsole;
	}

	bool GetConsoleInfo(PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBuffer)
	{
		if (hConsole == nullptr) return false;

		return GetConsoleScreenBufferInfo(hConsole, lpConsoleScreenBuffer);
	}

	bool GetConsolePosition(PCOORD xyCoord)
	{
		if (hConsole == nullptr) return false;

		CONSOLE_SCREEN_BUFFER_INFO csbi = {0};

		if (GetConsoleInfo(&csbi))
		{
			xyCoord->X = csbi.dwCursorPosition.X;
			xyCoord->Y = csbi.dwCursorPosition.Y;
			return true;
		}

		return false;
	}
	bool SetConsolePosition(COORD xyCoord)
	{
		if (hConsole == nullptr) return false;

		return SetConsoleCursorPosition(hConsole, xyCoord);
	}

	bool SetConsoleColor(WORD wAttr)
	{
		if (hConsole == nullptr) return false;

		return SetConsoleTextAttribute(hConsole, wAttr);
	}
	bool GetConsoleColor(WORD& wAttr)
	{
		if (hConsole == nullptr) return false;

		CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };

		if (GetConsoleInfo(&csbi))
		{
			wAttr = csbi.wAttributes;
			return true;
		}

		return false;
		
	}
	void TestColor()
	{
		for (WORD k = 0; k < 256; k++)
		{
			console::SetConsoleColor(k);
			std::cout << k << " Hello World!!" << std::endl;
		}
	}





	// custom -- methods
	PCOORD pCoordRestore = nullptr;
	void StorePos()
	{
		if (pCoordRestore != nullptr)
			delete pCoordRestore;

		if (pCoordRestore == nullptr)
		{
			pCoordRestore = new COORD{ 0 };
			if (!GetConsolePosition(pCoordRestore))
				throw std::exception("get c pos failed!!");
		}
	}
	void RestorePos()
	{
		if (pCoordRestore == nullptr)
			return;

		if (SetConsolePosition(*pCoordRestore))
		{
			delete pCoordRestore;
			pCoordRestore = nullptr;
		}
	}

	WORD wColorRestore = 0;
	void StoreColor()
	{
		if (!GetConsoleColor(wColorRestore))
			throw std::exception("get c clr failed!!");
	}
	void RestoreColor()
	{
		if(!SetConsoleColor(wColorRestore))
			throw std::exception("set c clr failed!!");
	}


	void StoreState()
	{
		StorePos();
		StoreColor();
	}

	void RestoreState()
	{
		RestorePos();
		RestoreColor();
	}

};