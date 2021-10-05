#pragma once

#include <iostream>

class Animal {
        public:
                Animal(); //Default Constructor
                Animal(std::string name, bool domestic = false, bool predator = false); //Parameterized Constructor

                /**Gives you that animal's name
                 *      @return the name_
                 * */
                std::string getName() const;

                /**Checks if animal is domestic
                 *      @return true if domestic_ is true
                 * */
                bool isDomestic() const;

                /**Checks if animal is a predator
                 *      @return true if predator_ is true
                 * */
                bool isPredator() const;

                /**Sets the animal's name to string name
                 *      @param name of the animal you want to set it to
                 * */
                void setName(std::string name);

                /**Mutates the animal's domestic_ variable to true
                 *      @post domestic_ is set to ture
                 * */
                void setDomestic();

                /**Mutates the animal's predator_ variable to true
                 *      @post predator_ is set to true
                 * */
                void setPredator();

                /**Display method that will be overwritten with each child class
                 *
                 * */
                virtual void display() = 0;

        protected:
                std::string name_; //Animal name
                bool domestic_; //Is domestic aniaml
                bool predator_; //is animal a predator
};