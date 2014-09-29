#ifndef RANDOMGENERATOR_HPP
#define RANDOMGENERATOR_HPP

#include <functional>
#include <random>
#include <chrono>

class RandomGenerator
{
public:
    static void init();
    static std::function<int()> dice;
    RandomGenerator();
};

#endif // RANDOMGENERATOR_HPP
