#pragma once

#include "Fish.hpp"

//Default Constructor
Fish::Fish() {
        venomous_ = false;
}

//Parameterized Constructor
Fish::Fish(std::string name, bool domestic, bool predator) {
        name_ = name;
        domestic_ = domestic;
        predator_ = predator;
        venomous_ = false;
}

//Returns if fish is venomous
bool Fish::isVenomous() const {
        return venomous_;
}

//Sets fish to venomous
void Fish::setVenomous() {
        venomous_ = true;
}

//Displays fish name and if domestic, if predator, and if venomous
void Fish::display() {
        std::string message = name_ + " is ";

        if (!domestic_) {
                message += "not ";
        }

        message += "domestic, ";

        if (predator_) {
                message += "it is ";
        }
        else {
                message += "is not ";
        }

        message += "a predator\nand it is ";

        if (!venomous_) {
                message += "not ";
        }

        message += "venomous.\n";

        std::cout << message;
}