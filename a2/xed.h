// Filename: xed.h
// CS 530, Spring 2019
// Assignment #2: SIC/XE Disassembler
// Brendan Le Tourneau: cssc0414 / 819897930
// Connor Rowland - TURN IN ACCOUNT: cssc0403 / 820436793
// Rahul Sharma: cssc0417 / 820943039

#ifndef xed_h
#define xed_h

using namespace std;



void createInstructionSet(map<int, string>  &name_map);

FILE* openFile(char * argv[], const char * ext);

void checkForInputErrors(int argc, char * argv[]);

void readSymFile(FILE **ptr, vector<string> &symbols, vector<int> &addresses,
                 vector<char> &flags, vector<string> &names, vector<string> &literals,
                 vector<int> &lengths, vector<int> &litAddresses);

int readObjFile(FILE **ptr, vector<string> &symbols, vector<int> &addresses,
                vector<char> &flags, vector<string> &names, vector<string> &literals,
                vector<int> &lengths, vector<int> &litAddresses);

void initializeData(FILE **ptr, vector<string> &symbols, vector<int> &addresses,
                    vector<char> &flags, int address, int endingAddress);

int findEndingAddress(FILE **ptr);

int main(int argc, char * argv[]);

#endif
