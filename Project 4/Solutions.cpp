/**
 * Name: Sorting Algorithms Implementations
 * Class: CSCI 235
 * Assignment: Project 4
 * Description: Implements the bubble, insertion, and merge sort algorithms
 * */

#include "DoublyLinkedList.hpp"
#include <string>

namespace solutions
{
  /* Bubblesort */
  template <typename Comparable, typename Comparator>
  void bubbleSort(DoublyLinkedList<Comparable> &a_list, const Comparator &comp) {
    //For every element in the list
    for (int i = 1; i <= a_list.getSize(); i++) {
      //For evey unsorted element in the list
      for (int j = 1; j <= a_list.getSize() - i; j++) {
        //If the order is incorrect
        if (comp(a_list.getAtPos(j + 1)->getItem(), a_list.getAtPos(j)->getItem())) {
          //swap the two elements
          a_list.swap(j, j + 1);
        }
      }
    }
  }

  /* Insertionsort */
  template <typename Comparable, typename Comparator>
  void insertionSort(DoublyLinkedList<Comparable> &a_list, const Comparator &comp) {
    //For every element in the list
    for (int i = 1; i <= a_list.getSize(); i++) {
      int index = i - 1;
      int compIndex = i;
      //While that element is in the wrong positions
      while(index > 0 && comp(a_list.getAtPos(compIndex)->getItem(), a_list.getAtPos(index)->getItem())) {
        //Swap the two elements
        a_list.swap(index, compIndex);
        index--;
        compIndex--;
      }
    }
  }

  /* Merge */
  template <typename Comparable, typename Comparator>
  void merge(DoublyLinkedList<Comparable> &a_list, int left_index, int middle_index, int right_index, const Comparator &comp) {
    DoublyLinkedList<Comparable> leftArr; //Left array to combine
    DoublyLinkedList<Comparable> rightArr; //Right array to combine

    //Fill the left array with the elements
    for (int i = left_index; i < middle_index; i++) {
      leftArr.insert(a_list.getAtPos(i)->getItem());
    }

    //Fill the right array with the elements
    for (int i = middle_index; i <= right_index; i++) {
      rightArr.insert(a_list.getAtPos(i)->getItem());
    }

    int left = leftArr.getSize();
    int right = rightArr.getSize();
    int curIndex = left_index; //current index in the main array

    //While there are elements that aren't sorted in yet
    while(left > 0 || right > 0) {
      //clear the space for the new element
      a_list.remove(curIndex);
      //if all elements in left array are inputed
      if (left == 0) {
        a_list.insert(rightArr.getAtPos(right)->getItem(), curIndex);
        curIndex++;
        right--;
      }
      //if all elements in right array are inputed
      else if (right == 0) {
        a_list.insert(leftArr.getAtPos(left)->getItem(), curIndex);
        curIndex++;
        left--;
      }
      else if (comp(leftArr.getAtPos(left)->getItem(), rightArr.getAtPos(right)->getItem())) {
        a_list.insert(leftArr.getAtPos(left)->getItem(), curIndex);
        curIndex++;
        left--;
      }
      else {
        a_list.insert(rightArr.getAtPos(right)->getItem(), curIndex);
        curIndex++;
        right--;
      }
    }
  }

  /* Mergesort */
  template <typename Comparable, typename Comparator>
  void mergeSort(DoublyLinkedList<Comparable> &a_list, int left_index, int right_index, const Comparator &comp) {
    //Base Case
    if (left_index >= right_index)
      return;

    //Sort left half of the array
    mergeSort(a_list, left_index, ((right_index - left_index) / 2) + left_index, comp);
    //Sort right half of the array
    mergeSort(a_list, ((right_index - left_index) / 2) + left_index + 1, right_index, comp);

    //Merge the two halves
    merge(a_list, left_index, ((right_index - left_index) / 2) + left_index + 1, right_index, comp);
  }

  /* Mergesort Wrapper */
	//this is what you use to test
  template <typename Comparable, typename Comparator>
  void mergeSort(DoublyLinkedList<Comparable> &a_list, const Comparator &comp) {
		mergeSort(a_list, 1, a_list.getSize(),comp);
  }

}; // namespace solutions