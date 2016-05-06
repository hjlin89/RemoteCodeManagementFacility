/////////////////////////////////////////////////////////////////////////////
// ServerMockChannel.cpp - Demo for CSE687 Project #4, Spring 2015         //
// ver 1.0                                                                 //
// Author:   Huijun Lin, hlin14@syr.edu                                    //
// Language: Visual C++, Visual Studio 2013                                //
// Platform: Window 8.1, MacBook Air                                       //
// Application: Project 4, CSE687 Spring15, Syracuse University            //
/////////////////////////////////////////////////////////////////////////////
/**
*  Package Operations:
*  ------------------------------------------------------------------
*  Class: Sendr
*  Description: implement Sendr by ISender, hold a queue to store msg
*  class Sendr : public ISendr
*  {
*   public:
*	  void postMessage(const Message& msg);
*	  BQueue& queue();
*	  bool connect(std::string addr, size_t port);
*	  bool shutdown();
*	  SocketConnecter& sc();
*   private:
*	  SocketSystem ss;
*	  SocketConnecter sc_;
*	  BQueue sendQ_;
*  };
*
*   ------------------------------------------------------------------
*	Class: Rcvr
*	Description: implement Rcvr by IRcvr, hold a queue to store msg
*	class Recvr : public IRecvr
*	{
*	public:
*		std::string getMessage();
*		BlockingQueue<std::string>& queue();
*	private:
*		BlockingQueue<std::string> recvQ_;
*	};
*
*   ------------------------------------------------------------
*	Class ServerMockChannel:
*   Description: implement IMockChannel to work as a server
*	class ServerMockChannel : public IMockChannel
*	{
*	public:
*		ServerMockChannel(ISendr* pSendr, IRecvr* pRecvr);
*		void start();
*		void stop();
*		void setPort(int port);
*	private:
*		SocketSystem ss;
*		SocketListener* sl;
*		ClientHandler ch;
*		std::thread thread_;
*		ISendr* pISendr_;
*		IRecvr* pIRecvr_;
*		bool stop_ = false;
*	};
*
*   ---------------------------------------------------------------
*	class ClientHandler:
*	Description: callable class to handle msg incoming from client
*   
*	class ClientHandler
*	{
*	public:
*		void operator()(Socket& socket_);
*		bool upLoadHandling(Socket& socket_, std::string filespec);
*		bool downLoadHandling(Socket& socket_, Message msg);
*		bool searchFileHandling(Socket& socket_, Message msg);
*		bool searchTextHandling(Socket& socket_, Message msg);
*		bool exploreHandling(Socket& socket_, Message msg);
*	};
*  Public Interface:
*  ------------------
*	Class Sendr:
*		Sendr sender;
*		sendr.postMessage(msg);
*		BQ =  sendr.queue();
*		bool isconnect = sendr.connect(addr,port);
*	    bool isshutdown = sendr.shutdown();
*	    SocketConnecter sc = sendr.sc();
*
*	Class Recvr:
*		Recvr rcvr;
*		std::string msg = rcvr.getMessage();
*		BQ = rcvr.queue();
*
*	Class ServerMockChannel:
*		ServerMockChannel serverMockChannel(pSendr, pRecvr);
*		serverMockChannel.start();
*		serverMockChannel.stop();
*
*	class ClientHandler:
*		ClientHandler ch;
*		ch(socket_);
*		ch.upLoadHandling(socket_, filespec);
*		ch.downLoadHandling(socket_, msg);
*		ch.searchFileHandling(socket_, msg);
*		ch.searchTextHandling(socket_, msg);
*		ch.exploreHandling(socket_, msg);
*
*  Required Files:
*  ---------------
*  Message.h, Message.cpp Cpp11-BlockingQueue.h Cpp11-BlockingQueue.cpp FileSystem.h FileSystem.cpp FileData.h FileData.cpp
*  Sockets.h Sockets.cpp WindowsHelpers.h WindowsHelpers.cpp AppHelpers.h AppHelpers.cpp FileMgr.h FileMgr.cpp
*  MockChannel.h ServerMockChannel.cpp
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 28 Apr 15
*  - first release
*/

#define IN_DLL
#include "../MockChannel/MockChannel.h"
#include "../MockChannel/Cpp11-BlockingQueue.h"
#include "../Sockets/Sockets/Sockets.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../FIleMgr/FileMgr.h"
#include <string>
#include <thread>
#include <iostream>

using BQueue = BlockingQueue <Message>;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
	void postMessage(const Message& msg);
	BQueue& queue();
private:
	BQueue sendQ_;
};

