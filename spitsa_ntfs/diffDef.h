#pragma once
#pragma pack(push,1)
typedef struct
{
	BYTE jumpCode[3];//0x0000
	BYTE OEM_Name[8];//0x0003
	WORD bytesPerSector;//0x000B
	BYTE sectorPerCluster;//0x000D
	WORD reservedSectors;//0x000E
	BYTE shouldBeZero[3];
	WORD noUsed1;
	BYTE mediaDescriptor;//0x0015
	WORD shouldBeZero2;
	WORD sectorPerTrack;//0x0018
	WORD numOfHeaders;//0x001A
	DWORD hiddenSectors;
	DWORD noUsed2;
	DWORD noUsed3;
	ULONGLONG sectorsInTheVolume;//0x0028
	ULONGLONG lcnOfMFT;//0x0030
	ULONGLONG lcnOfSMFTMirr;//0x0038
	DWORD clusterPerMftRecord;//0x0040
	BYTE clusterPerIndexRecord;//0x0044~
	BYTE noUsed4[3];
	ULONGLONG volumeID;//0x0048
	DWORD checksum;
	BYTE bigLoadingCode[426];
	WORD markerOfTheEnd;
} NTFS_BootRecordMain;

typedef void (*checkIsNTFS)(bool&,bool&,bool&,bool&, std::string);
typedef void (*checkPhys)(bool&, int, std::string);
typedef void (*checkVol)(bool&, bool&, bool&,std::string, std::string,std::string&);
typedef void (*noMrChcks)(std::string, NTFS_BootRecordMain&);
//commited from test branch
