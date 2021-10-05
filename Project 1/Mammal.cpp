#pragma once

#include "Mammal.hpp"

//Default Constructor
Mammal::Mammal() {
        name_ = "";
        domestic_ = false;
        predator_ = false;
        hair_ = false;
        airborne_ = false;
        aquatic_ = false;
        toothed_ = false;
        fins_ = false;
        tail_ = false;
        legs_ = 0;
}

//Parameterized Constructor
Mammal::Mammal(std::string name, bool domestic, bool predator) {
        name_ = name;
        domestic_ = domestic;
        predator_ = predator;
        hair_ = false;
        airborne_ = false;
        aquatic_ = false;
        toothed_ = false;
        fins_ = false;
        tail_ = false;
        legs_ = 0;
}

//Returns if mammal has hair
bool Mammal::hasHair() const {
        return hair_;
}

//Returns if mammal is airborne
bool Mammal::isAirborne() const {
        return airborne_;
}

//Returns if mammal is aquatic
bool Mammal::isAquatic() const {
        return aquatic_;
}

//Returns if mammal has teeth
bool Mammal::isToothed() const {
        return toothed_;
}

//Returns if mammal has fins
bool Mammal::hasFins() const {
        return fins_;
}

//Returns if mammal has a tail
bool Mammal::hasTail() const {
        return tail_;
}

//Returns number of legs mammal has
int Mammal::legs() const {
        return legs_;
}

//Sets hair_ to true
void Mammal::setHair() {
        hair_ = true;
}

//Sets airborne_ to true
void Mammal::setAirborne() {
        airborne_ = true;
}

//Sets aquatic_ to true
void Mammal::setAquatic() {
        aquatic_ = true;
}

//Sets toothed_ to true
void Mammal::setToothed() {
        toothed_ = true;
}

//Sets fins_ to true
void Mammal::setFins() {
        fins_ = true;
}

//Sets tail_ to true
void Mammal::setTail() {
        tail_ = true;
}

//Sets legs_ equal to legs
void Mammal::setLegs(int legs) {
        legs_ = legs;
}

//Displats mammal name and if domestic, if predator, if airborne, if aquatic,
//if has hair, if has teeth, if has fins, if has tail, and number of legs mammal has
void Mammal::display() {
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

        message += "aquatic,\nit has ";

        if (!hair_) {
                message += "no ";
        }

        message += "hair, ";

        if (!toothed_) {
                message += "no ";
        }

        message += "teeth, ";

        if (!fins_) {
                message += "no ";
        }

        message += "fins, ";

        if (!tail_) {
                message += "no ";
        }

        message += "tail and " + std::to_string(legs_) + " legs.\n";

        std::cout << message;
}