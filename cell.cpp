#include "cell.hpp"


bool Cell::detectCreaturesSmell(const Smells& smells, int index) const
{
    if ( smells.empty() ) { return false; }
    if ( smells.find(index) == smells.end() ) { return true; } // This creature don't smell here
    if (  ( smells.find(index) != smells.end() ) && smells.size() > 1 ) { return true; }    
    return false;
}

bool Cell::getRabbitWasHere(int index) const
{
    return detectCreaturesSmell(rabbitSmells, index);
}

bool Cell::getWolfWasHere(int index) const
{
    return detectCreaturesSmell(wolfSmells, index);
}

void Cell::setRabbitWasHere(bool value, int index)
{
    if(value){
        rabbitSmells[index] = 10;
        //rabbitSmell = 10;
    }else{
        rabbitSmells.clear();
        //rabbitSmell = 0;
    }
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

Cell::Cell(): grass(144){
    
}

Indexes Cell::getWolfIndexes() const
{
    return wolfIndexes;
}

const Indexes& Cell::getWolfIndexesRef()
{
    return wolfIndexes;
}

void Cell::decreaseWolfSmell()
{
    Indexes itdw;
    for (auto it = wolfSmells.begin(); it != wolfSmells.end(); ++it) {
        if(it->first) it->second--;
        else itdw.insert(it->first);
    }
    for(auto x : itdw){
        wolfSmells.erase(x);
    }
}

void Cell::decreaseRabbitSmell()
{
    Indexes itdr;
    for(auto it = rabbitSmells.begin(); it != rabbitSmells.end(); ++it){        
        if(it->first) it->second--;
        else itdr.insert(it->first);
    }
    for(auto x : itdr){
        rabbitSmells.erase(x);
    }
}

void Cell::growGrass()
{
    if(grass < 144) grass++;
}

void Cell::eatGrass()
{
    if(grass > 12) 
        grass-=12;
    else
        grass = 0;
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


void Cell::setWolfWasHere(bool value, int index)
{
    if(value){
        wolfSmells[index] = 10;
    }else{
        wolfSmells.clear();
    }
}

void Cell::addPredator(int index){ 
    wolfIndexes.insert(index);
}

void Cell::addVictim(int index){ 
    rabbitIndexes.insert(index); 
}

bool Cell::isThereGrass() const
{
    return grass >= 12;
}

void Cell::setGrass(bool value)
{
    grass = value;
}
