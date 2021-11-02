/****************************************************************************************************************************
Title        : DoublyLinkedList.cpp
Description  : implementation file of a Doubly-Linked List ADT
Dependencies : DoublyLinkedList.hpp, DoubleNode.cpp, DoubleNode.hpp
Addendum     : DO NOT ALTER THIS FILE 
****************************************************************************************************************************/

#include <iostream>
#include <cstdlib>

/* Default Constructor */
template <class ItemType>
DoublyLinkedList<ItemType>::DoublyLinkedList() : headPtr_{nullptr}, itemCount_{0}
{
}

/* Copy Constructor */
template <class ItemType>
DoublyLinkedList<ItemType>::DoublyLinkedList(const DoublyLinkedList &aList) : headPtr_{nullptr}, itemCount_{0}
{
    if (!aList.isEmpty())
    {
        DoubleNode<ItemType> *calledHead = aList.getHeadPtr();
        int add_counter = 1; // keeps track of 1-indexed caller node position

        // iterates through callee list and inserts nodes into caller
        while (add_counter <= aList.getSize())
        {
            insert(calledHead->getItem(), add_counter);
            add_counter++;
            calledHead = calledHead->getNext();
        }
    }
}

/* Destructor */
template <class ItemType>
DoublyLinkedList<ItemType>::~DoublyLinkedList()
{
    clear();
}

/* Accessor: itemCount_ */
template <class ItemType>
int DoublyLinkedList<ItemType>::getSize() const
{
    return itemCount_;
}

/**
    @param item an object of type ItemType that will be 
        sought after in the caller
    @return the index of target if it is in the caller;
        -1 otherwise 
*/
template <class ItemType>
int DoublyLinkedList<ItemType>::getIndex(const ItemType &item)
{
    for (size_t i = 1; i < itemCount_; i++)
    {
        if (getAtPos(i)->getItem() == item)
        {
            return i;
        }
    }
    return -1;
}

/** 
    finds the node at parameter pos
    @pre pos is a valid place in the list, otherwise nullptr will be returned 
    @post returns a pointer to the node at pos
*/
template <class ItemType>
DoubleNode<ItemType> *DoublyLinkedList<ItemType>::getAtPos(const int &pos) const
{
    // handles edge case of invalid pos parameter
    if (pos > itemCount_ || pos < 1)
    {
        return nullptr;
    }

    DoubleNode<ItemType> *iterator = headPtr_;
    int iteration_count = 1;

    // iterates through list until the node at position pos is pointed to by iterator
    while (iteration_count < pos)
    {
        iterator = iterator->getNext();
        iteration_count++;
    }
    return iterator;
}

/* Accessor: headPtr_ */
template <class ItemType>
DoubleNode<ItemType> *DoublyLinkedList<ItemType>::getHeadPtr() const
{
    return headPtr_;
}

/**
    inserts parameter item into caller list at parameter position
    @pre position is a valid place within the list, otherwise false will be returned
    @post returns true if the item has been inserted in the caller list 
*/
template <class ItemType>
bool DoublyLinkedList<ItemType>::insert(const ItemType &item, const int &position)
{
    // handles edge case of an invalid position parameter
    if ((position < 1 || position > itemCount_ + 1) || (position > 1 && itemCount_ == 0))
    {
        return false;
    }

    // creates node in which to place item parameter
    DoubleNode<ItemType> *aNode = new DoubleNode<ItemType>;
    aNode->setItem(item);

    // case: no nodes exist within callee list
    if (itemCount_ == 0)
    {
        headPtr_ = aNode;
    }

    else
    {
        DoubleNode<ItemType> *iterator;

        // case: inserting into head of the list
        if (position == 1)
        {
            aNode->setNext(headPtr_);
            headPtr_->setPrevious(aNode);
            headPtr_ = aNode;
        }

        // case: inserting into rear of the list
        else if (position == itemCount_ + 1)
        {
            iterator = getAtPos(itemCount_);
            aNode->setPrevious(iterator);
            iterator->setNext(aNode);
        }

        // case: inserting into a position that is not an extremity
        else
        {
            iterator = getAtPos(position);
            aNode->setPrevious(iterator->getPrevious());
            aNode->setNext(iterator);
            (iterator->getPrevious())->setNext(aNode);
            iterator->setPrevious(aNode);
        }
    }

    itemCount_++;
    return true;
} // end insert()

