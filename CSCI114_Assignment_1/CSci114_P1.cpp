/*
Saishnu Ramesh Kumar (300758706)
CSCI 114 - Assignment 1

To execute program:

    Locate the file destination and directory using: cd C:/Users/<username>/Desktop/CSCI114_Assignment_1
    mv <filename> data.in       //Moves the file within the specified filename to data.in
    g++ CSci114_P1.cpp          //Compiles the program
    ls                          //Lists the files and directories in current directory
    ./a.exe                     //Executes the compiled program in the current directory
    ls 
    mv data.out <filename>      //Moves the file into a new file with another name
    ls

*/ 


#include <iostream>
#include <fstream>

using namespace std;

int main(){
    ifstream readFile;
    ofstream outFile;

    readFile.open("data.in", ios::in | ios::binary);
    outFile.open("data.out", ios::out | ios::binary);

    char a;
    while(!readFile.eof()){
        readFile.read((char*)&a, sizeof(a));
        outFile.write((char*)&a, sizeof(a));
    }

    readFile.close();
    outFile.close();

return 0;

}