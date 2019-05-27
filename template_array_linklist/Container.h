#ifndef CONTAINER_H
#define CONTAINER_H

template<typename T>
class Container
{
public:
    class AbsItr
    {
    public:
        AbsItr(void* p = 0)
        {
            pointer = (void*)p;
        }
        virtual AbsItr & operator ++(int) = 0;
        virtual T & operator *() = 0;
        T&  operator = (const T& other)
        {
            pointer=&other;
            return *pointer;
        }
        AbsItr & operator = (const AbsItr& other)
        {
            pointer=other.pointer;
            return *this;
        }
        bool operator!=(AbsItr& other)
        {
            return (pointer!=other.pointer)?true:false;
        }
    public:
        void* pointer;
    };
    Container(){}
    virtual ~Container(){}
    virtual void Add(T e) = 0;
    virtual AbsItr & at_begin()  = 0;
    virtual AbsItr & at_end()  = 0;
protected:
private:
};

#endif // CONTAINER_H
