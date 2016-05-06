/////////////////////////////////////////////////////////////////////////////
// ClientMockChannel.cpp - Demo for CSE687 Project #4, Spring 2015         //
// ver 1.0                                                                 //
// Author:   Huijun Lin, hlin14@syr.edu                                    //
// Language: Visual C++, Visual Studio 2013                                //
// Platform: Window 8.1, MacBook Air                                       //
// Application: Project 4, CSE687 Spring15, Syracuse University            //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  Class: Sendr
*  Description: implement Sendr by ISender, hold a queue to store msg
*
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
*	Class: Rcvr
*	Description: implement Rcvr by IRcvr, hold a queue to store msg
*
*	class Recvr : public IRecvr
*	{
*	public:
*		std::string getMessage();
*		BlockingQueue<std::string>& queue();
*	private:
*		BlockingQueue<std::string> recvQ_;
*	};
*
*	Class: ClientMockChannel.cpp
*	Description: implement the client channel for handleing the operation by User interface
*
*	class ClientMockChannel : public IMockChannel
*	{
*	public:
*		ClientMockChannel(ISendr* pSendr, IRecvr* pRecvr);
*		void start();
*		void stop();
*	private:
*		void parseMsg(Message msg);
*		void uploadMsg(Message msg);
*		void DownLoadMsg(Message msg, std::string filespec);
*		void searchFileMsg(Message msg);
*		void searchTextMsg(Message msg);
*		void exploreMsg(Message msg);
*		std::string ItoS(long i);
*		std::thread thread_;
*		ISendr* pISendr_;
*		IRecvr* pIRecvr_;
*		bool stop_ = false;
*	};
*
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
*	Class ClientMockChannel:
*		ClientMockChannel clientMockChannel(pSendr, pRecvr);
*		clientMockChannel.start();
*		clientMockChannel.stop();
*
*  Required Files:
*  ---------------
*  Message.h, Message.cpp Cpp11-BlockingQueue.h Cpp11-BlockingQueue.cpp FileSystem.h FileSystem.cpp
*  Sockets.h Sockets.cpp WindowsHelpers.h WindowsHelpers.cpp AppHelpers.h AppHelpers.cpp
*  MockChannel.h ClientMockChannel.cpp
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 28 Apr 15
*  - first release
*/


#define IN_DLL
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include "../Sockets/Sockets/Sockets.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include <string>
#include <thread>
#include <iostream>
#include <chrono>

using BQueue = BlockingQueue < Message >;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
	void postMessage(const Message& msg);
	BQueue& queue();
	bool connect(std::string addr, size_t port);
	bool shutdown();
	SocketConnecter& sc();
private:
	SocketSystem ss;
	SocketConnecter sc_;
	BQueue sendQ_;
};

