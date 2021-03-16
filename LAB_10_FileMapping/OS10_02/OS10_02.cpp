// OS10_02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "OS10_HTAPI.h"
using namespace HT;

int main()
{
	LPWSTR FileName = (LPWSTR)L"E:\\3k2s\\LAB\\OS\\LAB_10_FileMapping\\LAB_10_FileMapping\\example.ht";
	//LPWSTR FileName = (LPWSTR)L"E:\\3k2s\\LAB\\OS\\LAB_10_FileMapping\\LAB_10_FileMapping\\example_13_03_2021__01_46_49.ht";

	std::string key1 = "key1";
	std::string data1 = "data1";
	std::string key2 = "key2";
	std::string data2 = "data2";
	std::experimental::filesystem::path path(FileName);
	std::cout << path.filename() << std::endl;

	HTHANDLE* hashTable = Create(20, 5, 20, 20, FileName);
	//HTHANDLE* hashTable = Open(FileName);
	//Update(hashTable, finded, dataX.c_str(), dataX.length());
	Element* element1 = new Element(key1.c_str(), key1.length(), data1.c_str(), data1.length());
	Element* element2 = new Element(key2.c_str(), key2.length(), data2.c_str(), data2.length());
	Insert(hashTable, element1);
	Insert(hashTable, element2);
	Element* finded1 = Get(hashTable, element1);
	if (finded1 != NULL)
	{
		std::cout << "Finded1: " << (char*)finded1->Key << " " << (char*)finded1->Payload << std::endl;
	}
	Element* finded2 = Get(hashTable, element2);
	if (finded2 != NULL)
	{
		std::cout << "Finded2: " << (char*)finded2->Key << " " << (char*)finded2->Payload << std::endl;
	}
	Delete(hashTable, finded2);
	if (Get(hashTable, finded2) == NULL)
	{
		std::cout << "Deleted successfuly!" << std::endl;
	}

	Snap(hashTable);
	Sleep(15000);
	Close(hashTable);
}
