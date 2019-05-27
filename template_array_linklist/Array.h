#ifndef ARRAY_H
#define ARRAY_H

#include <Container.h>

template<typename T>
class Array : public Container<T>
{
public:
    Array(int size=100):arr_lenth(0),arr_size(size)
    {
        arraies=new T[arr_size];
        myItr=0;
    }
    class Itr:public Container<T>::AbsItr
    {
    public:
        Itr(T* p = 0):Container<T>::AbsItr((void*)p) {}
        Itr& operator ++(int)
        {
            T* cur=(static_cast<T*>((this->Container<T>::AbsItr::pointer)));
            cur++;
           this->Container<T>::AbsItr::pointer=static_cast<void*>(cur);
            return *this;
        }

        T& operator *()
        {
            return *static_cast<T*>(this->Container<T>::AbsItr::pointer);
        }
    };
    ~Array() {}
    void Add(T e)
    {
        if(arr_lenth>=arr_size)
        {
            T * buf=arraies;
            arr_size+=100;
            arraies=new T[arr_size];
            for(int i=0; i<arr_lenth; i++)
            {
                arraies[i]=buf[i];
            }
        }
        arraies[arr_lenth++]=e;
    }
    Itr& at_begin()
    {
        if(myItr!=0)
            delete myItr;
        myItr=new Itr(arraies);
        return *myItr;
    }
    Itr& at_end()
    {
        if(myItr!=0)
            delete myItr;
        myItr=new Itr(static_cast<T*>(arraies+arr_lenth));
        return *myItr;
    }
protected:

private:
    Itr* myItr;
    T * arraies;
    int arr_lenth;
    int arr_size;
};

#endif // ARRAY_H
