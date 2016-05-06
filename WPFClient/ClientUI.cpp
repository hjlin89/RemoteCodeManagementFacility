/////////////////////////////////////////////////////////////////////////////
// ClientUI.cpp - Demo for CSE687 Project #4, Spring 2015                  //
// ver 1.0                                                                 //
// Author:   Huijun Lin, hlin14@syr.edu                                    //
// Language: Visual C++, Visual Studio 2013                                //
// Platform: Window 8.1, MacBook Air                                       //
// Application: Project 4, CSE687 Spring15, Syracuse University            //
/////////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "ClientUI.h"
using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo()
{
	// set up channel
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
	pChann_->start();
	delete pObjFact;
	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
	// set event handlers
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hClearButton0->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	hClearButton1->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	hClearButton11->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	// upload event
	hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::uploadfile);
	// search event
	AddAddrButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::addServerAddrs);
	clearAddrButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::ClearServerAddrs);
	AddWBButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::addWildCards);
	clearWBButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearWildCards);
	SearchFileButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchFile);
	SearchTextButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchText);
	//download event
	hExploreButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::explorefile);
	hDownLButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::downloadfile);
	downloadListbox->MouseDoubleClick += gcnew MouseButtonEventHandler(this, &WPFCppCliDemo::enableDownload);
	// set Window properties
	this->Title = "Remote Code Management Facility Client";
	this->Width = 800;
	this->Height = 635;
	// attach dock panel to Window
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	// setup Window controls and views
	setUpTabControl();
	setUpStatusBar();
	setSearchView();
	setUpUpDownView();
	setUpDownLoadView();	
}

WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}

// set status bar
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

// set tab control
void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hSearchTab->Header = "Search";
	hUploadTab->Header = "UpLoad";
	hDownloadTab->Header = "Explore/DownLoad";
	hTabControl->Items->Add(hUploadTab);
	hTabControl->Items->Add(hDownloadTab);
	hTabControl->Items->Add(hSearchTab);
}

//set tab view of upload
void WPFCppCliDemo::setUpDownLoadView()
{
	Console::Write("\n  setting up upload/download view");
	hDownloadGrid->Margin = Thickness(20);
	hDownloadTab->Content = hDownloadGrid;

	setTextListBlock();
	setAddrInfo();
	setDownLoadButton();
	setUsage();
}

// set text list block in upload view
void WPFCppCliDemo::setTextListBlock()
{
	RowDefinition^ hRow0Def = gcnew RowDefinition();
	hDownloadGrid->RowDefinitions->Add(hRow0Def);
	hRow0Def->Height = GridLength(32);
	hfilenamesLabel->Content = "Files in Server";
	hfilenamesLabel->FontWeight = FontWeights::Bold;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 150;
	hStackPanel10->Children->Add(hfilenamesLabel);
	hStackPanel10->Children->Add(hSpacer1);
	hStackPanel10->Orientation = Orientation::Horizontal;
	hStackPanel10->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hDownloadGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock3;
	hTextBlock3->Padding = Thickness(15);
	hTextBlock3->Text = "";
	hTextBlock3->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock3->FontWeight = FontWeights::Bold;
	hTextBlock3->FontSize = 16;
	hScrollViewer3->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer3->Content = hBorder1;
	hScrollViewer3->Width = 350;
	downloadListbox->Width = 350;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 30;
	hStackPanel11->Children->Add(hScrollViewer3);
	hStackPanel11->Children->Add(hSpacer);
	hStackPanel11->Children->Add(downloadListbox);
	hStackPanel11->Orientation = Orientation::Horizontal;
	hStackPanel11->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hDownloadGrid->SetRow(hStackPanel11, 1);
	hDownloadGrid->Children->Add(hStackPanel11);
	hDownloadGrid->SetRow(hStackPanel10, 0);
	hDownloadGrid->Children->Add(hStackPanel10);

}

