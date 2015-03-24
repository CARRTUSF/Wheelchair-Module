
#include "ConfigReader.h"
#include "SockStream.h"
#include "stringUtility.h"
#include "DMC-4143_Controller.h"

using namespace std;


client_tcpsocket DMC4143::sock;

// PUBLIC FUNCTIONS

DMC4143::DMC4143()
{
	initialized = setupSocket();
}

bool DMC4143::isInitialized()
{
	return initialized;
}

void DMC4143::stop()
{
	this->command("ST AB");
}

bool DMC4143::setPosition(long position,int motor)
{
	string command;
	if(motor == 1)
	{
		command = "DPA";
	}
	else if(motor == 2)
	{
		command = "DPB";
	}
	else
	{
		return false;
	}

	this->command(command + "=" + to_string(long double(position)));
	return true;            

}

bool DMC4143::setVelocity(long velocity,int motor)
{
	string command;
	if(motor == 1)
	{
		command = "SPA";
	}
	else if(motor == 2)
	{
		command = "SPB";
	}
	else
	{
		return false;
	}
			
	this->command(command + "=" + to_string(long double(velocity)));
	return true;
}

bool DMC4143::setAcceleration(long acceleration,int motor)
{
	string command;
	if(motor == 1)
	{
		command = "ACA";
	}
	else if(motor == 2)
	{
		command = "ACB";
	}
	else
	{
		return false;
	}
			
	this->command(command + "=" + to_string(long double(acceleration)));
	return true;
}

std::string DMC4143::command(std::string Command)
{
	char com[300];
	char ret[300];
	std::string c = Command + "\r";
	strcpy(com, c.c_str());
	commandGalil(com, ret, sizeof(ret));
	std::string ret_str(ret);
	return ret_str;
}




// PRIVATE FUNCTIONS

bool DMC4143::setupSocket()
{
	std::string IP = "192.168.1.22"; // #DEBUG - Change to wheelchair IP address

	const char * c = IP.c_str();
	sock.open(c,23);

	return DMC4143::sock.connected(); // #DEBUG - breaks if trying to setup sock while WMRA is off
	return sock.is_open();
}

int DMC4143::commandGalil(char* Command, char* Response, int ResponseSize) //returns the number of bytes read
{
	//command() sends an ASCII Command (e.g. "TPX") to the controller and retrieves a Response (e.g. "123\r\n:").
	//The size of Response should be supplied as ResponseSize so that unallocated memory is not overwritten.
	//If you statically allocate your response buffer (e.g. char buff[100]) use sizeof(buff).
	char acPartialResponse[512] = {0}; //buffer to contain partial responses (which will be concatenated together to form the final response)
	int iPartialBytesRead = 0; //number of bytes read each time through the loop
	int iTotalBytesRead = 0;   //the total number of bytes read.  Can't exceed ResponseSize.  
	Response[0] = 0; //set response to null string    
	sock.write(Command, strlen(Command));

	//keep reading until we (a) get a colon (b) get a question mark (c) fill up the callers Response buffer
	while(1)
	{
		iPartialBytesRead = sock.read(acPartialResponse, sizeof(acPartialResponse)); //read some characters

		if(iPartialBytesRead <= 0)   //nothing read, keep reading until :
			continue;
		else if(iTotalBytesRead + iPartialBytesRead > ResponseSize) //get out of the loop if we will fill up the caller's buffer, iPartialBytesRead >= 1
			break;
		else {
			strncat(Response, acPartialResponse, iPartialBytesRead); //add the partial response to the full response.  Response is null terminated
			iTotalBytesRead += iPartialBytesRead; //tally up the total number of bytes read
			if (acPartialResponse[iPartialBytesRead - 1] == ':' || acPartialResponse[iPartialBytesRead - 1] == '?') //got a colon, iPartialBytesRead >= 1
				break;
		}
	}
	return(iTotalBytesRead);
}


