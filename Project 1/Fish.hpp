#pragma once

#include "Animal.cpp"

class Fish : public Animal {
        public:
                Fish(); //Default Constructor
                Fish(std::string name, bool domestic = false, bool predator = false); //Parameterized Constructor

                /**Checks to see if venomous_ is true
                 *      @return venomous_
                 * */
                bool isVenomous() const;

                /**Mutates venomous_ and sets it to true
                 *      @post venomous_ is set to true
                 * */
                void setVenomous();

                /**@post displays fish data in given format
                 *
                 * */
                void display() override;

        protected:
                bool venomous_; //Is fish venomous
};