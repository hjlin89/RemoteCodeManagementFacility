//////////////////////////////////////////////////////////////
// Filename:     CAI.cpp
// Description:  Computer-Asisted Instruction implementation
// Author:       Huijun Lin
// Project:      Homework#3 CSE591
//////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;

class CAI
{
public:
	void StartTest();   
private:
	// private member function
	void randMulti();   // to generate a random multiplication question
	void randDivid();   // to generate a random division question
	void msgRight();    // to generate 'good job' msg while the answer is right 
	void msgWrong();    // to generate 'keep tring' msg while the answer is wrong
    void summary();     // to make a summary of this Quz test
	int  randNum(int);     // to generate a random number
	
	// private data members

	static const int MSG = 0; //  represent different kinds of mode
	static const int MUL = 1; //  in order to generate different random number
	static const int DIV = 2;

	int firstTryRight;
	double totalWrong;
};
