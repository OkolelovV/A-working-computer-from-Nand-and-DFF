#include <string>
#include <iostream>
#include "nand_gui.h"
#include "chip_library.h"

//Reference: adapted from ELEC2645 course material
void NandGUI::main_menu() {
	print_main_menu();
	int input = get_number_input(5);
	select_main_menu_item(input);
}

//Reference: adapted from ELEC2645 course material
void NandGUI::print_main_menu() {
  std::cout << "_________________________________________\n";
  std::cout << "-------- Nand and DFF calculator --------\n\n";
  std::cout << "1. Display the chip library (" << _components.library_size() << " elements)\n";
  std::cout << "2. Update the chip library\n";
  std::cout << "3. Calculate the Nand/DFF number\n";
  std::cout << "4. Compare two chip libraries\n";
  std::cout << "5. Back to the main menu\n";
}

//Reference: adapted from ELEC2645 course material
int NandGUI::get_number_input(int menu_items) {
  int input;
  std::string input_string;
  bool valid_input = false;
  do {
    std::cout << "\nSelect item: ";
    std::cin >> input_string;
    valid_input = _components.is_integer(input_string);
    // if input is not an integer, print an error message
    if (valid_input == false) {
      std::cout << "INPUT ERROR: Integer expected.\n";
    } else {  // if it is an int, check whether in range
      input = std::stoi(input_string);  // convert to int
      if (input >= 1 && input <= menu_items) {
        valid_input = true;
      } else {
        std::cout << "INPUT ERROR: Invalid menu item.\n";
        valid_input = false;
      }
    }
  } while (valid_input == false);
  return input;
}

//Reference: adapted from ELEC2645 course material
void NandGUI::select_main_menu_item(int input) {
  switch (input) {
    case 1:
      menu_item_1();
      break;
    case 2:
      menu_item_2();
      break;
    case 3:
      menu_item_3();
      break;
    case 4:
      menu_item_4();
      break;
  }
}

//Reference: ELEC2645 course material
void NandGUI::go_back_to_main_menu() {
  std::string input;
  do {
    std::cout << "\nEnter 'b' or 'B' to go back to the calculator menu: ";
    std::cin >> input;
  } while (input != "b" && input != "B");
  main_menu();
}

//Reference: adapted from ELEC2645 course material
void NandGUI::menu_item_1() {
  std::cout << "\nThe chip library:\n\n";
  display_all_components();
  go_back_to_main_menu();
}

//Reference: adapted from ELEC2645 course material
void NandGUI::menu_item_2() {
  update_library();
  go_back_to_main_menu();
}

void NandGUI::menu_item_3() {
  calculate_menu();
}

void NandGUI::menu_item_4() {
  compare();
	main_menu();
}

void NandGUI::display_all_components() {
	ComponentsMap components = _components.get_all_components();
	ComponentsMap::iterator i;		// iterate through the names of all components
	for (i = components.begin(); i != components.end(); i++) {
		std::cout << i->first << std::endl;		// name: number
	}
	std::cout << "\nThere are " << _components.library_size() << " elements in total.\n";
}

void NandGUI::update_library() {
	while (true) {
		std::string filename = "library.txt";
		std::string answer;
		std::cout << "\nUpdate the chip library from \"library.txt\"? (y/n): ";
		std::cin >> answer;
		if (answer != "Y" && answer != "y") {
				std::cout << "\nEnter the filelame: ";
				std::cin >> filename;
		}
	  bool result = _components.update_library_from(filename);
		if (result) {
			std::cout << "\nThe chip library has been updated successfully.\n";
			break;
		} else {
			std::cout << "\nWould you like to try again? (y/n): ";
			std::cin >> answer;
			if (answer != "Y" && answer != "y") { break; }
		}
	}
}

void NandGUI::calculate_menu() {
	print_calculate_menu();
  int input = get_number_input(5);
	select_calculate_menu_item(input);
}

void NandGUI::print_calculate_menu() {
  std::cout << "_________________________________________\n";
  std::cout << "**** Calculate the Nand/DFF number ****\n\n";
  std::cout << "1. The number of Nands and DFFs\n";
  std::cout << "2. The number of Nands\n";
  std::cout << "3. The number of DFFs\n";
  std::cout << "4. The number of Nands including DFFs\n";
  std::cout << "5. Back to the calculator menu\n";
}

void NandGUI::select_calculate_menu_item(int input) {
  switch (input) {
    case 1:
      calculate_item_1();
      break;
    case 2:
      calculate_item_2();
      break;
    case 3:
      calculate_item_3();
      break;
		case 4:
      calculate_item_4();
      break;
    default:
      main_menu();
      break;
  }
}

void NandGUI::calculate_item_1() {
	std::cout << "\n>> The number of Nands and DFFs\n";
	while (true) {
		std::string name = ask_name();
		if (name != " ") {
			calculate_Nand(name, _components);
			calculate_DFF(name, _components);
		}
		std::cout << "\nWould you like to try another component? (y/n): ";
		std::string answer;
		std::cin >> answer;
		if (answer != "y" && answer != "Y") {	break; }
	}
  calculate_menu();
}

void NandGUI::calculate_item_2() {
  std::cout << "\n>> The number of Nands\n";
	while (true) {
		std::string name = ask_name();
		if (name != " ") { calculate_Nand(name, _components); }
		std::cout << "\nWould you like to try another component? (y/n): ";
		std::string answer;
		std::cin >> answer;
		if (answer != "y" && answer != "Y") {	break; }
	}
  calculate_menu();
}

