#include <iostream>
#include <Windows.h>
#include <string>
#include "diffDef.h"

//rework for git
void showMe()
{
    std::cout <<"\n"<<"          Press ctrl+c to exit" << std::endl;
    std::string x;
    std::cin >> x;
    }
void finalNTFSOutput(NTFS_BootRecordMain ntfsStruc)
{
    std::cout << "   File system type (0x03) : " << ntfsStruc.OEM_Name << std::endl;
    std::cout << "   Bytes per sector (0x0B) : " << ntfsStruc.bytesPerSector << std::endl;
    WORD toShowsectorPerCluster = ntfsStruc.sectorPerCluster;
    std::cout << "   Sectors per cluster (0x0D) : " << toShowsectorPerCluster << std::endl;
    WORD toShowmediaDescriptor = ntfsStruc.mediaDescriptor;
    std::cout << "   Media descriptor (0x15) : " << toShowmediaDescriptor << std::endl;
    std::cout << "   Sectors per track (0x18) : " << ntfsStruc.sectorPerTrack << std::endl;
    std::cout << "   Number of heads (0x1A) : " << ntfsStruc.numOfHeaders << std::endl;
    std::cout << "   Number of sectors in the volume (0x28) : " << ntfsStruc.sectorsInTheVolume << std::endl;
    std::cout << "   LCN of VCN 0 of the $MFT (0x30) : " << ntfsStruc.lcnOfMFT << std::endl;
    std::cout << "   LCN of VCN 0 of the $MFTMirr (0x38) : " << ntfsStruc.lcnOfSMFTMirr << std::endl;
    std::cout << "   Clusters per MFT Record (0x40) : " << ntfsStruc.clusterPerMftRecord << std::endl;
    WORD toShowclusterPerIndexRecord = ntfsStruc.clusterPerIndexRecord;
    std::cout << "   Clusters per Index Record  (0x44): " << toShowclusterPerIndexRecord << std::endl;
    std::cout << "   Volume serial number  (0x48): " << ntfsStruc.volumeID << std::endl;
    showMe();
}

int calcAndShowAlldrives(checkPhys dllCheckPhysDrives)
{
    int foundDriveCount = 0;
    bool checkResult = false;
    std::string fileName = "\\\\.\\PhysicalDrive";
    std::cout << "       ------------------------------------" << std::endl;
    for (int DriveNum = 0; DriveNum < 30; DriveNum++)
    {
        checkResult = false;
        dllCheckPhysDrives(checkResult, DriveNum, fileName);
        if (checkResult == true)
        {
            std::cout << "       " << fileName << DriveNum << "             exist " << std::endl;
            foundDriveCount++;
        }
    }
    std::cout << "\n" << "       Summary physical drives founded : " << foundDriveCount << std::endl;
    std::cout << "       ------------------------------------\n" << "\n" << std::endl;
    return foundDriveCount;
}

int calcAndShowAllVolumes(checkVol dllCheckVolume)
{
    BYTE buffer[512];
    std::string fileName = "\\\\.\\";
    const int alphCount = 26;
    int foundVolumes = 0;
    std::string letters[alphCount] = { "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z" };
    for (int letterNum = 0; letterNum < 26; letterNum++)
    {
        bool checkResult = false;
        bool checkPointer = false;
        bool checkReading = false;
        std::string sysType = "";
        dllCheckVolume(checkResult, checkPointer, checkReading,  fileName, letters[letterNum], sysType);
        if (checkResult == true)
        {
            std::cout << "       " << fileName << letters[letterNum] << "           ";
            foundVolumes++;
            if (checkPointer == false)
            {
                std::cout << "       GOT PROBLEMS WITH READING ITS SYS TYPE (SetFilePointer PROBLEM) " << std::endl;
            }
            else
            {
                if (checkReading == false)
                {
                    std::cout << "       GOT PROBLEMS WITH READING ITS SYS TYPE (Reading info PROBLEM) " << std::endl;
                }
                else
                {
                    std::cout << sysType << std::endl;
                }
            }
        }
    };
    std::cout << "\n" << "       Summary volume drives founded : " << foundVolumes << std::endl;
    std::cout << "       ------------------------------------\n" << "\n" << std::endl;
    return foundVolumes;
}

