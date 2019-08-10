// Filename: xed.cpp
// CS 530, Spring 2019
// Assignment #2: SIC/XE Disassembler
// Brendan Le Tourneau: cssc0414 / 819897930
// Connor Rowland TURN IN ACCOUNT: cssc0403 / 820436793
// Rahul Sharma: cssc0417 / 820943039

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include "xed.h"

using namespace std;

//This function creates our instruction set using a dictionary
//passed by reference
void createInstructionSet(map<int, string> &name_map){
    name_map[0x18]= "ADD3";
    name_map[0x58]= "ADDF3";
    name_map[0x90]= "ADDR2";
    name_map[0x40]= "AND3";
    name_map[0xB4]= "CLEAR2";
    name_map[0x28]= "COMP3";
    name_map.insert(pair<int, string>(0x88,"COMPF3"));
    name_map.insert(pair<int, string>(0xA0,"COMPR2"));
    name_map.insert(pair<int, string>(0x24,"DIV3"));
    name_map.insert(pair<int, string>(0x64,"DIVF3"));
    name_map.insert(pair<int, string>(0x9C,"DIVR2"));
    name_map.insert(pair<int, string>(0xC4,"FIX1"));
    name_map.insert(pair<int, string>(0xC0,"FLOAT1"));
    name_map.insert(pair<int, string>(0xF4,"HIO1"));
    name_map.insert(pair<int, string>(0x3C,"J3"));
    name_map.insert(pair<int, string>(0x30,"JEQ3"));
    name_map.insert(pair<int, string>(0x34,"JGT3"));
    name_map.insert(pair<int, string>(0x38,"JLT3"));
    name_map.insert(pair<int, string>(0x48,"JSUB3"));
    name_map.insert(pair<int, string>(0x00,"LDA3"));
    name_map.insert(pair<int, string>(0x68,"LDB3"));
    name_map.insert(pair<int, string>(0x50,"LDCH3"));
    name_map.insert(pair<int, string>(0x70,"LDF3"));
    name_map.insert(pair<int, string>(0x8,"LDL3"));
    name_map.insert(pair<int, string>(0x6C,"LDS3"));
    name_map.insert(pair<int, string>(0x74,"LDT3"));
    name_map.insert(pair<int, string>(0x04,"LDX3"));
    name_map.insert(pair<int, string>(0xD0,"LPS3"));
    name_map.insert(pair<int, string>(0x20,"MUL3"));
    name_map.insert(pair<int, string>(0x60,"MULF3"));
    name_map.insert(pair<int, string>(0x98,"MULR2"));
    name_map.insert(pair<int, string>(0xC8,"NORM1"));
    name_map.insert(pair<int, string>(0x44,"OR3"));
    name_map.insert(pair<int, string>(0xD8,"RD3"));
    name_map.insert(pair<int, string>(0xAC,"RMO2"));
    name_map.insert(pair<int, string>(0x4C,"RSUB3"));
    name_map.insert(pair<int, string>(0xA4,"SHIFTL2"));
    name_map.insert(pair<int, string>(0xA8,"SHIFTR2"));
    name_map.insert(pair<int, string>(0xF0,"SIO1"));
    name_map.insert(pair<int, string>(0xEC,"SSK3"));
    name_map.insert(pair<int, string>(0x0C,"STA3"));
    name_map.insert(pair<int, string>(0x78,"STB3"));
    name_map.insert(pair<int, string>(0x54,"STCH3"));
    name_map.insert(pair<int, string>(0x80,"STF3"));
    name_map.insert(pair<int, string>(0xD4,"STI3"));
    name_map.insert(pair<int, string>(0x14,"STL3"));
    name_map.insert(pair<int, string>(0x7C,"STS3"));
    name_map.insert(pair<int, string>(0xE8,"STSW3"));
    name_map.insert(pair<int, string>(0x84,"STT3"));
    name_map.insert(pair<int, string>(0x10,"STX3"));
    name_map.insert(pair<int, string>(0x1C,"SUB3"));
    name_map.insert(pair<int, string>(0x5C,"SUBF3"));
    name_map.insert(pair<int, string>(0x94,"SUBR2"));
    name_map.insert(pair<int, string>(0xB0,"SVC2"));
    name_map.insert(pair<int, string>(0xE0,"TD3"));
    name_map.insert(pair<int, string>(0xF8,"TIO1"));
    name_map.insert(pair<int, string>(0x2C,"TIX3"));
    name_map.insert(pair<int, string>(0xB8,"TIXR2"));
    name_map.insert(pair<int, string>(0xDC,"WD3"));
}

