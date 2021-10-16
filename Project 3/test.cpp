#include "DoublyLinkedList.hpp"
#include "DoubleNode.hpp"
#include <iostream>

int main() {
    DoublyLinkedList<int> test = DoublyLinkedList<int>();
    test.insert(1, 1);
    test.insert(2, 2);
    test.insert(3, 3);
    test.insert(4, 4);
    test.insert(5, 5);


    DoublyLinkedList<int> test1 = DoublyLinkedList<int>(test);

    //test.displayBackwards();

    test.display();
    //test1.display();

    test.insert(6, 1);
    test.remove(5);

    test1.insert(8, 6);

    test.display();
    test1.display();

    //DoubleNode<int> node1(1, nullptr, nullptr);
    //DoubleNode<int> node2(2, nullptr, nullptr);

    //node1.setNext(&node2);

    return 0;
}