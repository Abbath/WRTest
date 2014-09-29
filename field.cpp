#include "field.hpp"

Field::Field() : creaturesCounter(0), stepCounter(0){  
    cells.resize(FIELD_WIDTH);
    for(auto& x : cells) {
        x.resize(FIELD_HEIGHT);
    }
    std::default_random_engine engine;
    std::uniform_int_distribution<int> distribution(0,FIELD_WIDTH+FIELD_HEIGHT);
    dice = std::bind(distribution, engine);
}

bool Field::isEmpty(){
    return creaturesCounter == 0; 
}

void Field::generatePopulations(){
    for (int i = 0; i < prCount; ++i) {
        Wolf w;
        int x = dice() % FIELD_WIDTH;
        int y = dice() % FIELD_HEIGHT;
        w.setField(this);
        w.setCoords(Coords(std::make_pair(x,y)));
        wolfsMap[w.getId()] = w;
        cells[x][y].addPredator(w.getId());
        creaturesCounter++;
    }
    for (int i = 0; i < viCount; ++i) {
        Rabbit r;
        int x = dice() % FIELD_WIDTH;
        int y = dice() % FIELD_HEIGHT;
        r.setField(this);
        r.setCoords(Coords(std::make_pair(x,y)));
        rabbitsMap[r.getId()] = r;
        cells[x][y].addVictim(r.getId());
        creaturesCounter++;            
    }
}

void Field::print(){
    for (int i = 0; i < FIELD_WIDTH ; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            if(cells[i][j].getWolfIndexes().size() && cells[i][j].getRabbitIndexes().size()){
                std::cout << "B";
            }else if(cells[i][j].getWolfIndexes().size()){
                std::cout << "W";
            }else if(cells[i][j].getRabbitIndexes().size()){
                std::cout << "R";
            }else if(wasWolfHere(Coords(std::make_pair(i, j)))){
                std::cout << "!";
            }else if(wasRabbitHere(Coords(std::make_pair(i,j)))){
                std::cout << "?";
            }else if(cells[i][j].isThereGrass()){
                std::cout << ",";
            }else{
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Step: "<< stepCounter << " W: " << wolfsMap.size() << " R: " << rabbitsMap.size() <<  " CC:" << creaturesCounter << std::endl;
}

void Field::fixCoords(){
    for(int i = 0; i < int(cells.size()); ++i){
        for (int j = 0; j < int(cells[i].size()); ++j) {
            Cell& c =  cells[i][j];
            const auto cached_pair = std::make_pair(i, j);
            Indexes itdw;
            for(int x : c.getWolfIndexes()){
                if(wolfsMap[x].getCoords() != cached_pair){
                    getCreatureCell(wolfsMap[x].getCoords()).addPredator(x);
                    itdw.insert(x);
                }
            }
            c.removeWolfIndexes(itdw);
            Indexes itdr;
            for(int x : c.getRabbitIndexes()){
                if(rabbitsMap[x].getCoords() != cached_pair){
                    getCreatureCell(rabbitsMap[x].getCoords()).addVictim(x);
                    itdr.insert(x);
                }
            }
            c.removeRabbitIndexes(itdr);
        }
    }
}

void Field::cleanDead(Indexes itdw, Indexes itdr){
    for (int x : itdw) {
        Cell& c = getCreatureCell(wolfsMap[x].getCoords());
        c.removeWolfIndex(x);
        wolfsMap.erase(x);
        creaturesCounter--;
    }
    for (int x : itdr) {
        Cell& c = getCreatureCell(rabbitsMap[x].getCoords());
        c.removeRabbitIndex(x);
        rabbitsMap.erase(x);
        creaturesCounter--;
    }         
}

void Field::bornNew(const Indexes &neww, const Indexes &newr)
{
    for (auto x : neww) {
        Wolf w;
        w.setParentId(x);
        w.setField(this);
        w.setCoords(wolfsMap[x].getCoords());
        wolfsMap[w.getId()] = w;
        getCreatureCell(w.getCoords()).addPredator(w.getId());
        creaturesCounter++;
    }
    for (auto x : newr) {
        Rabbit r;
        r.setParentId(x);
        r.setField(this);
        r.setCoords(rabbitsMap[x].getCoords());
        rabbitsMap[r.getId()] = r;
        getCreatureCell(r.getCoords()).addVictim(r.getId());
        creaturesCounter++;
    }
}

void Field::step(){
    for(int i = 0; i < int(cells.size()); ++i){
        for (int j = 0; j < int(cells[i].size()); ++j) {
            Cell& c =  cells[i][j];
            c.growGrass();
            c.decreaseRabbitSmell();
            c.decreaseWolfSmell();
        }
    }
    Indexes itdw;
    Indexes neww;
    for(auto it = wolfsMap.begin(); it != wolfsMap.end(); ++it){
        Cell& c = getCreatureCell(it->second.getCoords());
        if(!c.getRabbitIndexes().empty()){
            it->second.eat();
            rabbitsMap.erase(c.getFirstRabbitIndex());
            c.removeFirstRabbitIndex();
            creaturesCounter--;
        }
        if(c.getWolfIndexes().size() > 1){
            it->second.makePregnant(); 
        }
        if(it->second.timeToGiveBirth()){
            neww.insert(it->first);
        }
        if(!it->second.isAlive()){
            itdw.insert(it->first);
        }else{
            it->second.step();
        }
    }        
    Indexes itdr;
    Indexes newr;
    for(auto it = rabbitsMap.begin(); it != rabbitsMap.end(); ++it){
        Cell& c = getCreatureCell(it->second.getCoords());
        if(c.isThereGrass()){
            it->second.eat();
            c.eatGrass();
        }
        if(c.getRabbitIndexes().size() > 1){
           it->second.makePregnant(); 
        }
        if(it->second.timeToGiveBirth()){
            newr.insert(it->first);
        }
        if(!(it->second.isAlive())){
            itdr.insert(it->first);
        }else{
            it->second.step();
        }
    }  
    bornNew(neww, newr);
    cleanDead(itdw, itdr);
    fixCoords();
    stepCounter++;
}

bool Field::wasWolfHere(Coords p, int index)
{
    return cells[p.x()][p.y()].getWolfWasHere(index);
}

bool Field::wasRabbitHere(Coords p, int index)
{
    return cells[p.x()][p.y()].getRabbitWasHere(index);
}

void Field::wolfWasHere(Coords p, int index)
{
    cells[p.x()][p.y()].setWolfWasHere(true, index);
    cells[p.x()][p.y()].setRabbitWasHere(false, index);
}

void Field::rabbitWasHere(Coords p, int index)
{
    cells[p.x()][p.y()].setWolfWasHere(false, index);
    cells[p.x()][p.y()].setRabbitWasHere(true, index);
}

Cell &Field::getCreatureCell(Coords coords)
{
    return cells[coords.x()][coords.y()];
}