//This function open a file given the argument passed to
//the program, and a specified extension name. The function
//returns a file pointer to the now open file
FILE* openFile(char * argv[], const char * ext){
    FILE* fil;
    char buf[30];
    strcpy(buf, argv[1]);
    strcat(buf, ext);
    fil = fopen(buf, "r");
    return fil;
}

//This function will check for input errors and exit gracefully
//if an error occurs
void checkForInputErrors(int argc, char * argv[]){
    //If the command line arguments did not have the proper amout of arguments
    //We display a help message and exit
    if (argc != 2) {
        cout << "Command did not have two arguments" << endl
            << "Please use: xed <filename>" << endl;
        exit(EXIT_FAILURE);
    }
    //Here we open the two file we will be working with
    FILE *ptr[2] = {openFile(argv, ".obj"), openFile(argv, ".sym")};
    //If the file doesn't exist or can't open properly, we will then display
    //an error message, close the file(s) if we can and exit
    if (ptr[0] == NULL || ptr[1] == NULL) {
        cout << "One or more files failed to open." << endl
            << "Please specify a valid filename." << endl
            << "Use: xed <filename>" << endl;
        if (ptr[0] != NULL) fclose(ptr[0]);
        if (ptr[1] != NULL) fclose(ptr[1]);
        exit(EXIT_FAILURE);
    }
    //Since this is just a check, we don't need to return the file pointers
    //and can safely close these files
    fclose(ptr[0]);
    fclose(ptr[1]);
}

//This function populates the passed by reference vectors with the symbol
//files contents
void readSymFile(FILE **ptr, vector<string> &symbols, vector<int> &addresses,
        vector<char> &flags, vector<string> &names, vector<string> &literals,
        vector<int> &lengths, vector<int> &litAddresses){

    //temporary variable storage
    char line[150];
    char foo[8];
    int value;

    //move the file pointer over the symbol file headers
    fgets (line, 70, ptr[1]);
    fgets (line, 70, ptr[1]);

    //read symbol file contents
    while (true){

        //the fscanf(%s) means to scan the file until a list of characters
        //appears excluding whitespace and store the result in foo
        fscanf (ptr[1], "%s", foo);

        //If whats in foo is the string Name, then we have reached the
        //literal table and will break from this loop
        if(strcmp(foo, "Name") == 0) break;

        //otherwise, we will assume foo is now a symbol name and will
        //populate the symbols array
        symbols.push_back(foo);

        fscanf (ptr[1], "%x", &value);
        addresses.push_back(value);

        fscanf (ptr[1], "%s", foo);
        flags.push_back(foo[0]);
    }

    //Move the file pointer over the header info of the literal table
    fscanf (ptr[1], "%s", foo);
    fscanf (ptr[1], "%s", foo);
    fscanf (ptr[1], "%s", foo);
    fscanf (ptr[1], "%s", foo);

    //read until end of file
    int i = 0;
    while (fgets(line, 70, ptr[1])){
        fscanf (ptr[1], "%s", foo);
        //since a name doesn't have to be specified for a literal
        //we will check if foo is a name or a literal. The easiest
        //way to do that is see if the first character of foo is
        //an equals sign
        if (strncmp(&foo[0], "=", 1) == 0){
            //If it is, we know foo doesn't have a name
            names.push_back("");
            literals.push_back(foo);

            //This if statement corrects a bug where the fgets while
            //loop will sometimes get the last line twice and populate
            //accordingly, if that happens, we will get out of the function
            //entirely
            if (i != 0){
                if (strcmp(literals[i-1].c_str(),
                    literals[i].c_str()) == 0) {
                        literals.pop_back();
                        return;
                }
            }
            fscanf (ptr[1], "%x", &value);
            lengths.push_back(value);
            fscanf (ptr[1], "%x", &value);
            litAddresses.push_back(value);
        } else {
            //foo does have a name
            names.push_back(foo);
            if (i != 0){
                if (strcmp(names[i-1].c_str(),
                    names[i].c_str()) == 0) {
                        names.pop_back();
                        return;
                }
            }
            fscanf (ptr[1], "%s", foo);
            literals.push_back(foo);
            fscanf (ptr[1], "%x", &value);
            lengths.push_back(value);
            fscanf (ptr[1], "%x", &value);
            litAddresses.push_back(value);
        }
        i++;
    }
}

