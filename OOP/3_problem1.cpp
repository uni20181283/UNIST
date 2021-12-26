//1. Find the biggest number from a given random number set
#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    int numberofcase;
    int classnumber=1;
    int num;
    int max=0;

    ifstream inFile;
    inFile.open("input_1.txt");
    ofstream outFile;
    outFile.open("output_1.txt");
    inFile >> numberofcase;
    while(classnumber, inFile >> num)       //it means to read size of set of each line
    {
        int i=0;
        int arr[i];
        for(i=0; i<num ; i++)                 // array arr[i] from 0 to num-1 to make number num's array
        {
            inFile >> arr[i];
            cout << arr[i] << " ";
        }
        cout <<endl;
        int max = arr[0];                   // set the arr[0] is max.
        for(i=0;i<num;i++)                  // compare with max. And if arr[i] is bigger than max, max change to arr[i]
        {
            if(arr[i]>max)
            max=arr[i];
        }
        if(max>100000)
        {
            outFile << "#" << classnumber << " -1" << "\n";
        }
        else
        {
            outFile << "#" << classnumber << " "<< max << "\n";
        }
        classnumber++;
    }
    

    inFile.close();
    outFile.close();
    return 0;
}