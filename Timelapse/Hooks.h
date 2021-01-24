#pragma once
#include "Structs.h"
#include "resource.h"
#include "GeneralFunctions.h"
#include "Packet.h"
#include "detours.h"
#include <queue>
#include "Addresses.h"

#pragma comment(lib, "detours.lib")
#define CodeCave(name) static void __declspec(naked) ##name() { _asm
#define EndCodeCave } 
#define JMP(frm, to) (int)(((int)to - (int)frm) - 5);
//Set Hook for function 
bool SetHook(bool enable, void** function, void* redirection) {
	if (DetourTransactionBegin() != NO_ERROR) return FALSE;
	if (DetourUpdateThread(GetCurrentThread()) != NO_ERROR) return FALSE;
	if ((enable ? DetourAttach : DetourDetach)(function, redirection) != NO_ERROR) return FALSE;
	if (DetourTransactionCommit() == NO_ERROR) return true;
	DetourTransactionAbort();
	return false;
}

//Unmanaged Global Variables
namespace GlobalVars {
	static HMODULE hDLL;
	static HWND mapleWindow = nullptr;
}

namespace Assembly {
ULONG curHP = 0, maxHP = 0, curMP = 0, maxMP = 0, curEXP = 0, maxEXP = 0, mapNameAddr = 0x0;
int ItemX = 0, ItemY = 0, animDelay = 10; //TODO:: actually animDelay should be a Double
double hpPercent = 0.00, mpPercent = 0.00, expPercent = 0.00;
bool isItemLoggingEnabled = false, isItemFilterEnabled = false, isItemFilterWhiteList = true;
bool isMobLoggingEnabled = false, isMobFilterEnabled = false, isMobFilterWhiteList = true;
ULONG itemLogged = 0, itemFilterMesos = 0, mobLogged = 0;
static std::vector<ULONG> *itemList = new std::vector<ULONG>(), *mobList = new std::vector<ULONG>();
static std::vector<SpawnControlData*> *spawnControl = new std::vector<SpawnControlData*>();
static std::vector<COutPacket> *sendPacketLogQueue = new std::vector<COutPacket>();
SendPacketData *sendPacketData;
//COutPacket *packetLogSend;
std::string packetlogSendStr;
SendPacketData* Temp = new SendPacketData;

CInPacket  * recvPacketData;
//RecvPacketData *recvPacketData;

ULONG dupeXFoothold = 0;


//Find item name using item ID in the ItemsList resource
static String^ findItemNameFromID(int itemID) {
	try {
		std::string result = "", tmpStr = "";
		HRSRC hRes = FindResource(GlobalVars::hDLL, MAKEINTRESOURCE(ItemsList), _T("TEXT"));
		if (hRes == nullptr) return "";
		const HGLOBAL hGlob = LoadResource(GlobalVars::hDLL, hRes);
		if (hGlob == nullptr) return "";
		const CHAR* pData = reinterpret_cast<const CHAR*>(LockResource(hGlob));
		std::istringstream file(pData);

		while (file.good()) {
			std::getline(file, tmpStr);
			if (tmpStr.find(std::to_string(itemID)) == 0) {
				tmpStr = tmpStr.substr(tmpStr.find('[') + 1, tmpStr.find(']'));
				tmpStr = tmpStr.substr(0, tmpStr.length() - 2);
				result = tmpStr;
			}
		}
		UnlockResource(hRes);
		return ConvertStdToSystemStr(result);
	}
	catch (...) { return "Error"; }
}

//Find mob name using mob ID in the MobsList resource
static String^ findMobNameFromID(int mobID) {
	try {
		std::string result = "", tmpStr = "";
		HRSRC hRes = FindResource(GlobalVars::hDLL, MAKEINTRESOURCE(MobsList), _T("TEXT"));
		if (hRes == nullptr) return "";
		const HGLOBAL hGlob = LoadResource(GlobalVars::hDLL, hRes);
		if (hGlob == nullptr) return "";
		const CHAR* pData = reinterpret_cast<const CHAR*>(LockResource(hGlob));
		std::istringstream file(pData);

		while (file.good()) {
			std::getline(file, tmpStr);
			if (tmpStr.find(std::to_string(mobID)) == 0) {
				tmpStr = tmpStr.substr(tmpStr.find('[') + 1, tmpStr.find(']'));
				tmpStr = tmpStr.substr(0, tmpStr.length() - 2);
				result = tmpStr;
			}
		}
		UnlockResource(hRes);
		return ConvertStdToSystemStr(result);
	}
	catch (...) { return "Error"; }
}

static SpawnControlData* __stdcall getSpawnControlStruct() {
	if (spawnControl->size() == 0) return nullptr;
	for (SpawnControlData *spawnControlStruct : *spawnControl)
		if (spawnControlStruct->mapID == ReadPointer(UIMiniMapBase, OFS_MapID))
			return spawnControlStruct;
	return nullptr;
}

inline void __stdcall itemLog() {
	if (itemLogged < 50000) return; //Ignore mesos
		String^ itemName = findItemNameFromID(itemLogged);
	if (itemLogged > 0 && !Timelapse::MainForm::TheInstance->lbItemSearchLog->Items->Contains(itemName + " (" + itemLogged.ToString() + ")")
		&&!Timelapse::MainForm::TheInstance->lbItemFilter->Items->Contains(itemName + " (" + itemLogged.ToString() + ")"))
		Timelapse::MainForm::TheInstance->lbItemSearchLog->Items->Add(itemName + " (" + itemLogged.ToString() + ")");
}

inline bool __stdcall shouldItemBeFiltered() {
	if (isItemFilterWhiteList) {
		for (ULONG item : *itemList)
			if (item == itemLogged) return false;
		return true;
	} 
	for (ULONG item : *itemList)
		if (item == itemLogged) return true;
	return false;	
}

inline void __stdcall mobLog() {
	String^ mobName = findMobNameFromID(mobLogged);
	if (mobLogged > 0 && !Timelapse::MainForm::TheInstance->lbMobSearchLog->Items->Contains(mobName + " (" + mobLogged.ToString() + ")"))
		Timelapse::MainForm::TheInstance->lbMobSearchLog->Items->Add(mobName + " (" + mobLogged.ToString() + ")");
}

inline bool __stdcall shouldMobBeFiltered() {
	if (isMobFilterWhiteList) {
		for (ULONG mob : *mobList)
			if (mob == mobLogged) return false;
		return true;
	}
	for (ULONG mob : *mobList)
		if (mob == mobLogged) return true;
	return false;
}

//static std::queue<CInPacket*> *recvPacketQueue = new std::queue<CInPacket*>();
inline void __stdcall RecordPacketRecv() {
	CInPacket *packet1 = new CInPacket();
	//CInPacket *oldPacket1 = recvPacketData->packet;

	//packet1->Loopback = oldPacket1->Loopback;
	//UCHAR data = *oldPacket1->Data;
	////void unk = *oldPacket->Unk;
	//USHORT header = *oldPacket1->Header;
	//packet1->Data = &data;
	//packet1->Unk = oldPacket1->Unk;
	//packet1->Header = &header;
	//packet1->Size = oldPacket1->Size;
	//packet1->Offset = oldPacket1->Offset;
	//packet1->EncryptedByShanda = oldPacket1->EncryptedByShanda;

	String^ packetData = "";
	recvPacketData->pData->Data;
	BYTE* Bytes = recvPacketData->pData->Data;
	int packetSize = recvPacketData->Size - 2;


	//BYTE* Bytes = oldPacket1->pData->Data;
	//int packetSize = oldPacket1->Size - 2;

	for (int i = 0; i < packetSize; i++) {
		writeByte(packetData, Bytes[i]);
		Log::WriteLineToConsole(packetData);
	}
	/*recvPacketQueue->push(packet);*/
}

static std::queue<COutPacket*> *sendPacketQueue = new std::queue<COutPacket*>();
inline void __stdcall addSendPacket() {
	//void* packet = new COutPacket();
	//memcpy((void*)packet, (void*)sendPacketData->packet, sizeof(COutPacket));
	COutPacket *packet = new COutPacket();
	COutPacket *oldPacket = sendPacketData->packet;

	/*Assembly::packetLogSend = sendPacketData->packet;*/
	 

	packet->Loopback = oldPacket->Loopback;
	UCHAR data = *oldPacket->Data;
	//void unk = *oldPacket->Unk;
	USHORT header = *oldPacket->Header;

	//packet->Data = oldPacket->Data;
	//packet->Unk = oldPacket->Unk;
	//packet->Header = oldPacket->Header;
	//packet->Size = oldPacket->Size;
	//packet->Offset = oldPacket->Offset;
	//packet->EncryptedByShanda = oldPacket->EncryptedByShanda;

	packet->Data = &data;
	packet->Unk = oldPacket->Unk;
	packet->Header = &header;
	packet->Size = oldPacket->Size;
	packet->Offset = oldPacket->Offset;
	packet->EncryptedByShanda = oldPacket->EncryptedByShanda;

	String^ packetData = "";
	BYTE* Bytes = oldPacket->Data;
	int packetSize = oldPacket->Size - 2;
	for (int i = 0; i < packetSize; i++) {
		writeByte(packetData, Bytes[i]);
	}

	Assembly::packetlogSendStr = ConvertSystemToStdStr(packetData);

	//Timelapse::MainForm::TheInstance->tvSendPackets->Nodes[0]->Nodes->Add(packetData);
			//Windows::Forms::TreeNode^ packetNode = gcnew Windows::Forms::TreeNode(packetData);
			//packetNode->Name = packetData;
			/*Timelapse::MainForm::TheInstance->tvSendPackets->Nodes->Add("packet");*/

	//String^ packetData1 = "";
	//BYTE* Bytes1 = packet->Data;
	//int packetSize1 = packet->Size - 2;
	//for (int i = 0; i < packetSize1; i++)
	//	writeByte(packetData1, Bytes1[i]);
	//Log::WriteLineToConsole(packetData1);


	sendPacketQueue->push(packet);
}

//``````````````````````````````````````````````````````````````````````
//```````````````````````````````````````````````````````````````````` 
	/*CodeCave(AnimDelay) {
		add eax, [animDelay]
		jmp dword ptr[animDelayAddrRet]
	} EndCodeCave*/

