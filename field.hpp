#ifndef FIELD_HPP
#define FIELD_HPP

#include "wolf.hpp"
#include "rabbit.hpp"
#include "cell.hpp"
#include <functional>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <list>

class Field {
    std::vector<std::vector<Cell>> cells;
    std::unordered_map<int, Wolf> wolfs;
    std::unordered_map<int, Rabbit> rabbits;
    int creaturesCounter; 
    int allCreatureCounter = 0;
    int deadRabbits = 0;
    int deadWolfs = 0;
    int eatenRabbits = 0;
    int stepCounter;
    static constexpr int prCount = WOLF_NUMBER;
    static constexpr int viCount = RABBIT_NUMBER;
public:
    Field();
    Cell& getCell(Coords c);
    bool isEmpty(); 
    void generatePopulations();
    void print();
    void fixCoords();
    void cleanDead(Indexes itdw, Indexes itdr);
    void bornNew(const Indexes& neww, const Indexes& newr);
    void step();
    void check();
    bool wasWolfHere(Coords p, int index = -1);
    bool wasRabbitHere(Coords p, int index = -1);
    void wolfWasHere(Coords p, int index);
    void rabbitWasHere(Coords p, int index);
    Cell& getCreatureCell(Coords coords);
};
#endif // FIELD_HPP