void NandGUI::calculate_item_3() {
  std::cout << "\n>> The number of DFFs\n";
	while (true) {
		std::string name = ask_name();
		if (name != " ") { calculate_DFF(name, _components); }
		std::cout << "\nWould you like to try another component? (y/n): ";
		std::string answer;
		std::cin >> answer;
		if (answer != "y" && answer != "Y") {	break; }
	}
  calculate_menu();
}

void NandGUI::calculate_item_4() {
  std::cout << "\n>> The number of Nands including DFFs\n";
	std::string nand_in_dff;
	while (true) {
		std::cout << "\nEnter the number of Nands in one DFF: ";
		std::cin >> nand_in_dff;
		if (_components.is_integer(nand_in_dff)) {
			break;
		} else {
			std::cout << "INPUT ERROR: Integer expected.";
		}
	}
	while (true) {
		std::string name = ask_name();
		if (name != " ") { calculate_Nand_inc_DFF(name, stoi(nand_in_dff));	}
		std::cout << "\nWould you like to try another component? (y/n): ";
		std::string answer;
		std::cin >> answer;
		if (answer != "y" && answer != "Y") {	break; }
	}
  calculate_menu();
}

std::string NandGUI::ask_name() {
	while (true) {
		std::string name;
		std::cout << "\nEnter the name of the component: ";
		std::cin >> name;
		if (_components.component_in_library(name)) {
			return name;
		} else {
			std::cout << "There is no such component in the library.\n";
			std::cout << "Display the chip library? (y/n): ";
			std::string answer;
			std::cin >> answer;
			if (answer == "y" || answer == "Y") {
				std::cout << std::endl;
				display_all_components();
			} else { return " "; }
		}
	}
}

int NandGUI::calculate_Nand(std::string name, ChipLibrary library) {
	int number = library.get_Nand_num(name);
	std::cout << "The number of Nands: " << number << std::endl;
	return number;
}

int NandGUI::calculate_DFF(std::string name, ChipLibrary library) {
	int number = library.get_DFF_num(name);
	std::cout << "The number of DFFs: " << number << std::endl;
	return number;
}

void NandGUI::calculate_Nand_inc_DFF(std::string name, int nand_in_dff) {
	int nand_number = _components.get_Nand_num(name);
	int dff_number = _components.get_DFF_num(name);
	nand_number += nand_in_dff * dff_number;
	std::cout << "The number of Nands: " << nand_number  << std::endl;
	std::cout << "(including Nands in DFF)" << std::endl;
}

void NandGUI::compare() {
	ChipLibrary library_1;				// creating the 1st library
	bool result = false;
	std::string filename1;
	std::string filename2;
	while (true) {
		std::cout << "\nEnter the filename of the 1st chip library: ";
		std::cin >> filename1;
		result = library_1.update_library_from(filename1);
		if (result) {
			std::cout << "The chip library 1 has been created successfully.\n";
			break;
		} else {
			std::cout << "\nWould you like to try again? (y/n): ";	// in case a user entered wrong and want to try again
			std::string answer;
			std::cin >> answer;
			if (answer != "Y" && answer != "y") { break; }
		}
	}
	if (result) {										// if the 1st library created, creating the 2nd library
		ChipLibrary library_2;
		while (true) {
			std::cout << "\nEnter the filename of the 2nd chip library: ";
			std::cin >> filename2;
			if (filename2 == filename1) {
				std::cout << "INPUT ERROR: The filenames are the same.\n";
				result = false;
			} else {
				result = library_2.update_library_from(filename2);
			}
			if (result) {
				std::cout << "The chip library 2 has been created successfully.\n";
				break;
			} else {
				std::cout << "\nWould you like to try again? (y/n): ";		// in case a user entered wrong and want to try again
				std::string answer;
				std::cin >> answer;
				if (answer != "Y" && answer != "y") { break; }
			}
		}
		if (result) {														// if the 2nd library created, continue
			while (true) {
				std::string name;
				std::cout << "\nEnter the name of the component: ";
				std::cin >> name;
				if (!library_1.component_in_library(name)) {											// check the component in the first library
					std::cout << "\nThere is no such component in \"" << filename1 << "\"\n";
				} else if (!library_2.component_in_library(name)) {								// check the component in the second library
					std::cout << "\nThere is no such component in \"" << filename2 << "\"\n";
				}	else {
						std::cout << "\n\"" << filename1 << "\": " << name << "\n";
						int nand1 = calculate_Nand(name, library_1);
						int dff1 = calculate_DFF(name, library_1);
						std::cout << "\n\"" << filename2 << "\": " << name << "\n";
						int nand2 = calculate_Nand(name, library_2);
						int dff2 = calculate_DFF(name, library_2);
						std::cout << "\n\"" << filename2 << "\" relative to \"" << filename1 << "\": " << name << "\n";
						int result = nand2 - nand1;
						float result_per;
						std::cout << "The number of Nands: ";
						if (result > 0) { std::cout << "+"; }
						std::cout << result;
						if (nand1 != 0 && result != 0) {			// percentage difference calculation
							result_per = result / float(nand1) * 100;
							std::cout << " (";
							if (result > 0) { std::cout << "+"; }
							std::cout << result_per << "%)";
						}
						std::cout << "\nThe number of DFFs: ";
						result = dff2 - dff1;
						if (result > 0) { std::cout << "+"; }
						std::cout << result;
						if (dff1 != 0 && result != 0) {				// percentage difference calculation
							result_per = result / float(dff1) * 100;
							std::cout << " (";
							if (result > 0) { std::cout << "+"; }
							std::cout << result_per << "%)";
						}
						 std::cout << std::endl;
				}

				std::cout << "\nWould you like to try another component? (y/n): ";			// in case a user would like to contine comparison
				std::string answer;
				std::cin >> answer;
				if (answer != "y" && answer != "Y") { break; }
			}
		}
	}
}
