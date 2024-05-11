/*
Saishnu Ramesh Kumar (300758706)
CSCI 114 - Assignment 3

To execute program:

Locate the file destination and directory using cd
ls
g++ CSci114_P3.cpp
ls to locate a.exe file
./a.exe _ _ _ (The _ are the required sizes for the matrices with M N K dimensions)

Note: Do ensure the text files used in this program are located within the same folder as the program. 

*/

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>

using namespace std;

int M, N, K; // Global variables to store matrix dimensions
mutex mt; // Mutex for thread synchronization

int A[100][100], B[100][100]; // Arrays to store matrices A and B

string buf; // Temporary string buffer

// Thread Callback Function 
void threadCallback(int x, int y){
    
	mt.lock(); // Lock mutex to ensure thread safety
	
	int C = 0; // Initialize result for matrix C
    
    // Calculate Matrix C (Result of multiplication of matrices A and B)
    for(int i = 0; i < N; i++) {
    	C += A[x][i] * B[i][y];
	}
	
	// Print Matrix C
	cout << C << '\t';
	
	if(y >= K - 1) { // If last column, print new line
		cout << endl;
	}
    
    mt.unlock(); // Unlock mutex
}

// Main Function
int main(int argc, char** argv){

	if(argc < 4) { // Check if sufficient arguments are provided
		cout << "Insufficient arguments to run" << endl;
		exit(1);
	}
	M = atoi(argv[1]); // Get number of rows for matrix A
	N = atoi(argv[2]); // Get number of columns for matrix A and rows for matrix B
	K = atoi(argv[3]); // Get number of columns for matrix B
	
	ifstream FileA;
	ifstream FileB;
	
	FileA.open("A.txt", ios::in | ios::binary); // Open file A for reading
	FileB.open("B.txt", ios::out | ios::binary); // Open file B for reading
	
	// Read A.txt to array A
	for(int i = 0; i < M; i++) {
    	for(int j = 0; j < N; j++) {
				FileA >> A[i][j];
		}
	}
	
	// Read B.txt to array B
	for(int i = 0; i < N; i++) {
    	for(int j = 0; j < K; j++) {
				FileB >> B[i][j];
		}
	}
	
	FileA.close(); // Close file A
	FileB.close(); // Close file B
		
    thread myThreads[M][K]; // Array of threads for parallel computation
    
    cout << "Result for Matrix Multiplication:" << endl;
    
    // Multi-threading
    for(int i = 0; i < M; i++) {
    	for(int j = 0; j < K; j++) {
    		myThreads[i][j] = thread(threadCallback, i, j); // Create threads for each element of resulting matrix
		}
	}
	    	
    // Wait for all threads to finish
    for(int i = 0; i < M; i++) {
    	for(int j = 0; j < K; j++) {
	    	myThreads[i][j].join();
	    }
	}
	    	
    return 0;
}