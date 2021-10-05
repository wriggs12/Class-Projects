#pragma once

#include "Animal.cpp"

class Mammal : public Animal {
        public:
                Mammal(); //Default Constructor
                Mammal(std::string name, bool domestic = false, bool predator = false); //Parameterized Constructor

                /**Checks to see if mammal has hair
                 *      @return hair_
                 * */
                bool hasHair() const;

                /**Checks to see if mammal is airborne
                 *      @return airborne_
                 * */
                bool isAirborne() const;

                /**Checks to see if mammal is aquatic
                 *      @return aquatic_
                 * */
                bool isAquatic() const;

                /**Checks to see if mammal has teeth
                 *      @return toothed_
                 * */
                bool isToothed() const;

                /**Checks to see if mammal has fins
                 *      @return fins_
                 * */
                bool hasFins() const;

                /**Checks to see if mammal has tail
                 *      @return tail_
                 * */
                bool hasTail() const;

                /**Gets the number of legs the mammal has
                 *      @return legs_
                 * */
                int legs() const;

                /**Mutates hair_ and sets it to true
                 *      @post hair_ is set to true
                 * */
                void setHair();

                /**Mutates airborne_ to true
                 *      @post airborne_ is set to true
                 * */
                void setAirborne();

                /**Mutates aquatic_ to true
                 *      @post aquatic_ is set to true
                 * */
                void setAquatic();

                /**Mutates toothed_ to true
                 *      @post toothed_ is set tot true
                 * */
                void setToothed();

                /**Mutates fins_ to true
                 *      @fins_ is set to true
                 * */
                void setFins();

                /**Mutates tail_ to true
                 *      @post tail_ is set to true
                 * */
                void setTail();

                /**Mutates legs_ to new int
                 *      @post legs_ is set to legs parameter
                 * */
                void setLegs(int legs);

                /**@post displays mammal data in given format
                 *
                 * */
                void display() override;

        protected:
                bool hair_; //If mammal has hair
                bool airborne_; //If mammal is airborne
                bool aquatic_; //If mammal is aquatic
                bool toothed_; //If mammal has teeth
                bool fins_; //If mammal has fins
                bool tail_; //If mammal has a tail
                int legs_; //Number of legs mammal has
};