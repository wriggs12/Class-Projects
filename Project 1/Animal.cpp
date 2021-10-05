#pragma once

#include "Animal.hpp"

//Default Constructor
Animal::Animal() {
        name_ = ""; //Initializes the name string variable
        domestic_ = false; //Sets domestic_ to false
        predator_ = false; //Sets predator_ to false
}

//Paramaterized Constructor
Animal::Animal(std::string name, bool domestic, bool predator) {
        name_ = name;
        domestic_ = domestic;
        predator_ = predator;
}

//Returns the animals name
std::string Animal::getName() const {
        return name_;
}

//Returns whether or not the animal is domestic
bool Animal::isDomestic() const {
        return domestic_;
}

//Returns whether or not the animal is a predator
bool Animal::isPredator() const {
        return predator_;
}

//Sets the animals name to a new given name
void Animal::setName(std::string name) {
        name_ = name;
}

//Sets the animal to a domestic animal
void Animal::setDomestic() {
        domestic_ = true;
}

//Sets the animal to a predator
void Animal::setPredator() {
        predator_ = true;
}