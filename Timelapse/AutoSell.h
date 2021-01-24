#pragma once
#include "MainFormHeader.h"
#include "MapleFunction.h"
#include "MainForm.h"

using namespace Timelapse;
static int getMapIdForAutoSell(int comboBoxIndex) {
	switch (comboBoxIndex) {
	case 0:
		return 102000002;
	case 1:
		return 105070001;
	case 2:
		return 200000002;
	case 3:
		return 220000002;
	default:
		return -1;
		break;
	}
}
static void SellAllEQPByMouse() {
	int EQUIP_SLOTS = Convert::ToInt32(MainForm::TheInstance->tb_numOfclicksAutoSell->Text);
	MouseInput::Mouse mouse;
	HelperFuncs::SetMapleWindowToForeground();
	MainForm::TheInstance->lb_botStatus->Text = "Auto Clicking to sell inventory";
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
	// Close UI 
	SendKey(VK_ESCAPE);
	Sleep(2000);
}

static void rushToSell(int mapId) {
	MouseInput::Mouse mouse;
	if (mapId == 102000002) {//perion
		MainForm::TheInstance->lb_botStatus->Text = "Autoselling in perion";
		while (ReadPointer(UIMiniMapBase, OFS_MapID) != 102000002) {
			mapRush(102000002);
		}
		Log::WriteLineToConsole("Were at victoria, rushing to perion dep. store ...");
		Sleep(5000);
		Log::WriteLineToConsole("Opening trade UI ...");
		// Send open trade packet
		// Todo: this needs more work
		Teleport(-41, 121);
		mouse.moveTo(482, 485, false, false);
		Sleep(500);
		mouse.doubleLeftClick();
		//SendPacket("3A ** 8A 0B ** ** ** ** ** **");
		Sleep(2000);
		SendKey(0x44);
		SendKey(0x44);
		Sleep(2000);
		//Try selling items by mouse
		SellAllEQPByMouse();
	}

	if (mapId == 105070001) {//sleepywood
		MainForm::TheInstance->lb_botStatus->Text = "AutoSelling in Sleepywood";
		while (ReadPointer(UIMiniMapBase, OFS_MapID) != 105070001) {
			mapRush(105070001);
			Sleep(5000);
		}
		Log::WriteLineToConsole("Were at Victoria-dungeon, rushing to Ant Tunnel Park Rapist Van ...");
		Sleep(5000);
		Log::WriteLineToConsole("Opening trade UI ...");
		Teleport(378, -246);
		Sleep(2000);
		SendKey(0x44);
		SendKey(0x44);
		Sleep(2000);
		//Try selling items by mouse
		SellAllEQPByMouse();
	}

	if (mapId == 200000002) {//orbis
		MainForm::TheInstance->lb_botStatus->Text = "AutoSelling in Orbis";
		while (ReadPointer(UIMiniMapBase, OFS_MapID) != 200000002) {
			mapRush(200000002);
			Sleep(5000);
		}
		Log::WriteLineToConsole("Were at Ossyria, rushing to orbis Dep store ...");
		Sleep(5000);
		Log::WriteLineToConsole("Opening trade UI ...");
		Teleport(299, 187);
		Sleep(2000);
		SendKey(0x44);
		SendKey(0x44);
		Sleep(2000);
		//Try selling items by mouse
		SellAllEQPByMouse();
	}
	if (mapId == 220000002) {//ludi pharmacy
		MainForm::TheInstance->lb_botStatus->Text = "AutoSelling in Ludi";
		while (ReadPointer(UIMiniMapBase, OFS_MapID) != 220000002) {
			mapRush(220000002);
			Sleep(5000);
		}
		Log::WriteLineToConsole("Were at Ludi, rushing to ludi department store ...");
		Log::WriteLineToConsole("Teleporting close to NPC ...");
		Sleep(2000);
		Teleport(287, 125);
		Sleep(2000);
		Log::WriteLineToConsole("Opening trade UI ...");
		SendKey(0x44);
		SendKey(0x44);
		Sleep(2000);
		//Try selling items by mouse
		SellAllEQPByMouse();
	}
}
static void rushBackToMapId(int mapId) {
	MainForm::TheInstance->lb_botStatus->Text = "Rushing back to botting map";
	while (ReadPointer(UIMiniMapBase, OFS_MapID) != mapId) {
		mapRush(mapId);
		Sleep(2000);
	}
}