/*
Saishnu Ramesh Kumar (300758706)
CSCI 114 - Assignment 4

To execute program:

Locate the file destination and directory using cd
ls
g++ CSci114_P4.cpp
ls (To check for the output file)
./a.exe <n> 

Note: n is the maximum number of same direction cars that can enter the tunnel, this is where the user will input a number. Do not include < > symbol in terminal when executing the program.
*/

#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <vector>
#include <fstream>
#include <condition_variable>
#include <unistd.h>
#include <chrono>

using namespace std;
mutex mtx;

class Tunnel {
    private:
        // Number of cars waiting to enter the tunnel
        int waitNorth = 0;  
        int waitSouth = 0;

        // Number of cars in the tunnel
        int inNorth = 0;    
        int inSouth = 0;

        // Number of cars that have entered the tunnel 
        int countNorth = 0; 
        int countSouth = 0;

    public:
        condition_variable northConVar, southConVar;
        int n;

        // Go Function - Simulates a car entering and leaving the tunnel
        void go(char dir){ 
            if (dir == 'N'){
                northEnter();
                usleep(100000);     // Simulating car movement
                northLeave();
            }
            else{
                southEnter();
                usleep(100000);     // Simulating car movement
                southLeave();
            }
        }

        // Start Function - Starts the traffic simulation
        void start(int n){ 
            vector<thread> threads;
            this->n = n;
            ifstream file;
            file.open("cars.txt", ios::in); // Opens the file containing directions of cars

            char dir;
            // Reads each direction from file and creates thread for each car
            while (file >> dir){
                threads.push_back(std::thread( &Tunnel::go, this, dir));
                usleep(rand()%1000); // Random delay between car creation for simulation
            }
                
            // Wait for all threads to finish
            for (int i = 0; i < threads.size(); i++)
                threads.at(i).join();
        }
        
        // North Wait Function - Checks if a northbound car should wait before entering
        bool northShouldWait(){
            return (inNorth == n || inSouth > 0 || countNorth == n || waitSouth == n);
        } 
        
        // North Enter Function - Handles a northbound car entering the tunnel
        void northEnter(){ 
            mtx.lock();
            unique_lock<mutex> lock1(mtx, defer_lock);
            waitNorth++;
            
            cout<<"North waiting: "<<waitNorth<<endl;
            // Wait until it's safe for the northbound car to enter
            while (northShouldWait())
                northConVar.wait(lock1);
            
            waitNorth--;
            inNorth++;
            
            countNorth++;
            countSouth = 0;
            
            cout<<"North entering: "<<inNorth<<endl;
            mtx.unlock();
        };
        
        // North Leave Function - Handles a northbound car leaving the tunnel
        void northLeave(){ 
            mtx.lock();
            inNorth--;
            // Notify other cars waiting if the tunnel is empty or full
            if (inNorth == 0 || countNorth == n || waitSouth == n)
                southConVar.notify_all();
            else
                northConVar.notify_one();

            // Reset the count if all allowed cars have entered and no cars are waiting on the other side
            if (countNorth == n && waitSouth == 0){
                countNorth = 0;
            }
            
            cout<<"North leaving: "<<inNorth<<endl;
            mtx.unlock();
        }
        
        // South Wait Function - Checks if a southbound car should wait before entering
        bool southShouldWait(){ 
            return (inSouth == n || inNorth > 0 || countSouth == n || waitNorth == n);
        } 
        
        // South Enter Function - Handles a southbound car entering the tunnel
        void southEnter(){ 
            mtx.lock();
            unique_lock<mutex> lock2(mtx, defer_lock);
            waitSouth++;
            
            cout<<"South waiting: "<<waitSouth<<endl;
            // Wait until it's safe for the southbound car to enter
            while (southShouldWait())
                southConVar.wait(lock2);
            
            waitSouth--;
            inSouth++;
            
            countSouth++;
            countNorth = 0;
       
            cout<<"South entering: "<<inSouth<<endl;
            mtx.unlock();
        }
        
        // South Leave Function - Handles a southbound car leaving the tunnel
        void southLeave(){ 
            mtx.lock();
            inSouth--;
            // Notify other cars waiting if the tunnel is empty or full
            if (inSouth == 0 || countSouth == n || waitNorth == n)
                northConVar.notify_all();
            else
                southConVar.notify_one();
            
            // Reset the count if all allowed cars have entered and no cars are waiting on the other side
            if (countSouth == n && waitNorth == 0){
                countSouth = 0;
            }
            
            cout<<"South leaving: "<<inSouth<<endl;
            mtx.unlock();
        }
};

// Main Function 
int main(int argc, char** argv){

    Tunnel t;
    int n = stoi(argv[1]); // Retrieves the maximum number of same direction cars allowed

    t.start(n); // Starts the traffic simulation
}