	CodeCave(StatHook) {
		push eax
		mov eax, [ebp + 0x08]
		mov [curHP], eax
		mov eax, [ebp + 0x0C]
		mov [maxHP], eax
		mov eax, [ebp + 0x10]
		mov [curMP], eax
		mov eax, [ebp + 0x14]
		mov [maxMP], eax
		mov eax, [ebp + 0x18]
		mov [curEXP], eax
		mov eax, [ebp + 0x1C]
		mov [maxEXP], eax
		pop eax
		lea ecx, [eax + eax * 0x4]
		cmp ecx, ebx
		jmp dword ptr[statHookAddrRet]
	} EndCodeCave

	CodeCave(MapNameHook) {
		mov [mapNameAddr], ecx
		mov [ebp - 0x28], edi
		lea ecx, [ebp - 0x14]
		jmp dword ptr[mapNameHookAddrRet]
	} EndCodeCave

	CodeCave(ItemVacHook) {
		pushad
		mov ecx, [ebp + 0x8]
		mov ebx, [ebp - 0x24]
		mov [ecx], ebx
		mov [ecx + 0x4], eax
		mov ecx, eax
		mov eax, ebx
		lea edx, [eax - 0x19]
		mov [ebp - 0x34], edx
		lea edx, [ecx - 0x32]
		add eax, 0x19
		add ecx, 0xA
		mov [ebp - 0x30], edx
		mov [ebp - 0x2C], eax
		mov [ebp - 0x28], ecx
		popad

		push eax
		push dword ptr ss : [ebp - 0x24]
		lea eax, dword ptr ss : [ebp - 0x34]
		jmp dword ptr[itemVacAddrRet]
	} EndCodeCave

