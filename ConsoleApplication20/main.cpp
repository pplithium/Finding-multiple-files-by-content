#include"pch.h"
#include"file.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>

namespace fs = std::experimental::filesystem;

void get_files_from_path(const std::string &path, std::vector<file> *files);//function that is getting all the files to vector of files recursively

void process_to_v(std::vector<file> *files, std::vector<file> *files2);     //function that takes vector in format of "X : directory1, X : Directory2.."
																			//then creates other vector in format of "X: directory1, directory2... , Y: directory1.."

void v_output(std::vector<file> *files2);									//function that adds files with same directories together, and prints out.

int main()
{
	fs::path path;
	std::vector<file> files;
	std::vector<file> files2;
	std::string input;

	std::cout <<
		"Please enter a path in format of 'C:/Users/CANER/Desktop...' \n(If you leave it empty, it will display files for default directory): "
		<< std::endl;
	std::getline(std::cin, input);
	path = input;
	
	if (path == "") {														//If user hasn't entered anything, path equals to current path.
		path = fs::current_path().parent_path();
	}

	std::cout << std::endl << "Processing..." << std::endl;

	get_files_from_path(path.string(), &files);    

	if (files.empty()) {
		std::cout << "Could not find any files, please check your directory address, it might not be correct.\n" << std::endl;
		main();															   //If program cannot find any files, return to main, the address might be wrong.
	}

	process_to_v(&files, &files2);

	v_output(&files2);

}


void get_files_from_path(const std::string &path, std::vector<file> *files) {

	std::error_code ec;
	file file_template;

	for (const auto & p : fs::recursive_directory_iterator(path)) {
		if (fs::is_regular_file(p, ec)) {
			
			file_template.file_name = p.path().stem().string();
			file_template.dir_name = p.path().parent_path().string().erase(0, path.length()+1); //We need directory name in format of 
			file_template.size = file_size(p.path());                                           //"Directory A\Directory AA", so I erased before "Directory A"
			file_template.filepath = p.path().string();

			files->emplace_back(file_template);                       
		}
	}
}

void process_to_v(std::vector<file> *files, std::vector<file> *files2) {                

	std::vector<file>::iterator it_find;
	std::string directories;
	int i = 0;
	file template_file2;

	std::sort(files->begin(), files->end());

	for (std::vector<file>::iterator it = files->begin(); it != files->end(); ++it) {
		it_find = std::find(files2->begin(), files2->end(), *it);                  //this for, reads from first vector and writes to second vector
		if (it_find != files2->end()) {                                            //it collects directory elements from first vector,
			i++;																	//and writes to directories in the second vector. So that 
			it_find->directories = it_find->directories + it->dir_name + "\n\t";   //we have all directory names in one string

		}
		else {
			
			template_file2.file_name = it->file_name;
			template_file2.directories = it->dir_name + "\n\t";
			template_file2.size = it->size;
			template_file2.filepath = it->filepath;
			files2->emplace_back(template_file2);
			
		}

	}

	for (auto it = files2->begin(); it != files2->end(); ++it) {						//this flags the single files
		int cnt = std::count(it->directories.begin(), it->directories.end(), '\n');
		if (cnt < 2) it->flag = false;
	}
}

void v_output(std::vector<file> *files2) { 

	for (auto it = files2->begin(); it != files2->end(); ++it) {  //Level3 feature that checks the directories, and if the directories are same,
		for (auto it2 = it + 1; it2 != files2->end(); ++it2) {	  //adds the file name to first one, then flags the other to false

			if (it->directories == it2->directories) {

				it->file_name = it->file_name + ", " + it2->file_name;
				it2->flag = false;										
			}
		}
	}

	for (auto it = files2->begin(); it != files2->end(); ++it) { //if the flags are true, print out.

		if (it->flag == true){
			std::cout << it->file_name << "\n\t" << it->directories << std::endl;
			}
	}
}
