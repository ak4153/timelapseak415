#include "MainFormHeader.h"
#pragma once
static void autoCS(int delay) {
	bool dctimerFlag = false;
	bool autosellTimeFlag = false;
	bool autoRepotTimeFlag = false;
	//timers disable
	if (MainForm::TheInstance->autoSellTimer->Enabled){
		autosellTimeFlag = true;
		MainForm::TheInstance->autoSellTimer->Stop();
	}
	if (MainForm::TheInstance->autoPotTimer->Enabled) {
		autoRepotTimeFlag = true;
		MainForm::TheInstance->autoPotTimer->Stop();
	}
	if (MainForm::TheInstance->dcTimer->Enabled) {
		dctimerFlag = true;
		MainForm::TheInstance->dcTimer->Enabled = false;
	}
	MainForm::TheInstance->lb_botStatus->Text = "Auto Cashshop Triggered";
	Sleep(2000);
	SendPacket(gcnew String("28 00 ** ** ** 00")); //Send go to CS packet
	Sleep(delay * 1000);
	SendPacket(gcnew String("26 00")); //Send transfer back to field packet

	//check again for the timer
	if (dctimerFlag) {
		MainForm::TheInstance->dcTimer->Enabled = true;
		dctimerFlag = false;
	}
	if (autosellTimeFlag) {
		MainForm::TheInstance->autoSellTimer->Start();
		autosellTimeFlag = false;
	}
	if (autoRepotTimeFlag) {
		MainForm::TheInstance->autoPotTimer->Start();
		autoRepotTimeFlag = false;
	}
}
static void AutoCC(int toChannel) {
	int channel;
	int channelAmount = Convert::ToUInt16(MainForm::TheInstance->channelAmount->Text);
	//List<int> excludedChannels = {};
	// read string from settings	
	//String excludedChannelsText = MainForm::TheInstance->tbCSDelay->Text;
	//int i;
	//for (i=0, i<sizeof(excludedChannelsText), i++)
	// parse string by comma into excludedChanList
	//String firstchar = excludedChannelsText[1];

	if (toChannel == -1) {
		channel = rand() % channelAmount;
		while(channel == Convert::ToUInt32(PointerFuncs::getChannel()))
			channel = rand() % channelAmount;
	}
		
	else channel = toChannel;
	SendPacket(gcnew String("27 00 " + channel.ToString("X2") + " ** ** ** 00")); //Send Auto CC Packet
	MainForm::TheInstance->lb_botStatus->Text = "Auto CC Triggered";
	Sleep(200);
}

//takes index of selected combobox and delay cs=0, cc =1
static void autoCsCcLVLUP(int cscc, int delay) {
	if (cscc == 0) {

		autoCS(delay);

	}
	if (cscc == 1) {
		AutoCC(-1);
	}
}

static void autoCsCcPeople(int cscc,int delay) {
	if (cscc == 0) {
		autoCS(delay);
	}
	if (cscc == 1) {
		AutoCC(-1);
	}

}
static void autoCsCcTime(int cscc, int delay) {
	if (cscc == 0) {
		autoCS(delay);
	}
	if (cscc == 1) {
		AutoCC(-1);
	}

}