int readObjFile(FILE **ptr, vector<string> &symbols, vector<int> &addresses,
        vector<char> &flags, vector<string> &names, vector<string> &literals,
        vector<int> &lengths, vector<int> &litAddresses, ofstream &sicFile,
        ofstream &listing)
{

    //Create op table
    map<int, string> instructionSet;
    createInstructionSet(instructionSet);

    int address = 0; //Address to be printed on each line
    string label = ""; //Label for an address, only used if label is
                       //representative of an address

    string operand = ""; //Used to help print the operand
    string baseRegister = ""; //Used to keep track of label used in a LDB
                              //instruction

    int size = 150; //Max length of a line in record file
    int PIL; //Position In Line
    char buffer [size]; //Keep's the current line of the obj file in a buffer
    bool n,i,x,b,p,e = false; //To know what flags are set in the instruction
    bool lit = false; //Used to know if there is a literal operand
    bool LTORG = false; //Flag if LTORG has been printed
    bool firstLit = true; //Flag if we are still looking for the first literal
    bool op = false; //Flag for if the operand has a label
    bool ch = false; //Flag for if literal is one or more characters
    bool baseWasPrinted = false; //Flag to tell is BASE has been printed
    bool bRegisterLoaded = false; //Flag to tell if LDB had a label operand
    bool overboard = false;
    while( fgets (buffer , size , ptr[0])) //iterates per line in record file
    {
        PIL = 0; //Resetting PIL
        string line(buffer); //Creates a string that is the current line of the file, useful for substrings
        if(buffer[0] == 'H') { //Tesing for Header record
            //get the program name
            string programName = line.substr(1,6);
            PIL += 7;

            //reads starting address part of line and converts to hex int
            address = strtol(line.substr(PIL,6).c_str(), 0, 16);

            //write to file the first line of the program
            listing << setw(4) << setfill('0') << uppercase << hex << address << "  ";
            listing << setw(8) << setfill(' ') << left << programName;
            sicFile << setw(8) << setfill(' ') << left << programName;
            listing << " START   " << uppercase << address << endl;
            sicFile << " START   " << uppercase << address << endl;
        }
        if(buffer[0] == 'T') {
            address = strtol(line.substr(1,6).c_str(), 0, 16); //Gets starting address

            PIL+=9; //Position after 1 nibble T + 6 nibble address + 2 nibble record length
            while(buffer[PIL] != '\n') {
		if (overboard) break;
                n = false;
                i = false;
                x = false;
                b = false;
                op = false;
                firstLit = true;

                //Loop checking if current address has an associated symbol
                for(int j = 0; j < addresses.size(); j++) {
                    if(addresses[j] == address) {
                        label = symbols[j];
                    }
                }

                //Check if address is start of an LTORG
                for(int m=0; m < literals.size(); m++){
                    string typeOfLiteral = literals[m].substr(1,1);
                    string literal = literals[m].substr(3,lengths[m]);
                    string lit;
                    int actualLiteral = 0;
                    int possibleLiteral = 0;
                    int val = 0;

                    //See what type of literal it is
                    //This looks ahead from the current address plus the address
                    //listed in the literal file
                    if (strcmp (&typeOfLiteral[0], "X") == 0){
                        actualLiteral = strtol(literal.c_str(), 0, 16);
			//If we are not going to go over the length of the line
			//then search for it
                        if (line.size() > PIL + (2 * litAddresses[m]) + lengths[m] + 2){
                            possibleLiteral = strtol(line.substr(PIL + (2 * litAddresses[m]),
                                                     lengths[m]).c_str(), 0, 16);
                        }
                    } else if (strcmp (&typeOfLiteral[0], "D") == 0){
                        actualLiteral = strtol(literal.c_str(), 0, 10);
                        if (line.size() > PIL + (2 * litAddresses[m]) + lengths[m] + 2){
                            possibleLiteral = strtol(line.substr(PIL + (2 * litAddresses[m]),
                                                     lengths[m]).c_str(), 0, 16);
                        }
                    } else {
                        actualLiteral = 0;
                        for (int l=0; l < literal.size(); l++){
                            actualLiteral *= 256;
                            actualLiteral += literal[l];
                        }
                        if (line.size() > PIL + (2 * litAddresses[m]) + (2 * lengths[m]) + 2){
                            possibleLiteral = strtol(line.substr(PIL + (2 * litAddresses[m]),
                                                     (2 * lengths[m])).c_str(), 0, 16);
                        }
                    }

		    //If we have found our first literal, then lets print LTORG
                    if (possibleLiteral == actualLiteral && firstLit){
                        listing << setw(4) << setfill('0') << uppercase << hex << right << address << "  ";
                        listing << "         LTORG     " << endl;
                        sicFile << "         LTORG     " << endl;
                        firstLit = false;
                        LTORG = true;
                    }
		    //If we have found a literal before than we will see if we are
		    //currently at the spot in the text record where the literal occurs
		    //We are searching from the PIL, and not looking ahead for literals
                    if (LTORG){
                        string literal = literals[m].substr(3,lengths[m]);
                        string typeOfLiteral = literals[m].substr(1,1);
                        //See what type of literal it is
                        if (strcmp (&typeOfLiteral[0], "X") == 0){
                            actualLiteral = strtol(literal.c_str(), 0, 16);
                            if (line.size() > PIL + lengths[m] + 2){
                                possibleLiteral = strtol(line.substr(PIL, lengths[m]).c_str(), 0, 16);
                            }
                        } else if (strcmp (&typeOfLiteral[0], "D") == 0){
                            actualLiteral = strtol(literal.c_str(), 0, 10);
                            if (line.size() > PIL + lengths[m] + 2){
                                possibleLiteral = strtol(line.substr(PIL, lengths[m]).c_str(), 0, 16);
                            }
                        } else {
                            actualLiteral = 0;
		            //This converts the ascii character into a integer representation
                            for (int j=0; j < literal.size(); j++){
                                actualLiteral *= 256;
                                actualLiteral += literal[j];
                            }
                            //If checking for the literal means we reach after the line
                            //then don't check, its not there
                            if (line.size() > PIL + (2 * lengths[m]) + 2){
                                possibleLiteral = strtol(line.substr(PIL, (2 * lengths[m])).c_str(), 0, 16);
                            }
                            ch = true;
                        }
			//If we are currently at a literal in the text record, then we print it
                        if (possibleLiteral == actualLiteral){
                            listing << setw(4) << setfill('0') << uppercase << hex << right << address << "  ";
			    //Check if the literal has a name and print it
			    if (strcmp(&names[m][0], "") == 0){
				    listing << setw(8) << setfill(' ') << left << "*";
			    } else {
				    listing << setw(8) << setfill(' ') << left << names[m];
			    }
                            listing << setw(19) << setfill(' ') << left << literals[m];
                            listing << setw(10) << setfill(' ') << left << literal << endl;
	  		    //If this literal is a character we will update our position in
	  		    //line and address variables differently
                            if (ch) {
                                PIL += lengths[m] * 2;
                                address += lengths[m];
                                ch = false;
                            } else {
                                PIL += lengths[m];
                                address += (lengths[m] / 2);
                            }
                        }
                    }
                }
		if (PIL + 2 > line.size()) {
			overboard = true;
			continue;
		}

                int opCode = strtol(line.substr(PIL,2).c_str(), 0, 16);
                string fullObjCode;

                int remainder = opCode % 4; //Finds what n and i bit are set to
                opCode = opCode - remainder; //Sets opCode to the actual opCode

                //Separating instruction name from format type
                string instructionAndFormat = instructionSet[opCode];
                string instruction = instructionAndFormat.substr(0,instructionAndFormat.size()-1);
                int format = instructionAndFormat[instructionAndFormat.size()-1] - '0';

                //If the remainder isn't 0. It is a format 3 or 4 instruction
                if(remainder == 3) {
                    n = true;
                    i = true;
                } else if(remainder == 2) {
                    n = true;
                    i = false;
                } else if(remainder == 1) {
                    n = false;
                    i = true;
                } else {
                    n = false;
                    i = false;
                }

                if(format == 1) {
                    //Prints address
                    listing << setw(4) << setfill('0') << uppercase << hex << right << address << "  ";

                    //Prints label if there is one given the address
                    listing << setw(8) << setfill(' ') << left << label;
                    sicFile << setw(8) << setfill(' ') << left << label;

                    fullObjCode = line.substr(PIL,2); //Gets 1 byte, the OpCode
                    PIL+=2;
                    listing << " ";
                    sicFile << " ";
                    listing << setw(17) << setfill(' ') << left << instruction;
                    sicFile << setw(17) << setfill(' ') << left << instruction;
                    address+=1;

                } else if(format == 2) {
			if (PIL + 4 > line.size()) {
				overboard = true;
				continue;
			}
                    //Prints address
                    listing << setw(4) << setfill('0') << uppercase << hex << right << address << "  ";

                    //Prints label if there is one given the address
                    listing << setw(8) << setfill(' ') << left << label;
                    sicFile << setw(8) << setfill(' ') << left << label;

		    //Print instruction
                    listing << " ";
                    sicFile << " ";
                    listing << setw(8) << setfill(' ') << left << instruction;
                    sicFile << setw(8) << setfill(' ') << left << instruction;
                    fullObjCode = line.substr(PIL,4); //Gets 2 bytes

		    //Register arithmetic to match numbers to letters
                    int reg1 = strtol(fullObjCode.substr(2,1).c_str(), 0, 10);
                    int reg2 = strtol(fullObjCode.substr(3,1).c_str(), 0, 10);
                    char register1;
                    char register2;
                    char reg = reg1;
                    char registrar;
                    for (int i=0; i<2; i++){
                        switch (reg) {
                            case 0 :
                                registrar = 'A';
                                break;
                            case 1 :
                                registrar = 'X';
                                break;
                            case 2 :
                                registrar = 'L';
                                break;
                            case 3 :
                                registrar = 'B';
                                break;
                            case 4 :
                                registrar = 'S';
                                break;
                            case 5 :
                                registrar = 'T';
                                break;
                            case 6 :
                                registrar = 'F';
                                break;
                            case 8 :
                                registrar = '*';
                                break;
                            default :
                                registrar = ' ';
                        }
                        reg = reg2;
                        if (i == 0){
                            register1 = registrar;
                            registrar = register2;
                        }
                        if (i == 1){
                            register2 = registrar;
                        }
                    }
		    //Formatting the outputs based on their instructions
                    if ( opCode == 0xB4 || opCode == 0xB8 ){
                        listing << setw(9) << setfill(' ') << left << register1;
                        sicFile << setw(9) << setfill(' ') << left << register1;
                    } else if ( opCode == 0xA4 || opCode == 0xA8 ){
                        listing << register1 << "," << reg2 << "      ";
                        sicFile << register1 << "," << reg2 << "      ";
                    } else if ( opCode == 0xB0 ){
                        listing << setw(9) << setfill(' ') << left << reg1;
                        sicFile << setw(9) << setfill(' ') << left << reg1;
                    } else {
                        listing << register1 << "," << register2 << "      ";
                        sicFile << register1 << "," << register2 << "      ";
                    }
                    listing << fullObjCode;
                    PIL+=4;
                    address+=2;
                }
                //We clumped together format's 3 and 4 in our opcode table
                else if(format == 3) {
			if (PIL + 6 > line.size()) {
				overboard = true;
				continue;
			}
                    int xbpeInt = strtol(line.substr(PIL+2,1).c_str(), 0, 16);
                    int targetAddress = strtol(line.substr(PIL+3,3).c_str(), 0, 16);

                    //Check if b bit is set
                    if ((xbpeInt >= 4 && xbpeInt <= 7) || xbpeInt >= 12) {
                        //To help with printing the instruction
                        b = true;
                        //If the b register was previously loaded using LDB, then
                        //we will use the operand that was loaded to print the
                        //BASE assembly directive
                        if (bRegisterLoaded){
                            listing << setw(4) << setfill('0') << uppercase << hex << right << address << "  ";
                            listing << "         BASE    " << uppercase << baseRegister << endl;
                            sicFile << "         BASE    " << uppercase << baseRegister << endl;
                            operand = baseRegister;
                            baseWasPrinted = true;
                        }
                    }

                    //Check if p bit is set
                    if (xbpeInt == 2 || xbpeInt == 3 || xbpeInt == 6 || xbpeInt == 7 ||
                        xbpeInt == 10 || xbpeInt == 11 || xbpeInt == 14 || xbpeInt == 15){
                        //If we have been set to base relative addressing and we now have a PC
                        //relative addressing instruction, we then need to specify NOBASE
                        if (baseWasPrinted) {
                            listing << setw(4) << setfill('0') << uppercase << hex << right << address << "  ";
                            listing << "         NOBASE  " << uppercase << endl;
                            sicFile << "         NOBASE  " << uppercase << endl;
                            baseWasPrinted = false;
                        }
                    }

                    //Prints address
                    listing << setw(4) << setfill('0') << uppercase << hex << right << address << "  ";

                    //Prints label if there is one given the address
                    listing << setw(8) << setfill(' ') << left << label;
                    sicFile << setw(8) << setfill(' ') << left << label;

                    address+=3;

                    //Check if e bit is set, if it isn't we have a format 3 instruction
                    if (xbpeInt % 2 == 0) {
                        fullObjCode = line.substr(PIL,6); //Gets 3 bytes
                        //Check if x bit is set
                        if (xbpeInt >= 8) x = true;

                        //Check if p bit is set
                        if (xbpeInt == 2 || xbpeInt == 3 || xbpeInt == 6 || xbpeInt == 7 ||
                            xbpeInt == 10 || xbpeInt == 11 || xbpeInt == 14 || xbpeInt == 15){
                            targetAddress += address;
                        }

                        //If TA is 1XXX in hex, that means it was a backward reference
                        //and we need to only take the first three digits
                        if (targetAddress > 4096) targetAddress -= 4096;

                        //If there is something loaded into the b reg and we have a b flag
                        if (bRegisterLoaded && b){
                            //Lets note we are using the baseRegister string
                            bRegisterLoaded = false;
                        } else {
                            //Otherwise lets look for the possible operand
                            for(int j = 1; j < addresses.size(); j++) {
                                if(addresses[j] == targetAddress) {
                                    operand = symbols[j];
                                    op = true;
                                }
                            }
                            int tmp = PIL;
                            PIL = (targetAddress * 2) + 9;
                            //See if the target address is a literal
                            for(int n=0; n < literals.size(); n++){
                                string typeOfLiteral = literals[n].substr(1,1);
                                string literal = literals[n].substr(3,lengths[n]);
                                lit = false;
                                int actualLiteral = 0;
                                int possibleLiteral = 0;
                                int val = 0;

                                //See what type of literal it is
                                //This looks ahead from the current address plus the address
                                //listed in the literal file
                                if (strcmp (&typeOfLiteral[0], "X") == 0){
                                    actualLiteral = strtol(literal.c_str(), 0, 16);
                                    if (line.size() > PIL + lengths[n]){
                                        possibleLiteral = strtol(line.substr(PIL,
                                                                 lengths[n]).c_str(), 0, 16);
                                    }
                                } else if (strcmp (&typeOfLiteral[0], "D") == 0){
                                    actualLiteral = strtol(literal.c_str(), 0, 10);
                                    if (line.size() > PIL + lengths[n]){
                                        possibleLiteral = strtol(line.substr(PIL,
                                                                 lengths[n]).c_str(), 0, 16);
                                    }
                                } else {
                                    actualLiteral = 0;
                                    for (int i=0; i < literal.size(); i++){
                                        actualLiteral *= 256;
                                        actualLiteral += literal[n];
                                    }
                                    if (line.size() > PIL + (2 * lengths[n])){
                                        possibleLiteral = strtol(line.substr(PIL,
                                                                 (2 * lengths[n])).c_str(), 0, 16);
                                    }
                                }

                                if (possibleLiteral == actualLiteral){
                                        operand = literals[n];
                                        lit = true;
                                        break;
                                }
                            }
                            PIL = tmp;
			}

                        PIL += 6;
			//Print instruction
                        listing << " ";
                        sicFile << " ";
                        listing << setw(7) << setfill(' ') << left << instruction;
                        sicFile << setw(7) << setfill(' ') << left << instruction;
                        string format;

			//Format output based on what flags were set
                        if (i && !n && !x && !b){
                            if (op){
                                listing << "#" << setw(9) << setfill(' ') << operand;
                                sicFile << "#" << setw(9) << setfill(' ') << operand;
                            } else {
                                listing << "#" << setw(9) << setfill(' ') << targetAddress;
                                sicFile << "#" << setw(9) << setfill(' ') << targetAddress;
                            }
                        } else if (i && !n && !x && b){
                             listing << "#" << setw(9) << setfill(' ') << baseRegister;
                             sicFile << "#" << setw(9) << setfill(' ') << baseRegister;
                        } else if (!i && n && !x && !b){
                             listing << "@" << setw(9) << setfill(' ') << operand;
                             sicFile << "@" << setw(9) << setfill(' ') << operand;
                        } else if (!i && n && !x && b){
                             listing << "@" << setw(9) << setfill(' ') << baseRegister;
                             sicFile << "@" << setw(9) << setfill(' ') << baseRegister;
                        } else if (!x && !b){
			     //If a literal was found at the target address we will print that
                             if (lit){
                                 listing << setw(10) << setfill(' ') << operand;
                                 sicFile << setw(10) << setfill(' ') << operand;
                             } else {
                                 listing << " ";
                                 sicFile << " ";
                                 listing << setw(9) << setfill(' ') << operand;
                                 sicFile << setw(9) << setfill(' ') << operand;
                             }
                        } else if (!x && b){
                             if (lit){
                                 listing << setw(10) << setfill(' ') << operand;
                                 sicFile << setw(10) << setfill(' ') << operand;
                             } else {
                                 listing << " ";
                                 sicFile << " ";
                                 listing << setw(9) << setfill(' ') << baseRegister;
                                 sicFile << setw(9) << setfill(' ') << baseRegister;
			    }
                        } else if (x && !b){
                             listing << " ";
                             sicFile << " ";
                             format = operand + ",X";
                             listing << setw(9) << setfill(' ') << format;
                             sicFile << setw(9) << setfill(' ') << format;
                        } else if (x && b){
                             listing << " ";
                             sicFile << " ";
                             format = baseRegister + ",X";
                             listing << setw(9) << setfill(' ') << format;
                             sicFile << setw(9) << setfill(' ') << format;
                        }

                        listing << " ";
                        sicFile << " ";
                        listing << fullObjCode;
                    }
                    //Format 4 instruction
                    else {
			if (PIL + 8 > line.size()) {
				overboard = true;
				continue;
			}
                        int targetAddress4 = strtol(line.substr(PIL+3,5).c_str(), 0, 16);

                        //Find label for operand field
                        operand = "";
                        for(int j = 1; j < addresses.size() ; j++) {
                            if(addresses[j] == targetAddress4) {
                                operand = symbols[j];
                            }
                        }
                        int tmp = PIL;
                        PIL = (targetAddress4 * 2) + 9;
                        //See if the target address is a literal
                        for(int o=0; o < literals.size(); o++){
                            string typeOfLiteral = literals[o].substr(1,1);
                            string literal = literals[o].substr(3,lengths[o]);
                            lit = false;
                            int actualLiteral = 0;
                            int possibleLiteral = 0;
                            int val = 0;

                            //See what type of literal it is
                            //This looks ahead from the current address plus the address
                            //listed in the literal file
                            if (strcmp (&typeOfLiteral[0], "X") == 0){
                                actualLiteral = strtol(literal.c_str(), 0, 16);
                                if (line.size() > PIL + lengths[o] + 1){
                                    possibleLiteral = strtol(line.substr(PIL,
                                                             lengths[o]).c_str(), 0, 16);
                                }
                            } else if (strcmp (&typeOfLiteral[0], "D") == 0){
                                actualLiteral = strtol(literal.c_str(), 0, 10);
                                if (line.size() > PIL + lengths[o] + 1){
                                    possibleLiteral = strtol(line.substr(PIL,
                                                             lengths[o]).c_str(), 0, 16);
                                }
                            } else {
                                actualLiteral = 0;
                                for (int i=0; i < literal.size(); i++){
                                    actualLiteral *= 256;
                                    actualLiteral += literal[o];
                                }
                                if (line.size() > PIL + (2 * lengths[o]) + 1){
                                    possibleLiteral = strtol(line.substr(PIL,
                                                             (2 * lengths[o])).c_str(), 0, 16);
                                }
                            }

                            if (possibleLiteral == actualLiteral){
                                    operand = literals[o];
                                    lit = true;
                                    break;
                            }
                        }
                        PIL = tmp;

                        fullObjCode = line.substr(PIL,8); //Gets 4 bytes
			//Increment position in line and address
                        PIL += 8;
                        address += 1;
                        listing << "+";
                        sicFile << "+";
                        listing << setw(7) << setfill(' ') << left << instruction;
                        sicFile << setw(7) << setfill(' ') << left << instruction;

                        if (i && !n){
                             listing << "#" << setw(10) << setfill(' ') << operand;
                             sicFile << "#" << setw(10) << setfill(' ') << operand;
                        } else if (!i && n){
                             listing << "@" << setw(10) << setfill(' ') << operand;
                             sicFile << "@" << setw(10) << setfill(' ') << operand;
                        } else {
			    if (!lit){
			        listing << " ";
			        sicFile << " ";
			    }
                            listing << setw(10) << setfill(' ') << operand;
                            sicFile << setw(10) << setfill(' ') << operand;
                        }

                        //Print object code
                        listing << fullObjCode;
                }
		//If LDB was used, take note and remember the operand
                if (opCode == 0x68){
                    baseRegister = operand;
                    bRegisterLoaded = true;
                }
                label = "";
                operand = "";
                listing << endl;
                sicFile << endl;
		}
	    }
	}
    }
    return address;
}

