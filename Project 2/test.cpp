#include <iostream>
#include "ArrayBag.hpp"

int main()
{
        ArrayBag<std::string> arr1;
        ArrayBag<std::string> arr2;

        arr1.add("A");
        arr1.add("B");
        arr1.add("C");
        arr1.add("D");
        arr1.add("E");

        arr2.add("B");
        arr2.add("D");

        arr1 /= arr2;
        ;

        arr1.display();

        return 0;
}