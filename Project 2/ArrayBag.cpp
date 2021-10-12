/**
 * Name: ArrayBag Implementation
 * Class: CSCI 235
 * Assignment: Project 2
 * Description: Implements the ArrayBag<> class from ArrayBag.hpp
 * */

#include <iostream>

/** default constructor**/
template <class ItemType>
ArrayBag<ItemType>::ArrayBag() {
    item_count_ = 0;
}

/**
     @return item_count_ : the current size of the bag
    **/
template <class ItemType>
int ArrayBag<ItemType>::getCurrentSize() const {
    return item_count_;
}

/**
     @return true if item_count_ == 0, false otherwise
     **/
template <class ItemType>
bool ArrayBag<ItemType>::isEmpty() const {
    return item_count_ == 0;
}

/**
     @return true if new_entry was successfully added to items_, false otherwise
     **/
template <class ItemType>
bool ArrayBag<ItemType>::add(const ItemType &new_entry) {
    if (item_count_ == DEFAULT_CAPACITY)
        return false;
    
    items_[item_count_] = new_entry;
    item_count_++;

    return true;
}

/**
     @return true if an_entry was successfully removed from items_, false otherwise
     **/
template <class ItemType>
bool ArrayBag<ItemType>::remove(const ItemType &an_entry) {
    for (int i = 0; i < item_count_; i++) {
        if (items_[i] == an_entry) {
            item_count_--;
            items_[i] = items_[item_count_];
            return true;
        }
    }
    
    return false;
}

/**
     @post item_count_ == 0
     **/
template <class ItemType>
void ArrayBag<ItemType>::clear() {
    for (int i = 0; i < item_count_; i++) {
        remove(items_[i]);
    }
    item_count_ = 0;
}

/**
     @return true if an_entry is found in items_, false otherwise
     **/
template <class ItemType>
bool ArrayBag<ItemType>::contains(const ItemType &an_entry) const {
    for (int i = 0; i < item_count_; i++) {
        if (items_[i] == an_entry)
            return true;
    }

    return false;
}

/**
    @return the number of times an_entry is found in items_
  **/
template <class ItemType>
int ArrayBag<ItemType>::getFrequencyOf(const ItemType &an_entry) const {
    int frequency = 0;
    
    for (int i = 0; i < item_count_; i++) {
        if (items_[i] == an_entry)
            frequency++;
    }

    return frequency;
}

/**
    @return a vector having the same contents as items_
    **/
template <class ItemType>
std::vector<ItemType> ArrayBag<ItemType>::toVector() const {
    std::vector<ItemType> itemsVector;

    for (int i = 0; i < item_count_; i++) {
        itemsVector.push_back(items_[i]);
    }

    return itemsVector;
}

/**
    @post prints the contents of items_ to the standard output separated by commas and followed by a new line.
    Example: A, B, C, D, E\n
    Note how there is no comma and space for the last item.
	**/
template <class ItemType>
void ArrayBag<ItemType>::display() const {
    for (int i = 0; i < item_count_ - 1; i++) {
        std::cout << items_[i] << ", ";
    }

    if (getCurrentSize() >= 1)
        std::cout << items_[item_count_ - 1] << std::endl;
}

/**
      duplicate all the items in the bag 
      meaning that for every item in the bag we add another copy of it. 
      Example: we originally have [A, B] and after duplication we have [A, B, A, B] or [A, A, B, B] or how ever order you want so long as there are 2 A's and 2 B's
      Return true if successfully duplicated 
      but return false if there is nothing to duplicate or if we go above the DEFAULT_CAPACITY which is 10 for this project
    */
template <class ItemType>
bool ArrayBag<ItemType>::duplicateItems() {
    if ((item_count_ * 2) > DEFAULT_CAPACITY)
        return false;

    int currentCapacity = item_count_;

    for (int i = 0; i < currentCapacity; i++) {
        add(items_[i]);
    }

    return true;
}

/**
    @post every instance of an_entry is removed
  */
template <class ItemType>
void ArrayBag<ItemType>::removeAllInstances(const ItemType &an_entry) {
    for (int i = 0; i < item_count_; i++) {
        if (items_[i] == an_entry) {
            remove(items_[i]);
        }
    }
}

/**
    @return true if we managed to remove all items that have duplicates in the bag
    Example: a duplicate would be [A, B, C, C, D, D, D, E]
    What we should have after running this function: [A, B, C, D, E] (may be out of order depending on how you do it, that doesn't matter)
    @return false if there are 1 or less items in the bag
  */
template <class ItemType>
bool ArrayBag<ItemType>::removeDuplicates() {
    if (getCurrentSize() <= 1)
        return false;

    for (int i = 0; i < getCurrentSize(); i++)
        if (getFrequencyOf(items_[i]) > 1) {
            remove(items_[i]);
            i--;
        }

    return true;
}

/** 
    implements Set Union. The union of two sets A and B is the set of elements which are in A, in B, or in both A and B.  
		@param a_bag to be combined with the contents of this (the calling) bag
		@post adds as many items from a_bag as space allows    
	*/
template <class ItemType>
void ArrayBag<ItemType>::operator+=(const ArrayBag<ItemType> &a_bag) {
    for (int i = 0; i < a_bag.getCurrentSize(); i++) {
        
        if (item_count_ == DEFAULT_CAPACITY)
            i = a_bag.getCurrentSize();
        
        else if (getIndexOf(a_bag.items_[i]) == -1)
            add(a_bag.items_[i]);
    }
}

/** 
    implements Set Difference  The (set) difference between two sets A and B is the set that consists of the elements of A which are not elements of  B 
		@param a_bag to be subtracted from this (the calling) bag
		@post removes all data from items_ that is also found in a_bag
    Example if A has [1, 2, 3, 4, 5] and B has [4, 5, 6, 7], then A-=B will result in A containing [1, 2, 3] and B remaining the same
	*/
template <class ItemType>
void ArrayBag<ItemType>::operator-=(const ArrayBag<ItemType> &a_bag) {
    for (int i = 0; i < a_bag.getCurrentSize(); i++) {
        if (getIndexOf(a_bag.items_[i]) != -1)
            remove(a_bag.items_[i]);
    }
}

/** 
    implements Set Intersection  The intersection of two sets A and B is the set that consists of the elements that are in both A and B 
		@param a_bag to be intersected with this (the calling) bag
		@post items_ no longer contains data not found in a_bag    
    Example if bag A has [1,2,3,4,5] and Bag B has [7,8,5,9] then A /= B will result in A containing only 5 since 5 is found in both bag A and bag B
	*/
template <class ItemType>
void ArrayBag<ItemType>::operator/=(const ArrayBag<ItemType> &a_bag) {
    int curSize1 = getCurrentSize();
    int curSize2 = a_bag.getCurrentSize();
    
    for (int i = 0; i < curSize2; i++) {
        if (getIndexOf(a_bag.items_[i]) == -1)
            remove(a_bag.items_[i]);
    }

    for (int i = 0; i < curSize1; i++) {
        if (a_bag.getIndexOf(items_[i]) == -1)
            remove(items_[i]);
    }
}

/**
     @param target to be found in items_
     @return either the index target in the array items_ or -1,
     if the array does not contain the target.
     **/
template <class ItemType>
int ArrayBag<ItemType>::getIndexOf(const ItemType &target) const {
    for (int i = 0; i < item_count_; i++) {
        if (items_[i] == target)
            return i;
    }

    return -1;
}