//After we have read the object file, we need to declare the data at the end
//of the symbol file
void initializeData(FILE **ptr, vector<string> &symbols, vector<int> &addresses,
                    vector<char> &flags, int address, int endingAddress,
                    ofstream & sicFile, ofstream & listing){
    //Iterate through the addresses in the symbol file
    for(int j = 1; j < addresses.size() ; j++) {
        //Make the flag a string with a null pointer so we can compare it
        char flag[2] = {flags[j], '\0'};
        //If the symbol address if after the address of all the instructions
        if(address <= addresses[j]) {
            //We will see if it is a relative symbol
            if (strcmp(&flag[0], "R") == 0){
                //If so, we will print the address and the associated symbol
                listing << setw(4) << setfill('0') << uppercase << hex << right << addresses[j] << "  ";
                listing << setw(9) << setfill(' ') << left << symbols[j];
                sicFile << setw(9) << setfill(' ') << left << symbols[j];
                //If we are reading the last symbol, we need to calculate its
                //length to the ending address of the program
                if (j == addresses.size() - 1){
                    //We find the difference to the end of the program
                    int difference =  endingAddress - addresses[j];
                    //If the length is divisible by 3, we can use RESW
                    if (difference % 3 == 0){
                           listing << setw(8) << setfill(' ') << "RESW";
                           sicFile << setw(8) << setfill(' ') << "RESW";
                           listing << setw(10) << setfill(' ') << dec << (difference / 3) << endl;
                           sicFile << setw(10) << setfill(' ') << dec << (difference / 3) << endl;
                           listing << setw(15) << setfill(' ') << " ";
                           sicFile << setw(9) << setfill(' ') << " ";
                           listing << setw(8) << setfill(' ') << "END";
                           sicFile << setw(8) << setfill(' ') << "END";
                           listing << symbols[0] << endl;
                           sicFile << symbols[0] << endl;
                   //Else we will use RESB
                   } else {
                           listing << setw(8) << setfill(' ') << "RESB";
                           sicFile << setw(8) << setfill(' ') << "RESB";
                           listing << setw(10) << setfill(' ') << dec << difference << endl;
                           sicFile << setw(10) << setfill(' ') << dec << difference << endl;
                           listing << setw(15) << setfill(' ') << " ";
                           sicFile << setw(9) << setfill(' ') << " ";
                           listing << setw(8) << setfill(' ') << "END";
                           sicFile << setw(8) << setfill(' ') << "END";
                           listing << symbols[0] << endl;
                           sicFile << symbols[0] << endl;
                   }
                //If we are not reading the last symbol
                } else {
                    //We can calculate the length of data by indexing ahead
                    //in the symbol table
                    int difference = addresses[j+1] - addresses[j];
                    //If the memory is divisible by 3, we will use RESW
                    if (difference % 3 == 0){
                           listing << setw(8) << setfill(' ') << "RESW";
                           sicFile << setw(8) << setfill(' ') << "RESW";
                           listing << setw(10) << setfill(' ') << dec << (difference / 3) << endl;
                           sicFile << setw(10) << setfill(' ') << dec << (difference / 3) << endl;
                    //Else we will use RESB
                    } else {
                           listing << setw(8) << setfill(' ') << "RESB";
                           sicFile << setw(8) << setfill(' ') << "RESB";
                           listing << setw(10) << setfill(' ') << dec << difference << endl;
                           sicFile << setw(10) << setfill(' ') << dec << difference << endl;
                    }
                }
            }
        }
    }
}

