#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <vector>
#include <ctime>
#include <cstdlib>
#include "defs.hpp"
class Field;

class Creature{
protected:
    int idx;
    int parentId;
    std::pair<int, int> coords;
    bool male;
    int hungry;
    int age;
    int pregnancyTime;
    Field *field;
    int getid();
public:
    Creature(int _hungry);
    Creature() : Creature(1) {}
    virtual ~Creature(){}
    virtual std::pair<int, int> getCoords() const;
    virtual void setCoords(const std::pair<int, int> &value);
    virtual bool isMale() const;
    virtual void setIsMale(bool value);
    virtual bool timeToGiveBirth();
    virtual void eat();
    virtual int getId() const;
    virtual void makePregnant() = 0;
    virtual bool isAlive() = 0;
    virtual int getParentId() const;
    virtual void setParentId(int value);
    virtual Field *getField() const;
    virtual void setField(Field *value);
    virtual void step() = 0;
    virtual bool operator ==(const Creature &r);
    virtual bool operator !=(const Creature &r);    
    virtual void fixCoords(int &newx, int &newy);
};

#endif // CREATURE_HPP