// set addr info elemt in upload view
void WPFCppCliDemo::setAddrInfo()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hDownloadGrid->RowDefinitions->Add(hRowDef);
	hRowDef->Height = GridLength(20);
	RowDefinition^ hRow0Def = gcnew RowDefinition();
	hDownloadGrid->RowDefinitions->Add(hRow0Def);
	hRow0Def->Height = GridLength(30);
	hClientPortdLabel->Content = "ClientPort:";
	hServerAddrdLAbel->Content = "ServerAddr:";
	hServerPortdLabel->Content = "ServerPort:";
	hClientPortdTB->Text = "8080";
	hServerAddrdTB->Text = "localhost";
	hServerPortdTB->Text = "8081";
	Border^ hBorder0 = gcnew Border();
	hBorder0->Width = 120;
	hBorder0->Height = 30;
	hBorder0->BorderThickness = Thickness(1);
	hBorder0->BorderBrush = Brushes::Black;
	hBorder0->Child = hClientPortdTB;
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 120;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hServerAddrdTB;
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hServerPortdTB;
	hStackPanel12->Children->Add(hClientPortdLabel);
	hStackPanel12->Children->Add(hBorder0);
	hStackPanel12->Children->Add(hServerAddrdLAbel);
	hStackPanel12->Children->Add(hBorder1);
	hStackPanel12->Children->Add(hServerPortdLabel);
	hStackPanel12->Children->Add(hBorder2);
	hStackPanel12->Orientation = Orientation::Horizontal;
	hStackPanel12->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hDownloadGrid->SetRow(hStackPanel12, 3);
	hDownloadGrid->Children->Add(hStackPanel12);
}

// set down load button
void WPFCppCliDemo::setDownLoadButton()
{
	RowDefinition^ Row3Def = gcnew RowDefinition();
	Row3Def->Height = GridLength(10);
	hDownloadGrid->RowDefinitions->Add(Row3Def);
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(50);
	hDownloadGrid->RowDefinitions->Add(hRow3Def);
	hExploreButton->Content = "Explore";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hDownLButton->Content = "Download";
	hBorder2->Child = hExploreButton;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDownLButton;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 30;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 30;
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 120;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hClearButton11->Content = "Clear";
	hBorder4->Child = hClearButton11;
	hStackPanel13->Children->Add(hBorder2);
	hStackPanel13->Children->Add(hSpacer);
	hStackPanel13->Children->Add(hBorder3);
	hStackPanel13->Children->Add(hSpacer1);
	hStackPanel13->Children->Add(hBorder4);
	hStackPanel13->Orientation = Orientation::Horizontal;
	hStackPanel13->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hDownloadGrid->SetRow(hStackPanel13, 5);
	hDownloadGrid->Children->Add(hStackPanel13);
	hDownLButton->IsEnabled = false;
}

// set download usage
void WPFCppCliDemo::setUsage()
{
	RowDefinition^ Row3Def = gcnew RowDefinition();
	Row3Def->Height = GridLength(10);
	hDownloadGrid->RowDefinitions->Add(Row3Def);
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(30);
	hDownloadGrid->RowDefinitions->Add(hRow3Def);
	UsageLabel->Content = "Usage: Double Click the filespec to enable Download";
	UsageLabel->FontWeight = FontWeights::Bold;
	hStackPanel14->Children->Add(UsageLabel);
	hStackPanel14->Orientation = Orientation::Horizontal;
	hStackPanel14->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hDownloadGrid->SetRow(hStackPanel14, 7);
	hDownloadGrid->Children->Add(hStackPanel14);
}

// set Textblock in upload grid
void WPFCppCliDemo::setTextBlock0()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hUploadGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock0;
	hTextBlock0->Padding = Thickness(15);
	hTextBlock0->Text = "";
	hTextBlock0->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock0->FontWeight = FontWeights::Bold;
	hTextBlock0->FontSize = 16;
	hScrollViewer0->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer0->Content = hBorder1;
	hUploadGrid->SetRow(hScrollViewer0, 0);
	hUploadGrid->Children->Add(hScrollViewer0);
}