void Sendr::postMessage(const Message& msg)
{
	sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

SocketConnecter& Sendr::sc(){ return sc_;}

// connect method to a addr and port
bool Sendr::connect(std::string addr, size_t port)
{
	while (!sc_.connect(addr, port))
	{
		std::cout << "\nsender waiting to connect\n";
		::Sleep(100);
	}
	std::cout << "\nSender connect success: " << addr << " " << port << "\n";
	return true;
}

// shut down the sender
bool Sendr::shutdown()
{
	std::cout << "\nSender is shutting down.\n";
	return sc_.shutDown();
}

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
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class ClientMockChannel : public IMockChannel
{
public:
	ClientMockChannel(ISendr* pSendr, IRecvr* pRecvr);
	void start();
	void stop();

private:
	void parseMsg(Message msg);
	void uploadMsg(Message msg);
	void DownLoadMsg(Message msg, std::string filespec);
	void searchFileMsg(Message msg);
	void searchTextMsg(Message msg);
	void exploreMsg(Message msg);
	std::string ItoS(long i);
	std::thread thread_;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

ClientMockChannel::ClientMockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void ClientMockChannel::start()
{
  std::cout << "\n  MockChannel starting up";
  thread_ = std::thread(
    [this] {
    Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
    Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
    if (pSendr == nullptr || pRecvr == nullptr)
    {
      std::cout << "\n  failed to start Mock Channel\n\n";
      return;
    }
    BQueue& sendQ = pSendr->queue();
    BlockingQueue<std::string>& recvQ = pRecvr->queue();
    while (!stop_)
    {
      std::cout << "\n  channel deQing message";
      Message msg = sendQ.deQ();  // will block here so send quit message when stopping
	  parseMsg(msg);
	  std::cout << "\n  channel enQing message";
    }
    std::cout << "\n  Server stopping\n\n";
  });
}
//----< signal server thread to stop >---------------------------------------

void ClientMockChannel::stop() { stop_ = true; }

//----<helper func >---------------------------------------

std::string ClientMockChannel::ItoS(long i)
{
	std::string s;
	std::ostringstream oss;
	oss << i;
	s = oss.str();
	return s;
}

//----< parsemsg >---------------------------------------

void ClientMockChannel::parseMsg(Message msg)
{
	Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
	Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);

	std::string cmd = msg.getCmd();
	pRecvr->queue().enQ(msg.toString());

	SocketConnecter& sc = pSendr->sc();
	pSendr->connect(msg.getRecvAddr(), std::stoi(msg.getRecvPort()));

	if (cmd == "UPLOAD")
		uploadMsg(msg);
	else if (cmd == "DOWNLOAD")
	{
		std::string filespec = msg.getFilespec();
		int pos = filespec.find_last_of("/");
		if(pos>-1) filespec = filespec.substr(pos);
		DownLoadMsg(msg, filespec);
	}
	else if (cmd == "SEARCHFILE")
		searchFileMsg(msg);
	else if (cmd == "SEARCHTEXT")
		searchTextMsg(msg);
	else if (cmd == "EXPLORE")
		exploreMsg(msg);
	pSendr->shutdown();
}

//----< Explore Msg Handling >---------------------------------------

void ClientMockChannel::exploreMsg(Message msg)
{
	auto start = std::chrono::high_resolution_clock::now();

	std::cout << "\n\n==========MSG INFO==========\n";
	std::cout << msg.toString();
	std::cout << "\n";

	Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
	Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);

	SocketConnecter& sc = pSendr->sc();

	sc.sendString(msg.toString());
	msg.addWildCards("*.*");
	sc.sendString(msg.patternsString());

	while (true)
	{
		std::string temp1 = sc.recvString();
		/// put information into recv queue
		if (temp1 == "quit") break;
		pRecvr->queue().enQ(temp1);
		if (sc == INVALID_SOCKET)
		{
			pRecvr->queue().enQ("Explore failed:  ");
			return;
		}

	}
	pRecvr->queue().enQ("Explore in " + msg.getRecvAddr() + ":" + msg.getRecvPort() + " is done.");
	auto end = std::chrono::high_resolution_clock::now();
	auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	pRecvr->queue().enQ("Explore took " + ItoS(t) + " miliseconds.");
}

//----< Search file Msg Handling >---------------------------------------


void ClientMockChannel::searchFileMsg(Message msg)
{
	auto start = std::chrono::high_resolution_clock::now();

	std::cout << "\n\n==========MSG INFO==========\n";
	std::cout << msg.toString();
	std::cout << "\n";

	Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
	Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);

	SocketConnecter& sc = pSendr->sc();

	sc.sendString(msg.toString());
	sc.sendString(msg.patternsString());

	while (true)
	{
		std::string temp1 = sc.recvString();
		/// put information into recv queue
		if (temp1 == "quit") break;
		pRecvr->queue().enQ(temp1);
		if (sc == INVALID_SOCKET)
		{
			pRecvr->queue().enQ("SearchFile failed:  ");
			return;
		}

	}
	pRecvr->queue().enQ("FileSearch in " + msg.getRecvAddr() + ":" + msg.getRecvPort() +" is done.");
	auto end = std::chrono::high_resolution_clock::now();
	auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	pRecvr->queue().enQ("FileSearch took " + ItoS(t) + " miliseconds.");
}

//----< Search Text Msg Handling >---------------------------------------

void ClientMockChannel::searchTextMsg(Message msg)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "\n\n==========MSG INFO==========\n";
	std::cout << msg.toString();
	std::cout << "\n";
	Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
	Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);

	SocketConnecter& sc = pSendr->sc();

	sc.sendString(msg.toString());
	sc.sendString(msg.patternsString());

	while (true)
	{
		std::string temp1 = sc.recvString();
		/// put information into recv queue
		if (temp1 == "quit") break;
		pRecvr->queue().enQ(temp1);
		if (sc == INVALID_SOCKET)
		{
			pRecvr->queue().enQ("SearchText failed:  ");
			return;
		}

	}
	pRecvr->queue().enQ("SearchText in " + msg.getRecvAddr() + ":" + msg.getRecvPort() + " is done.");
	auto end = std::chrono::high_resolution_clock::now();
	auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	pRecvr->queue().enQ("SearchText took " + ItoS(t) + " miliseconds.");
}

