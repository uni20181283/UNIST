#include <iostream>
#include <fstream>
using namespace std;

int Number_of_PrimeNumber(int z)
{
    int NOP=0;
    if(z==1)
        NOP=1;
    else
    {
        for(int y=1; y < z; y++)
        {
            int sum =0;
            for(int x=1 ; x <= y ; x++)
            {
                bool result = (y % x ==0);
                sum += result;
            }
            if(sum==2)
                NOP++;
        }
        NOP++;
    }

    return NOP;
}

int main()
{
    int casenum;
    int num1, num2;
    int answernum=1;
    int a,b;

    ifstream inFile;
    inFile.open("input_1.txt");
    ofstream outFile;
    outFile.open("output_1.txt");

    inFile >> casenum;
    while(answernum, inFile>> num1 >>num2)
    {
        if(num1 <=0 || num2 <=0 || num1 > num2)
        {
            outFile << "#" << answernum << " -1" << endl;
            answernum ++;
        }
        else
        {
            a=Number_of_PrimeNumber(num2);
            b=Number_of_PrimeNumber(num1);
            cout << b << "\t" << a << endl;
            outFile << "#" << answernum << " " << a-b << endl;
            answernum ++;
        }
    }
    inFile.close();
    outFile.close();
    return 0;
}