// set text block in search grid
void WPFCppCliDemo::setTextBlocks1()
{
	ColumnDefinition^ hRow1Def = gcnew ColumnDefinition();
	hSearchGrid->ColumnDefinitions->Add(hRow1Def);
	Border^ hBorder0 = gcnew Border();
	hBorder0->BorderThickness = Thickness(1);
	hBorder0->BorderBrush = Brushes::Black;
	hBorder0->Child = hTextBlock2;
	hTextBlock2->Padding = Thickness(15);
	hTextBlock2->Text = "";
	hTextBlock2->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock2->FontWeight = FontWeights::Bold;
	hTextBlock2->FontSize = 16;
	hScrollViewer2->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer2->Content = hBorder0;
	hSearchGrid->SetColumn(hScrollViewer2, 0);
	hSearchGrid->Children->Add(hScrollViewer2);
	ColumnDefinition^ hRow3Def = gcnew ColumnDefinition();
	hRow3Def->Width = GridLength(50);
	hSearchGrid->ColumnDefinitions->Add(hRow3Def);
}

// set list block in searchgrid
void WPFCppCliDemo::setListBlocks1()
{
	ColumnDefinition^ hRow2Def = gcnew ColumnDefinition();
	hRow2Def->Width = GridLength(275);
	hSearchGrid->ColumnDefinitions->Add(hRow2Def);
	Border^ hBorder3 = gcnew Border();
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hAddresLabel->Content = "Vector<ServerAddr>";
	hBorder3->Child = hAddresLabel;
	Border^ hBorder4 = gcnew Border();
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hWildCardsLabel->Content = "Vector<WildCards>";
	hBorder4->Child = hWildCardsLabel;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hListBox0;
	hBorder1->Height = 125;
	hListBox0->Padding = Thickness(10);
	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hListBox1;
	hBorder2->Height = 125;
	hListBox1->Padding = Thickness(10);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Height = 20;
	hBorder1->Padding = Thickness(10);
	hBorder2->Padding = Thickness(10);
	hSP4Listbox->Children->Add(hBorder3);
	hSP4Listbox->Children->Add(hBorder1);
	hSP4Listbox->Children->Add(hSpacer);
	hSP4Listbox->Children->Add(hBorder4);
	hSP4Listbox->Children->Add(hBorder2);
	hSP4Listbox->Orientation = Orientation::Vertical;
	hSearchGrid->SetColumn(hSP4Listbox, 2);
	hSearchGrid->Children->Add(hSP4Listbox);
}

// set list boxes in upload

void WPFCppCliDemo::setTextBoxes0()
{
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hRow4Def->Height = GridLength(50);
	hUploadGrid->RowDefinitions->Add(hRow4Def);
	hFileSpec->Text = "*.*";
	hClientPort->Text = "8080";
	hClientPortLable->Content = "Client Port:";
	hFilespecLable->Content = "Filespec:";
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 80;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hFilespecLable;
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hFileSpec;
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 80;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hClientPortLable;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hClientPort;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 30;
	hStackPanel2->Children->Add(hBorder1);
	hStackPanel2->Children->Add(hBorder2);
	hStackPanel2->Children->Add(hSpacer);
	hStackPanel2->Children->Add(hBorder4);
	hStackPanel2->Children->Add(hBorder3);
	hStackPanel2->Orientation = Orientation::Horizontal;
	hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hUploadGrid->SetRow(hStackPanel2, 1);
	hUploadGrid->Children->Add(hStackPanel2);
}

// set Text boxes in upload

void WPFCppCliDemo::setTextBoxes1()
{
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hRow4Def->Height = GridLength(50);
	hUploadGrid->RowDefinitions->Add(hRow4Def);
	hServerAddr->Text = "localhost";
	hServerPort->Text = "8081";
	hServerAddrLable->Content = "Server Addr:";
	hServerPortLable->Content = "Server Port:";
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 80;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hServerAddrLable;
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hServerAddr;
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 80;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hServerPortLable;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hServerPort;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 30;
	hStackPanel3->Children->Add(hBorder1);
	hStackPanel3->Children->Add(hBorder2);
	hStackPanel3->Children->Add(hSpacer);
	hStackPanel3->Children->Add(hBorder4);
	hStackPanel3->Children->Add(hBorder3);
	hStackPanel3->Orientation = Orientation::Horizontal;
	hStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hUploadGrid->SetRow(hStackPanel3, 2);
	hUploadGrid->Children->Add(hStackPanel3);
}