	CodeCave(MouseFlyXHook) {
		push eax
		push ecx
		mov eax, [UserLocalBase]
		mov eax, [eax]
		mov ecx, [OFS_pID]
		mov eax, [eax + ecx]
		cmp esi, eax
		pop eax
		jne ReturnX
		mov eax, [InputBase]
		mov eax, [eax]
		mov ecx, [OFS_MouseLocation]
		mov eax, [eax + ecx]
		mov ecx, [OFS_MouseX]
		mov eax, [eax + ecx]

		ReturnX:
		pop ecx
		mov [ebx], eax
		mov edi, [ebp + 0x10]
		jmp dword ptr[mouseFlyXAddrRet]
	} EndCodeCave

	CodeCave(MouseFlyYHook) {
		push eax
		push ecx
		mov eax, [UserLocalBase]
		mov eax, [eax]
		mov ecx, [OFS_pID]
		mov eax, [eax + ecx]
		cmp esi, eax
		pop eax
		jne ReturnY
		mov eax, [InputBase]
		mov eax, [eax]
		mov ecx, [OFS_MouseLocation]
		mov eax, [eax + ecx]
		mov ecx, [OFS_MouseY]
		mov eax, [eax + ecx]

		ReturnY:
		pop ecx
		mov [edi], eax
		mov ebx, [ebp + 0x14]
		jmp dword ptr[mouseFlyYAddrRet]
	} EndCodeCave

