#include <iostream>
using namespace std;
int main()
{
    int z=67;
    int NOP=0;
    if(z==1)
    {
        NOP = 1;
        cout << NOP;
    }
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
            cout << sum << "\t";
            if(sum==2)
                NOP++;
        }
        NOP++;
        cout << NOP;
    }

    return 0;
}
// Check the number of prime number.