// set Button in upload

void WPFCppCliDemo::setButtons0()
{
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(50);
	hUploadGrid->RowDefinitions->Add(hRow3Def);
	hUploadButton->Content = "Upload File";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hUploadButton;
	hStackPanel0->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 30;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 30;
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 120;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hClearButton1->Content = "Clear";
	hBorder4->Child = hClearButton1;
	hStackPanel0->Children->Add(hSpacer);
	hStackPanel0->Children->Add(hBorder4);
	hStackPanel0->Orientation = Orientation::Horizontal;
	hStackPanel0->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hUploadGrid->SetRow(hStackPanel0, 3);
	hUploadGrid->Children->Add(hStackPanel0);
}

// set empty row in search gird

void WPFCppCliDemo::setNullSpace()
{
	// for the fuking space
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(330);
	hSearchGrid->RowDefinitions->Add(hRowDef);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Height = 335;
	hStackPanel00->Children->Add(hSpacer);
	hStackPanel00->Orientation = Orientation::Horizontal;
	hStackPanel00->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSearchGrid->SetRow(hStackPanel00, 0);
	hSearchGrid->Children->Add(hStackPanel00);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(20);
	hSearchGrid->RowDefinitions->Add(hRow2Def);
	TextBlock^ hSpacer0 = gcnew TextBlock();
	hSpacer0->Height = 20;
	hStackPanel02->Children->Add(hSpacer0);
	hSearchGrid->SetRow(hStackPanel02, 1);
	hSearchGrid->Children->Add(hStackPanel02);
}


// set Addr buttons in search grid

void WPFCppCliDemo::setAddrButtons()
{
// addddd 
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(30);
	hSearchGrid->RowDefinitions->Add(hRow1Def);
	hAddrPortLabel->Content = "Server Addr&Port:";
	hAddrTB->Text = "localhost"; 
	hPortTB->Text = "8081";
	hAddrTB->Width = 100;
	hPortTB->Width = 60;
	special->Content = ":";
	AddAddrButton->Content = "Add";
	AddAddrButton->Width = 70;
	clearAddrButton->Content = "Clear";
	clearAddrButton->Width = 70;
	TextBlock^ hSpacer90 = gcnew TextBlock();
	hSpacer90->Width = 90;
	TextBlock^ hSpacer91 = gcnew TextBlock();
	hSpacer91->Width = 40;

	hStackPanel01->Children->Add(hAddrPortLabel);
	hStackPanel01->Children->Add(hAddrTB);
	hStackPanel01->Children->Add(special);
	hStackPanel01->Children->Add(hPortTB);
	hStackPanel01->Children->Add(hSpacer90);
	hStackPanel01->Children->Add(AddAddrButton);
	hStackPanel01->Children->Add(hSpacer91);
	hStackPanel01->Children->Add(clearAddrButton);
	hStackPanel01->Orientation = Orientation::Horizontal;
	hStackPanel01->HorizontalAlignment = System::Windows::HorizontalAlignment::Stretch;
	hSearchGrid->SetRow(hStackPanel01, 2);
	hSearchGrid->SetColumnSpan(hStackPanel01, 3);

	hSearchGrid->Children->Add(hStackPanel01);

}

// set wildcard buttons in search grid

