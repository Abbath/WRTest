#include "rabbit.hpp"
#include "field.hpp"

Rabbit::Rabbit() : Creature(RABBIT_START_HUNGRY){}

void Rabbit::makePregnant(){ 
    if(pregnancyTime == 0 && !male && age > 365) pregnancyTime = RABBIT_PREGNANCY_TIME; 
}

void Rabbit::eat()
{
    if(hungry < RABBIT_START_HUNGRY) hungry+=4*2;
}

void Rabbit::step(){ 
    static std::pair<int, int> direction = {1, 1};
    if(pregnancyTime)pregnancyTime--; 
    hungry--; 
    age++;
    if(!field->getCell(coords).isThereGrass() || field->getCell(coords).getRabbitIndexes().size() < 2){
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
            if(field->getCell(coords).isThereGrass()){
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
    if(rand() < std::numeric_limits<int>::max() / 7.0){
        direction = std::make_pair(rand() % 3 - 1 , rand() % 3 - 1);
    }
    coords += direction;
    field->rabbitWasHere(coords, idx);
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
