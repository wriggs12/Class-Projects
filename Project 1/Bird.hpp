#pragma once

#include "Animal.cpp"
#include <iostream>

class Bird : public Animal {
        public:
                Bird(); //Default Constructor
                Bird(std::string name, bool demstic = false, bool predator = false); //Parameterized Constructor

                /**Checks to see if Bird is airborne_
                 *      @return airborne_
                 * */
                bool isAirborne() const;

                /**Checks to see if Bird aquatic_ is true
                 *      @return aquatic_
                 * */
                bool isAquatic() const;

                /**Mutates airborne_ and sets it to true
                 *      @post airborne_ is set to true
                 * */
                void setAirborne();

                /**Mustates aquatic_ and sets it to true
                 *      @post aquatic_ is set to true
                 * */
                void setAquatic();

                /**@post displays bird data in specific format
                 *
                 * */
                void display() override;

        protected:
                bool airborne_; //Is Bird airborne
                bool aquatic_; //Is bird aquatic
};