void WPFCppCliDemo::setWCButtons()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(10);
	hSearchGrid->RowDefinitions->Add(hRowDef);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(30);
	hSearchGrid->RowDefinitions->Add(hRow1Def);
	WildCardsLabel->Content = "WildCard:";
	WildCardsTB->Text = "*.*";
	WildCardsTB->Width = 100;

	AddWBButton->Content = "Add";
	AddWBButton->Width = 70;
	clearWBButton->Content = "Clear";
	clearWBButton->Width = 70;
	TextBlock^ hSpacer89 = gcnew TextBlock();
	hSpacer89->Width = 46;
	TextBlock^ hSpacer90 = gcnew TextBlock();
	hSpacer90->Width = 162;
	TextBlock^ hSpacer91 = gcnew TextBlock();
	hSpacer91->Width = 40;

	hStackPanel03->Children->Add(WildCardsLabel);
	hStackPanel03->Children->Add(hSpacer89);
	hStackPanel03->Children->Add(WildCardsTB);
	hStackPanel03->Children->Add(hSpacer90);
	hStackPanel03->Children->Add(AddWBButton);
	hStackPanel03->Children->Add(hSpacer91);
	hStackPanel03->Children->Add(clearWBButton);
	hStackPanel03->Orientation = Orientation::Horizontal;
	hStackPanel03->HorizontalAlignment = System::Windows::HorizontalAlignment::Stretch;
	hSearchGrid->SetRow(hStackPanel03, 4);
	hSearchGrid->SetColumnSpan(hStackPanel03, 3);

	hSearchGrid->Children->Add(hStackPanel03);
}


// set options in search grid

void WPFCppCliDemo::setOptionsContent()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(10);
	hSearchGrid->RowDefinitions->Add(hRowDef);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(30);
	hSearchGrid->RowDefinitions->Add(hRow1Def);
	StringsLabel->Content = "Text for Search:";
	StringsTB->Text = "*.*";
	StringsTB->Width = 100;
	PathLabel->Content = "Path for Search:(SeverRepository/)";
	PathTB->Text = "aaa/";
	PathTB->Width = 100;

	TextBlock^ hSpacer89 = gcnew TextBlock();
	hSpacer89->Width = 15;
	TextBlock^ hSpacer90 = gcnew TextBlock();
	hSpacer90->Width = 15;
	TextBlock^ hSpacer91 = gcnew TextBlock();
	hSpacer91->Width = 15;

	hStackPanel04->Children->Add(StringsLabel);
	hStackPanel04->Children->Add(hSpacer89);
	hStackPanel04->Children->Add(StringsTB);
	hStackPanel04->Children->Add(hSpacer90);
	hStackPanel04->Children->Add(PathLabel);
	hStackPanel04->Children->Add(hSpacer91);
	hStackPanel04->Children->Add(PathTB);

	hStackPanel04->Orientation = Orientation::Horizontal;
	hStackPanel04->HorizontalAlignment = System::Windows::HorizontalAlignment::Stretch;
	hSearchGrid->SetRow(hStackPanel04, 6);
	hSearchGrid->SetColumnSpan(hStackPanel04, 3);

	hSearchGrid->Children->Add(hStackPanel04);
}


// set search buttons in search grid

void WPFCppCliDemo::setSearchButtons()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(10);
	hSearchGrid->RowDefinitions->Add(hRowDef);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(30);
	hSearchGrid->RowDefinitions->Add(hRow1Def);
	XMLCB->Content = "XML";
	PortLabel->Content = "Client Port:";
	PortTB->Text = "8080";
	PortTB->Width = 100;
	SearchFileButton->Content = "SearchFile";
	SearchFileButton->Width = 70;
	SearchTextButton->Content = "SearchText";
	SearchTextButton->Width = 70;
	hClearButton0->Content = "ClearInfoBoard";
	hClearButton0->Width = 100;
	TextBlock^ hSpacer88 = gcnew TextBlock();
	hSpacer88->Width = 10;
	TextBlock^ hSpacer89 = gcnew TextBlock();
	hSpacer89->Width = 40;
	TextBlock^ hSpacer90 = gcnew TextBlock();
	hSpacer90->Width = 40;
	TextBlock^ hSpacer91 = gcnew TextBlock();
	hSpacer91->Width = 107;
	TextBlock^ hSpacer92 = gcnew TextBlock();
	hSpacer92->Width = 40;
	hStackPanel05->Children->Add(PortLabel);
	hStackPanel05->Children->Add(hSpacer89);
	hStackPanel05->Children->Add(PortTB);
	hStackPanel05->Children->Add(hSpacer88);
	hStackPanel05->Children->Add(XMLCB);
	hStackPanel05->Children->Add(hSpacer91);
	hStackPanel05->Children->Add(SearchFileButton);
	hStackPanel05->Children->Add(hSpacer90);
	hStackPanel05->Children->Add(SearchTextButton);
	hStackPanel05->Children->Add(hSpacer92);
	hStackPanel05->Children->Add(hClearButton0);
	hStackPanel05->Orientation = Orientation::Horizontal;
	hStackPanel05->HorizontalAlignment = System::Windows::HorizontalAlignment::Stretch;
	hSearchGrid->SetRow(hStackPanel05, 8);
	hSearchGrid->SetColumnSpan(hStackPanel05, 3);
	hSearchGrid->Children->Add(hStackPanel05);
}

