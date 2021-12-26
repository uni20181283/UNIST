//Fractification
#include <iostream>
#include <fstream>
using namespace std;

class Fractification{
    private:
        long double w;

        long double a;
        long double b;
        long double c;
        long double d;

        long long int ia;
        long long int ib;
        long long int ic;
        long long int id;
    public:
        Fractification(long long int x, long long int y, long long int z, long long int k)
        {
            if(x==0||y==0||z==0||k==0)
            {
                ia=x;ib=y;ic=z;id=k;
            }
            else
            {
                a=x; b=y; c=z; d=k;
                w=x/y+c/k;
            if(w> x/y+k/z)
            {
                w=x/y+k/z;
                a=x;b=y;c=k;d=z;
            }

            if(w> x/z+y/k)
            {
                w=x/z+y/k;
                a=x;b=z;c=y;d=k;
            }
            if(w> x/z+k/y)
            {
                w=x/z+k/y;
                a=x;b=z;c=k;d=y;
            }
            if(w> x/k+y/z)
            {
                w=x/k+y/z;
                a=x;b=k;c=y;d=z;
            }
            if(w> x/k+z/y)
            {
                w=x/k+z/y;
                a=x;b=k;c=z;d=y;
            }

            if(w> y/x+z/k)
            {
                w=y/x+z/k;
                a=y;b=x;c=z;d=k;
            }
            if(w>y/x+k/z)
            {
                w=y/x+k/z;
                a=y;b=x;c=k;d=z;
            }

            if(w>z/x+y/k)
            {
                w=z/x+y/k;
                a=z;b=x;c=y;d=k;
            }
            if(w>z/x+k/y)
            {
                w=z/x+k/y;
                a=z;b=x;c=k;d=y;
            }

            if(w>k/x+y/z)
            {
                w=k/x+y/z;
                a=k;b=x;c=y;d=z;
            }
            if(w>k/x+z/y)
            {
                w=k/x+z/y;
                a=k;b=x;c=z;d=y;
            }
            //여기까지 비교함수
            ia=static_cast <long long int>(a);
            ib=static_cast <long long int>(b);
            ic=static_cast <long long int>(c);
            id=static_cast <long long int>(d);
            }
            //long double로 계산을 해왔기에 형변환을 시켜준다.
            if(ia>ic)
            {
                long long int temp = ia;long long int temp2=ib;
                ic = temp; id=temp2;
                ia = ic; ib=id;
            }
            if(ia==ic && ib>id)
            {
                long long int temp = ia;long long int temp2=ib;
                ic = temp; id=temp2;
                ia = ic; ib=id;
            }
        }
        void print(int x)
        {
            ofstream outFile("output_2.txt",ios::app);
            if(ia<1 || ia>1000000000 || ib<1 || ib>1000000000 || ic<1 || ic>1000000000 || id<1 || id>1000000000)
            {
            outFile << "#" << x << " -1" << "\n";
            }
            else
            {
            outFile << "#" << x << " " << ia << " " << ib << " " << ic << " " << id << "\n";
            }
        }
};

int main()
{
    int classnumber=1;
    int numberofcase;
    
    long long int num1;
    long long int num2;
    long long int num3;
    long long int num4;
    ifstream inFile;
    inFile.open("input_2.txt");
    ofstream outFile;
    outFile.open("output_2.txt");

    inFile >> numberofcase;
    while(inFile >> num1 >> num2 >> num3 >> num4)
    {
        Fractification result(num1,num2,num3,num4);
        result.print(classnumber);
        classnumber++;
    }
    inFile.close();
    outFile.close();
    return 0;
}