#include "field.hpp"
#include <thread>

int main()
{
    srand(time(0));
    Field field;
    field.generatePopulations();
    while(!field.isEmpty()){
        field.step();
        field.print();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}






