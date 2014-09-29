#include "field.hpp"
#include <thread>

int main()
{
    srand(time(0));
    Field field;
    field.generatePopulations();
    while(true){
        field.step();
        field.print();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(field.isEmpty()){
            field.generatePopulations();
        } 
    }
    return 0;
}












