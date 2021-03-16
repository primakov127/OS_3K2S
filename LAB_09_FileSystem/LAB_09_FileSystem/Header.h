#pragma once
#include <Windows.h>
#include <iostream>

namespace LAB_09
{
	BOOL printFileInfo(LPWSTR FileName);
	BOOL printFileTxt(LPWSTR FileName);
	BOOL delRowFileTxt(LPWSTR FileName, DWORD row);
	BOOL insRowFileTxt(LPWSTR FileName, LPWSTR str, DWORD row);
	BOOL printWatchRowFileTxt(LPWSTR FileName, DWORD mlsec);
}