#ifndef CELL_HPP
#define CELL_HPP

#include <list>
#include <algorithm>

class Cell {
    std::list<int> wolfIndexes;
    std::list<int> rabbitIndexes;
    bool isThereGrass;
    bool wolfWasHere;
    bool rabbitWasHere;
public:
    Cell();
    void addPredator(int index);
    void addVictim(int index);
    bool getIsThereGrass() const;
    void setIsThereGrass(bool value);
    
    std::list<int> getWolfIndexes() const;
    void setWolfIndexes(const std::list<int> &value);
    std::list<int> getRabbitIndexes() const;
    void setRabbitIndexes(const std::list<int> &value);
    void removeWolfIndex(int index);
    void removeRabbitIndex(int index);
    int getLastRabbitIndex() const;
    void removeLastRabbitIndex();
    bool getWolfWasHere() const;
    void setWolfWasHere(bool value);
    bool getRabbitWasHere() const;
    void setRabbitWasHere(bool value);
    void fixIndexes(int index, bool iswolf = true);
    const std::list<int> &getWolfIndexesRef();
};

#endif // CELL_HPP
