#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include <ros/ros.h>

#include "CM730.h"
#include "dynamixel.h"

using namespace std;

int main(int argc, char** argv)
{
	int connectError;
	CM730* board = new CM730();

	cout << "Testing behaviour of CM730 Module" << endl << endl;
	cout << "Connect to CM730" << endl;
	connectError = board->connect();

	cout << "Resumed with Error: " << connectError << endl << endl;

	cout << "Ping to CM730" << endl;
	int pingErr;
	int pingSucc;
	pingSucc = board->ping(200);

	cout << "Resumed with Err: " << pingSucc << endl << endl;

	cout << "Write on CM730 LED's" << endl;
	int writeErr1;
	writeErr1 = board->writeByte(CM730::ID_CM, CM730::P_LED_PANNEL, 0x03);
	cout << "Resumed with Error " << writeErr1 << endl << endl;

	cout << "Read LED status from CM730" << endl;
	int readErr1, readValue = 0;
	readErr1 = board->readByte(CM730::ID_CM, CM730::P_BAUD_RATE, &readValue);
	cout << "Resumed with Error: " << readErr1 << " and Value: " << readValue << endl << endl;

	sleep(1);

	cout << "Write LED's via SyncWrite" << endl;
	int syncWriteErr;
	uint8_t param[2];
	param[0] = CM730::ID_CM;
	param[1] = 0x02;
	syncWriteErr = board->syncWrite(CM730::P_LED_PANNEL, 2, 1, param);
	cout << "Resumed with Error: " << syncWriteErr << endl << endl;

	board->writeByte(CM730::ID_CM, CM730::P_DXL_POWER, 1);
	cout << endl << endl;

	sleep(1);

	std::vector<int> servos;
	servos.push_back(1);
	servos.push_back(2);
	board->setQuerySet(servos);
	cout << "And now, try the BulkRead" << endl;
	int bulkReadErr;
	vector<BRData> brdata(2);
	BRBoard brboard;
	bulkReadErr = board->bulkRead(&brdata, &brboard);
	cout << "Resumed with Error: " << bulkReadErr << endl << endl;

	cout << "BulkRead Board Infos" << endl;
	cout << "Voltage: " << dec <<(int)brboard.voltage << endl;
	cout << "LED Pannel: " << (int)brboard.ledPannel << endl;
	cout << "AccelX: " << brboard.accelX << endl;
	cout << "AccelY: " << brboard.accelY << endl;
	cout << "AccelZ: " << brboard.accelZ << endl << endl;

	cout << "------------------------------------------------------" << endl;
	cout << "Ping servo 1" << endl;
	pingErr =  board->ping(1);
	cout << "Resumed with Error: " << pingErr << endl << endl;

	cout << "Read Servo 1" << endl;
	int readErrS1, readValueS1;
	readErrS1 = board->readWord(1, DynamixelMX::P_PRESENT_POSITION_L, &readValueS1);
	cout << "Resumed with Error: " << readErrS1 << " and Value: " << dec << readValueS1 << endl << endl;

	board->writeByte(1, DynamixelMX::P_TORQUE_ENABLE, 1);



	if (pingSucc == 0 &&
	    writeErr1 == 0 &&
	    readErr1 == 0 &&
	    syncWriteErr == 0 &&
	    bulkReadErr == 0 &&
	    pingErr == 0)
		cout << "hooray!" << endl;


	return 0;
}