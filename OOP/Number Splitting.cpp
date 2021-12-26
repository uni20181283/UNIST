//Number Splitting
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int thousand(long long int a, long long int b)
{
    long long int x, y ,z;
    int k=0;

    // 2/1/1
    x=a/100;y=(a%100)/10;z=a%10;
    if(x+y+z==b){k++;}
    // 1/2/1
    x=a/1000;y=(a%1000)/10;z=a%10;
    if(x+y+z==b){k++;}
    //1/1/2
    x=a/1000;y=(a%1000)/100;z=a%100;
    if(x+y+z==b){k++;}

    return k;
}
int TenThousand(int a, int b)
{
    int k=0;
    long long int x,y,z;

    // 2/2/1
    x=a/1000;y=(a%1000)/10;z=a%10;
    if(x+y+z==b){k++;cout<<b<<endl;}
    // 2/1/2
    x=a/1000;y=(a%1000)/100;z=a%100;
    if(x+y+z==b){k++;cout<<b<<endl;}
    // 1/2/2
    x=a/10000;y=(a%10000)/100;z=a%100;
    if(x+y+z==b){k++;cout<<b<<endl;}
    // 3/1/1
    x=a/100;y=(a%100)/10;z=a%10;
    if(x+y+z==b){k++;cout<<b<<endl;}
    // 1/3/1
    x=a/10000;y=(a%10000)/10;z=a%10;
    if(x+y+z==b){k++;cout<<b<<endl;}
    // 1/1/3
    x=a/10000;y=(a%10000)/1000;z=a%1000;
    if(x+y+z==b){k++;cout<<b<<endl;}
    z=0;
    // 3/2
    x=a/100;y=a%100;
    if(x+y==b){k++;cout<<b<<endl;}
    // 2/3
    x=a/1000;y=a%1000;
    if(x+y==b){k++;cout<<b<<endl;}

    return k;
}

int main()
{
    int classnumber=1;
    int numberofcase;
    long long int num;
    
    ifstream inFile;
    inFile.open("input_1.txt");
    ofstream outFile;
    outFile.open("output_1.txt");

    inFile >> numberofcase;
    while(inFile >> num)
    {
        if(num>9&&num<100000)
        {
            double num1 = sqrt(num);
            long long int num2 = round(sqrt(num));
            int result=0;
            if(num1==num2)
            {
                if(num>10000)
                {
                    result=TenThousand(num, num2);
                }
                else if(num>1000)
                {
                    if(num==10000)
                    result++;
                    else
                    result=thousand(num, num2);
                }
                else if(num>100)
                {
                    if(num==100)
                        result++;
                }
                else if(num>10)
                {
                    if(num==81)
                        result++;
                }

                if(result>0)
                outFile << "#" << classnumber << " YES\n";
                else
                outFile << "#" << classnumber <<" NO\n";
            }
            else
            outFile << "#" << classnumber << " NO\n";
        }            
        else
        outFile << "#" << classnumber << " -1\n";
        classnumber++;
    }
    inFile.close();
    outFile.close();
    return 0;
}