	CodeCave(MobFreezeHook) {
		mov [esi + 0x00000248], 0x06
		mov eax, [esi + 0x00000248]
		jmp dword ptr[mobFreezeAddrRet]
	} EndCodeCave

	CodeCave(MobAutoAggroHook) {
		call dword ptr[cVecCtrlWorkUpdateActiveCall] //calls CVecCtrl::WorkUpdateActive()
		push eax
		mov edx, [UserLocalBase]
		mov edx, [edx]
		mov eax, [OFS_pID]
		mov edx, [edx + eax]
		mov edx, [edx + 0x8]
		mov eax, [OFS_Aggro]
		mov [esi + eax], edx //Aggro Offset (first cmp before CVecCtrl::ChaseTarget)
		pop eax
		jmp dword ptr[mobAutoAggroAddrRet]
	} EndCodeCave

	CodeCave(SpawnPointHook) {
		push ecx
		push edx
		cmp dword ptr[esp + 0x8], 0x009BBD5D
		je Return //If Spawning Mob, skip
		cmp dword ptr[esp + 0x8], 0x009C1D90
		je Return //If Spawning NPC, skip
		call getSpawnControlStruct //returns SpawnControlStruct* with the mapID == current mapID
		cmp eax, 0
		je Return //If end of SpawnControl and no match, skip
		mov edx, [eax + 4]
		mov dword ptr[esp + 0x10], edx //x
		mov edx, [eax + 8]
		mov dword ptr[esp + 0x14], edx //y

		Return :
		pop edx
		pop ecx
		mov eax, 0x00AE56F4 //original code
		jmp dword ptr[spawnPointAddrRet]
	} EndCodeCave

	CodeCave(ItemHook) {
		cmp dword ptr[esp], 0x005047B8
		jne NormalAPICall //If return not in CDropPool::TryPickUpDrop, skip
		push eax
		mov eax, [esp + 0x0C]
		mov [ItemX], eax
		mov eax, [esp + 0x10]
		mov [ItemY], eax
		pop eax

		NormalAPICall :
		jmp dword ptr PtInRect
	} EndCodeCave

	CodeCave(ItemFilterHook) {
		push ebx
		cmp byte ptr[isItemLoggingEnabled], 0
		je Continue //Skip Logging Items
		push eax
		mov [itemLogged], eax
		call itemLog
		pop eax

		Continue :
		cmp byte ptr[isItemFilterEnabled], 0
		je EndItemFilter //Skip if Item Filter is disabled
		mov ebx, [itemFilterMesos]
		cmp eax, ebx //Assumes item is mesos because there the mesos drop limit is 50,000 whereas the smallest item id is greater than that
		jle RemoveMesos //Remove mesos if item id is less than or equal to the user set limit. 
		push eax
		mov [itemLogged], eax
		call shouldItemBeFiltered
		cmp eax, 0 //Item shouldn't be filtered
		pop eax
		je EndItemFilter
		mov eax, 0x00 //Remove Item
		jmp EndItemFilter

		RemoveMesos :
		mov [edi + 0x30], 0x00 //Remove Mesos

		EndItemFilter :
		pop ebx
		mov [edi + 0x34], eax
		mov edi, [ebp - 0x14]
		jmp [itemFilterAddrRet]
	} EndCodeCave

	CodeCave(MobFilter1Hook) {
		push ebx
		call [cInPacketDecode4Addr] //CInPacket::Decode4()
		cmp byte ptr[isMobFilterEnabled], 0
		je EndMobFilter //Skip if Mob Filter is disabled
		push eax
		mov [mobLogged], eax
		call shouldMobBeFiltered
		cmp eax, 0 //Mob shouldn't be filtered
		pop eax
		je EndMobFilter
		pop ebx
		jmp [mobFilter1JmpAddr]

		EndMobFilter:
		pop ebx
		jmp [mobFilter1AddrRet]
	} EndCodeCave

	CodeCave(MobFilter2Hook) {
		push ebx
		call [cInPacketDecode4Addr] //CInPacket::Decode4()
		cmp byte ptr[isMobLoggingEnabled], 0
		je Continue //Skip Logging Mobs
		push eax
		mov [mobLogged], eax
		call mobLog
		pop eax

		Continue :
		cmp byte ptr[isMobFilterEnabled], 0
		je EndMobFilter //Skip if Mob Filter is disabled
		push eax
		mov [mobLogged], eax
		call shouldMobBeFiltered
		cmp eax, 0 //Mob shouldn't be filtered
		pop eax
		je EndMobFilter
		pop ebx
		jmp [mobFilter2JmpAddr]

		EndMobFilter:
		pop ebx
		jmp [mobFilter2AddrRet]
	} EndCodeCave

