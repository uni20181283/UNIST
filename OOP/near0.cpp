// 3. Two numbers whose product is closest to zero
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int main()
{
    int classnumber=1;
    int numberofcase;
    int num;

    ifstream inFile;
    inFile.open("input_3.txt");
    ofstream outFile;
    outFile.open("output_3.txt");

    inFile >> numberofcase;
    while(inFile >>num)
    {
        int i=0;
        int arr[i];
        for(i=0;i<num;i++)
        {
            inFile >> arr[i];
        }
        if(arr[i]>100000 || arr[i]<-100000)
        outFile << "#" << classnumber << " -1\n";
        else{
        unsigned long int square[i];
        for(i=0;i<num;i++)
        {
            square[i]=round(pow(arr[i],2));
            cout << square[i] << " ";
        }
        cout << endl;
        int first=0;
        int second=0;
        if(square[0]<square[i])
        {
            first=square[0];
            second=square[1];
        }
        else
        {
        first=square[1];
        second=square[0];
        }
        for(i=0;i<num;i++)
        {
            if(square[i]<second && square[i]>first)
            {
                second=square[i];
            }
            if(square[i]<first)
            {
                second=first;
                first=square[i];
            }
        }
        int indexfirst=0;
        int indexsecond=0;
        for(i=0;i<num;i++)
        {
            if(first==square[i])
            indexfirst=i;
            if(second==square[i])
            indexsecond=i;
        }
        first=arr[indexfirst];
        second=arr[indexsecond];
        outFile << "#" << classnumber << " " << first << " " << second << "\n";
        }
        classnumber++;
    }
    inFile.close();
    outFile.close();
    return 0;
}
//arr만들고 그걸 제곱한 arr를 찾은 다음에 min value찾기