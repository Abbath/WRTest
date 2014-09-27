#include "rabbit.hpp"
#include "field.hpp"

Rabbit::Rabbit() : Creature(RABBIT_START_HUNGRY){}

void Rabbit::makePregnant(){ 
    if(pregnancyTime == 0) pregnancyTime = RABBIT_PREGNANCY_TIME; 
}

void Rabbit::fixCoords()
{
    if(coords.first < 0)
        coords.first = 0;
    if(coords.second < 0)
        coords.second = 0;
    if(coords.first > FIELD_SIZE - 1){
        coords.first = FIELD_SIZE - 1;
    }
    if(coords.second > FIELD_SIZE - 1){
        coords.second = FIELD_SIZE - 1;
    }    
}

void Rabbit::step(){ 
    if(pregnancyTime)pregnancyTime--; 
    hungry--; 
    age++;
    int newx = coords.first + rand() % 3 - 1;
    int newy = coords.second + rand() % 3 - 1;
    Creature::fixCoords(newx , newy);
    if(field->wasWolfHere(std::make_pair(newx, newy))){
        field->rabbitWasHere(coords);
        return;
    }
    coords.first = newx;
    coords.second = newy;
    fixCoords();
}

bool Rabbit::isAlive() {
    return hungry != 0 && age != RABBIT_MAX_AGE;
}

bool Rabbit::operator ==(const Rabbit &r){ 
    return this->idx == r.idx;
}

bool Rabbit::operator !=(const Rabbit &r){ 
    return this->idx != r.idx;
}
