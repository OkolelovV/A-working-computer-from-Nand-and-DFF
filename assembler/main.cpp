#include "assembler.h"

int main() {
	Assembler assembler;
	assembler.create_tables();
  assembler.read_file("Example_Add.asm");
	assembler.build_binary();

	assembler.create_tables();
  assembler.read_file("Example_Max.asm");
	assembler.build_binary();

	assembler.create_tables();
  assembler.read_file("Example_Rect.asm");
	assembler.build_binary();

	assembler.create_tables();
  assembler.read_file("Example_Pong.asm");
	assembler.build_binary();
	return 0;
}
