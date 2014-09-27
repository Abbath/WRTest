#include "field.hpp"

int main()
{
    srand(time(0));
    Field field;
    field.generatePopulations();
    while(!field.isEmpty()){
        field.step();
        field.print();
    }
    field.print();
    return 0;
}






