#include "Header.h"
using namespace std;

namespace LAB_09
{
	void task(LPCWSTR FileName);

	BOOL printFileInfo(LPWSTR FileName)
	{
		HANDLE fileHandle = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fileHandle == INVALID_HANDLE_VALUE)
			return false;


		_BY_HANDLE_FILE_INFORMATION fileInformation;
		if (!GetFileInformationByHandle(fileHandle, &fileInformation)) {
			CloseHandle(fileHandle);
			return false;
		}

		std::wstring wFilePath = std::wstring(FileName);
		std::string file = std::string(wFilePath.begin(), wFilePath.end());

		cout << "FILE NAME: " << file.c_str() << endl;

		cout << "FILE TYPE: ";
		DWORD fileType = GetFileType(fileHandle);
		switch (fileType) {
		case FILE_TYPE_CHAR: cout << "FILE_TYPE_CHAR" << std::endl; break;
		case FILE_TYPE_DISK: cout << "FILE_TYPE_DISK" << std::endl; break;
		case FILE_TYPE_PIPE: cout << "FILE_TYPE_PIPE" << std::endl; break;
		case FILE_TYPE_REMOTE: cout << "FILE_TYPE_REMOTE" << std::endl; break;
		case FILE_TYPE_UNKNOWN: cout << "FILE_TYPE_UNKNOWN" << std::endl; break;
		default: std::cout << "--------->PRIORITY -->? " << std::endl << std::endl; break;
		}

		LARGE_INTEGER lFileSize;
		LONGLONG fileSize = 0;
		BOOL bGetSize = GetFileSizeEx(fileHandle, &lFileSize);
		if (!bGetSize)
			return false;
		fileSize += lFileSize.QuadPart;
		cout << "FILE SIZE: " << fileSize << endl;

		FILETIME createFile = fileInformation.ftCreationTime;
		SYSTEMTIME createFileToSystemTime;
		FileTimeToSystemTime(&createFile, &createFileToSystemTime);
		cout << "CREATE DATE AND TIME: "
			<< "YEAR:" << createFileToSystemTime.wYear
			<< "\tMONTH:" << createFileToSystemTime.wMonth
			<< "\tDAY:" << createFileToSystemTime.wDay
			<< "\t" << createFileToSystemTime.wHour
			<< ":" << createFileToSystemTime.wMinute
			<< endl;

		FILETIME lastUpdate = fileInformation.ftLastWriteTime;
		SYSTEMTIME lastUpdateToSysTime;
		FileTimeToSystemTime(&lastUpdate, &lastUpdateToSysTime);
		cout << "Last update date and time: "
			<< "YEAR:" << lastUpdateToSysTime.wYear
			<< "\tMONTH:" << lastUpdateToSysTime.wMonth
			<< "\tDAY:" << lastUpdateToSysTime.wDay
			<< "\t" << lastUpdateToSysTime.wHour
			<< ":" << lastUpdateToSysTime.wMinute
			<< endl;

