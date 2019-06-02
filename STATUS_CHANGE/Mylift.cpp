#include "Mylift.h"
Mylift::Mylift(_status cur):CUR_ST(cur)
{
}

void Mylift::addMapping(_status s_buf,_button b_buf,_status v_buf)
{
    Status tem(s_buf,b_buf);
    ML_TAB.addMapping(tem,v_buf);
}

void Mylift::Pushbutton(_button cur )
{
     _status now=ML_TAB.Getstatus(cur,CUR_ST);
    if(CUR_ST!=now)
    CUR_ST=now;
}

void Mylift::whatsnow()
{
    switch(CUR_ST)
    {
    case running:cout<<"Lift is running now!\n"<<endl;break;
    case stoping:cout<<"Lift is stopping now!\n"<<endl;break;
    case opening:cout<<"Lift is opening now !\n"<<endl;break;
    case closing:cout<<"Lift is closing now!\n"<<endl;break;
    }
}


void Mylift_status::addMapping(Status cr,_status sr)
{
    this->cur_maptable.insert(pair<Status,_status>(cr,sr));
}

_status Mylift_status::Getstatus(_button cur,_status _now)
{
     Status tem(_now, cur);
     map<Status,_status>::iterator I_it;
     I_it= (cur_maptable.find(tem));
     if(I_it==cur_maptable.end())
        return _now;
     else
        return (I_it->second);
}
