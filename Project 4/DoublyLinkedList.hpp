/****************************************************************************************************************************
Title        : DoublyLinkedList.hpp
Description  : header/interface file of a Doubly-Linked List ADT
Dependencies : DoubleNode.cpp, DoubleNode.hpp
Addendum     : DO NOT ALTER THIS FILE 
****************************************************************************************************************************/

#include "DoubleNode.hpp"

#ifndef DOUBLY_LINKED_LIST_
#define DOUBLY_LINKED_LIST_

template <class ItemType>
class DoublyLinkedList
{
public:
    DoublyLinkedList();
    DoublyLinkedList(const DoublyLinkedList &aList);
    ~DoublyLinkedList();

    int getSize() const;
    int getIndex(const ItemType &item);
    DoubleNode<ItemType> *getAtPos(const int &pos) const;
    DoubleNode<ItemType> *getHeadPtr() const;

    bool insert(const ItemType &item, const int &position = 1);
    bool remove(const int &position);

    bool isEmpty() const;
    void clear();

    void display() const;
    void displayBackwards() const;

    void swap(int i, int j);

    DoublyLinkedList<ItemType> interleave(const DoublyLinkedList<ItemType> &aList);

private:
    DoubleNode<ItemType> *headPtr_;
    int itemCount_;
};

#include "DoublyLinkedList.cpp"
#endif
