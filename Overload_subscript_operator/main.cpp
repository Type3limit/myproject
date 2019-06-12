#include <iostream>
#include "CClass.h"
using namespace std;

int main()
{
    CClass yiban;
    yiban.AddStudent("001","Alice");
    yiban.AddStudent("002","Bob");
    yiban.AddStudent("003","Che");
    yiban.AddStudent("004","David");
    cout<<yiban<<endl;
    yiban["002"]="Bcd";
    cout<<yiban<<endl;
    yiban["005"]="Eva";
    cout<<yiban<<endl;
    return 0;
}
