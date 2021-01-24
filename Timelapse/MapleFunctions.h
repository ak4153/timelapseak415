#pragma once
#include <Windows.h>
#include <tchar.h>
#include "memory.h"
#include "Hooks.h"
#include "Packet.h"
#include "Mouse.h"
#include "MainForm.h"
//Get MS Window Handle
static HWND GetMSWindowHandle() {
	HWND msHandle = nullptr;
	TCHAR buf[256] = { 0 };
	ULONG procid;
	for (HWND hwnd = GetTopWindow(nullptr); hwnd != nullptr; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) {
		GetWindowThreadProcessId(hwnd, &procid);
		if (procid != GetCurrentProcessId()) continue;
		if (!GetClassName(hwnd, buf, 256)) continue;
		if (_tcscmp(buf, _T("MapleStoryClass")) != 0) continue;
		msHandle = hwnd;
	}
	if (!msHandle) return nullptr;

	return msHandle;
}

//Get MS ThreadID
static ULONG GetMSThreadID() {
	if (GlobalVars::mapleWindow == nullptr)
		GlobalVars::mapleWindow = GetMSWindowHandle();
	return GetWindowThreadProcessId(GlobalVars::mapleWindow, nullptr);
}

#pragma managed
//Get MS ProcessID
static String^ GetMSProcID() {
	int processID = GetCurrentProcessId();
	return "0x" + processID.ToString("X") + " (" + processID.ToString() + ")";
}

//Check if position is valid 
static bool isPosValid(int X, int Y) {
	return ((ReadPointerSigned(CWvsPhysicalSpace2DBase, OFS_LeftWall) <= X) && (ReadPointerSigned(CWvsPhysicalSpace2DBase, OFS_RightWall) >= X)
		&& (ReadPointerSigned(CWvsPhysicalSpace2DBase, OFS_TopWall) <= Y) && (ReadPointerSigned(CWvsPhysicalSpace2DBase, OFS_BottomWall) >= Y));
}

//Teleport to parameter position
static void Teleport(int X, int Y) {
	//if (isPosValid(X, Y)) {
	WritePointer(UserLocalBase, OFS_TeleX, X);
	WritePointer(UserLocalBase, OFS_TeleY, Y);
	WritePointer(UserLocalBase, OFS_Tele, 1);
	//WritePointer(UserLocalBase, OFS_Tele + 4, 1);
//}
}

//Teleport to parameter point
static void Teleport(POINT point) {
	if (isPosValid(point.x, point.y)) {
		WritePointer(UserLocalBase, OFS_TeleX, point.x);
		WritePointer(UserLocalBase, OFS_TeleY, point.y);
		WritePointer(UserLocalBase, OFS_Tele, 1);
		WritePointer(UserLocalBase, OFS_Tele + 4, 1);
	}
}
#pragma endregion

namespace PointerFuncs {
	bool isHooksEnabled = true; //For the future, disable pointers that requires Codecaves or function calls

	//Retrieve Char Level
	static String^ getCharLevel() {
		const UINT8 level = readCharValueZtlSecureFuse(*(ULONG*)CharacterStatBase + OFS_Level);
		if (level == 0) return "00";
		return Convert::ToString(level);
	}

	//Retrieve Char Job ID
	static SHORT getCharJobID() {
		return readShortValueZtlSecureFuse(*(ULONG*)CharacterStatBase + OFS_JobID);
	}

	//Retrieve Char Job
	static String^ getCharJob() {
		return gcnew String(GetJobName(getCharJobID()));
	}

	//Retrieve Char HP
	static String^ getCharHP() {
		if (isHooksEnabled)
			Jump(statHookAddr, Assembly::StatHook, 0);
		else
			WriteMemory(statHookAddr, 5, 0x8D, 0x0C, 0x80, 0x3B, 0xCB);

		if (Assembly::maxHP != 0)
			Assembly::hpPercent = ((double)Assembly::curHP / Assembly::maxHP) * 100.0;

		return Assembly::hpPercent.ToString("f2") + "%";
	}

	//Retrieve Char MP
	static String^ getCharMP() {
		if (isHooksEnabled)
			Jump(statHookAddr, Assembly::StatHook, 0);
		else
			WriteMemory(statHookAddr, 5, 0x8D, 0x0C, 0x80, 0x3B, 0xCB);

		if (Assembly::maxMP != 0)
			Assembly::mpPercent = ((double)Assembly::curMP / Assembly::maxMP) * 100.0;

		return Assembly::mpPercent.ToString("f2") + "%";
	}

