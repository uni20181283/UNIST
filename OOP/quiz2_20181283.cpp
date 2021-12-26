#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    int numberofcase;
    int classnumber=1;
    int num;
    unsigned long long int first=0;
    unsigned long long int second=0;
    unsigned long long int third=0;

    ifstream inFile;
    inFile.open("input.txt");
    ofstream outFile;
    outFile.open("output.txt");
    inFile >> numberofcase;
    while(classnumber, inFile >> num)
    {
        int i=0;
        int arr[i];
        for(i=0; i<num ; i++)
        {
            inFile >> arr[i];
            cout << arr[i] << " ";
        }
        first = arr[0];
        second = arr[1];
        third = arr[2];
        cout << first << second<<third;;
        for(i=0;i<num;i++)
        {
            if(arr[i]>third && arr[i]<second)
                third = arr[i];
            if(arr[i]>second && arr[i]<first)
            {
                third=second;
                second=arr[i];
            }
            if(arr[i]>first)
            {
                third = second;
                second = first;
                first = arr[i];
            }
        }
        outFile << "#" << classnumber << " "<< third << "\n";
        classnumber++;
    }
    

    inFile.close();
    outFile.close();
    return 0;
}