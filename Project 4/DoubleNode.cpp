/****************************************************************************************************************************
Title        : DoubleNode.cpp
Description  : implementation file of a Doubly-Linked Node class
Dependencies : DoubleNode.hpp
Addendum     : DO NOT ALTER THIS FILE 
****************************************************************************************************************************/

/* Default Constructor */
template <class ItemType>
DoubleNode<ItemType>::DoubleNode() : next_{nullptr}, prev_{nullptr}
{
}

/* Parameterized Constructor */
template <class ItemType>
DoubleNode<ItemType>::DoubleNode(const ItemType &anItem, DoubleNode<ItemType> *nextNodePtr, DoubleNode<ItemType> *previousNodePtr) : item_{anItem}, next_{nextNodePtr}, prev_{previousNodePtr}
{
}

/* Setter: item_ */
template <class ItemType>
void DoubleNode<ItemType>::setItem(const ItemType &anItem)
{
    item_ = anItem;
}

/* Setter: prev_ */
template <class ItemType>
void DoubleNode<ItemType>::setPrevious(DoubleNode<ItemType> *previousNodePtr)
{
    prev_ = previousNodePtr;
}

/* Setter: next_ */
template <class ItemType>
void DoubleNode<ItemType>::setNext(DoubleNode<ItemType> *nextNodePtr)
{
    next_ = nextNodePtr;
}

/* Accessor: item_ */
template <class ItemType>
ItemType DoubleNode<ItemType>::getItem() const
{
    return item_;
}

/* Accessor: next_ */
template <class ItemType>
DoubleNode<ItemType> *DoubleNode<ItemType>::getNext() const
{
    return next_;
}

/* Accessor: prev_ */
template <class ItemType>
DoubleNode<ItemType> *DoubleNode<ItemType>::getPrevious() const
{
    return prev_;
}