	//Retrieve Char EXP
	static int getCharEXP() {
		if (isHooksEnabled)
			Jump(statHookAddr, Assembly::StatHook, 0);
		else
			WriteMemory(statHookAddr, 5, 0x8D, 0x0C, 0x80, 0x3B, 0xCB);

		if (Assembly::maxEXP != 0)
			Assembly::expPercent = ((double)Assembly::curEXP / Assembly::maxEXP) * 100.0;

		/*return Assembly::expPercent.ToString("f2") + "%";*/
		return Assembly::expPercent;
	}

	//Retrieve Char Mesos
	static UINT getCharMesos() {
		return readLongValueZtlSecureFuse((ULONG*)(*(ULONG*)CharacterStatBase + OFS_Mesos));
	}

	//Retrieve Map Name
	static String^ getMapName() {
		if (isHooksEnabled)
			Jump(mapNameHookAddr, Assembly::MapNameHook, 1);
		else
			WriteMemory(mapNameHookAddr, 6, 0x89, 0x7D, 0xD8, 0x8D, 0x4D, 0xEC);

		if (Assembly::mapNameAddr == 0x0) return "Waiting..";

		char *mapNameBuff = (char*)(Assembly::mapNameAddr + 12);
		return gcnew String(mapNameBuff);
	}

	//Retrieve Left Wall coord
	static String^ getMapLeftWall() {
		return ReadPointerSignedInt(CWvsPhysicalSpace2DBase, OFS_LeftWall).ToString();
	}

	//Retrieve Right Wall coord
	static String^ getMapRightWall() {
		return ReadPointerSignedInt(CWvsPhysicalSpace2DBase, OFS_RightWall).ToString();
	}

	//Retrieve Top Wall coord
	static String^ getMapTopWall() {
		return ReadPointerSignedInt(CWvsPhysicalSpace2DBase, OFS_TopWall).ToString();
	}

	//Retrieve Bottom Wall coord
	static String^ getMapBottomWall() {
		return ReadPointerSignedInt(CWvsPhysicalSpace2DBase, OFS_BottomWall).ToString();
	}

	//Retrieve Char Name
	static String^ getCharName() {
		String^ charName = Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)(void*)(ReadPointerString(CharacterStatBase, OFS_Ign)));
		if (String::IsNullOrEmpty(charName)) return "CharName";
		return charName;
	}

	//Retrieve World
	static String^ getWorld() {
		const int worldID = ReadPointer(ServerBase, OFS_World);
		if (getCharName()->Equals("CharName")) return "Null";
		switch (worldID) {
		case 0:
			return("Scania");
		case 1:
			return("Bera");
		case 2:
			return("Broa");
		case 3:
			return("Windia");
		case 4:
			return("Khaini");
		case 5:
			return("Bellocan");
		case 6:
			return("Mardia");
		case 7:
			return("Kradia");
		case 8:
			return("Yellonde");
		case 9:
			return("Demethos");
		case 10:
			return("Galicia");
		case 11:
			return("El Nido");
		case 12:
			return("Zenith");
		case 13:
			return("Arcania");
		case 14:
			return("Chaos");
		case 15:
			return("Nova");
		case 16:
			return("Regenades");
		default:
			return("Null");
		}
	}

	//Retrieve Channel
	static String^ getChannel() {
		return (ReadPointer(ServerBase, OFS_Channel) + 1).ToString();
	}

	//Retrieve MapID
	static String^ getMapID() {
		return ReadPointerSignedInt(UIMiniMapBase, OFS_MapID).ToString();
	}

	//Retrieve Char Position
	static String^ getCharPos() {
		return "(" + ReadPointerSignedInt(UserLocalBase, OFS_CharX).ToString() + ", " + ReadPointerSignedInt(UserLocalBase, OFS_CharY).ToString() + ")";
	}

	//Retrieve Char X Position
	static String^ getCharPosX() {
		return ReadPointerSignedInt(UserLocalBase, OFS_CharX).ToString();
	}

	//Retrieve Char Y Position
	static String^ getCharPosY() {
		return ReadPointerSignedInt(UserLocalBase, OFS_CharY).ToString();
	}

	//Retrieve Mouse Position
	static String^ getMousePos() {
		return "(" + ReadMultiPointerSigned(InputBase, 2, OFS_MouseLocation, OFS_MouseX).ToString() + ", " + ReadMultiPointerSigned(InputBase, 2, OFS_MouseLocation, OFS_MouseY).ToString() + ")";
	}

	//Retrieve Mouse X Position
	static String^ getMousePosX() {
		return ReadMultiPointerSigned(InputBase, 2, OFS_MouseLocation, OFS_MouseX).ToString();
	}

	//Retrieve Mouse Y Position
	static String^ getMousePosY() {
		return ReadMultiPointerSigned(InputBase, 2, OFS_MouseLocation, OFS_MouseY).ToString();
	}

	//Retrieve Char Animation
	static String^ getCharAnimation() {
		return ReadPointerSignedInt(UserLocalBase, OFS_CharAnimation).ToString();
	}

	//Retrieve Char Foothold
	static String^ getCharFoothold() {
		return ReadMultiPointerSigned(UserLocalBase, OFS_pID, OFS_Foothold).ToString();
	}

	//Retrieve Attack Count
	static String^ getAttackCount() {
		return ReadPointerSignedInt(UserLocalBase, OFS_AttackCount).ToString();
	}

	//Retrieve Buff Count
	static String^ getBuffCount() {
		return (*(ULONG*)OFS_BuffCount).ToString();
	}

	//Retrieve Breath Count
	static String^ getBreathCount() {
		return ReadPointerSignedInt(UserLocalBase, OFS_Breath).ToString();
	}

	//Retrieve People Count
	static String^ getPeopleCount() {
		return ReadPointerSignedInt(UserPoolBase, OFS_PeopleCount).ToString();
	}

	//Retrieve Mob Count
	static String^ getMobCount() {
		return ReadPointerSignedInt(MobPoolBase, OFS_MobCount).ToString();
	}

	//Retrieve Item Count
	static String^ getItemCount() {
		return ReadPointerSignedInt(DropPoolBase, OFS_ItemCount).ToString();
	}

	//Retrieve Portal Count
	static String^ getPortalCount() {
		return ReadPointerSignedInt(PortalListBase, OFS_PortalCount).ToString();
	}

	//Retrieve NPC Count
	static String^ getNPCCount() {
		return ReadPointerSignedInt(NPCPoolBase, OFS_NPCCount).ToString();
	}
}

