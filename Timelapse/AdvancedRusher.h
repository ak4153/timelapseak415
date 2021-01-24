//#ifndef ADVANCEDRUSHER_H
//#define ADVANCEDRUSHER_H
//
//extern int rushToOrbisFromEllinia(int x);
//#endif
#include "MainFormHeader.h"
#include "MapleFunction.h"
#pragma once
static int rushToOrbisFromEllinia(int startMapId) {
	if (startMapId != 101000300) mapRush(101000300);
	Sleep(2000);
	Teleport(1582, 83); Sleep(2000);
	SendKey(0x44); Sleep(2000);
	/*SendNPCPacket(1032008, 1582, 83); Sleep(500);*/ //NPC Cherry
	SendKey(VK_RETURN); Sleep(2000); //Send Enter to press yes
	return 200000100;
}
static int rushToElliniaFromOrbis(int startMapId) {
	if (startMapId != 200000100) mapRush(200000100);
	Sleep(2000);
	//Teleport(2653,89);
	Teleport(2653, 85); Sleep(2000);
	SendKey(0x44); Sleep(2000);
	/*SendNPCPacket(2012006, 2653, 89); Sleep(500); *///NPC Platform Usher
	SendKey(VK_RETURN); Sleep(2000); //Send Enter to press victoria island
	SendKey(VK_RETURN); Sleep(2000); //Send Enter to press next
	SendKey(VK_RIGHT); Sleep(2000); //Send Enter to press yes
	SendKey(VK_RETURN); Sleep(2000); //Send Enter to press next
	mapRush(200000111); Sleep(3000);
	Teleport(-476, -634); Sleep(2000);
	SendKey(0x44); Sleep(2000);
	/*SendNPCPacket(2012001, -476, -634); Sleep(500);*/ //NPC Rini
	SendKey(VK_RETURN); Sleep(2000); //Send Enter to rush to elinia
	return 101000300;
}
static int rushToLudiFromOrbis(int startMapId) {
	if (startMapId != 200000100) mapRush(200000100);
	Sleep(4000);
	//Teleport(2653,89);
	Teleport(2653, 85); Sleep(2000);
	SendKey(0x44); Sleep(2000);
	/*SendNPCPacket(2012006, 2653, 89); Sleep(500); *///NPC Platform Usher
	SendKey(VK_DOWN); Sleep(2000);
	SendKey(VK_RETURN); Sleep(2000); //Send Enter to press Ludi
	SendKey(VK_RETURN); Sleep(2000); //Send Enter to press next
	SendKey(VK_RIGHT); Sleep(2000); //Send Enter to press yes
	SendKey(VK_RETURN); Sleep(2000); //Send Enter to press next
	mapRush(200000121); Sleep(3000); // to ludi station
	Teleport(1120, -175); Sleep(2000);//teleport to Sunny
	SendKey(0x44); Sleep(2000);
	SendKey(VK_RETURN); Sleep(2000); //Send Enter to rush to ludibrium
	return 220000100;
}
static int rushToOrbisFromLudi(int startMapId) {
	if (startMapId != 220000110) mapRush(220000110);
	Sleep(2000);
	Teleport(462, -140); Sleep(2000);
	SendKey(0x44); Sleep(2000);
	SendKey(VK_RETURN); Sleep(2000); //Send Enter to rush to orbis
	return 200000100;
}
static int rushToVictoriaFromFlorina(int startMapId) {
	if (startMapId != 110000000) mapRush(110000000); //Rush to the map that links to Victoria
	Sleep(1000);
	SendNPCPacket(1000000004, -273, 151); Sleep(500);
	SendKey(VK_RETURN); Sleep(500); //Send Enter to press next
	SendKey(VK_RIGHT); Sleep(500); //Send Right Arrow to select yes
	SendKey(VK_RETURN); Sleep(500); //Send Enter to press yes
	return 104000000;
}
static int rushToFlorinaFromVictoria(int startMapId) {
	if (PointerFuncs::getCharMesos() < 1500) {
		return -1;
	}
	if (startMapId != 104000000) mapRush(104000000); //Rush to the map that links to Florina Beach
	Sleep(1000);
	SendNPCPacket(1000000008, 1746, 647); Sleep(500); //NPC Pason
	SendKey(VK_RETURN); Sleep(500); //Send Enter to press yes
	return 110000000;
}