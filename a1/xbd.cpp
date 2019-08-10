//Brendan LeTourneau and Connor Rowland
//CSSC0414 and CSSC0403
//CS 570, Spring 2019
//Lab #1, Hexadecimal Dump - XBD
//xbd.cpp
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
string numToBinary(int);
void outputHex(unsigned char *, long);
void outputBinary(unsigned char *, long);
unsigned char * readFile(const char *);

long lSize;

int main (int argc, char* argv[]) {
    
    // position of the filename in argv
    int fileIndex = (argc == 3)? 2:1;

    // read all bytes into a buffer
    unsigned char * buffer = readFile(argv[fileIndex]);
   
    // output binary if flag is given, otherwise output hex 
    (argc == 3)? outputBinary(buffer, lSize) : outputHex(buffer, lSize);
    cout << endl;
    
    //delete buffer
    free(buffer);
    return 0;
}

unsigned char * readFile(const char * fileName)
{
    FILE * pFile;

    // open file for reading in binary bytes
    pFile = fopen(fileName, "rb");

    // if we can't open the file an error message will be displayed
    if (pFile == NULL) {fputs ("file error",stderr); exit (1);}

    // find the end of the file and report how many bytes there are
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    
    unsigned char * buffer = (unsigned char*) malloc (sizeof(char)*lSize);

    //read into an unsigned character array the amount of bytes determined by lSize
    fread(buffer,1,lSize,pFile);
    fclose(pFile);
    
    return buffer;
}

void outputHex(unsigned char * buffer, long lSize)
{
    
    int address = 0000000;
    int bytesPerRow = 16;
    
    // for each byte in the character array we will,   
    for(int i = 0; i < lSize; i++){

        // print the ASCII if we are not at byte 0 but are a multiple of 16
        if(i % bytesPerRow == 0 && i != 0){
            cout << " ";
            for(int j = bytesPerRow; j > 0; j--){
                if(buffer[i-j] < ' ' || buffer[i-j] > '~')
                    cout << '.';
                else
                    cout<< buffer[i-j];
            }
            cout << endl;
        }
       
        // print the relative address in the file if we are at byte 0 or a byte multiple of 16 
        if(i % bytesPerRow == 0){
            cout << setfill('0') << setw(7) << hex << address << ": ";
            address += bytesPerRow;
        }
        cout << setfill('0') << setw(2) << hex << int(buffer[i]);

        // print a space to separate two bytes
        if((i % 2) - 1 == 0) cout<< " ";

        // on the last line we will 
        // calculate the amount of remaining spaces needed, print them, 
        // then print the rest of the file's ASCII content
        if(i == lSize - 1){

            // calculate how many bytes we display in the last row
            int bytesInLastRow = lSize % bytesPerRow;

            // calculate how many bytes we need to fill with spaces
            int bytesRemaining = bytesPerRow - bytesInLastRow;

            // if the file has an odd number of bytes, 
            // we need to add a space to fill the remaining hexadecimal character
            int offset = (bytesRemaining % 2);

            // calculate the amount of spaces we need to display between the number
            // of bytes we have remaining to display
            int inBetweeners = bytesRemaining / 2;

            // we add 2 at the end because we normally print those spaces before
            // printing the ASCII
            int numberOfSpaces = (bytesRemaining * 2) + inBetweeners + offset + 2;
            cout << setfill(' ') << setw(numberOfSpaces);

            // print the remaining ASCII bytes in the last row
            for(int j = bytesInLastRow - 1; j >= 0; j--){
                if(buffer[i-j] < ' ' || buffer[i-j] > '~')
                    cout << '.';
                else
                    cout<< buffer[i-j];
            }
        }
    }
}

void outputBinary(unsigned char * buffer, long lSize)
{
    int address = 0000000;
    int bytesPerRow = 6;

    // for each byte in the file, we will
    for(int i = 0; i < lSize; i++){

        // print the ASCII if we are not at byte 0 but are a multiple of 6
        if(i % bytesPerRow == 0 && i != 0){
            cout << " ";
            for(int j = bytesPerRow; j > 0; j--){
                if(buffer[i-j] < ' ' || buffer[i-j] > '~')
                    cout << '.';
                else
                    cout<< buffer[i-j];
            }
            cout << endl;
        }

        // print the address if we are at a multiple of 6 or at byte 0
        if(i % bytesPerRow == 0){
            cout << setfill('0') << setw(7) << hex << address << ": ";
            address += bytesPerRow;
        }

        // convert hex byte to binary and print it
        cout << numToBinary(int(buffer[i]));

        // print remaining spaces and ASCII if we are in the last row
        if(i == lSize - 1){
            int bytesInLastRow = lSize % bytesPerRow;
            int numberOfBytesRemaining = bytesPerRow - bytesInLastRow;
            int numberOfSpaces = (numberOfBytesRemaining * 9) + 3;
            cout << setfill(' ') << setw(numberOfSpaces);
            for(int j = bytesInLastRow - 1; j >= 0; j--){
                if(buffer[i-j] < ' ' || buffer[i-j] > '~')
                    cout << '.';
                else
                    cout<< buffer[i-j];
            }
        }
        // print a space after each byte
        cout << " ";
    }
    
}

// convert hexadecimal number to binary
string numToBinary(int num){
    string returnBinary = "";

    // find first digit in a hexadecimal byte
    int digit = num/16;
    for(int i = 0; i < 2; i++)
    {
        switch(digit)
        {
                
            case 0:
                returnBinary.append("0000");
                break;
            case 1:
                returnBinary.append("0001");
                break;
            case 2:
                returnBinary.append("0010");
                break;
            case 3:
                returnBinary.append("0011");
                break;
            case 4:
                returnBinary.append("0100");
                break;
            case 5:
                returnBinary.append("0101");
                break;
            case 6:
                returnBinary.append("0110");
                break;
            case 7:
                returnBinary.append("0111");
                break;
            case 8:
                returnBinary.append("1000");
                break;
            case 9:
                returnBinary.append("1001");
                break;
            case 10:
                returnBinary.append("1010");
                break;
            case 11:
                returnBinary.append("1011");
                break;
            case 12:
                returnBinary.append("1100");
                break;
            case 13:
                returnBinary.append("1101");
                break;
            case 14:
                returnBinary.append("1110");
                break;
            case 15:
                returnBinary.append("1111");
                break;
        }
        // find second hexadecimal digit in byte
        digit = num%16;
    }
    return returnBinary;
}