/**
    removes node at parameter position
    @pre position is a valid place within the list, otherwise false will be returned
    @pre returns true if the item at position has been removed from the caller list 
*/
template <class ItemType>
bool DoublyLinkedList<ItemType>::remove(const int &position)
{
    // handles edge case of invalid position parameter
    if (position < 1 || position > itemCount_)
    {
        return false;
    }

    DoubleNode<ItemType> *iterator = getAtPos(position);

    // case: removing only node in list
    if (itemCount_ == 1)
    {
    }

    // case: removing from the end
    else if (position == itemCount_)
    {
        (iterator->getPrevious())->setNext(nullptr);
    }

    // case: removing from the beginning
    else if (position == 1)
    {
        (iterator->getNext())->setPrevious(nullptr);
        headPtr_ = iterator->getNext();
    }

    // case: removing from a position that is not an extremity
    else
    {
        (iterator->getPrevious())->setNext(iterator->getNext());
        (iterator->getNext())->setPrevious(iterator->getPrevious());
    }

    delete iterator;
    itemCount_--;
    return true;
} // end remove()

/* Accessor: itemCount_ == 0 ? */
template <class ItemType>
bool DoublyLinkedList<ItemType>::isEmpty() const
{
    return (itemCount_ == 0);
}

/**
    removes all items from the caller list
    @post no nodes remain in the list 
*/
template <class ItemType>
void DoublyLinkedList<ItemType>::clear()
{
    // handles edge case of empty list
    if (itemCount_ == 0)
    {
        return;
    }

    // iterates to the end of the list so that we can remove nodes from back to front
    DoubleNode<ItemType> *iterator = headPtr_;
    while (iterator->getNext() != nullptr)
    {
        iterator = iterator->getNext();
    }

    // removes nodes from back to front
    while (iterator != headPtr_)
    {
        DoubleNode<ItemType> *clone = iterator;
        iterator = iterator->getPrevious();
        delete clone;
        itemCount_--;
    }

    delete headPtr_;
    headPtr_ = nullptr;
    itemCount_ = 0;
} // end clear()

/** 
    iteratively outputs the contents of the caller list on the console 
*/
template <class ItemType>
void DoublyLinkedList<ItemType>::display() const
{
    // handles edge case of empty list
    if (itemCount_ == 0)
    {
        std::cout << std::endl;
        return;
    }

    DoubleNode<ItemType> *iterator = headPtr_;

    // iterates through nodes and prints their contents
    while (iterator != nullptr)
    {
        if (iterator->getNext() == nullptr)
        {
            std::cout << iterator->getItem() << std::endl;
            return;
        }
        else
        {
            std::cout << iterator->getItem() << " ";
        }
        iterator = iterator->getNext();
    }
    return;
} // end display()

/** 
    iteratively outputs the contents of the caller list backwards on the console 
*/
template <class ItemType>
void DoublyLinkedList<ItemType>::displayBackwards() const
{
    // handles edge case of empty list
    if (itemCount_ == 0)
    {
        std::cout << std::endl;
        return;
    }

    // iterate over list until iterator is the last node
    DoubleNode<ItemType> *iterator = headPtr_;
    while (iterator->getNext() != nullptr)
    {
        iterator = iterator->getNext();
    }

    // output the contents of the nodes from back to front
    while (iterator != nullptr)
    {
        if (iterator == headPtr_)
        {
            std::cout << iterator->getItem() << std::endl;
        }
        else
        {
            std::cout << iterator->getItem() << " ";
        }
        iterator = iterator->getPrevious();
    }
    return;
} // end displayBackwards()

/**
    returns a list which is the interleaved list of the caller and the callee 
*/
template <class ItemType>
DoublyLinkedList<ItemType> DoublyLinkedList<ItemType>::interleave(const DoublyLinkedList<ItemType> &aList)
{
    DoubleNode<ItemType> *iterator_1, *iterator_2;
    DoublyLinkedList<ItemType> interleaved;

    int insert_count = 1;
    int count = itemCount_ + aList.getSize();

    /* Iterates through each list and adds to interleaved in order */
    for (size_t i = 1; i < count + 1; i++)
    {
        iterator_1 = getAtPos(i);
        iterator_2 = aList.getAtPos(i);

        if (iterator_1 == nullptr)
        {
        }
        else
        {
            interleaved.insert(iterator_1->getItem(), insert_count);
            insert_count++;
        }
        if (iterator_2 == nullptr)
        {
        }
        else
        {
            interleaved.insert(iterator_2->getItem(), insert_count);
            insert_count++;
        }
    }

    return interleaved;
} // end interleave()

