//////////////////////////////////////////////////////////////////////
// FileName: FileData.cpp - repository of file catalogue           //
// Author:   Huijun Lin, hlin14@syr.edu                             //
// Language: Visual C++, Visual Studio 2013                         //
// Platform: Window 8.1, MacBook Air                                //
// Application: Project 1, CSE687 Spring15, Syracuse University     //
//////////////////////////////////////////////////////////////////////

#include "FileData.h"
#include <iostream>

// save file specification according to filespec
void FileData::save(const std::string& filespec)
{
	File file;
	Path path;

	size_t pos = filespec.find_last_of("/");
	if (pos >= filespec.length())
		pos = filespec.find_last_of("\\");
	
	path = filespec.substr(0, pos+1);
	file = filespec.substr(pos+1, filespec.length()-pos);

	save(file, path);
}

// save file specification according to filename and path seperately
void FileData::save(const std::string& filename, const std::string& path)
{
	PathIter p_it = paths.find(path);	

	// check if the path have been stored
	if (p_it == paths.end())
		paths.insert(path);
	p_it = paths.find(path);

	Iterator s_it = store.find(filename);
	ListOfIter listOfPathIter;
	// check if the filename have been stored ever
	if (s_it == end())
	{
		listOfPathIter.push_back(p_it);
		store[filename] = listOfPathIter;
	}
	else
	{
		listOfPathIter = store[filename];
		for (auto item : listOfPathIter)
		{
			if ((*item).compare(path) != 0)
				continue;
			else
				return;
		}
		store[filename].push_back(p_it);
	}
}



#ifdef TEST_FILECATADATA

int main()
{
	FileData ds;

	std::cout << "============================================" << std::endl;
	std::cout << "Add element in Maps" << std::endl;
	std::cout << "============================================" << std::endl;

	// test1 - void DataStore::save(const std::string& filespec);
	ds.save("C:/Users/Huijun Lin/Documents/Visual Studio 2013/Projects/FileCatalogue/DataStore/DataStore.h");
	ds.save("C:/Users/Huijun Lin/Documents/Visual Studio 2013/Projects/FileCatalogue/DataStore.cpp");

	// test - void save(const std::string& filename, const std::string& path);
	ds.save("DataStore.h","C:/Users/Huijun Lin/Documents/Visual Studio 2013/Projects/FileCatalogue/DataStore/");
	ds.save("DataStore.cpp", "C:/Users/Huijun Lin/Documents/Visual Studio 2013/Projects/FileCatalogue/");
	ds.save("DataStore.cpp", "C:/Users/Huijun Lin/Documents/Visual Studio 2013/Projects/FileCatalogue/DataStore/newfolder/");
	ds.save("DataStore.cpp", "C:/Users/Huijun Lin/Documents/Visual Studio 2013/Projects/FileCatalogue/DataStore/");


	std::cout << "============================================" << std::endl;
	std::cout << "Show Maps"<<std::endl;
	std::cout << "============================================" << std::endl;

	for (auto elem : ds)
	{
		std::cout << "\nFilename: " << elem.first;
		std::cout << "\nPath:";

		for (auto item : elem.second)
		{
			std::cout<< "\n"<<*item;
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";

	//ds.printpath();

	return 0;

}

#endif