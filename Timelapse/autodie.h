#pragma once
#include "MainFormHeader.h"
#include "MapleFunctions.h"
#include "MainForm.h"
#include "Packet.h"
#include "Mouse.h"

static bool checkIfNeedToDie(int targetExp, int currExp) {
	MouseInput::Mouse mouse;
	if (currExp >= targetExp) {
		return true;
	}
	else return false;
}
void autoDie() {
	HelperFuncs::SetMapleWindowToForeground();
	MouseInput::Mouse mouse;
	Timelapse::MainForm::TheInstance->lb_botStatus->Text = "Auto Die sequence";
	Log::WriteLine("entered autodie sequence");
	SendPacket("30 00 72 76 9D 00 FD 00 00 BB 00 00 00 00 00");
	Sleep(3000);
	mouse.moveTo(517, 252, false, false);
	mouse.leftClick();
	Sleep(3000);
}