#ifndef NAND_GUI_H
#define NAND_GUI_H

#include <string>
#include "chip_library.h"

class NandGUI {
	public:
		void main_menu();														// the calculator menu
	private:
		void print_main_menu();											// prints the calculator menu
		int get_number_input(int menu_items);				// allows a user to select the number from 0 to menu_items
		void select_main_menu_item(int input);
		void go_back_to_main_menu();
		void menu_item_1() ;
		void menu_item_2();
		void menu_item_3();
		void menu_item_4();
		void display_all_components();									// displays all components in the library
		void update_library();													// updates the chip library from a file
		void calculate_menu();
		void print_calculate_menu();
		void select_calculate_menu_item(int input);
		void calculate_item_1();
		void calculate_item_2();
		void calculate_item_3();
		void calculate_item_4();
		std::string ask_name();													// asks the name of a component and check that it is in the chip library
		int calculate_Nand(std::string name, ChipLibrary library);					// calculates the number of Nands
		int calculate_DFF(std::string name, ChipLibrary library);						// calculates the number of DFFs
		void calculate_Nand_inc_DFF(std::string name, int nand_in_dff);  // calculates the number of Nands including the Nands in DFFs
		void compare();																	// compares a selected component from two libraries
		ChipLibrary _components;												// the main chip library
};

#endif