//----< Download Msg Handling >---------------------------------------

void ClientMockChannel::DownLoadMsg(Message msg, std::string filename)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "\n\n==========MSG INFO==========\n" << msg.toString() << "\n";
	Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
	Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
	SocketConnecter& sc = pSendr->sc();
	sc.sendString(msg.toString());
	const size_t BufLen = 512; Socket::byte buffer[BufLen]; bool ok;
	filename = "./ClientRepository/" + filename;
	FileSystem::File bufferOut(filename);
	bufferOut.open(FileSystem::File::out, FileSystem::File::binary);
	if (!bufferOut.isGood())
	{
		std::cout << "\n  could not open \"" << filename << "\" for writing\n\n";
		pRecvr->queue().enQ("Could not open:  " + filename); return;
	}
	else
		std::cout << "\n  opening: \"" << filename << "\" for writing";
	while (true)
	{
		std::string temp1 = sc.recvString();
		if (temp1 == "quit") break;
		msg.parseMsg(temp1);
		ok = sc.recv(msg.getNumOfBody(), buffer);
		if (sc == INVALID_SOCKET || !ok)
		{
			pRecvr->queue().enQ("Download failed:  " + filename);
			return;
		}
		std::string temp;
		for (size_t i = 0; i < msg.getNumOfBody(); ++i)
			temp += buffer[i];
		std::cout << "\nserver recvd : " + temp;
		bufferOut.putBuffer(msg.getNumOfBody(), buffer);
		if (!bufferOut.isGood()) break;
		std::cout << "\n";
	}
	std::cout << "\n  Closing: \"" << filename << "\" for writing";
	bufferOut.close();
	std::string result = "File " + msg.getFilespec() + " has been download.";
	pRecvr->queue().enQ("Result From Server:  " + result);
	auto end = std::chrono::high_resolution_clock::now();
	auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	pRecvr->queue().enQ("Download took " + ItoS(t) + " miliseconds.");
}

//----< Upload Msg Handling >---------------------------------------

void ClientMockChannel::uploadMsg(Message msg)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "\n\n==========MSG INFO==========\n";
	std::cout << msg.toString();
	std::cout << "\n";
	Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
	Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
	SocketConnecter& sc = pSendr->sc();
	FileSystem::File bufferIn("./ClientRepository/" + msg.getFilespec());
	bufferIn.open(FileSystem::File::in, FileSystem::File::binary);
	if (!bufferIn.isGood())
	{
		std::cout << "\ncould not open \"" << msg.getFilespec() << "\" for uploading";
		pRecvr->queue().enQ("Could not open file:  " + msg.getFilespec());
		return;
	}
	else
		std::cout << "\nopening: \"" << msg.getFilespec() << "\" for uploading";
	std::cout << "\n\nStart sending.";
	sc.sendString(msg.toString());
	const size_t bufLen = 512;
	FileSystem::File::byte buffer[bufLen];
	while (true)
	{
		size_t resultSize = bufferIn.getBuffer(bufLen, buffer);
		std::string temp1(buffer, resultSize);
		std::cout << "\nsend body:" << temp1;
		if (temp1.size() == 0)
			break;
		msg.setNumOfBody(resultSize);
		sc.sendString(msg.toString());
		sc.send(resultSize, buffer);
		if (resultSize < bufLen || !bufferIn.isGood())
		{
			std::cout << "\nclose file.";
			bufferIn.close();
			break;
		}
	}
	sc.sendString("quit");
	std::string result = sc.recvString();
	std::cout << "\n\n\nResult From Server:  " << result << "\n\n";
	pRecvr->queue().enQ("Result From Server:  "+result);
	auto end = std::chrono::high_resolution_clock::now();
	auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	pRecvr->queue().enQ("Upload took " + ItoS(t) + " miliseconds.");
}

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr) 
{ 
	return new ClientMockChannel(pISendr, pIRecvr);
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
