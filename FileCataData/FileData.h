//////////////////////////////////////////////////////////////////////
// FileName: FileData.h - repository of file catalogue             //
// ver 1.0                                                          //
// Author:   Huijun Lin, hlin14@syr.edu                             //
// Language: Visual C++, Visual Studio 2013                         //
// Platform: Window 8.1, MacBook Air                                //
// Application: Project 1, CSE687 Spring15, Syracuse University     //
//////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module provides a data structure of File Specification.
* with some of function to access properties of DataStore
*
* Public Interface:
* =================
* DataStore ds;
* fd.save("C:/Users/Huijun Lin/Documents/Visual Studio 2013/Projects/FileCatalogue/DataStore/DataStore.h");
* fd.save("DataStore.h","C:/Users/Huijun Lin/Documents/Visual Studio 2013/Projects/FileCatalogue/DataStore/");
* 
* Required Files:
* ===============
*  FileData.h,  FileData.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_FILECATADATA FileData.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 03 Feb 2015
* - first release
*/

#ifndef FILEDATA_H
#define FILEDATA_H

#include <vector>
#include <list>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include "../XmlElement/XmlElement.h"


class FileData
{
public:
	using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement > ;
	using Path = std::string;
	using Paths = std::set < Path > ;
	using PathIter = Paths::iterator;
	using ListOfIter = std::list < PathIter > ;
	using File = std::string;
	using Store = std::map < File, ListOfIter > ;
	using Iterator = Store::iterator;
	using Const_Iterator = Store::const_iterator;

	void printpath()
	{
		for (auto a : paths)
			std::cout << a<<"\n";
	}

	int fileCount() const
	{
		return store.size();
	}

	int pathCount() const
	{
		return paths.size();
	}

	void save(const std::string& filespec);
	void save(const std::string& filename, const std::string& path);
	Iterator begin()  { return store.begin(); }
	Iterator end() { return store.end(); }
	Const_Iterator begin() const { return store.begin();}
	Const_Iterator end() const { return store.end(); }
	sPtr root;

private:
	Store store;
	Paths paths;
};

#endif