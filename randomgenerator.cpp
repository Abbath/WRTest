#include "randomgenerator.hpp"

std::function<int()> RandomGenerator::dice;

void RandomGenerator::init()
{
    std::default_random_engine engine;
    std::uniform_int_distribution<int> distribution(0, std::numeric_limits<int>::max());
    dice = std::bind(distribution, engine);
}

RandomGenerator::RandomGenerator()
{
}
