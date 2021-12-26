#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    int count =1;
    unsigned long long int x=206;
    while(x>1)
    {
        if(x%2==0)
        {
            x=x/2;
            count++;
            cout << x << "\t";
        }
        else if(x%2 != 0 )
        {
            x= (x*3)+1;
            count++;
            cout << x << "\t";
        }
    }
    cout << endl << count;
    return 0;
}

//이 함수로 주어진 두 수 사이의 모든 수를 대입해서 그 중에 가장 큰 수를 output으로