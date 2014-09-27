#ifndef WOLF_HPP
#define WOLF_HPP

#include "creature.hpp"

class Wolf : public Creature{
public:
    Wolf();
    void makePregnant();   
    void step();
    bool isAlive();  
    bool operator ==(const Wolf& w);    
    bool operator !=(const Wolf& w);
    void fixCoords();
};

#endif // WOLF_HPP
