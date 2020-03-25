// Drums.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include <Mmsystem.h>
#include <mmsystem.h>
#include <mciapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SerialPort.h"

using std::cout;
using std::endl;

/*Portname must contain these backslashes, and remember to
replace the following com port*/
char *port_name = "\\\\.\\COM3";

//String for incoming data
char incomingData[MAX_DATA_LENGTH];

int main()
{
	SerialPort arduino(port_name);
	if (arduino.isConnected()) cout << "Connection Established" << endl;
	else cout << "ERROR, check port name";
	//PlaySound(TEXT("K.wav"), NULL, SND_FILENAME | SND_ASYNC);
	while (arduino.isConnected()) {
		//Check if data has been read or not
		int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
		//prints out data
		if (strlen(incomingData) == 3) {
			puts(incomingData);
			
			if (incomingData[0] == '0' && incomingData[1] == '0' && incomingData[2] == '1') {
				PlaySound(TEXT("K.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			if (incomingData[0] == '0' && incomingData[1] == '1' && incomingData[2] == '0') {
				PlaySound(TEXT("S.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			if (incomingData[0] == '1' && incomingData[1] == '0' && incomingData[2] == '0') {
				PlaySound(TEXT("H.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			if (incomingData[0] == '0' && incomingData[1] == '1' && incomingData[2] == '1') {
				PlaySound(TEXT("KS.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			if (incomingData[0] == '1' && incomingData[1] == '0' && incomingData[2] == '1') {
				PlaySound(TEXT("KH.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			if (incomingData[0] == '1' && incomingData[1] == '1' && incomingData[2] == '0') {
				PlaySound(TEXT("SH.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			if (incomingData[0] == '1' && incomingData[1] == '1' && incomingData[2] == '1') {
				PlaySound(TEXT("SH.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}

			
		}
		
		//wait a bit
		//Sleep(1);
	}
}
