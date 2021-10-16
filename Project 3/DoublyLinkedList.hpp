#ifndef DOUBLY_LINKED_LIST_
#define DOUBLY_LINKED_LIST_

#include "DoubleNode.hpp"

template <class ItemType>
class DoublyLinkedList
{
public:
    DoublyLinkedList(); //default constructor, sets headPtr_ to null and itemCount_ to 0

    /*
        Copy constructor that will make an exact copy of the list parameter, aList.
        This will make a deep copy of the list in the given parameter.
        You must iterate through the callee list parameter and insert nodes from aList into the caller list (the current one that isn't the parameter)
    */
    DoublyLinkedList(const DoublyLinkedList &aList);

    ~DoublyLinkedList(); //destructor that calls the clear function

    int getSize() const; //return itemCount_

    /* * 
        * Description: finds the node at parameter pos
        * Pre: pos is a valid place in the list, otherwise nullptr will be returned 
        * Post: returns a pointer to the node at pos
    * */
    DoubleNode<ItemType> *getAtPos(const int &pos) const;

    DoubleNode<ItemType> *getHeadPtr() const; //return *headPtr_

    /* *
        * Description: inserts parameter item in caller list at parameter position
            *IMPORTANT: Position 1 is the first position, not 0
        * Pre: position is a valid place within the list, otherwise false will be returned
        * Post: returns true if the item has been inserted in the caller list 
        * Handles edge case of an invalid position parameter
        * Case: Inserting into head of the list
        * Case: Inserting into rear of the list
        * Case: Inserting into a position that is not an extremity 
    * */
    bool insert(const ItemType &item, const int &position = 1);

    /* *
        * Description: removes node at parameter position
            *IMPORTANT: Position 1 is the first position, not 0
        * Pre: position is a valid place within the list, otherwise false will be returned
        * Post: returns true if the item at position has been removed from the caller list 
        * Handles edge case of invalid position parameter
        * Case: removing only node in list
        * Case: removing from the end
        * Case: removing from the beginning
        * Case: removing from a position that is not an extremity
    * */
    bool remove(const int &position);

    bool isEmpty() const; //returns true if itemCount_ is 0

    /* *
        * Description: removes all items from the caller list
        * Traverse through the linked list and delete each individual node
        * Post: no nodes remain in the list and itemCount_ is at 0
    * */
    void clear();

    //Iteratively outputs the contents of the caller list on the console
    //Example "A B C D E"
    //Note how theres no space at the end
    void display() const;

    //Iteratively outputs the contents of the caller list backwards on the console
    //Example if we had A B C D E we would instead display "E D C B A"
    void displayBackwards() const;

    // Reverses the list such that if my list was A B C D it will now be D C B A
    // Remember to change both previous and next pointers
    // Will be tested with both display and displayBackwards, make sure you have those working
    void invert();

private:
    DoubleNode<ItemType> *headPtr_; //points to the first item in the linked list
    int itemCount_;                 //lists how many items are currently in the list
};

#include "DoublyLinkedList.cpp"
#endif
