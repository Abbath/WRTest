#include "wolf.hpp"
#include "field.hpp"


Wolf::Wolf() : Creature(WOLF_START_HUNGRY){}

void Wolf::makePregnant(){ 
    if(pregnancyTime == 0 && !male && age > 365*1.3) pregnancyTime = WOLF_PREGNANCY_TIME; 
}

void Wolf::step(){ 
    static std::pair<int, int> direction = {1, 1};    
    if(pregnancyTime)pregnancyTime--;
    hungry--;
    age++; 
    for(int i = 0; i < 8; ++i){
        int newx = coords.x() + (RandomGenerator::dice() % 3 - 1);
        int newy = coords.y() + (RandomGenerator::dice() % 3 - 1);
        Coords::fixCoords(newx, newy);
        if(field->wasRabbitHere(std::make_pair(newx, newy)) || 
                field->getCreatureCell(std::make_pair(newx, newy)).getRabbitIndexes().size() > 0){
            coords = Coords(std::make_pair(newx, newy));
            field->wolfWasHere(coords, idx); 
            return;
        }
    }
    for(int i = 0; i < 3; ++i){
        int newx = coords.x() + (RandomGenerator::dice() % 3 - 1);
        int newy = coords.y() + (RandomGenerator::dice() % 3 - 1);
        Coords::fixCoords(newx, newy);
        if(field->wasWolfHere(std::make_pair(newx, newy), idx)){
            coords = Coords(std::make_pair(newx, newy));
            field->wolfWasHere(coords, idx); 
            return;
        }
    }
    if(RandomGenerator::dice() < std::numeric_limits<int>::max() / 7.0){
        direction = std::make_pair(RandomGenerator::dice() % 3 - 1, RandomGenerator::dice() % 3 - 1);
    }
    coords += direction;
    field->wolfWasHere(coords, idx);    
}

void Wolf::eat()
{
    if(hungry < WOLF_START_HUNGRY) hungry+=4*9;
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