/**
    swaps element at index i with that at index j in caller
*/
template <class T>
void DoublyLinkedList<T>::swap(int i, int j)
{
    // ensures that the indeces are in the proper order for the operations that follow

    if (i > j)
    {
        int temp = i;
        i = j;
        j = temp;
    }

    // handles boundary conditions
    if (i < 1 || j > getSize())
    {
        std::cerr << "Indexing out of bounds." << std::endl;
        return;
    }

    else if (i == j) // trivial swap of an element with itself
    {
        return;
    }

    // initializes necessary pointers
    DoubleNode<T> *ptr_i = getAtPos(i);
    DoubleNode<T> *ptr_i_next = ptr_i->getNext();
    DoubleNode<T> *ptr_i_prev = ptr_i->getPrevious();
    DoubleNode<T> *ptr_j = getAtPos(j);
    DoubleNode<T> *ptr_j_next = ptr_j->getNext();
    DoubleNode<T> *ptr_j_prev = ptr_j->getPrevious();

    /* ACTUAL SWAPPING PROCEDURES FOLLOW */
    if (i == 1) // cases of 1st element swap
    {
        if (j == getSize()) // case: swap first and last nodes
        {
            ptr_j->setNext(ptr_i_next);
            ptr_j->setPrevious(nullptr);
            ptr_i_next->setPrevious(ptr_j);

            ptr_i->setNext(nullptr);
            ptr_i->setPrevious(ptr_j_prev);
            ptr_j_prev->setNext(ptr_i);

            headPtr_ = ptr_j;
        }

        else if (j == 2) // case: swap 1st and 2nd nodes
        {
            ptr_i->setNext(ptr_j_next);
            ptr_i->setPrevious(ptr_j);
            ptr_j->setNext(ptr_i);
            ptr_j->setPrevious(nullptr);
            ptr_j_next->setPrevious(ptr_i);

            headPtr_ = ptr_j;
        }

        else // case: swap 1st node with some node that
        {    //       occurs in the middle of the list
            ptr_i->setNext(ptr_j_next);
            ptr_j_next->setPrevious(ptr_i);

            ptr_i->setPrevious(ptr_j_prev);
            ptr_j_prev->setNext(ptr_i);

            ptr_i_next->setPrevious(ptr_j);
            ptr_j->setNext(ptr_i_next);

            ptr_j->setPrevious(nullptr);
            headPtr_ = ptr_j;
        }

        return;
    }

    else if (j != getSize()) // cases of middle elements swap
    {
        if (j - i == 1) // case: swap successive middle elements
        {
            ptr_i->setNext(ptr_j_next);
            ptr_i->setPrevious(ptr_j);

            ptr_j->setNext(ptr_i);
            ptr_j->setPrevious(ptr_i_prev);

            ptr_i_prev->setNext(ptr_j);
            ptr_j_next->setPrevious(ptr_i);
        }

        else // case: swap middle elements that are not successive
        {
            ptr_i->setNext(ptr_j_next);
            ptr_i->setPrevious(ptr_j_prev);

            ptr_j->setNext(ptr_i_next);
            ptr_j->setPrevious(ptr_i_prev);

            ptr_i_prev->setNext(ptr_j);
            ptr_j_prev->setNext(ptr_i);

            ptr_j_next->setPrevious(ptr_i);
            ptr_i_next->setPrevious(ptr_j);
        }
        return;
    }

    else if (j == getSize()) // cases of swapping the last element
    {                        // with one that is not the first
        if (j - 1 == i)      // case: swap last element with 2nd-to-last element
        {
            ptr_i->setNext(nullptr);
            ptr_i->setPrevious(ptr_j);

            ptr_j->setNext(ptr_i);
            ptr_j->setPrevious(ptr_i_prev);

            ptr_i_prev->setNext(ptr_j);
        }

        else // case: swap last element with any element that
        {    //       occurs in the middle of the list
            ptr_i->setNext(ptr_j_next);
            ptr_i->setPrevious(ptr_j_prev);

            ptr_j->setNext(ptr_i_next);
            ptr_j->setPrevious(ptr_i_prev);

            ptr_j_prev->setNext(ptr_i);

            ptr_i_prev->setNext(ptr_j);
            ptr_i_next->setPrevious(ptr_j);
        }
        return;
    }
}