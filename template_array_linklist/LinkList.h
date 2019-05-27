#ifndef LINKLIST_H
#define LINKLIST_H
#include <Container.h>

template <typename T>
struct node
{
    node(T c_data,node* c_next=0):data(c_data),next(c_next) {}
    T data;
    node * next;
};


template<typename T>
class LinkList:public Container<T>
{
public:
    LinkList(T c_data=0)
    {
        head=new node<T>(c_data);
        tail=head->next;
        list_size=0;
    }
    virtual ~LinkList(){}
    class Itr:public Container<T>::AbsItr
    {
    public:
        Itr(void* p = 0):Container<T>::AbsItr(p) {}
        Itr& operator ++(int)
        {
            (this->Container<T>::AbsItr::pointer)= static_cast<void*>(static_cast<node<T>*>(this->Container<T>::AbsItr::pointer)->next);
            return *this;
        }
        T & operator *()
        {
            return (static_cast<node<T>*>(this->Container<T>::AbsItr::pointer)->data);
        }
    };
    void Add(T e)
    {
        node<T> * cur_data=new node<T>(e);
        node <T>* index=head;
        while(index->next!=0)
            index=index->next;
        index->next=cur_data;
        list_size++;
        tail=cur_data->next;
    }
    Itr& at_begin()
    {
        if(myItr!=0)
            delete myItr;
        myItr=new Itr(head->next);
        return *myItr;
    }
    Itr& at_end()
    {
        if(myItr!=0)
            delete myItr;
        myItr=new Itr(tail);
        return *myItr;
    }
protected:

private:
    Itr* myItr;
    node<T>* head;
    node<T>* tail;
    int list_size;

};
#endif // LINKLIST_H