// set search view

void WPFCppCliDemo::setSearchView()
{
	Console::Write("\n  setting up search view");
	hSearchGrid->Margin = Thickness(20);
	hSearchTab->Content = hSearchGrid;

	setTextBlocks1();
	setListBlocks1();
	setNullSpace();
	setAddrButtons();
	setWCButtons();
	setOptionsContent();
	setSearchButtons();
}

// set uplaod view

void WPFCppCliDemo::setUpUpDownView()
{
	Console::Write("\n  setting up upload/download view");
	hUploadGrid->Margin = Thickness(20);
	hUploadTab->Content = hUploadGrid;

	setTextBlock0();
	setTextBoxes0();
	setTextBoxes1();
	setButtons0();

}

//upload file event
void WPFCppCliDemo::uploadfile(Object^ obj, RoutedEventArgs^ args)
{
	hStatus->Text = "Upload File";

	Message msg;
	msg.setCmd("UPLOAD");
	msg.setFileSpec( toStdString(hFileSpec->Text));
	msg.setSendAddr("localhost");
	msg.setSendPort(toStdString(hClientPort->Text));
	msg.setRecvAddr(toStdString(hServerAddr->Text));
	msg.setRecvPort(toStdString(hServerPort->Text));
	pSendr_->postMessage(msg);
}

//explore file event
void WPFCppCliDemo::explorefile(Object^ obj, RoutedEventArgs^ args)
{
	hDownLButton->IsEnabled = false;
	hStatus->Text = "Explore File";
	downloadListbox->Items->Clear();
	Message msg;
	msg.setCmd("EXPLORE");
	msg.setSendAddr("localhost");
	msg.setSendPort(toStdString(hClientPortdTB->Text));
	msg.setRecvAddr(toStdString(hServerAddrdTB->Text));
	msg.setRecvPort(toStdString(hServerPortdTB->Text));
	pSendr_->postMessage(msg);
}



//download file event
void WPFCppCliDemo::downloadfile(Object^ obj, RoutedEventArgs^ args)
{
	hStatus->Text = "Download File";
	Message msg;
	msg.setCmd("DOWNLOAD");
	String^ temp =  downloadListbox->SelectedItem->ToString()->Substring(17);
	msg.setFileSpec(toStdString(temp));
	msg.setSendAddr("localhost");
	msg.setSendPort(toStdString(hClientPortdTB->Text));
	msg.setRecvAddr(toStdString(hServerAddrdTB->Text));
	msg.setRecvPort(toStdString(hServerPortdTB->Text));
	pSendr_->postMessage(msg);
}

// add Server addrs button
void WPFCppCliDemo::addServerAddrs(Object^ obj, RoutedEventArgs^ args)
{
	hStatus->Text = "Add ServerAddr";

	String^ a = hAddrTB->Text + ":" + hPortTB->Text;
	listServerAddrs->Add(a);
	hListBox0->Items->Insert(0,a);
}

// clear Server addrs button
void WPFCppCliDemo::ClearServerAddrs(Object^ obj, RoutedEventArgs^ args)
{
	hStatus->Text = "Clear ServerAddr";

	listServerAddrs->Clear();
	hListBox0->Items->Clear();
}

