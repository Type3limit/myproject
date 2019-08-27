#include <iostream>
using namespace std;
template <typename T>
struct my_stack
{
    T* arries;
    T* top_buf;
    T* cur_top;
    T* end_buf;
    T* buffer;
    my_stack(int cur_size);
    bool push(T);
    T pop();
    void list();
}; 

template<typename T>
my_stack<T>::my_stack(int cur_size)
{
    arries= new T[cur_size];
    top_buf=&arries[cur_size+1];
    end_buf=(&arries[0]-1);
    cur_top=buffer=arries;
}

template<typename T>
bool my_stack<T>::push(T cur_data)
{
    if(cur_top==top_buf)
       return false;
    *buffer=cur_data;
    cur_top=++buffer;    
}

template<typename T>
T my_stack<T>:: pop()
{
    if(buffer==end_buf)
       return 0;
    T* cur_buffer=(buffer--);
    cur_top=buffer;
    return *cur_buffer;
}

template <typename T>
void my_stack<T>::list()
{
    if(buffer==(end_buf+1))
       cout<<"nothing in this Stack"<<endl;
    else
    {
        for(T*cur_buf=(cur_top-1);cur_buf!=end_buf;cur_buf--)
        {
            cout<<*cur_buf<<" ";
        }
        cout<<endl;
    }
    
}

int main()
{
    my_stack<char> thisone(10);
    for (char x='A';x<'K';x++)
        thisone.push(x);
    thisone.list();
    for(int x=0;x<3;x++)
        thisone.pop();
    thisone.list();
    for(char x='a';x<'c';x++)
        thisone.push(x);
    thisone.list();
    return 0;
}