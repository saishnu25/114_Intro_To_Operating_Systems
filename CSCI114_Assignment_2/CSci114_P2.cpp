/*
Saishnu Ramesh Kumar (300758706)
CSCI 114 - Assignment 2

To execute program:

Locate the file destination and directory using cd
ls                          //Lists the files and directories in current directory
g++ CSci114_P2.cpp          //Compiles the program     
mv file1 data.in            // Moves 'file1' to 'data.in'         
mv data.in a.out            // Moves 'data.in' to 'a.out'
mv a.out file2              // Moves 'a.out' to 'file2'
ls

The contents would be successfully copied and file2 can now be opened
*/

#include <iostream>
#include <fstream>
#include <unistd.h>         //Symbolic Constants
#include <sys/types.h>      //Primitive System Data Types
#include <errno.h>          //Errors 
#include <stdio.h>          //Input/Output 
#include <sys/wait.h>       //Wait for Process Termination 
#include <stdlib.h>         //General Utilities 

using namespace std;

int A_to_B[2];  //Global array for pipe file descriptors

int main ()
{
  int status;
  ifstream infile ("data.in", ios::in | ios::binary );   //Input file for reading
  ofstream outfile ("data.out", ios::out | ios::binary); //Output file for writing

  pipe (A_to_B); //Creating a pipe for communication between the two processes

  //Child process A
  if (fork () == 0){

      printf ("IN PA\n");
      close (A_to_B[0]); //Closing the read end of pipe 

      char c;

      //Reads the input file until the end of file
      while (!infile.eof ()){ 
          infile.read ((char *) &c, sizeof (char)); //Reads one character from the input file
          write (A_to_B[1], (char *) &c, sizeof (char)); //Writes the character to the pipe 
      }

      sleep (1);
      printf ("PA:");

      close (A_to_B[1]); //Closes the write end of the pipe
      return 0;
    }

  //Child process B
  if (fork () == 0){
      char c;
      
      printf ("IN PB\n");
      close (A_to_B[1]); //Closes the write end of the pipe

      sleep (1);

      //Reads from the pipe until the end of the file 
      while (!infile.eof ()){
        read (A_to_B[0], (char *) &c, sizeof (char)); //Reads the character from the pipe
        outfile.write ((char *) &c, sizeof (char)); //Writes character to the output file
      }

      printf ("PB:");

      close (A_to_B[0]); //Closes the read end of the pipe
      return (0);
    }

  infile.close (); //Closes the input file 
  outfile.close (); //Closes the output file

  wait (&status); //Waits for the process to finish
  wait (&status);
  
  printf ("END\n");
}
