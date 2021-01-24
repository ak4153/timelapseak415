#include "MainFormHeader.h"
#include "Macro.h"
#include <msclr\marshal_cppstd.h>

#pragma once
using namespace msclr::interop;
static String^ spoofMAC() {
	String^ macAddress = "";
		for (int i = 0; i < 12; i++) {
			if (i != 0 && i % 2 == 0)
				macAddress += "-";

			srand(time(NULL));
			Sleep(300);
			macAddress += IntToHex(rand() % 16);
		}
	return macAddress;
}

static String^ spoofHWID( String^ mac) {
	String^ hwid = "";
		mac = mac->Replace("-", "");
		hwid += mac;
		hwid += "_";
		for (int i = 0; i < 8; i++) {
			srand(time(NULL));
			hwid += IntToHex(rand() % 16);	
		}
			

	return hwid;
}
static void writeBytesM(String^% packet, array<BYTE>^ bytes) {
	for each (BYTE byte in bytes) packet += byte.ToString("X2") + " ";
}

 static void writeStringM(String^% packet, String^ str) {
	writeBytes(packet, System::Text::Encoding::UTF8->GetBytes(str));
}

void writeIntM(String^% packet, int num) {
	 writeByte(packet, (BYTE)num);
 }

static std::vector<char> HexToBytes(const std::string& hex) {
	std::vector<char> bytes;

	for (unsigned int i = 0; i < hex.length(); i += 2) {
		std::string byteString = hex.substr(i, 2);
		char byte = (char)strtol(byteString.c_str(), NULL, 16);
		bytes.push_back(byte);
	}

	return bytes;
}


static String^  writeSpoofedPacket() {
	//TODO: summon spoof from main.cpp
	String^ spoofedPacket = "";
	int picLength = Convert::ToUInt32(MainForm::TheInstance->tbAutoLoginPIC->Text->Length);
	String^ header = "1E 00 06 00";
	String^ pic = MainForm::TheInstance->tbAutoLoginPIC->Text; ;//todo send pic from main form by user

	//try one of them
	//String^ blabla = "** ** 0B 00 37 00";
	//try one of them
	String^ blabla = "C7 37 0B 00 37 00";

	String^ fakeWifi = spoofMAC();
	String^ space12 = "2C 20";
	//wifi
	//space12
	String^ fakeEthernet = spoofMAC();
	String^ space3 = "15 00";
	String^ fakeBlueTooth = spoofMAC();
	String^ space4 = "5F";
	String^ hwidAndBluetooth = spoofHWID(fakeBlueTooth);

	String^ loc_ign = MainForm::TheInstance->tb_charID->Text;
	std::string normalized_ign = marshal_as<std::string>(loc_ign);
	std::vector<char> loc_ignB = HexToBytes(normalized_ign);



	//pic length packet
	//writeBytesM(spoofedPacket, gcnew array<BYTE>{0x1E, 0x00, 0x06, 0x00 });
	writeBytesM(spoofedPacket, gcnew array<BYTE>{0x1E, 0x00});
	writeIntM(spoofedPacket, picLength);
	writeBytesM(spoofedPacket, gcnew array<BYTE>{0x00});
	writeStringM(spoofedPacket, pic);

	//char id from server

	writeBytesM(spoofedPacket, gcnew array<BYTE>{loc_ignB[0], loc_ignB[1]});
	writeBytesM(spoofedPacket, gcnew array<BYTE>{0x0C, 0x00, 0x11, 0x00 });
	//writeBytesM(spoofedPacket, gcnew array<BYTE>{0xC8, 0x00, 0x0C, 0x00, 0x11, 0x00 });
	writeStringM(spoofedPacket, fakeWifi);

	//writeBytesM(spoofedPacket, gcnew array<BYTE>{0x2C, 0x20 });
	//writeStringM(spoofedPacket, fakeWifi);
	//writeBytesM(spoofedPacket, gcnew array<BYTE>{0x2C, 0x20 });
	//writeStringM(spoofedPacket, fakeEthernet);
	writeBytesM(spoofedPacket, gcnew array<BYTE>{0x15, 0x00 });
	//writeStringM(spoofedPacket, fakeBlueTooth);
	//writeBytesM(spoofedPacket, gcnew array<BYTE>{0x5F});
	writeStringM(spoofedPacket, hwidAndBluetooth);
	Log::WriteLine(spoofedPacket);
	return spoofedPacket;
}
