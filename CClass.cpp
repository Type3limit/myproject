#include "CClass.h"

 ostream& operator<<(ostream& o,CClass& c)
{
    for(int i=0;i<c.num;i++)
       o<<(c.all[i].no)<<"|"<<(c.all[i].name)<<endl;
    return o;
}


CClass::CClass(int cur_max):capacity(cur_max)
{
    all=new Student[capacity];
    num=0;
}

CClass::~CClass()
{
    delete all;
}

void CClass::AddStudent(string no,string name)
{
    if(num>=capacity)
    {
        Student *tmp=all;
        all=new Student[capacity*2];
        for(int i=0;i<capacity;i++)
        {
            all[i].name=tmp[i].name;
            all[i].no=tmp[i].no;
        }
        capacity+=capacity;
    }

    this->all[num].no=no;
    this->all[num].name=name;
    num++;
}

string& CClass::operator[](const string key)
{
    for(int i=0;i<num;i++)
    {
        if(all[i].no==key)
            return all[i].name;
    }
    all[num].no=key;
    return all[num++].name;
}
