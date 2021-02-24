#include "chip_library.h"
#include <map>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>

ChipLibrary::ChipLibrary() {
	component Nand;																								// add Nand manually to the library
	Nand.subcomponents["Nand"] = 1;
	Nand.Nand_num = 1;
	Nand.DFF_num = 0;
	_components["Nand"] = Nand;

	component DFF;																								// add DFF manually to the library
	DFF.subcomponents["DFF"] = 1;
	DFF.Nand_num = 0;
	DFF.DFF_num = 1;
	_components["DFF"] = DFF;
}

int ChipLibrary::library_size() {
	// Reference: .size() method - https://www.cplusplus.com/reference/map/map/size/
	return _components.size();
}

bool ChipLibrary::update_library_from(std::string filename) {
	// Reference: .clear() method - https://www.cplusplus.com/reference/map/map/clear/
	_components.clear();  // delete the previous library

	component Nand;														// add Nand manually to the library
	Nand.subcomponents["Nand"] = 1;
	Nand.Nand_num = 1;
	Nand.DFF_num = 0;
	_components["Nand"] = Nand;

	component DFF;														// add DFF manually to the library
	DFF.subcomponents["DFF"] = 1;
	DFF.Nand_num = 0;
	DFF.DFF_num = 1;
	_components["DFF"] = DFF;

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
			if (name == "Nand" || name == "DFF") {											// attempt to redefine Nand or DFF
				std::cout << "INPUT FILE ERROR: \"" << filename << "\" attempts to redefine Nand or DFF which is a prohibited operation.\n";
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
						std::cout << "Please ensure that every component consists only of the elements described above it and/or Nand or FFP.\n";
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
				this->update_Nand_and_DFF_num(name);	  // assign Nand number
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

int ChipLibrary::get_Nand_num(std::string name) {
	return _components[name].Nand_num;
}

int ChipLibrary::get_DFF_num(std::string name){
	return _components[name].DFF_num;
}

// Reference: ELEC2645 course material
bool ChipLibrary::is_integer(std::string num) {
  return std::regex_match(num, std::regex("[+-]?[0-9]+"));
}

void ChipLibrary::update_Nand_and_DFF_num(std::string name) {
	std::map<std::string, int>::iterator i;		// iterate through the names of the subcomponents
	int Nand_number = 0;
	int DFF_number = 0;
	for (i = _components[name].subcomponents.begin(); i != _components[name].subcomponents.end(); i++) {
		Nand_number += i->second * _components[i->first].Nand_num;		// number of these components * number of Nands in each
		DFF_number += i->second * _components[i->first].DFF_num;		// number of these components * number of DFFs in each
	}
	_components[name].Nand_num = Nand_number;		// update the Nand number
	_components[name].DFF_num = DFF_number;		// update the DFF number
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
