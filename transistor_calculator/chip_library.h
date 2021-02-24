// Please check library.txt to find the rules for creating a input text file.
#ifndef CHIP_LIBRARY_H
#define CHIP_LIBRARY_H

#include <map>
#include <string>

// a general structure for every element
struct component{
	std::map<std::string, int> subcomponents;	 								// contains name and number of each subcomponent
	int tran_num;																							// the number of transistors in a given component
};

// Reference: "typedef" specifier - https://en.cppreference.com/w/cpp/language/typedef
typedef std::map<std::string, component> ComponentsMap;

class ChipLibrary {
	public:
		ChipLibrary();							 														// constructor which reads info from a .txt file
		int library_size();																			// returns the number of components in a library
		bool component_in_library(std::string name);	 					// checks if a component has been added to the library before
		bool update_library_from(std::string filename);					// updates the library from a file; return true if it's sucessful
		ComponentsMap get_all_components();											// returns all components in the library
		int get_tran_num(std::string name);						 					// returns the number of transistors in the component
		bool is_integer(std::string num);												// checks if a number is integer
	private:
		ComponentsMap _components;															// main map with all components
		void update_tran_num(std::string name);					// counts the number of transistors in the given component and updates the number
		int count_occurrences(std::string sub, std::string line);	// counts the number sub appears in line
};

#endif
