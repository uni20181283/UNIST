//3. Sum of two numbers
#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    int numberofcase;
    int classnumber=1;
    unsigned long long int num1, num2;
    ifstream inFile;
    inFile.open("input_3.txt");
    ofstream outFile;
    outFile.open("output_3.txt");
    inFile >> numberofcase;
    while(classnumber, inFile >> num1 >> num2)
    {
        if(num1<1000000000000000 && num1>-1 || num2>-1 && num2<1000000000000000)
        {
            outFile<<"#" << classnumber << " " << num1+num2 << "\n";
            classnumber++;
        }
        else
        {
            outFile<<"#" << classnumber << " -1" << "\n";
            classnumber++;
        }
    }

    inFile.close();
    outFile.close();
    return 0;
}