#include "wolf.hpp"
#include "field.hpp"


Wolf::Wolf() : Creature(WOLF_START_HUNGRY){}

void Wolf::makePregnant(){ 
    if(pregnancyTime == 0 && !male && hungry > WOLF_START_HUNGRY/2 && age > 5) pregnancyTime = WOLF_PREGNANCY_TIME; 
}

void Wolf::fixCoords()
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

void Wolf::step(){ 
    if(pregnancyTime)pregnancyTime--;
    hungry--;
    age++; 
    for(int i = -1; i <=1; ++i){
        for(int j = -1; j <=1; ++j){
            if(i+j != 0){
                int newx = coords.first + i;
                int newy = coords.second + j;
                Creature::fixCoords(newx, newy);
                //std::cerr << "sixth" << std::endl;                
                if(!field->getCreatureCell(std::make_pair(newx, newy)).getRabbitIndexes().empty()){
                    coords.first += i;
                    coords.second += j;
                    fixCoords();                    
                    field->wolfWasHere(coords);
                    return; 
                }
                if(field->wasRabbitHere(std::make_pair(newx, newy))){
                    coords.first += i;
                    coords.second += j;
                    fixCoords();                    
                    field->wolfWasHere(coords);
                    return;
                }
            }
        }
    }
    coords.first += rand() % 3 - 1;
    coords.second += rand() % 3 - 1;
    fixCoords();
    field->wolfWasHere(coords);    
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
