#ifndef CELL_HPP
#define CELL_HPP

#include <list>
#include <algorithm>
#include <unordered_set>

typedef std::unordered_set<int> Indexes;
class Cell {
    Indexes wolfIndexes;
    Indexes rabbitIndexes;
    bool isThereGrass;
    bool wolfWasHere;
    bool rabbitWasHere;
public:
    Cell();
    void addPredator(int index);
    void addVictim(int index);
    bool getIsThereGrass() const;
    void setIsThereGrass(bool value);
    Indexes getWolfIndexes() const;
    void setWolfIndexes(const Indexes &value);
    Indexes getRabbitIndexes() const;
    void setRabbitIndexes(const Indexes &value);
    void removeWolfIndex(int index);
    void removeRabbitIndex(int index);
    int getFirstRabbitIndex() const;
    void removeFirstRabbitIndex();
    bool getWolfWasHere() const;
    void setWolfWasHere(bool value);
    bool getRabbitWasHere() const;
    void setRabbitWasHere(bool value);
    void removeRabbitIndexes(const Indexes& indexes);
    void removeWolfIndexes(const Indexes& indexes);
    const Indexes &getWolfIndexesRef();
};

#endif // CELL_HPP
