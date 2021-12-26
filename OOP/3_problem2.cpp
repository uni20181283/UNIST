//2. Fibonacci sequence
#include <iostream>
#include <fstream>
using namespace std;
unsigned long long int Fibonacci(int x)
{
    unsigned long long int a=0;
    unsigned long long int b=1;
    int y=1;
    unsigned long long int c;
    if(x==0)
    return 0;
    else if(x==1)
    return 1;
    else
    {
    while(y<x)
    {
        c=a+b;
        a=b;
        b=c;
        y++;
    }
   // cout << typeid(y).name() << endl;
    return c;
    }
}
//This function calculate the Fibonacci.
//At first, if x=0, return 0.
//And second, if x=1, return 1.
//Lastely, if x>1, add the number that is locate before 2 number.??????

int main()
{
    int num;
    int classnumber=1;
    int numberofcase;
    ifstream inFile;
    inFile.open("input_2.txt");
    ofstream outFile;
    outFile.open("output_2.txt");
    inFile >> numberofcase;
    cout << numberofcase << endl;
    while(classnumber, inFile >> num)
    {
        if(num>90)
        {
            outFile << "#" << classnumber << " -1"<< "\n";
            classnumber++;
        }
        else
        {
            num -= 1;       //I got first is 1, but in problem, first is 0. To make form of problem. num is num-1.
            unsigned long long int result = Fibonacci(num);
            outFile << "#" << classnumber << " "<<  result << "\n";
            classnumber++;
        }

    }

    inFile.close();
    outFile.close();
    return 0;
}