#include "chip_library.h"
#include <map>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>

ChipLibrary::ChipLibrary() {
	component Transistor;																								// add Transistor manually to the library
	Transistor.subcomponents["Transistor"] = 1;
	Transistor.tran_num = 1;
	_components["Transistor"] = Transistor;
}

int ChipLibrary::library_size() {
	// Reference: .size() method - https://www.cplusplus.com/reference/map/map/size/
	return _components.size();
}

bool ChipLibrary::update_library_from(std::string filename) {
	// Reference: .clear() method - https://www.cplusplus.com/reference/map/map/clear/
	_components.clear();  // delete the previous library

	component Transistor;																								// add Transistor manually to the library
	Transistor.subcomponents["Transistor"] = 1;
	Transistor.tran_num = 1;
	_components["Transistor"] = Transistor;

	// Reference: opening a file and reading it line by line - https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
	std::string line;
  std::ifstream inputfile;
	inputfile.open(filename);
  if (!inputfile.is_open()) {
		std::cout << "INPUT FILE ERROR: Unable to open \"" << filename << "\".\n";
		return false;
	} else {
		while (getline(inputfile, line)) {															// read file line by line
			if (line.length() == 0 || line.find("//") == 0){
				continue;
			}
			if ((count_occurrences("[", line) != 1) || (count_occurrences("]", line) != 1)) {
				std::cout << "INPUT FILE ERROR: Could not parse the line \"" << line << "\".\n";
				std::cout << "The wrong number of brackets [].\n";
				return false;
			}
			line = line.substr(0, line.find("]") + 1);	// ignore everything after "]"

			if ( (count_occurrences(",", line) != (count_occurrences(":", line) - 1) ) || (count_occurrences(":", line) != count_occurrences(" ", line)) ) {
				std::cout << "INPUT FILE ERROR: Could not parse the line \"" << line << "\".\n";
				std::cout << "Please check \",\", \":\", and \" \".\n";
				return false;
			}

			int pos = line.find(" ");
			std::string name = line.substr(0, pos);											// extract the name of the component
			if (name == "Transistor") {											// attempt to redefine Transistor
				std::cout << "INPUT FILE ERROR: \"" << filename << "\" attempts to redefine Transistor which is a prohibited operation.\n";
				return false;
			} else {
				component* new_component = new component;									// creating memory for the new component
				std::string subcomponents_line = line.substr(pos + 2, line.length() - pos - 3);			// extract the line with subcomponents
				int start = 0;
				do {																											// extracting all subcomponents
					pos = subcomponents_line.find(", ", start);
					std::string subline = subcomponents_line.substr(start, pos - start);  // line subname:subnumber
					int del = subline.find(":");

					if (del == std::string::npos) {
						std::cout << "INPUT FILE ERROR: Could not parse the line \"" << line << "\" because " << "\":\" is missing.\n";
						delete new_component;
						return false;
					}

					std::string subname = subline.substr(0, del);
					if (!component_in_library(subname)) {										// checks if a subcomponent has been added to the library before
						std::cout << "INPUT FILE ERROR: \"" << name << "\" attempts to use \"" << subname << "\", which has not been defined before.\n";
						std::cout << "Please ensure that every component consists only of the elements described above it and/or transistors.\n";
						delete new_component;
						return false;
					}
					std::string subnumb = subline.substr(del + 1);

					if (!is_integer(subnumb)) {										// checks if the number of subcomponent is an integer
						std::cout << "INPUT FILE ERROR: The number of \"" << subname << "\" in \"" << name << "\" is not an integer.\n";
						delete new_component;
						return false;
					}

					new_component->subcomponents[subname] = stoi(subnumb);  // save info about subcomponent
					start = pos + 2;
				} while (pos != std::string::npos && start < subcomponents_line.length());

				_components[name] = *new_component;			// save component
				this->update_tran_num(name);	  // assign transistor number
			}
    }
    inputfile.close();
  }
	return true;
}

bool ChipLibrary::component_in_library(std::string name) {
	ComponentsMap::iterator i;		// iterate through the names of all components
	for (i = _components.begin(); i != _components.end(); i++) {
		if (name == i->first) { return true;	}
	}
	return false;
}

ComponentsMap ChipLibrary::get_all_components() {
	return _components;
}

int ChipLibrary::get_tran_num(std::string name) {
	return _components[name].tran_num;
}

// Reference: ELEC2645 course material
bool ChipLibrary::is_integer(std::string num) {
  return std::regex_match(num, std::regex("[+-]?[0-9]+"));
}

void ChipLibrary::update_tran_num(std::string name) {
	std::map<std::string, int>::iterator i;		// iterate through the names of the subcomponents
	int tran_number = 0;
	for (i = _components[name].subcomponents.begin(); i != _components[name].subcomponents.end(); i++) {
		tran_number += i->second * _components[i->first].tran_num;		// number of these components * number of transistors in each
	}
	_components[name].tran_num = tran_number;		// update the transistor number
}

int ChipLibrary::count_occurrences(std::string sub, std::string line) {
  int result = 0;
	int pos = line.find(sub);
	while (pos != std::string::npos) {
	  pos += sub.length();
		pos = line.find(sub, pos);
		result++;
	}
	return result;
}
