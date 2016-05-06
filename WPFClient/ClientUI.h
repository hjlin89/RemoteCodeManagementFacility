#ifndef CLIENTUI_H
#define CLIENTUI_H
/////////////////////////////////////////////////////////////////////////////
// ClientUI.h - Demo for CSE687 Project #4, Spring 2015                    //
// ver 1.0                                                                 //
// Author:   Huijun Lin, hlin14@syr.edu                                    //
// Language: Visual C++, Visual Studio 2013                                //
// Platform: Window 8.1, MacBook Air                                       //
// Application: Project 4, CSE687 Spring15, Syracuse University            //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package build a C++\CLI WPF application.  
*  It provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  It implement GUI of the project 4 by using C++\CLI.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, ClientMochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv WPFClient.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 28 Apr 2015
*  - design to demo RCMF GUI, first release
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;
using namespace System::Collections::Generic;


#include "../MockChannel/MockChannel.h"
#include <iostream>
#include <vector>


namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		// MockChannel references

		ISendr* pSendr_;
		IRecvr* pRecvr_;
		IMockChannel* pChann_;

		// Controls for Window
		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hSearchTab = gcnew TabItem();
		TabItem^ hUploadTab = gcnew TabItem(); // upload tab
		TabItem^ hDownloadTab = gcnew TabItem(); // download tab
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();

		//controls for download
		Grid^ hDownloadGrid = gcnew Grid();
		ScrollViewer^ hScrollViewer3 = gcnew ScrollViewer();
		StackPanel^ hStackPanel10 = gcnew StackPanel();
		Label^ hfilenamesLabel = gcnew Label();
		StackPanel^ hStackPanel11 = gcnew StackPanel();
		TextBlock^ hTextBlock3 = gcnew TextBlock();
		ListBox^ downloadListbox = gcnew ListBox();
		StackPanel^ hStackPanel12 = gcnew StackPanel();
		Label^ hClientPortdLabel = gcnew Label();
		TextBox^ hClientPortdTB = gcnew TextBox();
		Label^ hServerAddrdLAbel = gcnew Label();
		TextBox^ hServerAddrdTB = gcnew TextBox();
		Label^ hServerPortdLabel = gcnew Label();
		TextBox^ hServerPortdTB = gcnew TextBox();
		StackPanel^ hStackPanel13 = gcnew StackPanel();
		Button^ hExploreButton = gcnew Button();
		Button^ hDownLButton = gcnew Button();
		Button^ hClearButton11 = gcnew Button();
		StackPanel^ hStackPanel14 = gcnew StackPanel();
		Label^ UsageLabel = gcnew Label();

		// Controls for Search View
		Grid^ hSearchGrid = gcnew Grid();
		ScrollViewer^ hScrollViewer2 = gcnew ScrollViewer();
		TextBlock^ hTextBlock2 = gcnew TextBlock();
		TextBlock^ hNullTB = gcnew TextBlock();
		StackPanel^ hSP4Listbox = gcnew StackPanel();
		Label^ hAddresLabel = gcnew Label();
		Label^ hWildCardsLabel = gcnew Label();
		ListBox^ hListBox0 = gcnew ListBox();
		ListBox^ hListBox1 = gcnew ListBox(); 
		Grid^ hSearchGrid1 = gcnew Grid();
		StackPanel^ hStackPanel00 = gcnew StackPanel();
		StackPanel^ hStackPanel01 = gcnew StackPanel();
		StackPanel^ hStackPanel02 = gcnew StackPanel();
		Label^ hAddrPortLabel = gcnew Label();
		TextBox^ hAddrTB = gcnew TextBox();
		TextBox^ hPortTB = gcnew TextBox();
		Label^ special = gcnew Label();
		Button^ AddAddrButton = gcnew Button();
		Button^ clearAddrButton = gcnew Button();
		StackPanel^ hStackPanel03 = gcnew StackPanel();
		Label^ WildCardsLabel = gcnew Label();
		TextBox^ WildCardsTB = gcnew TextBox();
		Button^ AddWBButton = gcnew Button();
		Button^ clearWBButton = gcnew Button();
		StackPanel^ hStackPanel04 = gcnew StackPanel();
		Label^ StringsLabel = gcnew Label();
		TextBox^ StringsTB = gcnew TextBox();
		Label^ PathLabel = gcnew Label();
		TextBox^ PathTB = gcnew TextBox();
		StackPanel^ hStackPanel05 = gcnew StackPanel();
		CheckBox^ XMLCB = gcnew CheckBox();
		Label^ PortLabel = gcnew Label();
		TextBox^ PortTB = gcnew TextBox();
		Button^ SearchFileButton = gcnew Button();
		Button^ SearchTextButton = gcnew Button();
		Button^ hClearButton0 = gcnew Button();

		// Controls for Upload View
		Grid^ hUploadGrid = gcnew Grid();
		Button^ hUploadButton = gcnew Button();
		TextBox^ hFileSpec = gcnew TextBox();
		TextBox^ hServerAddr = gcnew TextBox();
		TextBox^ hServerPort = gcnew TextBox();
		TextBox^ hClientPort = gcnew TextBox();
		TextBlock^ hTextBlock0 = gcnew TextBlock();
		ScrollViewer^ hScrollViewer0 = gcnew ScrollViewer();
		StackPanel^ hStackPanel0 = gcnew StackPanel();
		StackPanel^ hStackPanel2 = gcnew StackPanel();
		StackPanel^ hStackPanel3 = gcnew StackPanel();
		Label^ hFilespecLable = gcnew Label();
		Label^ hClientPortLable = gcnew Label();
		Label^ hServerAddrLable = gcnew Label();
		Label^ hServerPortLable = gcnew Label();
		Button^ hClearButton1 = gcnew Button();


		// receive thread
		Thread^ recvThread;

	public:
		WPFCppCliDemo();
		~WPFCppCliDemo();

		void setUpStatusBar();
		void setUpTabControl();
		void setUpUpDownView();
		void setUpDownLoadView();
		void setSearchView();

		// upload event
		void uploadfile(Object^ obj, RoutedEventArgs^ args);

		// download and explore event
		void explorefile(Object^ obj, RoutedEventArgs^ args);
		void downloadfile(Object^ obj, RoutedEventArgs^ args);

		// search event
		void addServerAddrs(Object^ obj, RoutedEventArgs^ args);
		void ClearServerAddrs(Object^ obj, RoutedEventArgs^ args);
		void addWildCards(Object^ obj, RoutedEventArgs^ args);
		void clearWildCards(Object^ obj, RoutedEventArgs^ args);
		void searchFile(Object^ obj, RoutedEventArgs^ args);
		void searchText(Object^ obj, RoutedEventArgs^ args);
		void enableDownload(Object^ sender, MouseButtonEventArgs^ e);


		void addText(String^ msg);
		void addListBox(String^ msg);
		void getMessage();
		void clear(Object^ sender, RoutedEventArgs^ args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);

	private:
		List<String^>^ listServerAddrs = gcnew List<String^>();
		List<String^>^ listWildCards = gcnew List<String^>();
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str); 
		//for upload/download view
		void setTextBlock0();
		void setTextBoxes0();
		void setTextBoxes1();
		void setButtons0();
		//for search view
		void setTextBlocks1();
		void setListBlocks1();
		void setNullSpace();
		void setAddrButtons();
		void setWCButtons();
		void setOptionsContent();
		void setSearchButtons();
		// for download view
		void setTextListBlock();
		void setAddrInfo();
		void setDownLoadButton();
		void setUsage();
	};
}

#endif
