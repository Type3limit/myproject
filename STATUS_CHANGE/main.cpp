#include <iostream>
#include "Mylift.h"
using namespace std;

void addrule(Mylift&);
int main()
{
    Mylift _thisone(running);
    addrule(_thisone);
    _thisone.whatsnow();
    cout<<"Push Close button!"<<endl;
    _thisone.Pushbutton(Close);
    _thisone.whatsnow();
    cout<<"Push run button!"<<endl;
    _thisone.Pushbutton(Run);
    _thisone.whatsnow();
    cout<<"Push open button!"<<endl;
    _thisone.Pushbutton(Open);
    _thisone.whatsnow();
    cout<<"Push Stop button!"<<endl;
    _thisone.Pushbutton(Stop);
    _thisone.whatsnow();

    return 0;
}

void addrule(Mylift& thisone)
{
    thisone.addMapping(running,Stop,stoping);
    thisone.addMapping(stoping,Run,running);
    thisone.addMapping(opening,Close,closing);
    thisone.addMapping(closing,Open,opening);
    thisone.addMapping(stoping,Open,opening);
    thisone.addMapping(closing,Run,running);
}
