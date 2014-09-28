#include "field.hpp"

Field::Field() : creaturesCounter(0), stepCounter(0){  
    cells.resize(FIELD_SIZE);
    for(auto& x : cells) {
        x.resize(FIELD_SIZE);
    }
    std::default_random_engine engine;
    std::uniform_int_distribution<int> distribution(0,FIELD_SIZE-1);
    dice = std::bind(distribution, engine);
}

bool Field::isEmpty(){
    return creaturesCounter == 0; 
}

void Field::generatePopulations(){
    for (int i = 0; i < prCount; ++i) {
        Wolf w;
        int x = dice();
        int y = dice();
        w.setField(this);
        w.setCoords(std::make_pair(x,y));
        wolfsMap[w.getId()] = w;
        cells[x][y].addPredator(w.getId());
        creaturesCounter++;
    }
    for (int i = 0; i < viCount; ++i) {
        Rabbit r;
        int x = dice();
        int y = dice();
        r.setField(this);
        r.setCoords(std::make_pair(x,y));
        rabbitsMap[r.getId()] = r;
        cells[x][y].addVictim(r.getId());
        creaturesCounter++;            
    }
}

void Field::print(){
    for (int i = 0; i < FIELD_SIZE ; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            if(cells[i][j].getWolfIndexes().size() && cells[i][j].getRabbitIndexes().size()){
                std::cout << "B";
            }else if(cells[i][j].getWolfIndexes().size()){
                std::cout << "W";
            }else if(cells[i][j].getRabbitIndexes().size()){
                std::cout << "R";
            }else if(wasWolfHere({i,j})){
                std::cout << "!";
            }else if(wasRabbitHere({i,j})){
                std::cout << "?";
            }else if(cells[i][j].getIsThereGrass()){
                std::cout << ",";
            }else{
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Step: "<< stepCounter << " W: " << wolfsMap.size() << " R: " << rabbitsMap.size() <<  " CC:" << creaturesCounter << std::endl;
}

void Field::feedRabbits(Cell &c){
    for (auto x : c.getRabbitIndexes()) {
        rabbitsMap[x].eat();
    }     
}

void Field::feedWolfs(Cell &c){
    auto wi = c.getWolfIndexesRef().begin();
    for (unsigned i = 0; i < c.getWolfIndexes().size() ; ++i) {
        wolfsMap[*(wi++)].eat();
        if(!c.getRabbitIndexes().empty()){
            //std::cout << "Rabbit " <<  c.getFirstRabbitIndex() << " was eaten!" << std::endl;
            int index = c.getFirstRabbitIndex();
            rabbitsMap.erase(index);
            c.removeFirstRabbitIndex();
            creaturesCounter--;
        }else{
            break;
        }
    }
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
            if(!c.getRabbitIndexes().empty()){
                if(c.getIsThereGrass()){
                    feedRabbits(c);
                }
                c.setIsThereGrass(false);
                if(!c.getWolfIndexes().empty()){
                    feedWolfs(c);
                }
            }else{
                c.setIsThereGrass(true);
            }
        }
    }
    Indexes itdw;
    Indexes neww;
    for(auto it = wolfsMap.begin(); it != wolfsMap.end(); ++it){
        if(getCreatureCell(it->second.getCoords()).getWolfIndexes().size() > 1){
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
        if(getCreatureCell(it->second.getCoords()).getRabbitIndexes().size() > 1){
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

bool Field::wasWolfHere(std::pair<int, int> p)
{
    return cells[p.first][p.second].getWolfWasHere();
}

bool Field::wasRabbitHere(std::pair<int, int> p)
{
    return cells[p.first][p.second].getRabbitWasHere();
}

void Field::wolfWasHere(std::pair<int, int> p)
{
    cells[p.first][p.second].setWolfWasHere(true);
    cells[p.first][p.second].setRabbitWasHere(false);
}

void Field::rabbitWasHere(std::pair<int, int> p)
{
    cells[p.first][p.second].setWolfWasHere(false);
    cells[p.first][p.second].setRabbitWasHere(true);
}

Cell &Field::getCreatureCell(std::pair<int, int> coords)
{
    return cells[coords.first][coords.second];
}