// add WildCards button
void WPFCppCliDemo::addWildCards(Object^ obj, RoutedEventArgs^ args)
{
	hStatus->Text = "Add WildCards";

	String^ a = WildCardsTB->Text;
	listWildCards->Add(a);
	hListBox1->Items->Insert(0,a);
}

// clear wildcards button
void WPFCppCliDemo::clearWildCards(Object^ obj, RoutedEventArgs^ args)
{
	hStatus->Text = "Clear WildCards";

	listWildCards->Clear();
	hListBox1->Items->Clear();
}

// search File button
void WPFCppCliDemo::searchFile(Object^ obj, RoutedEventArgs^ args)
{
	hStatus->Text = "Search File";

	Message msg;
	msg.setCmd("SEARCHFILE");
	msg.setSendAddr("localhost");
	msg.setSendPort(toStdString(PortTB->Text));
	if (XMLCB->IsChecked)
		msg.setIsXML(true);
	else 
		msg.setIsXML(false);
	msg.setPath(toStdString(PathTB->Text));
	for each (String^ wildcard in listWildCards)
	{
		msg.addWildCards(toStdString(wildcard));
	}
	for each (String^ serverAddr in listServerAddrs)
	{
		std::string serveraddr = toStdString(serverAddr);
		int pos = serveraddr.find(":");
		std::string addr = serveraddr.substr(0, pos);
		std::string port = serveraddr.substr(pos + 1);
		msg.setRecvAddr(addr);
		msg.setRecvPort(port);
		pSendr_->postMessage(msg);
	}
}

// search text button
void WPFCppCliDemo::searchText(Object^ obj, RoutedEventArgs^ args)
{
	hStatus->Text = "Search Text";

	Message msg;
	msg.setCmd("SEARCHTEXT");
	msg.setSendAddr("localhost");
	msg.setSendPort(toStdString(PortTB->Text));
	if (XMLCB->IsChecked)
		msg.setIsXML(true);
	else
		msg.setIsXML(false);
	msg.setPath(toStdString(PathTB->Text));
	msg.setText(toStdString(StringsTB->Text));
	for each (String^ wildcard in listWildCards)
	{
		msg.addWildCards(toStdString(wildcard));
	}
	for each (String^ serverAddr in listServerAddrs)
	{
		std::string serveraddr = toStdString(serverAddr);
		int pos = serveraddr.find(":");
		std::string addr = serveraddr.substr(0, pos);
		std::string port = serveraddr.substr(pos + 1);
		msg.setRecvAddr(addr);
		msg.setRecvPort(port);
		pSendr_->postMessage(msg);
	}
}

// enable download button event
void WPFCppCliDemo::enableDownload(Object^ sender, MouseButtonEventArgs^ e)
{
	hDownLButton->IsEnabled = true;
}

// system string to std string
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

// std string to system string
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

// add infomation to textblock
void WPFCppCliDemo::addText(String^ msg)
{
	hTextBlock0->Text += msg + "\n\n";
	hTextBlock2->Text += msg + "\n\n";
	hTextBlock3->Text += msg + "\n\n";

}

// add content list box
void WPFCppCliDemo::addListBox(String^ msg)
{
	downloadListbox->Items->Add(msg);
}

// get msg thread
void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function

	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		std::string msg = pRecvr_->getMessage();
		String^ sMsg = toSystemString(msg);
		array<String^>^ args = gcnew array<String^>(1);
		if (sMsg->Contains("<filefordownload>"))
		{
			sMsg = sMsg->Substring(17);
			args[0] = sMsg;
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addListBox);
			Dispatcher->Invoke(act, args);  // must call addText on main UI thread
		}
		else
		{
			args[0] = sMsg;
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
			Dispatcher->Invoke(act, args);  // must call addText on main UI thread
		}
	}
}

// cllear text blocks
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hTextBlock0->Text = "";
	hTextBlock2->Text = "";
	hTextBlock3->Text = "";
}

void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting RCMF Client GUI");

	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo());
	Console::WriteLine(L"\n\n");
}