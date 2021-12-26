//Largest Prime
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

long int Largest(long int a, long int b)
{
    long int LP=0;
    for(int i=1; i<=a;i++)
    {
        int sum=0;
        for(int j=1;j<=i;j++)
        {
            bool result = (i%j==0);
            sum +=result;
        }
        if(sum==2)
        {
            if(a%i==0 && b%i==0)
                LP = i;
        }
    }
    return LP;
}

int main()
{
    int classnumber=1;
    int numberofcase;
    long long int num1;
    long long int num2;
    
    ifstream inFile;
    inFile.open("input_2.txt");
    ofstream outFile;
    outFile.open("output_2.txt");

    inFile >> numberofcase;
    while(inFile >> num1 >> num2)
    {
        if(num1>=1 && num1 <=1000 && num2 >=10 && num2 <=100000)
        {
            long long int Prime=0;
            long long int result=0;
            for(int i=1;(round(pow(i+1,2))+num1) <= num2;i++)
            {
                unsigned long long int x=round(pow(i,2))+num1;
                unsigned long long int y=round(pow(i+1,2))+num1;
                Prime=Largest(x,y);
                if(Prime>result)
                    result=Prime;
            }
            if(result!=0)
            outFile << "#" << classnumber << " " << result << "\n";
            else
            outFile << "#" << classnumber << " -1\n";

        }
        else
            outFile << "#" << classnumber << " -1\n";
        classnumber++;
    }
    inFile.close();
    outFile.close();
    return 0;
}