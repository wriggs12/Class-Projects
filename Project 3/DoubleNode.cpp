/**
 * Name: DoublyLinkedList Implementation
 * Class: CSCI 235
 * Assignment: Project 3
 * Description: Implements a node
 * */

#include <iostream>

/** default constructor **/
template <class ItemType>
DoubleNode<ItemType>::DoubleNode() {
    next_ = nullptr;
    prev_ = nullptr;
}

/** parameterized constructor **/
template <class ItemType>
DoubleNode<ItemType>::DoubleNode(const ItemType &anItem, DoubleNode<ItemType> *nextNodePtr, DoubleNode<ItemType> *previousNodePtr) {
    item_ = anItem;
    next_ = nextNodePtr;
    prev_ = previousNodePtr;
}

/**
 * @post item_ is changed to anItem
 * */
template <class ItemType>
void DoubleNode<ItemType>::setItem(const ItemType &anItem) {
    item_ = anItem;
}

/**
 * @post prev_ is changed to previousNodePtr
 * */
template <class ItemType>
void DoubleNode<ItemType>::setPrevious(DoubleNode<ItemType> *previousNodePtr) {
    prev_ = previousNodePtr;
}

/**
 * @post next_ is changed to nextNodePtr
 * */
template <class ItemType>
void DoubleNode<ItemType>::setNext(DoubleNode<ItemType> *nextNodePtr) {
    next_ = nextNodePtr;
}

/**
 * @return ItemType that is in the node
 **/
template <class ItemType>
ItemType DoubleNode<ItemType>::getItem() const {
    return item_;
}

/**
 * @return DoubleNode<ItemType>* that the node points to next 
 * */
template <class ItemType>
DoubleNode<ItemType>* DoubleNode<ItemType>::getNext() const {
    return next_;
}

/**
 * @return DoubleNode<ItemType>* that the node points to before
 **/
template <class ItemType>
DoubleNode<ItemType>* DoubleNode<ItemType>::getPrevious() const {
    return prev_;
}