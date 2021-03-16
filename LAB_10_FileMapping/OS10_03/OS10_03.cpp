// OS10_03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "OS10_HTAPI.h"

using namespace HT;

bool testCase()
{
    LPWSTR FileName = (LPWSTR)L"E:\\3k2s\\LAB\\OS\\LAB_10_FileMapping\\LAB_10_FileMapping\\testCase.ht";
    std::string key1 = "key1";
    std::string data1 = "data1";
    Element* element1 = new Element(key1.c_str(), key1.length(), data1.c_str(), data1.length());
    HTHANDLE* hashTable = Create(20, 10, 20, 20, FileName);
    Element* finded1 = Get(hashTable, element1);
    if (strcmp((char*)element1->Key, (char*)finded1->Key) && strcmp((char*)element1->Payload, (char*)finded1->Payload))
    {
        Close(hashTable);
        return true;
    }

    Close(hashTable);
    return false;
}

int main()
{
    LPWSTR FileName1 = (LPWSTR)L"E:\\3k2s\\LAB\\OS\\LAB_10_FileMapping\\LAB_10_FileMapping\\example1.ht";
    LPWSTR FileName2 = (LPWSTR)L"E:\\3k2s\\LAB\\OS\\LAB_10_FileMapping\\LAB_10_FileMapping\\example2.ht";

    std::string key1 = "key1";
    std::string data1 = "data1";
    std::string key2 = "key2";
    std::string data2 = "data2";
    Element* element1 = new Element(key1.c_str(), key1.length(), data1.c_str(), data1.length());
    Element* element2 = new Element(key2.c_str(), key2.length(), data2.c_str(), data2.length());

    HTHANDLE* hashTable1 = Create(20, 5, 20, 20, FileName1);
    HTHANDLE* hashTable2 = Create(20, 5, 20, 20, FileName2);

    Insert(hashTable1, element1);
    Insert(hashTable2, element2);

    if (Get(hashTable1, element1) != NULL)
    {
        std::cout << "Element 1 in hashTable1" << std::endl;
    }
    if (Get(hashTable1, element2) != NULL)
    {
        std::cout << "Element 2 in hashTable1" << std::endl;
    }

    if (Get(hashTable1, element1) != NULL)
    {
        std::cout << "Element 1 in hashTable2" << std::endl;
    }
    if (Get(hashTable1, element2) != NULL)
    {
        std::cout << "Element 2 in hashTable2" << std::endl;
    }

    Close(hashTable1);
    Close(hashTable2);

    //if (testCase())
    //{
    //    std::cout << "Test successfuly!" << std::endl;
    //}
    //else
    //    std::cout << "Test faild" << std::endl;
}

