#ifndef DMC_4143_CONTROLLER_H
#define DMC_4143_CONTROLLER_H

#define _USE_MATH_DEFINES  // for M_PI
#include <math.h>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <exception>
#include <vector>
#include <ctype.h>

class client_tcpsocket;

using namespace std;

class DMC4143 {
public:
	DMC4143();
	bool isInitialized();
	void stop();
	std::string command(std::string Command); // user command structure, used by MotorController
private:
	bool initialized;
	int commandGalil(char* Command, char* Response, int ResponseSize); // Galil Controller command structure, used by command()
	static client_tcpsocket sock; // The socket class used to communicate with galil controller
	bool setupSocket();
};

#endif;