#include <iostream>
#include <string>
#include <thread>

using namespace std;

void threadCallback(int x, int y)
{
    cout << "Cell "<< x << "," << y << endl;
}
int main()  
{
    thread myThreads[10][10];
    string str = "Sample String";
    for(int i=0;i<10;i++)
    	for(int j=0;j<10;j++)
	    	myThreads[i][j] = thread(threadCallback, i, j);
    for(int i=0;i<10;i++)
    	for(int j=0;j<10;j++)
	    	myThreads[i][j].join();
    return 0;
}