void Sendr::postMessage(const Message& msg)
{
	sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	std::string getMessage();
	BlockingQueue<std::string>& queue();
private:
	BlockingQueue<std::string> recvQ_;
};

std::string Recvr::getMessage()
{
	return recvQ_.deQ();
}

BlockingQueue<std::string>& Recvr::queue()
{
	return recvQ_;
}

/////////////////////////////////////////////////////////////////////////////
// ClientHandler class
// - handler the msg accepted by server
//
class ClientHandler
{
public:
	void operator()(Socket& socket_);
	bool upLoadHandling(Socket& socket_, std::string filespec);
	bool downLoadHandling(Socket& socket_, Message msg);
	bool searchFileHandling(Socket& socket_, Message msg);
	bool searchTextHandling(Socket& socket_, Message msg);
	bool exploreHandling(Socket& socket_, Message msg);
};

// clienthandler for process the receiver child thread
void ClientHandler::operator()(Socket& socket_)
{
	while (true)
	{
		std::string header = socket_.recvString();
		if (header.size() == 0)
			break;
		Message msg;
		msg.parseMsg(header);
		if (msg.getCmd() == "UPLOAD")
		{
			upLoadHandling(socket_, msg.getFilespec());
		}
		else if (msg.getCmd() == "DOWNLOAD")
		{
			downLoadHandling(socket_, msg);
		}
		else if (msg.getCmd() == "SEARCHFILE")
		{
			searchFileHandling(socket_, msg);
		}
		else if (msg.getCmd() == "SEARCHTEXT")
		{
			searchTextHandling(socket_, msg);
		}
		else if (msg.getCmd() == "EXPLORE")
		{
			exploreHandling(socket_, msg);
		}
	}
	socket_.shutDown();
	socket_.close();
}

// explore Msg
bool ClientHandler::exploreHandling(Socket& socket_, Message msg)
{
	std::string patternsString = socket_.recvString();
	msg.parsePatternString(patternsString);
	std::vector<std::string> wildCards = msg.getWildCards();
	std::string path = msg.getPath();
	FileData fd;
	FileMgr fm(fd);
	fm.setPatterns(wildCards);
	fm.setRecursive(true);
	fd.root = XmlProcessing::makeTaggedElement("ServerRepository/" + path);
	fm.find("ServerRepository/" + path, fd.root);
	for (auto elem : fd)
	{
		for (auto item : elem.second)
			socket_.sendString( "<filefordownload>" + *item + elem.first);
	}
	socket_.sendString("quit");
	return true;
}

// Search File Msg
bool ClientHandler::searchFileHandling(Socket& socket_, Message msg)
{
	std::string patternsString = socket_.recvString();
	msg.parsePatternString(patternsString);
	std::vector<std::string> wildCards = msg.getWildCards();
	std::string path = msg.getPath();

	FileData fd;
	FileMgr fm(fd);
	fm.setPatterns(wildCards);
	fm.setRecursive(true);
	fd.root = XmlProcessing::makeTaggedElement("ServerRepository/" + path);
	fm.find("ServerRepository/" + path, fd.root);

	if (msg.getIsXML())
	{
		socket_.sendString(fd.root->toString());
	}
	else
	{
		for (auto elem : fd)
		{
			for (auto item : elem.second)
				socket_.sendString(msg.getRecvAddr()+":"+msg.getRecvPort()+"/"+ *item + elem.first);
		}
	}
	socket_.sendString("quit");
	return true;
}

// Search Text Msg handling
bool ClientHandler::searchTextHandling(Socket& socket_, Message msg)
{
	std::string patternsString = socket_.recvString();
	msg.parsePatternString(patternsString);
	std::vector<std::string> wildCards = msg.getWildCards();
	std::string path = msg.getPath();
	std::string text = msg.getText();
	
	FileData fd;
	FileMgr fm(fd);
	fm.setPatterns(wildCards);
	fm.setRecursive(true);
	fd.root = XmlProcessing::makeTaggedElement("ServerRepository/" + path);
	fm.findText("ServerRepository/" + path, fd.root, text);
	if (msg.getIsXML())
	{
		socket_.sendString(fd.root->toString());
	}
	else
	{
		for (auto elem : fd)
		{
			for (auto item : elem.second)
			{
				std::string file_ = *item + elem.first;
				std::string info = msg.getRecvAddr() + ":" + msg.getRecvPort() + "/" + file_;
				socket_.sendString(info);
			}
		}
	}
	socket_.sendString("quit");

	return true;
}


