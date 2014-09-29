#include "wolf.hpp"
#include "field.hpp"


Wolf::Wolf() : Creature(WOLF_START_HUNGRY){}

void Wolf::makePregnant(){ 
    if(pregnancyTime == 0 && !male) pregnancyTime = WOLF_PREGNANCY_TIME; 
}

void Wolf::step(){ 
    if(pregnancyTime)pregnancyTime--;
    hungry--;
    age++; 
    for(int i = 0; i < 8; ++i){
        int a = rand() % 3 - 1;
        int b = rand() % 3 - 1;
        int newx = coords.x() + a;
        int newy = coords.y() + b;
        Coords::fixCoords(newx, newy);
        if(field->wasRabbitHere(std::make_pair(newx, newy)) || 
                field->getCreatureCell(std::make_pair(newx, newy)).getRabbitIndexes().size() > 0){
            coords = Coords(std::make_pair(coords.x() + a, coords.y() + b));
            field->wolfWasHere(coords, idx); 
            return;
        }
    }
    for(int i = 0; i < 8; ++i){
        int newx = coords.x() + (rand() % 3 - 1);
        int newy = coords.y() + (rand() % 3 - 1);
        Coords::fixCoords(newx, newy);
        if(field->wasWolfHere(std::make_pair(newx, newy), idx) || 
                field->getCreatureCell(std::make_pair(newx, newy)).getWolfIndexes().size() > 0){
            coords = Coords(std::make_pair(newx, newy));
            field->wolfWasHere(coords, idx); 
            return;
        }
    }
    coords += std::make_pair(rand() % 3 - 1, rand() % 3 - 1);
    field->wolfWasHere(coords, idx);    
}

void Wolf::eat()
{
    if(hungry < WOLF_START_HUNGRY) hungry++;
}

bool Wolf::isAlive() {
    return hungry > 0 && age <= WOLF_MAX_AGE;
}

bool Wolf::operator ==(const Wolf &w){ 
    return this->idx == w.idx;
}

bool Wolf::operator !=(const Wolf &w){
    return this->idx != w.idx;
}
