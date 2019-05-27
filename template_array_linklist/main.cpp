#include <iostream>
#include <string>
#include "Array.h"
#include "LinkList.h"
using namespace std;

int main()
{

    cout << "in Array :"<<endl;
    Array<int> vi;
    for(int i=0; i<10; i++)
    vi.Add(i);
    Array<int>::Itr itrV;
    for(itrV = vi.at_begin(); itrV!=vi.at_end(); itrV++)
        {
            cout<<*itrV<<" ";
        }
    cout<<endl;

    for(itrV = vi.at_begin(); itrV!=vi.at_end(); itrV++)
        *itrV += 10;
    for(itrV = vi.at_begin(); itrV!=vi.at_end(); itrV++)
        cout<<*itrV<<" ";
    cout<<endl;


    cout<<"in list:"<<endl;
    LinkList<int> Li;
    for(int i=0; i<10; i++)
        Li.Add(i+10);
    LinkList<int>::Itr itrL;
    for(itrL = Li.at_begin(); itrL!=Li.at_end(); itrL++)
        cout<<*itrL<<" ";
    cout<<endl;

    for(itrL = Li.at_begin(); itrL!=Li.at_end(); itrL++)
        *itrL+= 10;
    for(itrL = Li.at_begin(); itrL!=Li.at_end(); itrL++)
        cout<<*itrL<<" ";
    cout<<endl;

    return 0;
}