// handle Download Msg
bool ClientHandler::downLoadHandling(Socket& socket_, Message msg)
{

	std::string filename = "./ServerRepository/" + msg.getFilespec();
	FileSystem::File bufferIn(filename);
	bufferIn.open(FileSystem::File::in, FileSystem::File::binary);
	if (!bufferIn.isGood())
	{
		std::cout << "\ncould not open \"" << filename << "\" for downloading";
		return false;
	}
	else
		std::cout << "\nopening: \"" << filename << "\" for uploading";
	std::cout << "\n\nStart sending.";

	const size_t bufLen = 512;
	FileSystem::File::byte buffer[bufLen];
	while (true)
	{
		size_t resultSize = bufferIn.getBuffer(bufLen, buffer);
		std::string temp1(buffer, resultSize);
		std::cout << "\nsend body:" << temp1;
		//bufferOut.putBuffer(resultSize, buffer);
		msg.setNumOfBody(resultSize);
		socket_.sendString(msg.toString());
		socket_.send(resultSize, buffer);
		if (resultSize < bufLen || !bufferIn.isGood())
		{
			std::cout << "\nclose file.";
			bufferIn.close();
			break;
		}
	}
	socket_.sendString("quit");

	std::string succ = "File " + msg.getFilespec() + " has been download.";
	return true;
}


// handle upload Msg
bool ClientHandler::upLoadHandling(Socket& socket_, std::string filespec)
{
	Message msg;
	const size_t BufLen = 512;
	Socket::byte buffer[BufLen];
	bool ok;
	filespec = "./ServerRepository/" + filespec;
	FileSystem::File bufferOut(filespec);
	bufferOut.open(FileSystem::File::out, FileSystem::File::binary);
	if (!bufferOut.isGood())
	{
		std::cout << "\n  could not open \"" << filespec << "\" for writing\n\n";
		return false;
	}
	else
	{
		std::cout << "\n  opening: \"" << filespec << "\" for writing";
	}
	std::cout << "\n";
	while (true)
	{
		std::string temp1 = socket_.recvString();
		if (temp1 == "quit") break;
		msg.parseMsg(temp1);
		ok = socket_.recv(msg.getNumOfBody(), buffer);
		if (socket_ == INVALID_SOCKET || !ok)
			return false;
		std::string temp;
		for (size_t i = 0; i < msg.getNumOfBody(); ++i)
			temp += buffer[i];
		//Verbose::show("bytes recvd at server: " + toString(BufLen));
		std::cout << "\nserver recvd : " + temp;
		bufferOut.putBuffer(msg.getNumOfBody(), buffer);
		if (!bufferOut.isGood())
			break;
		std::cout << "\n";
	}
	std::string succ = "File " + msg.getFilespec() + " has been uploaded.";
	std::cout << "\n  Closing: \"" << filespec << "\" for writing";
	bufferOut.close();
	std::cout << "\n  Reply to sender: " << succ << "\n";
	socket_.sendString(succ);
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class ServerMockChannel : public IMockChannel
{
public:
	ServerMockChannel(ISendr* pSendr, IRecvr* pRecvr);
	void start();
	void stop();
	void setPort(int port);
private:
	SocketSystem ss;
	SocketListener* sl;
	ClientHandler ch;
	std::thread thread_;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

ServerMockChannel::ServerMockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) { }

void ServerMockChannel::setPort(int port)
{
	SocketListener* sl_ = new SocketListener(port);
	sl = sl_;
}


//----< creates thread to read from sendQ and echo back to the recvQ >-------

void ServerMockChannel::start()
{
	Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
	Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
	std::cout << "\n  MockChannel starting up";

	bool issucc = sl->start(ch);
	if (!issucc)	std::cout << "\n  Start failed";
	//if (issucc)	std::cout << "\n  Start Success";
}
//----< signal server thread to stop >---------------------------------------

void ServerMockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr)
{
	return new ServerMockChannel(pISendr, pIRecvr);
}

int main(int argc, char** argv)
{
	std::cout << "\n==========Server Port: " << argv[1] << " =============\n\n";
	ObjectFactory objFact;
	ISendr* pSendr = objFact.createSendr();
	IRecvr* pRecvr = objFact.createRecvr();
	ServerMockChannel* pMockChannel = dynamic_cast<ServerMockChannel*>(objFact.createMockChannel(pSendr, pRecvr));
	Verbose v(true);
	pMockChannel->setPort(std::stoi(argv[1]));
	pMockChannel->start();
	//pMockChannel->stop();
	std::cin.get();
	Message msg;
	msg.setCmd("quit");
	pSendr->postMessage(msg);
}

