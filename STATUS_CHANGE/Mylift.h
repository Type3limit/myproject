#ifndef MYLIFT_H
#define MYLIFT_H
#include <iostream>
#include <string>
#include <map>
using namespace std;

enum _status{running=0,stoping,opening,closing};
enum _button{Run=0,Stop,Open,Close};

class Status
{
public:
    Status(_status s_buf,_button b_buf):s_cur(s_buf),s_bton(b_buf){}
    _status s_cur;
    _button s_bton;
    bool operator < (const Status cur) const
    {
        if(s_cur!=cur.s_cur)
           return s_cur<cur.s_cur;
        else
           return s_bton<cur.s_bton;
    }
};

class Mylift_status
{
public:
    void addMapping(Status,_status);
     _status Getstatus(_button,_status);
private:
    map<Status,_status> cur_maptable;
};

class Mylift
{
    public:
        Mylift(_status);
       void  addMapping(_status,_button,_status);
       void Pushbutton(_button);
       void whatsnow();
    private:
       Mylift_status ML_TAB;
        _status CUR_ST;
};


#endif // MYLIFT_H
