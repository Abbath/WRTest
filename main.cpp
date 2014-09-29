#include "field.hpp"
#include <thread>

int main()
{
    RandomGenerator::init();
    Field field;
    field.generatePopulations();
    while(true){
        field.step();
        field.print();
        //field.check();
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if(field.isEmpty()){
            field.write();
            field.generatePopulations();
        } 
    }
    return 0;
}












