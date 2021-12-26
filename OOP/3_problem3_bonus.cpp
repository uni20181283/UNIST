//3. Sum of two numbers
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;
char* sum(char* str,char* str1,char* str2)
{

    int a = strlen(str1);
    int b = strlen(str2);
    int c,d;
    char* big;
    char* small;
    
    if(a>=b)
    {
        big = str1;
        small = str2;
        c=a;
        d=b;
    }
    else
    {
        big = str2;
        small = str1;
        c=b;
        d=a;
    }
    for(int i=0;i<c-d;i++)
    {
        int x = big[i] - '0';
        str[i]= x+'0';
    }
    for(int i=c-1;i>=c-d;i--)
    {
        int x = big[i] - '0';
        int y = small[i-c+d] -'0';
        str[i]=x+y+'0';

    }
    //make str array using for loop
    for(int i=c-1;i>0;i--)
    {
        if(str[i]>'9')
        {
            str[i-1]= str[i-1]+1 ;
            str[i]=str[i]-10;
        }
    }
    //set the number of digits
    return str;
}
int main()
{
    int classnumber=1;
    int numberofcase;
    char result[301];
    string arr;
    ifstream inFile;
    inFile.open("input_3_bonus.txt");
    ofstream outFile;
    outFile.open("output_3_bonus.txt");
    getline(inFile,arr);
    while(getline(inFile, arr))
    {
        char arr1[1000];
        strcpy(arr1,arr.c_str());
        char *arr2 = strtok(arr1, " ");
        arr2 = strtok(NULL," ");
        sum(result,arr1,arr2);
        int g = strlen(result);

        cout << result;
        for(int i=1;i<g;i++)
        {
            if(result[i]>58||result[i]<47)
            result[i]='\0';
        }
        if(result[0]>'9')
        {
            result[0]=result[0]-10;
            outFile << "#" << classnumber << " 1" << result <<"\n";
        }
        else
            outFile << "#" << classnumber << " " << result << "\n";
        classnumber++;
    }

    inFile.close();
    outFile.close();

    return 0;
}