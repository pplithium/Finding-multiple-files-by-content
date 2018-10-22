#include "pch.h"
#include "file.h"
#include<fstream>
#include<iostream>

file::file() {
	flag = true;
}

file::~file() {}

bool file::operator==(const file &other) const {
	if (file_name == other.file_name)      
		return compare_content(other);
	else
		return false;												
}

bool file::operator<(const file &other) const {

	if (file_name.size() < other.file_name.size()) return true;     //Sorting according to rules in the assignment 
	if (file_name.size() > other.file_name.size()) return false;

	if (file_name < other.file_name) return true;					
	if (file_name > other.file_name) return false;

	if (dir_name.size() < other.dir_name.size()) return true;	    
	if (dir_name.size() > other.dir_name.size()) return false;

	if (dir_name < other.dir_name) return true;
	if (dir_name > other.dir_name) return false;

	return file_name < other.file_name;								
}

bool file::compare_content(const file &other) const {
	if (size == other.size) {       //checking size before checking content of files.
		std::ifstream lFile(filepath.c_str(), std::ifstream::in | std::ifstream::binary);
		std::ifstream rFile(other.filepath.c_str(), std::ifstream::in | std::ifstream::binary);

		if (!lFile.is_open() || !rFile.is_open())
		{
			std::cout << "Could not check content on files: " << file_name << ", " << other.file_name << std::endl;
			return false;
		}

		std::unique_ptr<char[]> lBuffer(new char[4096]);               
		std::unique_ptr<char[]> rBuffer(new char[4096]);

		do {
			lFile.read(lBuffer.get(), 4096);
			rFile.read(rBuffer.get(), 4096);
			if (std::memcmp(lBuffer.get(), rBuffer.get(), 4096) != 0)
			{
				return false;
			}
		} while (lFile.good() || rFile.good());
		
		return true;
	}
	else 
		return false;
	
}