/////////////////////////////////////////////////////////////////////////
// Message.cpp                                                         //
//                                                                     //
// Huijun Lin, Syracuse University, hlin14@syr.edu                     //
// Application: OOD Projects #3                                        //
// Platform:    Visual Studio 2013, Win 8.1 pro                        //
/////////////////////////////////////////////////////////////////////////

#include "Message.h"

// constructor
Message::Message(std::string cmd_, std::string sAddr, std::string sPort, std::string rAddr, std::string rPort, std::string filespec_)
	: command(cmd_), sendAddr(sAddr), sendPort(sPort), recvAddr(rAddr), recvPort(rPort), filespec(filespec_), numOfBody(0)
{

}

// get filespec method
std::string Message::getFilespec()
{
	return filespec;
}

// get command method
std::string Message::getCmd()
{
	return command;
}

// get sendAddr method
std::string Message::getSendAddr()
{
	return sendAddr;
}

// get RecvAddr method
std::string Message::getRecvAddr()
{
	return recvAddr;
}

// get SendPort method
std::string Message::getSendPort()
{
	return sendPort;
}

// get RecvPort method
std::string Message::getRecvPort()
{
	return recvPort;
}

std::string Message::getText()
{
	return text;
}

std::string Message::getPath()
{
	return path;
}

// get Numofbody method
int Message::getNumOfBody()
{
	return numOfBody;
}

// set command
void Message::setCmd(std::string cmd)
{
	command = cmd;
}

// set Sendaddr
void Message::setSendAddr(std::string sAddr)
{
	sendAddr = sAddr;
}

// set RecvAddr
void Message::setRecvAddr(std::string rAddr)
{
	recvAddr = rAddr;
}

// set SenderPort
void Message::setSendPort(std::string sPort)
{
	sendPort = sPort;
}

// set RecvPort
void Message::setRecvPort(std::string rPort)
{
	recvPort = rPort;
}

// set NumofBody
void Message::setNumOfBody(int num)
{
	numOfBody = num;
}

// set file spec
void Message::setFileSpec(std::string filespec_)
{
	filespec = filespec_;
}

void Message::setText(std::string text_)
{
	text = text_;
}

void Message::setPath(std::string Path_)
{
	path = Path_;
}

std::vector<std::string> Message::getWildCards()
{
	return vWildCards;
}

void Message::addWildCards(std::string wildCard)
{
	vWildCards.push_back(wildCard);
}

void Message::clearWildCards()
{
	vWildCards.clear();
}

void Message::setIsXML(bool isXML_)
{
	isXML = isXML_;
}

bool Message::getIsXML()
{
	return isXML;
}

// arrange attribute to string
std::string Message::toString()
{

	std::string all = "Command:" + command + "\n";
	all += "SendAddr:" + sendAddr + "\n";
	all += "SendPort:" + sendPort + "\n";
	all += "RecvAddr:" + recvAddr + "\n";
	all += "RecvPort:" + recvPort + "\n";
	all += "ContentLength:" + std::to_string(numOfBody);
	all += "\n";
	all += "FileSpec:" + filespec +"\n";
	all += "Path:" + path + "\n";
	all += "SearhText:" + text + "\n";
	if (isXML)
		all += "isXML:True\n";
	else
		all += "isXML:Flase\n";

	return all;
}

// parse msg from string
void Message::parseMsg(std::string msg)
{
	while (msg.find_first_of(":") != std::string::npos)
	{
		int pos = msg.find_first_of(":");
		int next = msg.find_first_of("\n");
		std::string key = msg.substr(0, pos);
		std::string value = msg.substr(pos + 1, next-pos-1);
		helpParse1(key, value);
		helpParse2(key, value);
		if (key == "isXML")
		{
			if (value == "True")
				isXML = true;
			else
				isXML = false;
			break;
		}
		msg = msg.substr(next+1); // may + 2 // need test
	}
}

void Message::helpParse1(std::string key, std::string value)
{
	if (key == "Command") command = value;
	else if (key == "SendAddr") sendAddr = value;
	else if (key == "SendPort") sendPort = value;
	else if (key == "RecvAddr") recvAddr = value;
	else if (key == "RecvPort") recvPort = value;
}

void Message::helpParse2(std::string key, std::string value)
{
	if (key == "ContentLength") numOfBody = stoi(value);
	else if (key == "FileSpec") filespec = value;
	else if (key == "Path") path = value;
	else if (key == "SearhText") text = value;
}

// put pattern into a string
std::string Message::patternsString()
{
	std::string patternsString = vWildCards[0];
	for (int i = 1; i < vWildCards.size(); i++)
	{
		patternsString += (" " + vWildCards[i]);
	}
	return patternsString;
}

// parse patterns from a string
void Message::parsePatternString(std::string patternsString)
{
	while (patternsString.find(" ") != std::string::npos)
	{
		int pos = patternsString.find(" ");
		std::string temp = patternsString.substr(0, pos);
		vWildCards.push_back(temp);
		patternsString = patternsString.substr(pos+1);
	}
	vWildCards.push_back(patternsString);
}


#ifdef TEST_MESSAGE

void main()
{
	Message msg;
	msg.setCmd("UPLOAD");
	msg.setFileSpec("./input.txt");
	msg.setText("asd");
	msg.setRecvAddr("http://localhost");
	msg.setRecvPort("8080");
	msg.setSendAddr("http://localhost");
	msg.setSendPort("8080");
	msg.setNumOfBody(40);
	std::cout << msg.toString();

	Message msg2;
	msg2.parseMsg(msg.toString());
	std::cout << msg2.toString();

	msg2.addWildCards("*.*");
	msg2.addWildCards("aaa.txt");
	msg2.addWildCards("wildcard.h");
	std::string temp = msg2.patternsString();
	std::cout << temp << "\n";
	msg.parsePatternString(temp);
	temp = msg.patternsString();
	std::cout << temp << "\n";
	std::cin.get();
}

#endif // TEST_MESSAGE
