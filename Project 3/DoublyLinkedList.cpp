/**
 * Name: DoublyLinkedList Implementation
 * Class: CSCI 235
 * Assignment: Project 3
 * Description: Implements a two-way linked list
 * */

#include <iostream>

/** default constructor **/
template <class ItemType>
DoublyLinkedList<ItemType>::DoublyLinkedList() {
    headPtr_ = nullptr;
    itemCount_ = 0;
}

/** parameterized constructor **/
template <class ItemType>
DoublyLinkedList<ItemType>::DoublyLinkedList(const DoublyLinkedList &aList) {
    int size = aList.getSize();
    itemCount_ = 0;

    for (int i = 1; i <= size; i++) {
        ItemType curItem = aList.getAtPos(i)->getItem();
        insert(curItem, i);
    }
}

/** destructor **/
template <class ItemType>
DoublyLinkedList<ItemType>::~DoublyLinkedList() {
    clear();
}

/**
 * @return itemCount_: the current number of elements in the list
 * */
template <class ItemType>
int DoublyLinkedList<ItemType>::getSize() const {
    return itemCount_;
}

/**
 * @return DoubleNode<ItemType> : the node that is at position pos
 * */
template <class ItemType>
DoubleNode<ItemType>* DoublyLinkedList<ItemType>::getAtPos(const int &pos) const {
    DoubleNode<ItemType> *curPointer = headPtr_;

    for (int i = 0; i < pos; i++) {
        curPointer = curPointer->getNext();
    }

    return curPointer;
}

/**
 * @return headPts_ : the head node in the linked list
 * */
template <class ItemType>
DoubleNode<ItemType>* DoublyLinkedList<ItemType>::getHeadPtr() const {
    return headPtr_;
}

/**
 * @return true is insertion was successful
 * */
template <class ItemType>
bool DoublyLinkedList<ItemType>::insert(const ItemType &item, const int &position) {
    if (position > itemCount_ + 1 || position < 1)
        return false;
    
    if (itemCount_ == 0) {
        headPtr_ = new DoubleNode<ItemType>();
        DoubleNode<ItemType> *newNode = new DoubleNode<ItemType>(item, nullptr, headPtr_);

        headPtr_->setNext(newNode);
    }
    else {
        DoubleNode<ItemType> *curNode = headPtr_;
        
        for (int i = 0; i < position - 1; i++) {
            curNode = curNode->getNext();
        }

        DoubleNode<ItemType> *newNode = new DoubleNode<ItemType>(item, curNode->getNext(), curNode);
        
        curNode->setNext(newNode);
        
        if (position != itemCount_ + 1)
            newNode->getNext()->setPrevious(newNode);
    }

    itemCount_++;

    return true;
}

/**
 * @return true if deletion was successful
 * */
template <class ItemType>
bool DoublyLinkedList<ItemType>::remove(const int &position) {
    if (position > itemCount_ || position < 1)
        return false;

    if (itemCount_ == 1) {
        delete headPtr_->getNext();
        headPtr_->setNext(nullptr);
    }
    else if (position == 1) {
        headPtr_->setNext(headPtr_->getNext()->getNext());

        delete headPtr_->getNext()->getPrevious();

        headPtr_->getNext()->setPrevious(headPtr_);
    }
    else {
        DoubleNode<ItemType> *curNode = headPtr_;
        
        for (int i = 0; i < position - 1; i++) {
            curNode = curNode->getNext();
        }

        DoubleNode<ItemType> *nodeToBeRemoved = curNode->getNext();

        curNode->setNext(nodeToBeRemoved->getNext());

        delete nodeToBeRemoved;

        if (position != itemCount_)
            curNode->getNext()->setPrevious(curNode);
    }

    itemCount_--;

    return true;
}

/**
 * @return true if list is empty
 * */
template <class ItemType>
bool DoublyLinkedList<ItemType>::isEmpty() const {
    return itemCount_ == 0;
}

/**
 * @post all nodes in list are removed and deleted, headPtr_ is set to nullptr
 * */
template <class ItemType>
void DoublyLinkedList<ItemType>::clear() {
    int size = getSize();

    for (int i = 0; i < size; i++) {
        remove(itemCount_);
    }

    headPtr_ = nullptr;
}

/**
 * @post Prints out the list to the console
 * */
template <class ItemType>
void DoublyLinkedList<ItemType>::display() const {
    if (itemCount_ > 0) {
        DoubleNode<ItemType> *curNode = headPtr_->getNext();
    
        for (int i = 0; i < itemCount_ - 1; i++) {
            std::cout << curNode->getItem() << " ";
            curNode = curNode->getNext();
        }

            std::cout << curNode->getItem() << std::endl;
    }
}

/**
 * @post prints out list backwards to the console
 * */
template <class ItemType>
void DoublyLinkedList<ItemType>::displayBackwards() const {
    if (itemCount_ > 0) {
        DoubleNode<ItemType> *curNode = headPtr_;

        while(curNode->getNext() != nullptr) {
            curNode = curNode->getNext();
        }
    
        for (int i = 0; i < itemCount_ - 1; i++) {
            std::cout << curNode->getItem() << " ";
            curNode = curNode->getPrevious();
        }

            std::cout << curNode->getItem() << std::endl;
    }
}

/**
 * @post list is reversed
 * */
template <class ItemType>
void DoublyLinkedList<ItemType>::invert() {
    ItemType elements [itemCount_];

    for (int i = 0; i < itemCount_; i++) {
        elements[i] = getAtPos(i + 1)->getItem();
    }

    DoubleNode<ItemType> *curNode = headPtr_;

    for (int i = 0; i < itemCount_; i++) {
        curNode = curNode->getNext();
        curNode->setItem(elements[itemCount_ - i - 1]);
    }
}