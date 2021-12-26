//who is winner?
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int main()
{
    int classnumber=1;
    int numberofcase;
    int num1;
    int num2;

    ifstream inFile;
    inFile.open("input_3.txt");
    ofstream outFile;
    outFile.open("output_3.txt");

    inFile >> numberofcase;
    while(inFile >> num1 >> num2)
    {
        int participant=round(pow(2,num1));
        int arr[participant];
        int Winners[participant];
        int WinnersPower[participant];
        int NEWWinners[participant];
        int index[participant]={0,1,2,3,4,5,6,7};

        for(int i=0;i<participant;i++)
        {
            inFile >> arr[i];
            Winners[i]=arr[i];
        }
        
        for(int j=0;j<num1;j++)
        {
            for(int i=0;i<participant/(2*(j+1));i++)
            {
                if(arr[2*i]<arr[2*i+1])
                {
                    index[i]=index[2*i+1];
                    if(arr[2*i+1]-arr[2*i]+num2<arr[2*i+1])
                        WinnersPower[i]=arr[2*i+1]-arr[2*i]+num2;
                    else
                        WinnersPower[i]=arr[2*i+1];
                }
                else
                {
                    index[i]=index[2*i];
                    if(arr[2*i]-arr[2*i+1]+num2<arr[2*i])
                        WinnersPower[i]=arr[2*i]-arr[2*i+1]+num2;
                    else
                        WinnersPower[i]=arr[2*i];
                }
            }
            if(j!=num1-1)
            {
                for(int i=0;i<participant/(2*(j+1));i++)
                {
                    arr[i]=WinnersPower[i];
                    WinnersPower[i]={};
                }
            }
            else
            index[0]=index[0]+1;
        }

        int k=0;
        for(int i=0;i<participant;i++)
        {
            if(arr[i]>1000 || arr[i]<1)
            {
                k++;
            }
        }
        if(k==0)
        outFile << "#" << classnumber << " " << index[0] << "\n";
        else
        outFile << "#" << classnumber << " -1";
    
        
        classnumber++;
    }
    inFile.close();
    outFile.close();
    return 0;
}