	CodeCave(SendPacketLogHook) {
		mov dword ptr [sendPacketData], esp
		pushad
		call [addSendPacket]
		popad
		jmp [cOutPacketAddrRet]
	} EndCodeCave


			CodeCave(RecievePacketLogHook) {
			lea    ebx, [ebp - 0x0C]
				mov recvPacketData, ebx
				pushad
				call[RecordPacketRecv]
				popad
				pop ebx
				leave
				ret 4
		} EndCodeCave

		//	CodeCave(RecievePacketLogHook) {
		//	push eax
		//		mov eax, ebp
		//		sub eax, 0ch
		//		mov dword ptr[recvPacketData], eax
		//		pop eax
		//		pushad
		//		call[RecordPacketRecv]
		//		popad
		//		pop esi
		//		pop ebx
		//		leave
		//		ret 4
		//} EndCodeCave

	CodeCave(DupeXHook) {
		pushfd
		push eax
		push ecx
		mov eax, [UserLocalBase]
		mov eax, [eax]
		test eax, eax
		je NullPlatform
		mov ecx, [OFS_pID]
		mov eax, [eax + ecx]
		lea ecx, [eax - 0x0c] //account id offset?
		test ecx, ecx
		je NullPlatform
		mov eax, [ecx + 0x00000114] //kb offset?
		test eax, eax
		je NullPlatform
		mov dword ptr[dupeXFoothold], eax
		cmp esi, ecx
		je Normal
		mov edi, [dupeXFoothold]
		jmp Normal

		NullPlatform:
		mov dword ptr [dupeXFoothold], 0x00
		jmp Normal

		Normal:
		pop ecx
		pop eax
		popfd
		mov[esi + 0x00000114], edi
		jmp dword ptr[dupeXAddrRet]
	} EndCodeCave
}

namespace Hooks {
	//For botting
	bool CUserLocal__Update_Hook(bool enable) {
		typedef void(__stdcall *pfnCUserLocal__Update)(PVOID, PVOID);
		static auto CUserLocal__Update = reinterpret_cast<pfnCUserLocal__Update>(userlocalUpdateAddr);

		pfnCUserLocal__Update hook = [](PVOID ecx, PVOID edx) {

			return CUserLocal__Update(ecx, edx);
		};

		return SetHook(enable, reinterpret_cast<void**>(&CUserLocal__Update), hook);
	}

	bool ChangeChannel(int channel) {
		typedef int(__stdcall *pfnCField__SendTransferChannelRequest)(int nTargetChannel); // Changes Channel
		static auto CField__SendTransferChannelRequest = reinterpret_cast<pfnCField__SendTransferChannelRequest>(ccAddr);

		if(*reinterpret_cast<void**>(ServerBase) != nullptr && !ReadPointerSignedInt(UIMiniMapBase, OFS_MapID) == 0) {
			WritePointer(UserLocalBase, OFS_Breath, 0); //Set Breath Value to 0
			CField__SendTransferChannelRequest(channel);
			return true;
		}

		return false;
	}

	bool CLogin__OnRecommendWorldMessage_Hook(bool enable) {
		typedef void(__stdcall *pfnCLogin__OnRecommendWorldMessage)(PVOID, PVOID);
		static pfnCLogin__OnRecommendWorldMessage CLogin__OnRecommendWorldMessage = reinterpret_cast<pfnCLogin__OnRecommendWorldMessage>(cloginOnRecommendWorldAddr);

		//typedef void(__stdcall *pfnCLogin__SendLoginPacket)(PVOID, PVOID, int world, int channel);
		//static auto CLogin__SendLoginPacket = reinterpret_cast<pfnCLogin__SendLoginPacket>(cloginSendLoginPacketAddr);

		pfnCLogin__OnRecommendWorldMessage hook = [](PVOID ecx, PVOID num) -> void {
			//CLogin__SendLoginPacket(ecx, edx, 0, 1);
			CLogin__OnRecommendWorldMessage(ecx, num);
		};

		return SetHook(enable, reinterpret_cast<void**>(&CLogin__OnRecommendWorldMessage), hook);
	}
}