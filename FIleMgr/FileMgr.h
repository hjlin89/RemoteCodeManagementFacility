//////////////////////////////////////////////////////////////////////
// FileName: FileMgr.h                                              //
// ver 1.1                                                          //
// Author:   Huijun Lin, hlin14@syr.edu                             //
// Language: Visual C++, Visual Studio 2013                         //
// Platform: Window 8.1, MacBook Air                                //
// Application: Project 1, CSE687 Spring15, Syracuse University     //
//////////////////////////////////////////////////////////////////////

// FileMgr.h - Collects file specifications from directory tree

/*
* Module Operations:
* ==================
* This module provides function to manage the file searched.
* FileMgr accept a path and a DataStore object reference as member.
* It can call function to add patterns and find files according to patterns
* and path. Then store it in DataStore.
* Iterator here is DataStore's iterator.
*
* Public Interface:
* =================
*
* DataStore ds;
* DataStore::Iterator iter = ds.begin();
* std::string path = "../";
* FileMgr fm(path, ds);
* fm.setRecursive(true);
* fm.addPattern("*.h");
* fm.addPattern("*.cpp");
* fm.addPattern("*.obj");
* fm.find(path,root);
* fm.findText(path, root, text);

*
* Required Files:
* ===============
* FileData.h FileData.cpp, FileSystem.h, FileSystem.cpp, FileMgr.h, FileMgr.cpp XmlElement.h XmlElement.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_FILEMGR FileMgr.cpp
*
* Maintenance History:
* ====================
* ver 1.1 : 28 Apr 2015
* - version for CSE687 project4
*
* ver 1.0 : 03 Feb 2015
* - first release
*/

#ifndef FILEMGR_H
#define FILEMGR_H

#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../FileCataData/FileData.h"
#include <vector>
#include <string>

class FileMgr
{
public:
	using patterns = std::vector < std::string > ;
	using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;

	FileMgr(FileData& filedata) : filedata_(filedata)
	{
		patterns_.push_back("*.*");
	}
	void addPattern(const std::string& patt);
	void setRecursive(const bool isRecurse);
	void find(const std::string& path, sPtr& root);
	void findText(const std::string& path, sPtr& root, std::string);
	void setPatterns(std::vector<std::string>);

private:
	FileData& filedata_;
	patterns patterns_;

	bool isRecurse_ = false;
};
#endif
