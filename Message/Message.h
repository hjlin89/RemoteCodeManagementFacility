#ifndef MESSAGE_H
#define MESSAGE_H
/////////////////////////////////////////////////////////////////////////
// Message.h                                                           //
//                                                                     //
// Huijun Lin, Syracuse University, hlin14@syr.edu                     //
// Application: OOD Projects #3                                        //
// Platform:    Visual Studio 2013, Win 8.1 pro                        //
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  Provide a class that represent the message been transmit
*
*  Public Interface:
*  ------------------
*  Message msg;
*  msg.setCmd("UPLOAD");
*  msg.setFileSpec("./input.txt");
*  msg.setRecvAddr("http://localhost");
*  msg.setRecvPort("8080");
*  msg.setSendAddr("http://localhost");
*  msg.setSendPort("8080");
*  msg.setNumOfBody(40);
*  std::cout << msg.toString();
*  std::string cmd = msg.getCmd();
*  std::string filesepc = msg.getFileSpec();
*  std::string rAddr = msg.getRecvAddr();
*  std::string rPort = msg.getRecvPort();
*  std::string sAddr = msg.getSendAddr();
*  std::string sPort = msg.getSendPort();
*  int num = msg.getNumOfBody();
*  Message msg;
*  msg2.parseMsg(msg.toString());
*  std::cout << msg2.toString();
*
*  Required Files:
*  ---------------
*  Message.h, Message.cpp
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 09 Apr 15
*  - first release
*/

#include <vector>
#include <string>
#include <iostream>
class Message
{
public:
	Message(){ numOfBody = 0; }
	Message(std::string, std::string, std::string, std::string, std::string, std::string);
	~Message(){}
	void parseMsg(std::string msg);
	std::string patternsString();
	void parsePatternString(std::string);
	std::string getCmd();
	std::string getSendAddr();
	std::string getRecvAddr();
	std::string getSendPort();
	std::string getRecvPort();
	std::string getFilespec();
	std::string getText();
	std::string getPath();
	int getNumOfBody();
	void setCmd(std::string cmd);
	void setSendAddr(std::string sAddr);
	void setRecvAddr(std::string rAddr);
	void setSendPort(std::string sPort);
	void setRecvPort(std::string rPort);
	void setFileSpec(std::string filespec_);
	void setText(std::string text_);
	void setPath(std::string Path_);
	void setNumOfBody(int num);
	std::string toString();
	void setIsXML(bool isXML_);
	bool getIsXML();


	std::vector<std::string> getWildCards();
	void addWildCards(std::string);
	void clearWildCards();

private:
	void helpParse1(std::string key, std::string value);
	void helpParse2(std::string key, std::string value);
	bool isXML;
	std::string command;
	std::string sendAddr;
	std::string recvAddr;
	std::string sendPort;
	std::string recvPort;
	std::string filespec;
	std::string text;
	std::string path;
	std::vector<std::string> vWildCards;
	int numOfBody;
};

#endif