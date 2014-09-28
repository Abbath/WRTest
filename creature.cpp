#include "creature.hpp"
#include "field.hpp"
#include <cassert>
int Creature::getid(){
    static int id = 0;
    return id++;
}

Creature::Creature(int _hungry) : idx(getid()), male(rand() % 2), hungry(_hungry), age(0), pregnancyTime(0) {
    
}

bool Creature::timeToGiveBirth(){
    return pregnancyTime == 1;
}

void Creature::eat(){ 
    hungry++; 
}

int Creature::getId() const {
    return idx;
}

bool Creature::operator ==(const Creature &r){ 
    return this->idx == r.idx;
}

bool Creature::operator !=(const Creature &r){ 
    return this->idx != r.idx;
} 

bool Creature::isMale() const
{
    return male;
}

void Creature::setIsMale(bool value)
{
    male = value;
}

int Creature::getParentId() const
{
    return parentId;
}

void Creature::setParentId(int value)
{
    parentId = value;
}

Field *Creature::getField() const
{
    return field;
}

void Creature::setField(Field *value)
{
    assert(value);
    field = value;
}
std::pair<int, int> Creature::getCoords() const
{
    return coords;
}

void Creature::setCoords(const std::pair<int, int> &value)
{
    coords = value;
}

void Creature::fixCoords(int& newx, int& newy){
    if(newx < 0)
        newx = 0;
    if(newy < 0)
        newy = 0;
    if(newx > FIELD_SIZE - 1){
        newx = FIELD_SIZE - 1;
    }
    if(newy > FIELD_SIZE - 1){
        newy = FIELD_SIZE - 1;
    }
}
