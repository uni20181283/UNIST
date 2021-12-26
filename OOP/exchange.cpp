//1. Exchange two numbers using reference and pointer
#include <iostream>
#include <fstream>
using namespace std;

void exchange_using_reference(int &a,int &b)
{
    // (item 1) Need to implement: exchanging two values using reference
    int temp;
    temp=a;
    a=b;
    b=temp;

}
void exchanging_using_pointer(int *a,int *b)
{
    // (item 2) Need to implement: exchanging two values using pointer
    int temp;
    temp =*a;
    *a = *b;
    *b= temp;
}
int main()
{
    int a,b;
    int classnumber=1;
    int numberofcase;

    ifstream inFile;
    inFile.open("input_1.txt");
    ofstream outFile;
    outFile.open("output_1.txt");

    inFile >> numberofcase;
    while(inFile >> a >> b)
    {
        if(a<-1000 || a>1000 || b<-1000 || b>1000)
        outFile << "#" << classnumber << " -1\n";
        else
        {
// (item 3) need to implement: read multiple inputs (you may need to use loop)
            exchange_using_reference(a,b);
            outFile <<"#" << classnumber << " " << a << " " << b;
// (item 4) need to implement: print a and b;
            exchanging_using_pointer(&a,&b);
            outFile << " " << a << " " << b << "\n";
// (item 5) need to implement: print a and b;
        }
        cout << a << b << endl;

        classnumber++;
    }
    
    inFile.close();
    outFile.close();
    return 0;
}
//time limit for each case will be 1 min.