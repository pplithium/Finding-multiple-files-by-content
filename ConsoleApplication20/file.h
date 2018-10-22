#ifndef FILE_H
#define FILE_H
#include<string>

class file
{
public:

	file();
	~file();

	size_t size;
	bool flag;
	std::string file_name;
	std::string dir_name;
	std::string directories;
	std::string filepath;
	
	bool compare_content(const file &other) const;

	bool operator==(const file &other) const;										
	
	bool operator<(const file &other) const;

};

#endif
