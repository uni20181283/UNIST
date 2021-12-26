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
        for(int y=1; y <= z; y++)
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
    int num1=1;
    int num2=100;
    int a,b;

    a= Number_of_PrimeNumber(num1);
    b= Number_of_PrimeNumber(num2);
    cout << a << "\t" << b <<endl;
    return 0;
}