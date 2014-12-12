
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
	//this->command("
	return true;
}

bool DMC4143::setVelocity(long velocity,int motor)
{
	// #DEBUG - convert velocity into a string then make:  "SP " + vel
	//if(motor==1)
		//this->command("SP velocity");
	return true;
}

bool DMC4143::setAcceleration(long acceleration,int motor)
{
	return true;
}

bool DMC4143::setTorque(long torque,int motor)
{
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
