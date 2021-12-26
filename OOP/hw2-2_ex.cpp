#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main()
{
    unsigned long long int result=0;
    unsigned long long int y=0;
    int num = 15;
    for(int a=0; a<=num ; a++)
    {
        unsigned long long int x=0;
        for(int b=0; b<a ; b++)
        {
            result = round(pow(10,b));
            x += result;
        }
        cout << x << endl;
        y += x;
    }
    cout << "Thus" << y;
    return 0;
}