//This function finds the ending address of the file
int findEndingAddress(FILE **ptr){
        int size = 100;
        char buffer[size];
        rewind(ptr[0]);
        fgets (buffer, size , ptr[0]);
        string line(buffer);
        int endingAddress = strtol(line.substr(13,6).c_str(), 0, 16);
        return endingAddress;
}

int main(int argc, char * argv[]){
    //check that the files exist and can open
    checkForInputErrors(argc, argv);

    //open files and return their pointers
    FILE *ptr[2] = {openFile(argv, ".obj"), openFile(argv, ".sym")};

    //Allocate memory to store the symbols and literals from symbol table
    //Symbols
    vector<string> symbols;
    vector<int> addresses;
    vector<char> flags;

    //Literals
    vector<string> names;
    vector<string> literals;
    vector<int> lengths;
    vector<int> litAddresses;
    readSymFile(ptr, symbols, addresses, flags, names,
            literals, lengths, litAddresses);

    //Create output files
    ofstream sicFile, listing;
    char buff[30];
    strcpy(buff, argv[1]);
    strcat(buff, ".sic");
    sicFile.open(buff);

    char buff2[30];
    strcpy(buff2, argv[1]);
    strcat(buff2, ".lis.txt");
    listing.open(buff2);

    int address = readObjFile(ptr, symbols, addresses, flags, names,
            literals, lengths, litAddresses, sicFile, listing);

    int endingAddress = findEndingAddress(ptr);

    initializeData(ptr, symbols, addresses, flags, address, endingAddress,
                   sicFile, listing);

    sicFile.close();
    listing.close();
    fclose(ptr[0]);
    fclose(ptr[1]);
    return 0;
}
