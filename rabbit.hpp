#ifndef RABBIT_HPP
#define RABBIT_HPP

#include "creature.hpp"

class Rabbit : public Creature{   
public:
    Rabbit();
    void makePregnant();
    void step();
    bool isAlive();   
    bool operator ==(const Rabbit& r);
    bool operator !=(const Rabbit& r);
    void fixCoords();
};

#endif // RABBIT_HPP
