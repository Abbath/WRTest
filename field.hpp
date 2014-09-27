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
    std::unordered_map<int, Wolf> wolfsMap;
    std::unordered_map<int, Rabbit> rabbitsMap;
    int creaturesCounter;    
    static constexpr int prCount = WOLF_NUMBER;
    static constexpr int viCount = RABBIT_NUMBER;
    std::function<int()> dice;
public:
    Field();
    Cell getCell(int x, int y);
    bool isEmpty(); 
    void generatePopulations();
    void print();
    void feedRabbits(Cell& c);
    void feedWolfs(Cell& c);
    void fixCoords();
    void cleanDead(std::list<Wolf> itdw, std::list<Rabbit> itdr);
    void step();
    bool wasWolfHere(std::pair<int, int> p);
    bool wasRabbitHere(std::pair<int, int> p);
    void wolfWasHere(std::pair<int, int> p);
    void rabbitWasHere(std::pair<int, int> p);
    Cell& getCreatureCell(std::pair<int, int> coords);
};
#endif // FIELD_HPP
