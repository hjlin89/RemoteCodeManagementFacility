//////////////////////////////////////////////////////////////////////
// FileName: FileMgr.cpp                                            //
// Author:   Huijun Lin, hlin14@syr.edu                             //
// Language: Visual C++, Visual Studio 2013                         //
// Platform: Window 8.1, MacBook Air                                //
// Application: Project 1, CSE687 Spring15, Syracuse University     //
//////////////////////////////////////////////////////////////////////


// FileMgr.cpp - Collects file specifications from directory tree

#include "FileMgr.h"
#include <iostream>

// find files in the given path
void FileMgr::find(const std::string& path, sPtr& root)
{

	for (auto patt : patterns_)
	{
		std::vector<std::string> files = FileSystem::Directory::getFiles(path, patt);
		for (auto f : files)
		{
			//std::cout << "\n  file is: " << f;
			std::string path_ = FileSystem::Path::toLower(path);
			f = FileSystem::Path::toLower(f);
			filedata_.save(f, path_);
			root->addChild(XmlProcessing::makeTextElement(f));
		}

		if (isRecurse_)
		{

			std::vector<std::string> dirs = FileSystem::Directory::getDirectories(path);

			// remove "." and ".." from dirs

			auto iter = std::find(std::begin(dirs), std::end(dirs), ".");
			if (iter != std::end(dirs))
			{
				dirs.erase(iter);
			}
			iter = std::find(std::begin(dirs), std::end(dirs), "..");
			if (iter != std::end(dirs))
			{
				dirs.erase(iter);
			}

			for (auto d : dirs)
			{
				sPtr child = XmlProcessing::makeTaggedElement(d);
				root->addChild(child);
				std::string recurPath = path + d + "/";
				find(recurPath, child);
			}
		}

	}
}

void FileMgr::findText(const std::string& path, sPtr& root, std::string text)
{

	for (auto patt : patterns_)
	{
		std::vector<std::string> files = FileSystem::Directory::getFiles(path, patt);
		for (auto f : files)
		{
			std::string path_ = FileSystem::Path::toLower(path);
			f = FileSystem::Path::toLower(f);
			std::string file_ = path_ + f;
			FileSystem::File file(file_);
			file.open(FileSystem::File::in);
			std::string all = file.readAll();
			file.close();
			int pos = all.find(text);
			if (pos > -1)
			{
				filedata_.save(f, path_);
				root->addChild(XmlProcessing::makeTextElement(f));
			}
		}
		if (isRecurse_)
		{
			std::vector<std::string> dirs = FileSystem::Directory::getDirectories(path);
			// remove "." and ".." from dirs
			auto iter = std::find(std::begin(dirs), std::end(dirs), ".");
			if (iter != std::end(dirs))
			{
				dirs.erase(iter);
			}
			iter = std::find(std::begin(dirs), std::end(dirs), "..");
			if (iter != std::end(dirs))
			{
				dirs.erase(iter);
			}

			for (auto d : dirs)
			{
				sPtr child = XmlProcessing::makeTaggedElement(d);
				root->addChild(child);
				std::string recurPath = path + d + "/";
				findText(recurPath, child, text);
			}
		}

	}
}



// add patterns for searching
void FileMgr::addPattern(const std::string& patt)
{
	if (patterns_.size() == 1 && patterns_[0] == "*.*")
		patterns_.pop_back();
	patterns_.push_back(patt);
}

// ser a notification to search sub-directory
void FileMgr::setRecursive(const bool isRecurse)
{
	isRecurse_ = isRecurse;
}

void FileMgr::setPatterns(std::vector<std::string> v)
{
	patterns_ = v;
}




#ifdef TEST_FILEMGR

int main()
{
  std::cout << "\n  Testing FileMgr class";
  std::cout << "\n =======================\n";

  std::string path = "../";
  FileData fd;
  FileMgr fm(fd);
  fm.setRecursive(true);
  fm.addPattern("*.h");
  fm.addPattern("*.cpp");
  fm.addPattern("*.obj");


  return 0;
}

#endif