bool DMC4143::setDefaults()
{
	ConfigReader reader;
	reader.parseFile("settings_wheelchair_controller.conf");
	reader.setSection("MOTOR_CONTROLLER_DEFAULTS");

	//if(reader.keyPresent("encoderPerRevolution1")){			
	//	enc2Radian[0] = 2*M_PI/reader.getInt("encoderPerRevolution1"); //calculate conversion values
	//	radian2Enc[0] = 1/enc2Radian[0];
	//}
	//else{
	//	cout << "'encoderPerRevolution1' default not found" << endl;			
	//	return 0;
	//}
	//if(reader.keyPresent("encoderPerRevolution2")){
	//	enc2Radian[1] = 2*M_PI/reader.getInt("encoderPerRevolution2"); //calculate conversion values
	//	radian2Enc[1] = 1/enc2Radian[1];
	//}
	//else{
	//	cout << "'encoderPerRevolution2' default not found" << endl;			
	//	return 0;
	//}
	//if(reader.keyPresent("encoderPerRevolution3"))
	//{
	//	enc2Radian[2] = 2*M_PI/reader.getInt("encoderPerRevolution3"); //calculate conversion values
	//	radian2Enc[2] = 1/enc2Radian[2];
	//}
	//else
	//{
	//	cout << "'encoderPerRevolution3' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("encoderPerRevolution4"))
	//{
	//	enc2Radian[3] = 2*M_PI/reader.getInt("encoderPerRevolution4"); //calculate conversion values
	//	radian2Enc[3] = 1/enc2Radian[3];
	//}
	//else
	//{
	//	cout << "'encoderPerRevolution4' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("encoderPerRevolution5"))
	//{
	//	enc2Radian[4] = 2*M_PI/reader.getInt("encoderPerRevolution5"); //calculate conversion values
	//	radian2Enc[4] = 1/enc2Radian[4];
	//}
	//else
	//{
	//	cout << "'encoderPerRevolution5' default not found" << endl;
	//	return 0;
	//}

	//if(reader.keyPresent("encoderPerRevolution6"))
	//{
	//	enc2Radian[5] = -2*M_PI/reader.getInt("encoderPerRevolution6"); //calculate conversion values
	//	radian2Enc[5] = 1/enc2Radian[5];
	//}
	//else
	//{
	//	cout << "'encoderPerRevolution6' default not found" << endl;
	//	return 0;
	//}

	//if(reader.keyPresent("encoderPerRevolution7"))
	//{
	//	enc2Radian[6] = 2*M_PI/reader.getInt("encoderPerRevolution7"); //calculate conversion values
	//	radian2Enc[6] = 1/enc2Radian[6];
	//}
	//else
	//{
	//	cout << "'encoderPerRevolution7' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("encoderPerRevolution8")) //#Debug: encoderPerRevolution8 value is incorrect
	//{
	//	enc2Radian[7] = 2*M_PI/reader.getInt("encoderPerRevolution8"); //calculate conversion values
	//	radian2Enc[7] = 1/enc2Radian[7];
	//}
	//else
	//{
	//	cout << "'encoderPerRevolution8' default not found" << endl;
	//	return 0;
	//}

	//// set motor type to brushed motor
	//if(reader.keyPresent("brushedMotor1"))
	//	brushedMotors[0] = reader.getInt("brushedMotor1");
	//else
	//{
	//	cout << "'brushedMotor1' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("brushedMotor2"))
	//	brushedMotors[1] = reader.getInt("brushedMotor2"); 
	//else
	//{
	//	cout << "'brushedMotor2' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("brushedMotor3"))
	//	brushedMotors[2] = reader.getInt("brushedMotor3");
	//else
	//{
	//	cout << "'brushedMotor3' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("brushedMotor4"))
	//	brushedMotors[3] = reader.getInt("brushedMotor4"); 
	//else
	//{
	//	cout << "'brushedMotor4' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("brushedMotor5"))
	//	brushedMotors[4] = reader.getInt("brushedMotor5"); 
	//else
	//{
	//	cout << "'brushedMotor5' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("brushedMotor6"))
	//	brushedMotors[5] = reader.getInt("brushedMotor6"); 
	//else
	//{
	//	cout << "'brushedMotor6' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("brushedMotor7"))
	//	brushedMotors[6] = reader.getInt("brushedMotor7"); 
	//else
	//{
	//	cout << "'brushedMotor7' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("brushedMotor8"))
	//	brushedMotors[7] = reader.getInt("brushedMotor8"); 
	//else
	//{
	//	cout << "'brushedMotor8' default not found" << endl;
	//	return 0;
	//}

	////set all motors to position tracking mode
	//if(reader.keyPresent("motorMode"))
	//	motorMode = LINEAR;  // DEBUG - Needs to read from settings file
	////motorMode = reader.getInt("motorMode");
	////motorMode = reader.getString("motorMode");
	//else
	//{
	//	cout << "'motorMode' default not found" << endl;
	//	return 0;
	//}

	//// Set Max Velocity
	//if(reader.keyPresent("maxVelocity1"))
	//	motorVelocity[0] = reader.getDouble("maxVelocity1"); 
	//else
	//{
	//	cout << "'maxVelocity1' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("maxVelocity2"))
	//	motorVelocity[1] = reader.getDouble("maxVelocity2"); 
	//else
	//{
	//	cout << "'maxVelocity2' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("maxVelocity3"))
	//	motorVelocity[2] = reader.getDouble("maxVelocity3"); 
	//else
	//{
	//	cout << "'maxVelocity3' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("maxVelocity4"))
	//	motorVelocity[3] = reader.getDouble("maxVelocity4"); 
	//else
	//{
	//	cout << "'maxVelocity4' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("maxVelocity5"))
	//	motorVelocity[4] = reader.getDouble("maxVelocity5"); 
	//else		
	//{
	//	cout << "'maxVelocity5' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("maxVelocity6"))
	//	motorVelocity[5] = reader.getDouble("maxVelocity6"); 
	//else
	//{
	//	cout << "'maxVelocity6' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("maxVelocity7"))
	//	motorVelocity[6] = reader.getDouble("maxVelocity7"); 
	//else
	//{
	//	cout << "'maxVelocity7' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("maxVelocity8"))
	//	motorVelocity[7] = reader.getDouble("maxVelocity8"); 
	//else
	//{
	//	cout << "'maxVelocity8' default not found" << endl;
	//	return 0;
	//}

	////Set Acceleration
	//if(reader.keyPresent("acceleration1"))
	//	motorAccel[0] = reader.getDouble("acceleration1"); 
	//else 
	//{
	//	cout << "'acceleration1' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("acceleration2"))
	//	motorAccel[1] = reader.getDouble("acceleration2"); 
	//else
	//{
	//	cout << "'acceleration2' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("acceleration3"))
	//	motorAccel[2] = reader.getDouble("acceleration3"); 
	//else
	//{
	//	cout << "'acceleration3' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("acceleration4"))
	//	motorAccel[3] = reader.getDouble("acceleration4"); 
	//else
	//{
	//	cout << "'acceleration4' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("acceleration5"))
	//	motorAccel[4] = reader.getDouble("acceleration5"); 
	//else		
	//{
	//	cout << "'acceleration5' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("acceleration6"))
	//	motorAccel[5] = reader.getDouble("acceleration6"); 
	//else
	//{
	//	cout << "'acceleration6' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("acceleration7"))
	//	motorAccel[6] = reader.getDouble("acceleration7"); 
	//else
	//{
	//	cout << "'acceleration7' default not found" << endl;
	//	return 0;
	//}

	//if(reader.keyPresent("acceleration8"))
	//	motorAccel[7] = reader.getDouble("acceleration8"); 
	//else
	//{
	//	cout << "'acceleration8' default not found" << endl;
	//	return 0;
	//}

	////Set Decceleration
	//if(reader.keyPresent("deceleration1"))
	//	motorDecel[0] = reader.getDouble("deceleration1"); 
	//else
	//{
	//	cout << "'deceleration1' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("deceleration2"))
	//	motorDecel[1] = reader.getDouble("deceleration2"); 
	//else
	//{
	//	cout << "'deceleration2' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("deceleration3"))
	//	motorDecel[2] = reader.getDouble("deceleration3"); 
	//else
	//{
	//	cout << "'deceleration3' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("deceleration4"))
	//	motorDecel[3] = reader.getDouble("deceleration4"); 
	//else
	//{
	//	cout << "'deceleration4' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("deceleration5"))
	//	motorDecel[4] = reader.getDouble("deceleration5"); 
	//else
	//{
	//	cout << "'deceleration5' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("deceleration6"))
	//	motorDecel[5] = reader.getDouble("deceleration6"); 
	//else
	//{
	//	cout << "'deceleration6' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("deceleration7"))
	//	motorDecel[6] = reader.getDouble("deceleration7"); 
	//else
	//{
	//	cout << "'deceleration7' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("deceleration8"))
	//	motorDecel[7] = reader.getDouble("deceleration8"); 
	//else
	//{
	//	cout << "'deceleration8' default not found" << endl;
	//	return 0;
	//}

	///*set accelaration smoothing from default*/ 
	//if(reader.keyPresent("smoothing"))
	//	smoothingVal = reader.getDouble("smoothing");
	//else
	//{
	//	cout << "'smoothing' default not found" << endl;
	//	return 0;
	//}

	///*set default ready position*/
	//if(reader.keyPresent("readyPosition1"))
	//	readyPosition[0] = reader.getDouble("readyPosition1"); 
	//else
	//{
	//	cout << "'readyPosition1' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("readyPosition2"))
	//	readyPosition[1] = reader.getDouble("readyPosition2"); 
	//else
	//{
	//	cout << "'readyPosition2' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("readyPosition3"))
	//	readyPosition[2] = reader.getDouble("readyPosition3"); 
	//else
	//{
	//	cout << "'readyPosition3' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("readyPosition4"))
	//	readyPosition[3] = reader.getDouble("readyPosition4"); 
	//else
	//{
	//	cout << "'readyPosition4' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("readyPosition5"))
	//	readyPosition[4] = reader.getDouble("readyPosition5"); 
	//else
	//{
	//	cout << "'readyPosition5' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("readyPosition6"))
	//	readyPosition[5] = reader.getDouble("readyPosition6"); 
	//else
	//{
	//	cout << "'readyPosition6' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("readyPosition7"))
	//	readyPosition[6] = reader.getDouble("readyPosition7"); 
	//else
	//{
	//	cout << "'readyPosition7' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("readyPosition8"))
	//	readyPosition[7] = reader.getDouble("readyPosition8"); 
	//else
	//{
	//	cout << "'readyPosition8' default not found" << endl;
	//	return 0;
	//}

	///*set default park position*/
	//if(reader.keyPresent("parkPosition1"))
	//	parkPosition[0] = reader.getDouble("parkPosition1"); 
	//else
	//{
	//	cout << "'parkPosition1' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("parkPosition2"))
	//	parkPosition[1] = reader.getDouble("parkPosition2"); 
	//else
	//{
	//	cout << "'parkPosition2' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("parkPosition3"))
	//	parkPosition[2] = reader.getDouble("parkPosition3"); 
	//else
	//{
	//	cout << "'parkPosition3' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("parkPosition4"))
	//	parkPosition[3] = reader.getDouble("parkPosition4"); 
	//else
	//{
	//	cout << "'parkPosition4' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("parkPosition5"))
	//	parkPosition[4] = reader.getDouble("parkPosition5"); 
	//else
	//{
	//	cout << "'parkPosition5' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("parkPosition6"))
	//	parkPosition[5] = reader.getDouble("parkPosition6"); 
	//else
	//{
	//	cout << "'parkPosition6' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("parkPosition7"))
	//	parkPosition[6] = reader.getDouble("parkPosition7"); 
	//else
	//{
	//	cout << "'parkPosition7' default not found" << endl;
	//	return 0;
	//}
	//if(reader.keyPresent("parkPosition8"))
	//	parkPosition[7] = reader.getDouble("parkPosition8"); 
	//else
	//{
	//	cout << "'parkPosition8' default not found" << endl;
	//	return 0;
	//}

	return 1;
}