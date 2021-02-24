#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <map>

class Assembler {
	public:
		void create_tables();
		bool read_file(std::string filename);
		bool build_binary();
	private:
		std::vector <std::string> _asm_file;
		std::map <std::string, int> _symbol_table;
		std::map <std::string, std::string> _c_comp;
		std::map <std::string, std::string> _c_dest;
		std::map <std::string, std::string> _c_jump;
		std::string _name;
};

#endif
