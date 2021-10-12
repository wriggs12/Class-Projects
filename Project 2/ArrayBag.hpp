#ifndef ARRAY_BAG_
#define ARRAY_BAG_

#include <vector>

template <class ItemType>
class ArrayBag
{

public:
  /** default constructor**/
  ArrayBag();

  /**
     @return item_count_ : the current size of the bag
    **/
  int getCurrentSize() const;

  /**
     @return true if item_count_ == 0, false otherwise
     **/
  bool isEmpty() const;

  /**
     @return true if new_entry was successfully added to items_, false otherwise
     **/
  bool add(const ItemType &new_entry);

  /**
     @return true if an_entry was successfully removed from items_, false otherwise
     **/
  bool remove(const ItemType &an_entry);

  /**
     @post item_count_ == 0
     **/
  void clear();

  /**
     @return true if an_entry is found in items_, false otherwise
     **/
  bool contains(const ItemType &an_entry) const;

  /**
    @return the number of times an_entry is found in items_
  **/
  int getFrequencyOf(const ItemType &an_entry) const;

  /**
    @return a vector having the same contents as items_
    **/
  std::vector<ItemType> toVector() const;

  /**
    @post prints the contents of items_ to the standard output separated by commas and followed by a new line.
    Example: A, B, C, D, E\n
    Note how there is no comma and space for the last item.
	**/
  void display() const;

  /**
      duplicate all the items in the bag 
      meaning that for every item in the bag we add another copy of it. 
      Example: we originally have [A, B] and after duplication we have [A, B, A, B] or [A, A, B, B] or how ever order you want so long as there are 2 A's and 2 B's
      Return true if successfully duplicated 
      but return false if there is nothing to duplicate or if we go above the DEFAULT_CAPACITY which is 10 for this project
    */
  bool duplicateItems();

  /**
    @post every instance of an_entry is removed
  */
  void removeAllInstances(const ItemType &an_entry);

  /**
    @return true if we managed to remove all items that have duplicates in the bag
    Example: a duplicate would be [A, B, C, C, D, D, D, E]
    What we should have after running this function: [A, B, C, D, E] (may be out of order depending on how you do it, that doesn't matter)
    @return false if there are 1 or less items in the bag
  */
  bool removeDuplicates();

  /* ---------- OPERATOR OVERLOADS ----------*/

  /** 
    implements Set Union. The union of two sets A and B is the set of elements which are in A, in B, or in both A and B.  
		@param a_bag to be combined with the contents of this (the calling) bag
		@post adds as many items from a_bag as space allows    
	*/
  void operator+=(const ArrayBag<ItemType> &a_bag);

  /** 
    implements Set Difference  The (set) difference between two sets A and B is the set that consists of the elements of A which are not elements of  B 
		@param a_bag to be subtracted from this (the calling) bag
		@post removes all data from items_ that is also found in a_bag
    Example if A has [1, 2, 3, 4, 5] and B has [4, 5, 6, 7], then A-=B will result in A containing [1, 2, 3] and B remaining the same
	*/
  void operator-=(const ArrayBag<ItemType> &a_bag);

  /** 
    implements Set Intersection  The intersection of two sets A and B is the set that consists of the elements that are in both A and B 
		@param a_bag to be intersected with this (the calling) bag
		@post items_ no longer contains data not found in a_bag    
    Example if bag A has [1,2,3,4,5] and Bag B has [7,8,5,9] then A /= B will result in A containing only 5 since 5 is found in both bag A and bag B
	*/
  void operator/=(const ArrayBag<ItemType> &a_bag);

protected:
  static const int DEFAULT_CAPACITY = 10; //max size of items_ at 10 by default for this project
  ItemType items_[DEFAULT_CAPACITY];      // Array of bag items
  int item_count_;                        // Current count of bag items

  /**
     @param target to be found in items_
     @return either the index target in the array items_ or -1,
     if the array does not contain the target.
     **/
  int getIndexOf(const ItemType &target) const;

}; // end ArrayBag

#include "ArrayBag.cpp"
#endif
