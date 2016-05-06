//////////////////////////////////////////////////////////////////
// Filename:     CAI.cpp
// Description:  Difine member funcitons in the interface CAI.h
// Author:       Huijun Lin
// Project:      Homework#3 CSE591
//////////////////////////////////////////////////////////////////


#include "CAI.h"

void CAI::StartTest()
{
	srand(time(0));
	firstTryRight = 0;
	totalWrong = 0;

	cout<<"\nWelcome to the multiplication and division exercise.";
	cout<<"\n===================================================="<<endl;

	//For loop to invoke randMulti() 5 times to generate 5 different quz
	cout<<"\n\nMultiplication:"<<endl;
	for (int i = 1; i <= 5; i++)
	{
		cout<<"\nMul Question "<<i<<" :";
		randMulti();
	}

	//For loop to invoke randDivid() 5 times to generate 5 different quz
	cout<<"\n\nDevision:"<<endl;
	for (int i = 1; i <= 5; i++)
	{
		cout<<"\nDiv Question "<<i<<" :";
		randDivid();
	}

	summary();
}

int CAI::randNum(int mode)
{
	//According to different mode to generate different random number
	switch (mode)
	{
	case MSG:
		return 1+rand()%4;
	case MUL:
		return 1+rand()%9;
	case DIV:
		return 1+rand()%9;
	default:
		return -1;
	}
}


void CAI::randMulti()
{
	bool firstTry = true;
	bool correct = false;

	// to generate two 1 to 9 integer in order to multiply
	int x = randNum(MUL);
	int y = randNum(MUL);
	
	
	//while loop to let student answer the same quz until he got correct
	while (!correct)
	{
		int answer;
		cout<<"\nHow much is "<<x<<" times "<<y<<"?";
		cin>>answer;
		if(answer==x*y)
		{
			correct = true;
			msgRight(); // call msg when answer is correct
			if(firstTry)
			{
				firstTryRight++; // determine whether this quz is correct when first try
			}
		}
		else
		{
			totalWrong++;
			firstTry = false; //once got wrong, it is not the fisrt try anymore
			msgWrong(); // call msg when answer is wrong
		}

	}
}

void CAI::randDivid()
{
	bool firstTry = true;
	bool correct = false;

	// to generate 2 integer, one is the divisor, the other one the answer
	int x = randNum(DIV);
	int y = randNum(DIV);

	//while loop to let student answer the same quz until he got correct
	while (!correct)
	{
		int dividend = x*y; // generate the dividen
		cout<<"\nHow much is "<<dividend<<" divide by "<<x<<"?";
		int answer;
		cin>>answer;
		if(answer==y)
		{
			correct = true;
			msgRight();
			if(firstTry)
			{
				firstTryRight++;
			}
		}
		else
		{
			totalWrong++;
			firstTry = false;
			msgWrong();
		}

	}
}


void CAI::msgRight()
{
	cout<<"\n";
	int msgNum = randNum(MSG);

	// randomly cout a msg
	switch (msgNum)
	{
	case 1:
		cout<<"Very good!";
		break;
	case 2:
		cout<<"Excellent!";
		break;
	case 3:
		cout<<"Nice work!";
		break;
	case 4:
		cout<<"Keep up the good work!";
		break;
	default:
		break;
	}
	cout<<endl;
}


void CAI::msgWrong()
{
	int msgNum = randNum(MSG);
	cout<<"\n";
	switch (msgNum)
	{
	case 1:
		cout<<"No. please try again.";
		break;
	case 2:
		cout<<"Wrong. Try once more";
		break;
	case 3:
		cout<<"Don't give up!";
		break;
	case 4:
		cout<<"No. keep tring.";
		break;
	default:
		break;
	}
	cout<<endl;
}

void CAI::summary()
{
	//////////////////////////////////////////////////////////////////////////////
	//  a summary about first try times, average tries, average wrong guesses

	cout<<"\n====================================================================";
	cout<<"\nSummary:";
	cout<<"\nThere are "<<firstTryRight<<" times you got right on the first try.";
	cout<<"\nOn average, you made "<<totalWrong/10<<" wrong guesses to get each of the quz corret";
	cout<<"\nOn average, you made "<<(10+totalWrong)/10<<" tries to get each of the quz correct.";
	cout<<"\n===================================================================="<<endl;
}
