# Nand and DFF calculator
This program can calculates the number of Nand and DFF in any given chip (C++). Version 2 of this program can calculate the number of transistors in any given chip (C++) and can be found in a different folder.
I  recorded a video where I explain how to use Nand & D Flip-flop calculator, you can find it on YouTube: https://youtu.be/_LZWHakxQ3Q

ChipLibrary: This class  represents a chip library. It stores the data of each component and provide methods for accessing and mutating the library. The data stored is the name, the number of Nands, and the number of DFF of each component.

NandGUI: This class is all about the program interface and the functions of the calculator. It stores one ChipLibrary as the main chip library. Most of the methods are for displaying different menus and for manipulating and displaying the data returned from ChipLibrary. The update_library()  method invokes the mian ChipLibrary updating. The compare() methods creates two ChipLibrary classes and compares them.