namespace HelperFuncs {

	static void SetMapleWindowToForeground()
	{
		SetForegroundWindow(GlobalVars::mapleWindow);
	}

	static RECT GetMapleWindowRect()
	{
		RECT msRect;
		GetWindowRect(GlobalVars::mapleWindow, &msRect);

		return  msRect;
	}

	static bool IsInGame()
	{
		const int mapID = Convert::ToInt32(PointerFuncs::getMapID());

		if (!mapID == 0) //&& !PointerFuncs::getCharName()->Equals("CharName")
			return true;

		return false;
	}

	static bool ValidToAttack()
	{
		const int attCnt = Convert::ToInt32(PointerFuncs::getAttackCount());
		const int mobCnt = Convert::ToInt32(PointerFuncs::getMobCount());
		// check for weapon and ammo
		if (!(attCnt > 99) && (mobCnt > 0) && IsInGame()) //!UsingBuff
			return true;

		return false;
	}

	static bool ValidToLoot()
	{
		const int pplCnt = Convert::ToInt32(PointerFuncs::getPeopleCount());

		if (!(pplCnt > 0) && IsInGame())
			return true;

		return false;
	}

	// Check if item count > 50 and AutoLoot is Checked
	static bool IsInventoryFull()
	{
		const int itemCnt = Convert::ToInt32(PointerFuncs::getItemCount());

		if (itemCnt > 1) // TODO: check if looting
			return true;

		return false;
	}
}

// Assumes trade UI open and maplewindow is in foreground
// These mouse pos should be absolute so it will work when the window is at 800x600 at to left corner
/*static void SellAllEQPByMouse() {
	const int EQUIP_SLOTS = 280; //actually this depends on server settings, can be extended ingame via CS
	MouseInput::Mouse mouse;
	HelperFuncs::SetMapleWindowToForeground();
	Log::WriteLineToConsole("Moving mouse to Ok button ...");
	mouse.moveTo(558, 450, false, false);
	Sleep(1400);
	// sell items by one
	for (int i = 0; i < EQUIP_SLOTS; i++) {
		mouse.doubleLeftClick();
		mouse.leftClick();
		Sleep(80);
	}
	Log::WriteLineToConsole("Closing trade UI ... ");
	Sleep(5000);
	// Close UI by mouse click
	keybd_event(VK_ESCAPE, 0x1B, 0, 0);
	Sleep(2000);
	mouse.leftClick(); 
}*/

