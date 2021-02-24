#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <regex>
#include "assembler.h"

bool is_integer(std::string num) {
  return std::regex_match(num, std::regex("[+-]?[0-9]+"));
}

void Assembler::create_tables() {
  // symbol table initiation
  _symbol_table.clear();
  _asm_file.clear();
  _symbol_table["R0"] = 0;
  _symbol_table["R1"] = 1;
  _symbol_table["R2"] = 2;
  _symbol_table["R3"] = 3;
  _symbol_table["R4"] = 4;
  _symbol_table["R5"] = 5;
  _symbol_table["R6"] = 6;
  _symbol_table["R7"] = 7;
  _symbol_table["R8"] = 8;
  _symbol_table["R9"] = 9;
  _symbol_table["R10"] = 10;
  _symbol_table["R11"] = 11;
  _symbol_table["R12"] = 12;
  _symbol_table["R13"] = 13;
  _symbol_table["R14"] = 14;
  _symbol_table["R15"] = 15;

  _symbol_table["SP"] = 0;
  _symbol_table["LCL"] = 1;
  _symbol_table["ARG"] = 2;
  _symbol_table["THIS"] = 3;
  _symbol_table["THAT"] = 4;

  _symbol_table["SCREEN"] = 16384;
  _symbol_table["KBD"] = 24576;

  // c-instruction comp
  _c_comp["0"] =   "0101010";
  _c_comp["1"] =   "0111111";
  _c_comp["-1"] =  "0111010";
  _c_comp["D"] =   "0001100";
  _c_comp["A"] =   "0110000";
  _c_comp["M"] =   "1110000";
  _c_comp["!D"] =  "0001101";
  _c_comp["!A"] =  "0110001";
  _c_comp["!M"] =  "1110001";
  _c_comp["-D"] =  "0001111";
  _c_comp["-A"] =  "0110011";
  _c_comp["-M"] =  "1110011";
  _c_comp["D+1"] = "0011111";
  _c_comp["A+1"] = "0110111";
  _c_comp["M+1"] = "1110111";
  _c_comp["D-1"] = "0001110";
  _c_comp["A-1"] = "0110010";
  _c_comp["M-1"] = "1110010";
  _c_comp["D+A"] = "0000010";
  _c_comp["D+M"] = "1000010";
  _c_comp["D-A"] = "0010011";
  _c_comp["D-M"] = "1010011";
  _c_comp["A-D"] = "0000111";
  _c_comp["M-D"] = "1000111";
  _c_comp["D&A"] = "0000000";
  _c_comp["D&M"] = "1000000";
  _c_comp["D|A"] = "0010101";
  _c_comp["D|M"] = "1010101";

  // c-instruction dest
  _c_dest[""] =    "000";
  _c_dest["M"] =   "001";
  _c_dest["D"] =   "010";
  _c_dest["MD"] =  "011";
  _c_dest["A"] =   "100";
  _c_dest["AM"] =  "101";
  _c_dest["AD"] =  "110";
  _c_dest["AMD"] = "111";

  // c-instruction jump
  _c_jump[""] =    "000";
  _c_jump["JGT"] = "001";
  _c_jump["JEQ"] = "010";
  _c_jump["JGE"] = "011";
  _c_jump["JLT"] = "100";
  _c_jump["JNE"] = "101";
  _c_jump["JLE"] = "110";
  _c_jump["JMP"] = "111";
}

bool Assembler::read_file(std::string filename) {
    std::ifstream inputfile;
    inputfile.open(filename);
    if (!inputfile.is_open()) {
      std::cout << "Unable to open .asm file";
      return false;
    }

    _name = filename.substr(0, filename.find("."));  // save filename for the binary file

    std::string line;
    int line_count = 0;  // for labels
    while (std::getline(inputfile, line) ) {
      line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());  // remove all spaces
      line = line.substr(0, line.find("//"));   // remove all comments
      if (line.length() != 0) {           // remove empty lines
        if (line.find("(") == 0) {        // handle labels
          int pos = line.find(")");
          if ((pos != line.length() - 1) || line.length() < 3) {
            std::cout << "ERROR: incorrect label declaration in \"" << line << "\"\n";
            return false;
          }
          std::string label = line.substr(1, line.length() - 2);
          if (_symbol_table.count(label)) {
            std::cout << "ERROR: label already exists in the table: \"" << line << "\"\n";
            return false;
          }
          _symbol_table[label] = line_count;  // add to the symbol table

        } else {
          _asm_file.push_back(line);  // save line
          line_count++;
        }
      }
    }
    inputfile.close();
    return true;
}

bool Assembler::build_binary() {
  std::ofstream outputfile;
  outputfile.open(_name + ".hack");
  if (!outputfile.is_open()) {
    std::cout << "Unable to open .hack file";
    return false;
  }

  int variable_counter = 16;
  for (std::vector<std::string>::iterator line = _asm_file.begin(); line < _asm_file.end(); line++) {
    if (line->find("@") == 0) {           // A-instruction
      std::string value = line->substr(1);
      if (is_integer(value)) {
        std::bitset<16> bit_value(stoi(value));
        outputfile << bit_value << std::endl;
      }
      else if (_symbol_table.count(value)) {     // label, pre-defined symbol, or existing variable
        std::bitset<16> bit_value(_symbol_table[value]);
        outputfile << bit_value << std::endl;
      } else {                                  // new variable
        std::bitset<16> bit_value(variable_counter);
        outputfile << bit_value << std::endl;
        _symbol_table[value] = variable_counter;
        variable_counter++;
      }
    }

    else {            // C-instruction
      std::string comp, dest, jump;
      int pos1 = line->find("=");
      int pos2 = line->find(";");
      if (pos1 != std::string::npos) { dest = line->substr(0, pos1); }  // there is dest
      else {
        pos1 = -1;
        dest = "";
      }
      if (pos2 != std::string::npos) { jump = line->substr(pos2 + 1); }   // there is jump
      else {
        jump = "";
      }
      comp = line->substr(pos1 + 1, pos2);
      outputfile << "111" + _c_comp[comp] + _c_dest[dest] + _c_jump[jump] << std::endl;
    }
  }
  outputfile.close();
  return true;


}
