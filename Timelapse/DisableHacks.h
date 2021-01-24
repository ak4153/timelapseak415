#pragma once
#include "MainForm.h"
ref struct Flags {
static bool dctimerFlag = false;
static bool autosellTimeFlag = false;
static	bool autoRepotTimeFlag = false;
static bool timerActionTimeFlag = false;
static	bool autocccspeopleFlag = false;
static	bool autocccslvlupFlag = false;
static bool kamiFlag = false;
static bool kamiLootFlag = false;
static bool dupexFlag = false;


};
void disableTimers() {

	if (Timelapse::MainForm::TheInstance->autoSellTimer->Enabled) {
		Flags::autosellTimeFlag = true;
		Timelapse::MainForm::TheInstance->autoSellTimer->Stop();
	}
	if (Timelapse::MainForm::TheInstance->autoPotTimer->Enabled) {
		Flags::autoRepotTimeFlag = true;
		Timelapse::MainForm::TheInstance->autoPotTimer->Stop();
	}
	if (Timelapse::MainForm::TheInstance->dcTimer->Enabled) {
		Flags::dctimerFlag = true;
		Timelapse::MainForm::TheInstance->dcTimer->Enabled = false;
	}

		if (Timelapse::MainForm::TheInstance->timerActionTime->Enabled)
		{
			Flags::timerActionTimeFlag = true;
			Timelapse::MainForm::TheInstance->timerActionTime->Stop();
		}

		if (Timelapse::MainForm::TheInstance->cb_actionPeople->Checked)
		{
			Timelapse::MainForm::TheInstance->cb_actionPeople->Checked = false;
			Flags::autocccspeopleFlag = true;
		}

		if (Timelapse::MainForm::TheInstance->cb_actionLvlUp->Checked) {
			Timelapse::MainForm::TheInstance->cb_actionLvlUp->Checked = false;
			Flags::autocccslvlupFlag = true;
		}
}

void enableTimers() {
	if (Flags::dctimerFlag) {
		Timelapse::MainForm::TheInstance->dcTimer->Enabled = true;
		Flags::dctimerFlag = false;
	}
	if (Flags::autosellTimeFlag) {
		Timelapse::MainForm::TheInstance->autoSellTimer->Start();
		Flags::autosellTimeFlag = false;
	}
	if (Flags::autoRepotTimeFlag) {
		Timelapse::MainForm::TheInstance->autoPotTimer->Start();
		Flags::autoRepotTimeFlag = false;
	}
	if (Flags::autocccslvlupFlag) {
		Timelapse::MainForm::TheInstance->cb_actionLvlUp->Checked = true;
	}

	if (Flags::autocccspeopleFlag) {
		Timelapse::MainForm::TheInstance->cb_actionPeople->Checked = true;
	}
	if (Flags::timerActionTimeFlag) {
		Timelapse::MainForm::TheInstance->timerActionTime->Start();
		Flags::timerActionTimeFlag = false;
	}
}

void disableVacs() {
	//if (Timelapse::MainForm::TheInstance->cbKami->Checked) {
	//	Timelapse::MainForm::TheInstance->cbKami->Checked = false;
	//	Flags::kamiFlag = true;
	//}
	//if (Timelapse::MainForm::TheInstance->cbKamiLoot->Checked) {
	//	Timelapse::MainForm::TheInstance->cbKamiLoot->Checked = false;
	//	Flags::kamiLootFlag = true;
	//}
	if (Timelapse::MainForm::TheInstance->cbDupeX->Checked) {
		Timelapse::MainForm::TheInstance->cbDupeX->Checked = false;
		Flags::dupexFlag = true;
	}
}

void enableVacs() {
	//if (Flags::kamiFlag == true) {
	//	Timelapse::MainForm::TheInstance->cbKami->Checked = true;
	//	Timelapse::MainForm::TheInstance->cbAttack->Checked = true;
	//	Timelapse::MainForm::TheInstance->cbLoot->Checked = true;
	//	Timelapse::MainForm::TheInstance->cbMP->Checked = true;
	//	Timelapse::MainForm::TheInstance->cbHP->Checked = true;
	//}
	//if (Flags::kamiLootFlag == true) {
	//	Timelapse::MainForm::TheInstance->cbKamiLoot->Checked = true;
	//	Timelapse::MainForm::TheInstance->cbAttack->Checked = true;
	//	Timelapse::MainForm::TheInstance->cbLoot->Checked = true;
	//	Timelapse::MainForm::TheInstance->cbMP->Checked = true;
	//	Timelapse::MainForm::TheInstance->cbHP->Checked = true;
	//}
	if (Flags::dupexFlag) {
		Timelapse::MainForm::TheInstance->cbDupeX->Checked = true;
		Flags::dupexFlag = false;

	}
}
void disableMacros() {
	Timelapse::MainForm::TheInstance->cbAttack->Checked = false;
	Timelapse::MainForm::TheInstance->cbLoot->Checked = false;
	Timelapse::MainForm::TheInstance->cbMP->Checked = false;
	Timelapse::MainForm::TheInstance->cbHP->Checked = false;
}
void enableMacros() {
	Timelapse::MainForm::TheInstance->cbAttack->Checked = true;
	Timelapse::MainForm::TheInstance->cbLoot->Checked = true;
	Timelapse::MainForm::TheInstance->cbMP->Checked = true;
	Timelapse::MainForm::TheInstance->cbHP->Checked = true;
}