#include "Solutions.cpp"

using namespace solutions;

int main() {
    DoublyLinkedList<int> foo;

    foo.insert(1);
    foo.insert(3);
    foo.insert(2);
    foo.insert(4);

    foo.display();

    mergeSort(foo, std::less<int>{});
    
    foo.display();
    
    return 0;
}