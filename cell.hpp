#ifndef CELL_HPP
#define CELL_HPP

#include <list>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <map>

typedef std::unordered_set<int> Indexes;
typedef std::map<int, unsigned int> Smells;

class Cell {
    Indexes wolfIndexes;
    Indexes rabbitIndexes;
    Smells wolfSmells;
    Smells rabbitSmells;
    unsigned grass;
public:
    Cell();
    void addPredator(int index);
    void addVictim(int index);
    bool isThereGrass() const;
    void setGrass(bool value);
    Indexes getWolfIndexes() const;
    void setWolfIndexes(const Indexes &value);
    Indexes getRabbitIndexes() const;
    void setRabbitIndexes(const Indexes &value);
    void removeWolfIndex(int index);
    void removeRabbitIndex(int index);
    int getFirstRabbitIndex() const;
    void removeFirstRabbitIndex();
    bool getWolfWasHere(int index = -1) const;
    void setWolfWasHere(bool value, int index = 0);
    bool getRabbitWasHere(int index = -1) const;
    void setRabbitWasHere(bool value, int index = 0);
    void removeRabbitIndexes(const Indexes& indexes);
    void removeWolfIndexes(const Indexes& indexes);
    const Indexes &getWolfIndexesRef();
    void decreaseWolfSmell();
    void decreaseRabbitSmell();
    void growGrass();
    void eatGrass();
    void detectCreaturesSmell(int index, bool result) const;
    bool detectCreaturesSmell(const Smells &smells, int index) const;
};

#endif // CELL_HPP
