#ifndef CCLASS_H
#define CCLASS_H
#include<iostream>
#include<string>
using namespace std;
struct Student
{
    Student(string nno="null",string nname="null"):no(nno),name(nname){};
    string no;
    string name;
};

class CClass
{
    public:
        friend ostream& operator<<(ostream& o,CClass& c);
        CClass(int capacity=100);
        virtual ~CClass();
        void AddStudent(string,string);
        string & operator[](const string);
        void operator=(const string);
    protected:

    private:
        Student *all;
        int num;
        int capacity;
};

#endif // CCLASS_H