		CloseHandle(fileHandle);
		return true;
	}

	BOOL printFileTxt(LPWSTR FileName)
	{
		HANDLE fileHandle = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fileHandle == INVALID_HANDLE_VALUE)
			return false;

		DWORD n = NULL;
		char buf[1024];

		ZeroMemory(buf, sizeof(buf));
		bool readFile = ReadFile(fileHandle, &buf, 1024, &n, NULL);

		std::cout << "Read file:\n" << buf << std::endl;
		CloseHandle(fileHandle);
	}

	BOOL delRowFileTxt(LPWSTR FileName, DWORD row)
	{
		HANDLE fileHandle = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fileHandle == INVALID_HANDLE_VALUE)
			return false;

		DWORD n = NULL;
		char buf[1024];

		ZeroMemory(buf, sizeof(buf));
		bool readFile = ReadFile(fileHandle, &buf, 1024, &n, NULL);

		int i = -1;
		int currentRowNum = 1;
		int filePointer = 0;
		char wbuf[1024];
		while (buf[i++] != '\0')
		{
			if (currentRowNum == row)
			{
				while (buf[i] != '\n' && buf[i] != '\0') 
				{
					i++;
				}

				int y = 0;
				while (buf[i] != '\0')
				{
					i++;
					wbuf[y++] = buf[i];
				}

				if (y == 0)
					filePointer -= 2;

				SetFilePointer(fileHandle, filePointer, NULL, FILE_BEGIN);
				WriteFile(fileHandle, &wbuf, sizeof(char) * y - 1, &n, NULL);
				SetEndOfFile(fileHandle);
			}

			

			if (buf[i] == '\n')
				currentRowNum++;
			if (currentRowNum == row)
				filePointer = i + 1;
		}

		CloseHandle(fileHandle);

		if (currentRowNum < row)
			return false;

		return true;
	}

	BOOL insRowFileTxt(LPWSTR FileName, LPWSTR str, DWORD row)
	{
		HANDLE fileHandle = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fileHandle == INVALID_HANDLE_VALUE)
			return false;

		DWORD n = NULL;
		char buf[1024];

		ZeroMemory(buf, sizeof(buf));
		bool readFile = ReadFile(fileHandle, &buf, 1024, &n, NULL);

		char sbuf[1024];
		wcstombs(sbuf, str, 1024);
		DWORD byteCount = NULL;

		if (row == 0 || row == 1)
		{
			SetFilePointer(fileHandle, 0, NULL, FILE_BEGIN);
			WriteFile(fileHandle, sbuf, wcslen(str), &byteCount, NULL);
			WriteFile(fileHandle, (LPWSTR)"\r\n", sizeof(char) * 2, &byteCount, NULL);
			WriteFile(fileHandle, buf, n, &n, NULL);
			CloseHandle(fileHandle);
			return true;
		}

		if (row == -1)
		{
			WriteFile(fileHandle, (LPWSTR)"\r\n", sizeof(char) * 2, &byteCount, NULL);
			WriteFile(fileHandle, sbuf, wcslen(str), &byteCount, NULL);
			SetEndOfFile(fileHandle);
			CloseHandle(fileHandle);
			return true;
		}

		if (row > 0)
		{
			int i = -1;
			int currentRowNum = 1;
			char wbuf[1024];
			while (buf[i++] != '\0')
			{
				if (currentRowNum == row)
				{
					SetFilePointer(fileHandle, i, NULL, FILE_BEGIN);
					WriteFile(fileHandle, sbuf, wcslen(str), &byteCount, NULL);
					WriteFile(fileHandle, (LPWSTR)"\r\n", sizeof(char) * 2, &byteCount, NULL);

					int y = 0;
					while (buf[i] != '\0')
					{
						wbuf[y++] = buf[i];
						i++;
					}

					WriteFile(fileHandle, &wbuf, sizeof(char) * y, &n, NULL);
					SetEndOfFile(fileHandle);
				}

				if (buf[i] == '\n')
					currentRowNum++;
			}

			if (currentRowNum < row)
			{
				for (; currentRowNum < row; currentRowNum++)
				{
					WriteFile(fileHandle, (LPWSTR)"\r\n", sizeof(char) * 2, &byteCount, NULL);
				}
				WriteFile(fileHandle, sbuf, wcslen(str), &byteCount, NULL);
				SetEndOfFile(fileHandle);
			}

		}

		CloseHandle(fileHandle);
		return false;
	}

	BOOL printWatchRowFileTxt(LPWSTR FileName, DWORD mlsec)
	{
		HANDLE thread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)task, (LPVOID)FileName, NULL, FALSE);
		WaitForSingleObject(thread2, mlsec);
		TerminateThread(thread2, -1);

		return true;
	}

	// Helpers

	int getStringsCount(LPCWSTR FileName) {
		char rbuf[10240];
		Sleep(1000);
		HANDLE hf = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD n = NULL;

		ZeroMemory(rbuf, sizeof(rbuf));
		BOOL b = ReadFile(hf, &rbuf, sizeof(rbuf), &n, NULL);

		int i = 0;
		int strCount = 0;
		while (rbuf[i] != '\0') {
			if (rbuf[i] == '\n')
				strCount++;
			i++;
		}

		CloseHandle(hf);
		return strCount + 1;
	}

	void task(LPCWSTR FileName) {

		std::wstring wFilePath = std::wstring(FileName);
		std::string file = std::string(wFilePath.begin(), wFilePath.end());

		std::string dir = std::string(wFilePath.begin(), wFilePath.end());

		for (int i = dir.length() - 1; i >= 0; i--) {
			if (dir[i] != '\\')
				dir.pop_back();
			else {
				break;
			}
		}
		HANDLE hNotification = FindFirstChangeNotificationA(dir.c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE); //Начинаем отслеживать изменения в заданной папке посредством специальной функции
		if (hNotification == INVALID_HANDLE_VALUE) //Если создать описатель не удалось, значит заданного пути не существует
		{
			MessageBox(NULL, L"Wrong path", L"Error", MB_OK);
		}

		int fileStrsCount = getStringsCount(FileName);

		while (1) //Бесконечный цикл
		{
			FindNextChangeNotification(hNotification); //И включаем ожидание следующего изменения в папке
			DWORD dwResult = WaitForSingleObject(hNotification, INFINITE); //Ждём
			int newfileStrsCount = getStringsCount(FileName);
			if (fileStrsCount != newfileStrsCount) {
				std::cout << "Strings:  " << newfileStrsCount << std::endl;
				fileStrsCount = newfileStrsCount;
			}
		}
		FindCloseChangeNotification(hNotification);
	}
}