#include "cell.hpp"


bool Cell::getRabbitWasHere() const
{
    return rabbitWasHere;
}

void Cell::setRabbitWasHere(bool value)
{
    rabbitWasHere = value;
}

void Cell::fixIndexes(int index, bool iswolf)
{
     if(iswolf){
        for(auto& x : wolfIndexes){
            if(index < x){
                x--;
            }
        }
    }else{
        for(auto& x : rabbitIndexes){
            if(index < x){
                x--;
            }
        }
    }
}
Cell::Cell(): isThereGrass(true){
    
}

std::list<int> Cell::getWolfIndexes() const
{
    return wolfIndexes;
}

const std::list<int>& Cell::getWolfIndexesRef()
{
    return wolfIndexes;
}

void Cell::setWolfIndexes(const std::list<int> &value)
{
    wolfIndexes = value;
}

std::list<int> Cell::getRabbitIndexes() const
{
    return rabbitIndexes;
}

void Cell::setRabbitIndexes(const std::list<int> &value)
{
    rabbitIndexes = value;
}

void Cell::removeWolfIndex(int index){
    wolfIndexes.erase(std::find(wolfIndexes.begin(), wolfIndexes.end(), index));
}

void Cell::removeRabbitIndex(int index){
    rabbitIndexes.erase(std::find(rabbitIndexes.begin(), rabbitIndexes.end(), index));
}

int Cell::getLastRabbitIndex() const {
    return rabbitIndexes.back(); 
}

void Cell::removeLastRabbitIndex() { 
    rabbitIndexes.pop_back();
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
    wolfIndexes.push_back(index);
}

void Cell::addVictim(int index){ 
    rabbitIndexes.push_back(index); 
}

bool Cell::getIsThereGrass() const
{
    return isThereGrass;
}

void Cell::setIsThereGrass(bool value)
{
    isThereGrass = value;
}