/*static void SellAtEquipMapId(int islandID) {
	if (islandID == 22) { // Ludi-Path of time
		Log::WriteLineToConsole("Were at Ludi, rushing to ludi department store ...");
		Log::WriteLineToConsole("Teleporting close to NPC ...");
		Sleep(2000);
		Teleport(287, 125);
		Sleep(2000);
		Log::WriteLineToConsole("Opening trade UI ...");
		keybd_event(VK_SPACE, 0x20, 0, 0);
		Sleep(2000);
		//Try selling items by mouse
		SellAllEQPByMouse();
	}
	if (islandID == 10) { // Victoria
		Log::WriteLineToConsole("Were at victoria, rushing to perion dep. store ...");
		Sleep(5000);
		Log::WriteLineToConsole("Opening trade UI ...");
		// Send open trade packet
		// Todo: this needs more work
		Teleport(-41, 121);
		Sleep(2000);
		keybd_event(VK_SPACE, 0x20, 0, 0);
		Sleep(2000);
		//Try selling items by mouse
		SellAllEQPByMouse();
	}
	if (islandID == 20) { // Ossyria - orbis
		Log::WriteLineToConsole("Were at Ossyria, rushing to orbis Dep store ...");
		Sleep(5000);
		Log::WriteLineToConsole("Opening trade UI ...");
		Teleport(299,187);
		Sleep(2000);
		keybd_event(VK_SPACE, 0x20, 0, 0);
		Sleep( 2000);
		//Try selling items by mouse
		SellAllEQPByMouse();
	}
	if (islandID == 99) {
		Log::WriteLineToConsole("Were at Victoria-dungeon, rushing to Ant Tunnel Park Rapist Van ...");
		Sleep(5000);
		Log::WriteLineToConsole("Opening trade UI ...");
		Teleport(378, -246);
		Sleep(2000);
		keybd_event(VK_SPACE, 0x20, 0, 0);
		Sleep(2000);
		//Try selling items by mouse
		SellAllEQPByMouse();
	}

}
*/
static int AutoRepotTown(int indexOfComboBox) {
	switch (indexOfComboBox)
	{
	case 0:
		return 220000002;
	case 1:
		return 102000002;
	case 2:
		return 105070001;
	default:
		return 0;
		break;
	}
}
//todo add support for perion store
static void BuyPotions(int mapID,int hpToBuy,int mpToBuy) {
	MouseInput::Mouse mouse;
	HelperFuncs::SetMapleWindowToForeground();
	bool autoselltimerFlag = false;
	bool actionTimeFlag = false;
	
	if (Timelapse::MainForm::TheInstance->autoSellTimer->Enabled) {
		autoselltimerFlag = true;
		Timelapse::MainForm::TheInstance->autoSellTimer->Stop();
	}
	if (mapID == 220000002) { // Ludi
		Log::WriteLineToConsole("Were at Ludi, rushing to ludi department store ...");
		Log::WriteLineToConsole("Teleporting close to NPC ...");
		Sleep(2000);
		Teleport(287, 125);
		Sleep(2000);
		Log::WriteLineToConsole("Opening trade UI ...");
		keybd_event(VK_SPACE, 0x20, 0, 0);
		keybd_event(VK_SPACE, 0x20, 0, 0);
		Sleep(2000);
		mouse.moveTo(501, 566, false, false);
		Sleep(1000);
		mouse.leftClick();
		Sleep(1000);
		mouse.leftClick();
		Sleep(1000);
		mouse.leftClick();
		Sleep(1000);
		mouse.leftClick();
		Sleep(1000);
		mouse.leftClick();
		Sleep(1000);
		mouse.leftClick();
		if (hpToBuy > 0) {
			mouse.moveTo(360, 396, false, false);
			for (int i = hpToBuy / 100; i > 0; i--) {
				Sleep(500);
				mouse.doubleLeftClick();
				Sleep(1000);
				System::Windows::Forms::SendKeys::Send("100");
				Sleep(500);
				mouse.moveTo(559, 464, false, false);
				Sleep(500);
				mouse.leftClick();
				Sleep(500);
				mouse.moveTo(360, 396, false, false);
			}
		}
				if (mpToBuy > 0) {
					mouse.moveTo(371, 557, false, false);
					for (int i = mpToBuy / 100; i > 0; i--) {
						Sleep(500);
						mouse.doubleLeftClick();
						Sleep(1000);
						System::Windows::Forms::SendKeys::Send("100");
						Sleep(500);
						//moves to ok
						mouse.moveTo(559, 464, false, false);
						Sleep(500);
						mouse.leftClick();
						//moves to potion
						Sleep(500);
						mouse.moveTo(371, 557, false, false);
					}
			}
	}
	if (mapID == 102000002) { // Victoria

		Log::WriteLineToConsole("Were at victoria, rushing to perion dep. store ...");
		Sleep(2000);
		mouse.moveTo(482, 485, false, false);
		Sleep(500);
		mouse.doubleLeftClick();
		//Teleport(-41, 121);
		Sleep(2000);
		Log::WriteLineToConsole("Opening trade UI ...");
		keybd_event(VK_SPACE, 0x20, 0, 0);
		keybd_event(VK_SPACE, 0x20, 0, 0);
		Sleep(1000);
		if (hpToBuy > 0) {
			mouse.moveTo(363, 478, false, false);
			for (int i = hpToBuy / 100; i > 0; i--) {
				Sleep(500);
				mouse.doubleLeftClick();
				Sleep(1000);
				System::Windows::Forms::SendKeys::Send("100");
				Sleep(500);
				mouse.moveTo(559, 464, false, false);
				Sleep(500);
				mouse.leftClick();
				Sleep(500);
				mouse.moveTo(363, 478, false, false);
			}
		}
		if (mpToBuy > 0) {
			mouse.moveTo(360, 554, false, false);
			for (int i = mpToBuy / 100; i > 0; i--) {
				Sleep(500);
				mouse.doubleLeftClick();
				Sleep(1000);
				System::Windows::Forms::SendKeys::Send("100");
				Sleep(500);
				//moves to ok
				mouse.moveTo(559, 464, false, false);
				Sleep(500);
				mouse.leftClick();
				//moves to potion
				Sleep(500);
				mouse.moveTo(360, 554, false, false);
			}
		}
	}
	if (mapID == 200000002) { // Ossyria - orbis
		Log::WriteLineToConsole("Were at Ossyria, rushing to orbis Dep store ...");
		Sleep(5000);
		Log::WriteLineToConsole("Opening trade UI ...");
		Teleport(299, 187);
		Sleep(2000);
		keybd_event(VK_SPACE, 0x20, 0, 0);
		keybd_event(VK_SPACE, 0x20, 0, 0);
		Sleep(2000);
		//Try selling items by mouse
	}
	if (mapID == 105070001) {//sleepywood dungeon
		Log::WriteLineToConsole("Were at Victoria-dungeon, rushing to Ant Tunnel Park Rapist Van ...");
		Sleep(5000);
		Log::WriteLineToConsole("Opening trade UI ...");
		Teleport(378, -246);
		Sleep(2000);
		keybd_event(VK_SPACE, 0x20, 0, 0);
		keybd_event(VK_SPACE, 0x20, 0, 0);
		Sleep(2000);
		mouse.moveTo(501, 566, false, false);
		Sleep(2000);
		mouse.leftClick();
		Sleep(2000);
		mouse.leftClick();
		Sleep(2000);
		mouse.leftClick();
		if (hpToBuy > 0) {
			mouse.moveTo(356, 443, false, false);
			for (int i = hpToBuy / 100; i > 0; i--) {
				Sleep(500);
				mouse.doubleLeftClick();
				Sleep(1000);
				System::Windows::Forms::SendKeys::Send("100");
				Sleep(500);
				mouse.moveTo(559, 464, false, false);
				Sleep(500);
				mouse.leftClick();
				Sleep(500);
				mouse.moveTo(356, 443, false, false);
			}
		}
		if (mpToBuy > 0) {
			mouse.moveTo(371, 554, false, false);
			for (int i = mpToBuy / 100; i > 0; i--) {
				Sleep(500);
				mouse.doubleLeftClick();
				Sleep(1000);
				System::Windows::Forms::SendKeys::Send("100");
				Sleep(500);
				//moves to ok
				mouse.moveTo(559, 464, false, false);
				Sleep(500);
				mouse.leftClick();
				//moves to potion
				Sleep(500);
				mouse.moveTo(371, 554, false, false);
			}
		}
	}
	Sleep(2000);
	keybd_event(VK_ESCAPE, 0x1B, 0, 0);
	if (autoselltimerFlag) {
		Timelapse::MainForm::TheInstance->autoSellTimer->Start();
		autoselltimerFlag = false;
	}
}
//get AP's to assign and assigns them
static void AutoAPAllocation(int str, int dex, int intel, int luk) {
	for (int i = 0; i < str; i++) {
		SendPacket("57 00 ** ** ** ** 40 00 00 00");
	}

	for (int i = 0; i < dex; i++) {
		SendPacket("57 00 ** ** ** ** 80 00 00 00 ");
	}
	for (int i = 0; i < intel; i++) {
		SendPacket("57 00 ** ** ** ** 00 01 00 00 ");
	}
	for (int i = 0; i < luk; i++) {
		SendPacket("57 00 ** ** ** ** 00 02 00 00");
	}
}