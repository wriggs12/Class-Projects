#pragma once

#include "Bird.hpp"

//Default Constructor
Bird::Bird() {
        name_ = "";
        airborne_ = false;
        aquatic_ = false;
        airborne_ = false;
        aquatic_ = false;
}

//Parameterized Constructor
Bird::Bird(std::string name, bool domestic, bool predator) {
        name_ = name;
        domestic_ = domestic;
        predator_ = predator;
        airborne_ = false;
        aquatic_ = false;
}

//Returns airborne_
bool Bird::isAirborne() const {
        return airborne_;
}

//Returns aquatic_
bool Bird::isAquatic() const {
        return aquatic_;
}

//Sets airborne_ to true
void Bird::setAirborne() {
        airborne_ = true;
}

//Sets aquatic_ to true
void Bird::setAquatic() {
        aquatic_ = true;
}

//Displays a message stating the name, if domestic, if predator, if airborne, and if aquatic
void Bird::display() {
        std::string message = name_ + " is ";

        if (!domestic_) {
                message += "not ";
        }

        message += "domestic and ";

        if (predator_) {
                message += "it is ";
        }
        else {
                message += "is not ";
        }

        message += "a predator,\nit is ";

        if (!airborne_) {
                message += "not ";
        }

        message += "airborne and it is ";

        if (!aquatic_) {
                message += "not ";
        }

        message += "aquatic.\n";

        std::cout << message;
}