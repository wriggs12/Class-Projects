/****************************************************************************************************************************
Title        : DoubleNode.hpp
Description  : header/interface file of a Doubly-Linked Node class
Dependencies : DoubleNode.cpp
Addendum     : DO NOT ALTER THIS FILE 
****************************************************************************************************************************/

#ifndef DOUBLE_NODE_
#define DOUBLE_NODE_

template <class ItemType>
class DoubleNode
{
public:
    DoubleNode();
    DoubleNode(const ItemType &anItem, DoubleNode<ItemType> *nextNodePtr = nullptr, DoubleNode<ItemType> *previousNodePtr = nullptr);
    void setItem(const ItemType &anItem);
    void setPrevious(DoubleNode<ItemType> *previousNodePtr);
    void setNext(DoubleNode<ItemType> *nextNodePtr);

    ItemType getItem() const;
    DoubleNode<ItemType> *getNext() const;
    DoubleNode<ItemType> *getPrevious() const;

private:
    ItemType item_;
    DoubleNode<ItemType> *next_;
    DoubleNode<ItemType> *prev_;
};

#include "DoubleNode.cpp"
#endif