#include "cell.hpp"


bool Cell::getRabbitWasHere() const
{
    return rabbitWasHere;
}

void Cell::setRabbitWasHere(bool value)
{
    rabbitWasHere = value;
}

void Cell::removeRabbitIndexes(const Indexes &indexes)
{
    for(auto x : indexes){
        rabbitIndexes.erase(x);
    }
}

void Cell::removeWolfIndexes(const Indexes &indexes)
{
    for(auto x : indexes){
        wolfIndexes.erase(x);
    }
}

Cell::Cell(): isThereGrass(true){
    
}

Indexes Cell::getWolfIndexes() const
{
    return wolfIndexes;
}

const Indexes& Cell::getWolfIndexesRef()
{
    return wolfIndexes;
}

void Cell::setWolfIndexes(const Indexes &value)
{
    wolfIndexes = value;
}

Indexes Cell::getRabbitIndexes() const
{
    return rabbitIndexes;
}

void Cell::setRabbitIndexes(const Indexes &value)
{
    rabbitIndexes = value;
}

void Cell::removeWolfIndex(int index){
    wolfIndexes.erase(index);
}

void Cell::removeRabbitIndex(int index){
    rabbitIndexes.erase(index);
}

int Cell::getFirstRabbitIndex() const {
    return *(rabbitIndexes.begin()); 
}

void Cell::removeFirstRabbitIndex() { 
    rabbitIndexes.erase(rabbitIndexes.begin());
}

bool Cell::getWolfWasHere() const
{
    return wolfWasHere;
}

void Cell::setWolfWasHere(bool value)
{
    wolfWasHere = value;
}

void Cell::addPredator(int index){ 
    wolfIndexes.insert(index);
}

void Cell::addVictim(int index){ 
    rabbitIndexes.insert(index); 
}

bool Cell::getIsThereGrass() const
{
    return isThereGrass;
}

void Cell::setIsThereGrass(bool value)
{
    isThereGrass = value;
}
