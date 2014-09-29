#include "rabbit.hpp"
#include "field.hpp"

Rabbit::Rabbit() : Creature(RABBIT_START_HUNGRY){}

void Rabbit::makePregnant(){ 
    if(pregnancyTime == 0 && !male) pregnancyTime = RABBIT_PREGNANCY_TIME; 
}

void Rabbit::eat()
{
    if(hungry < RABBIT_START_HUNGRY) hungry++;
}

void Rabbit::step(){ 
    if(pregnancyTime)pregnancyTime--; 
    hungry--; 
    age++;
    if(!field->getCreatureCell(coords).isThereGrass() || field->getCreatureCell(coords).getRabbitIndexes().size() < 2){
//        int newx = coords.x() + rand() % 3 - 1;
//        int newy = coords.y() + rand() % 3 - 1;
//        Creature::fixCoords(newx , newy);
//        if(field->wasWolfHere(std::make_pair(newx, newy))){
//            field->rabbitWasHere(coords);
//            return;
//        }
        for(int i = 0; i < 8; ++i){
            int a = rand() % 3 - 1;
            int b = rand() % 3 - 1;
            int newx = coords.x() + a;
            int newy = coords.y() + b;
            Coords::fixCoords(newx, newy);
            if(field->wasWolfHere(std::make_pair(newx, newy)) || 
                    field->getCreatureCell(std::make_pair(newx, newy)).getWolfIndexes().size() > 0){
                coords = Coords(std::make_pair(coords.x() - a, coords.y() - b));
                field->rabbitWasHere(coords, idx); 
                return;
            }
        }
        for(int i = 0; i < 8; ++i){
            int newx = coords.x() + (rand() % 3 - 1);
            int newy = coords.y() + (rand() % 3 - 1);
            Coords::fixCoords(newx, newy);
            if(field->getCreatureCell(coords).isThereGrass()){
                coords = Coords(std::make_pair(newx, newy));
                field->rabbitWasHere(coords, idx); 
                return;
            }
        }
        for(int i = 0; i < 8; ++i){
            int newx = coords.x() + (rand() % 3 - 1);
            int newy = coords.y() + (rand() % 3 - 1);
            Coords::fixCoords(newx, newy);
            if(field->wasRabbitHere(std::make_pair(newx, newy), idx)){
                coords = Coords(std::make_pair(newx, newy));
                field->rabbitWasHere(coords, idx); 
                return;
            }
        }
    }
}

bool Rabbit::isAlive() {
    return hungry > 0 && age <= RABBIT_MAX_AGE;
}

bool Rabbit::operator ==(const Rabbit &r){ 
    return this->idx == r.idx;
}

bool Rabbit::operator !=(const Rabbit &r){ 
    return this->idx != r.idx;
}