int main()
{
    HMODULE hLib = LoadLibrary(L"assist.dll");
    if (hLib == NULL)
    {
        std::cout << "No able to attach dll !" << std::endl;
        showMe();
    }
    else
    {
        std::cout << "       ###########   MAIN MENU  ###########" << std::endl;
        checkPhys dllCheckPhysDrives = (checkPhys)GetProcAddress(hLib, "CheckPhysDrives");
        if (!dllCheckPhysDrives)
        {
            std::cout << "Error while getting func address (Phys)" << std::endl; 
            showMe();
        }
        else
        {
            if (calcAndShowAlldrives(dllCheckPhysDrives) == 0)
            {
                std::cout << "\n" << "  No drives exist, stopping programm..." << std::endl;
                showMe();
            }
            else
            {
                checkVol dllCheckVolume = (checkVol)GetProcAddress(hLib, "CheckVolume");
                if (!dllCheckVolume)
                {
                    std::cout << "Error while getting func address (Volumes)"<< std::endl;
                    showMe();
                }
                else
                {   
                    if (calcAndShowAllVolumes(dllCheckVolume) == 0)
                    {
                        std::cout << "\n" << "  No Volumes exist, stopping programm..." << std::endl;
                        showMe();
                    }
                    else
                    {   checkIsNTFS dllcheckIsNTFS = (checkIsNTFS)GetProcAddress(hLib, "checkIsNTFS");
                        if (!dllcheckIsNTFS)
                        {
                            std::cout << "Error while getting func address (checkIsNTFS)" << std::endl; 
                            showMe();
                        }
                        else
                        {
                            std::cout << "\n" << "   Enter the volume you are interested in (ONLY NTFS able atm) :  ";
                            bool checkResult;
                            bool checkPointer;
                            bool checkReading;;
                            bool IsNTFSResult;
                            int nTry = 0;
                            const int maxTry = 3;
                            std::string choosenVolume = "";
                            while (nTry < maxTry)
                            { 
                                std::cin >> choosenVolume;
                                
                                dllcheckIsNTFS(checkResult, checkPointer, checkReading, IsNTFSResult, choosenVolume);
                                if (checkResult == true && checkPointer == true && checkReading == true && IsNTFSResult == true)
                               {
                                  nTry = 3; // Exiting with msg
                                  std::cout << "   All checks done , System is NTFS, searching more info..." << std::endl;
                                  noMrChcks dllNTFSstruc = (noMrChcks)GetProcAddress(hLib, "noMoreChecks");
                                  if (!dllNTFSstruc)
                                  {
                                      std::cout << "Error while getting func address noMoreChecks" << std::endl;
                                      showMe();
                                  }
                                  else
                                  {
                                      NTFS_BootRecordMain _sentStuff;
                                      std::string fileName = "\\\\.\\";
                                      std::string fullPath = fileName + choosenVolume + ":";
                                      dllNTFSstruc(fullPath,_sentStuff);
                                      finalNTFSOutput(_sentStuff);                                          
                                  }
                               }
                                else
                                {
                                    nTry++;
                                    if (checkResult == false)
                                    {
                                        std::cout << "   Error: this path not found (cant open file). Check your input." << std::endl;
                                    }
                                    else
                                    {
                                        if (checkPointer == false)
                                        {
                                            std::cout << "   File: opened +" << std::endl;
                                            std::cout << "   Error: problems with setting pointer" << std::endl;
                                        }
                                        else
                                        {
                                            if (checkReading == false)
                                            {
                                                std::cout << "   File: opened +" << std::endl;
                                                std::cout << "   Pointer: set +" << std::endl;
                                                std::cout << "   Error: problems with reading file" << std::endl;
                                            }
                                            else
                                            {
                                                std::cout << "   File: opened +" << std::endl;
                                                std::cout << "   Pointer: set +" << std::endl;
                                                std::cout << "   File: read +" << std::endl;
                                                std::cout << "   Error: it's not NTFS (only NTFS supported atm)." << std::endl;
                                            }
                                        }
                                    }
                                    if (maxTry - nTry > 0)
                                    {
                                        std::cout << " You have " << maxTry - nTry << " more try..." << std::endl;
                                        std::cout << "\n" << "   Enter the volume you are interested in :  ";
                                    }
                                    else
                                    {
                                        std::cout << "   You have no more try...stopping programm" << std::endl;
                                        showMe();
                                    }
                                }
                            }
                        }
                    }
                }
            }
      
            FreeLibrary(hLib); }
    }
}

