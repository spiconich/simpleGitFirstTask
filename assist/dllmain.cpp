//comment from simpleBranch
#include "pch.h"
#include <iostream>
#include <string>
#include "dllHeader.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

std::string byte2ch(BYTE* data, int size)
{
    return std::string((char*)data, size);
}
bool checkNTFSstring(std::string OEM)
{
    std::string str = "NTFS";
    const char* cRez;
    const char* cStr = str.c_str();
    const char* cOEM = OEM.c_str();
    cRez = strstr(cOEM, cStr);
    if (cRez != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}
extern "C" __declspec(dllexport) void checkIsNTFS(bool* rez, bool* pointerRez, bool* readRez,bool* ntfsRez, std::string volumeLetter)
{
    *rez = false;
    *pointerRez = false;
    *readRez = false;
    *ntfsRez = false;
    std::string fileName = "\\\\.\\";
    std::string fullPath = fileName + volumeLetter + ":";
    HANDLE fileHandle = CreateFileA(
        fullPath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        *rez = true;
        ULONGLONG startOffset = 0;
        BYTE buffer[512];
        DWORD bytesToRead = 512;
        DWORD bytesRead;
        LARGE_INTEGER sectorOffset;
        sectorOffset.QuadPart = 0;
        NTFS_BootRecord* pNTFS_BootRecord = reinterpret_cast <NTFS_BootRecord*> (buffer);
        unsigned long currentPosition = SetFilePointer(fileHandle, sectorOffset.LowPart, NULL, FILE_BEGIN);
        if (currentPosition == sectorOffset.LowPart)
        {
            *pointerRez = true;
            bool readResult = ReadFile(fileHandle, buffer, bytesToRead, &bytesRead, NULL);
            if (readResult && bytesRead == bytesToRead)
            {
                *readRez = true;
                *ntfsRez = checkNTFSstring(byte2ch(pNTFS_BootRecord->OEM_Name, 8).c_str());
            };
        };
    }
    else
    {
        *rez = false;
    }
    CloseHandle(fileHandle);
}

extern "C" __declspec(dllexport) void CheckPhysDrives(bool* rez,int DriveNum,std::string fileName)
{

    std::string fullPath = fileName + std::to_string(DriveNum);
    HANDLE fileHandle = CreateFileA(
        fullPath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        *rez = true;
    }
    else
    {
        *rez = false;      
    }
    CloseHandle(fileHandle);
}
extern "C" __declspec(dllexport) void CheckVolume(bool* rez,bool* pointerRez ,bool* readRez,std::string fileName,std::string letter,std::string* dllSysType)
{
    std::string fullPath = fileName + letter+":";
    HANDLE fileHandle = CreateFileA(
        fullPath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        *rez = true;
        ULONGLONG startOffset = 0;
        static BYTE buffer[512];
        DWORD bytesToRead = 512;
        DWORD bytesRead;
        LARGE_INTEGER sectorOffset;
        sectorOffset.QuadPart = 0;
        NTFS_BootRecord *pNTFS_BootRecord = reinterpret_cast <NTFS_BootRecord*> (buffer);
        unsigned long currentPosition = SetFilePointer(fileHandle, sectorOffset.LowPart, &sectorOffset.HighPart, FILE_BEGIN);
        if (currentPosition == sectorOffset.LowPart)
        {
            *pointerRez = true;
            bool readResult = ReadFile(fileHandle, buffer, bytesToRead, &bytesRead, NULL);
            if (readResult && bytesRead == bytesToRead)
            {
                *readRez = true;
                *dllSysType = byte2ch(pNTFS_BootRecord->OEM_Name, 8).c_str();
            };
        };
    }
    else
    {
        *rez = false;
    }
    CloseHandle(fileHandle);

}

extern "C" __declspec(dllexport) void noMoreChecks(std::string fullPath, NTFS_BootRecord* strucToReturn)
{
    HANDLE fileHandle = CreateFileA(fullPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ULONGLONG startOffset = 0;
    BYTE buffer[512];
    memset(buffer, 0, 512);
    DWORD bytesToRead = 512;
    DWORD bytesRead;
    LARGE_INTEGER sectorOffset;
    sectorOffset.QuadPart = 0;
    unsigned long currentPosition = SetFilePointer(fileHandle, sectorOffset.LowPart, NULL, FILE_BEGIN);
    bool stopSpamTrash = ReadFile(fileHandle, buffer, bytesToRead, &bytesRead, NULL);
    *strucToReturn = *reinterpret_cast<NTFS_BootRecord*>(buffer);
    CloseHandle(fileHandle);
}
