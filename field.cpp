#include "field.hpp"

Field::Field() : creaturesCounter(0){  
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
    std::cout << "W: " << wolfsMap.size() << " R: " << rabbitsMap.size() <<  " CC:" << creaturesCounter << std::endl;
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
            std::cout << "Rabbit " <<  c.getLastRabbitIndex() << " was eaten!" << std::endl;
            int index = c.getLastRabbitIndex();
            rabbitsMap.erase(index);
            c.removeLastRabbitIndex();
            creaturesCounter--;
        }else{
            break;
        }
    }
}

void Field::fixCoords(){
    std::vector<int> idmw;
    std::vector<int> idmr;
    for(int i = 0; i < int(cells.size()); ++i){
        for (int j = 0; j < int(cells[i].size()); ++j) {
            Cell& c =  cells[i][j];
            const auto cached_pair = std::make_pair(i, j);
            for(int x : c.getWolfIndexes()){
                if(wolfsMap[x].getCoords() != cached_pair){
                    getCreatureCell(wolfsMap[x].getCoords()).addPredator(x);
                }
                c.removeWolfIndex(x);
            }
            for(int x : c.getRabbitIndexes()){
                if(rabbitsMap[x].getCoords() != cached_pair){
                    getCreatureCell(rabbitsMap[x].getCoords()).addVictim(x);
                }
                c.removeRabbitIndex(x);
            }
        }
    }
}

void Field::cleanDead(std::list<Wolf> itdw, std::list<Rabbit> itdr){
    for (Wolf& x : itdw) {
        Cell& c = getCreatureCell(x.getCoords());
        std::list<int> idx;
        for(auto y : c.getWolfIndexes()){
            if(wolfsMap[y] != x){
                idx.push_back(y);
            }
        }
        c.setWolfIndexes(idx);
        wolfsMap.erase(x.getId());
        creaturesCounter--;
    }
    for (Rabbit& x : itdr) {
        Cell& c = getCreatureCell(x.getCoords());
        std::list<int> idx;
        for(auto y : c.getRabbitIndexes()){
            if(rabbitsMap[y] != x){
                idx.push_back(y);
            }
        }
        c.setRabbitIndexes(idx);
        rabbitsMap.erase(x.getId());
        creaturesCounter--;
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
    std::list<Wolf> itdw;
    for(auto it = wolfsMap.begin(); it != wolfsMap.end(); ++it){
        if(getCreatureCell(it->second.getCoords()).getWolfIndexes().size() > 1 && rand() % 2){
           it->second.makePregnant(); 
        }
        if(it->second.timeToGiveBirth()){
            Wolf w;
            w.setParentId(it->first);
            w.setField(this);
            w.setCoords(it->second.getCoords());
            wolfsMap[w.getId()] = w;
            getCreatureCell(w.getCoords()).addPredator(w.getId());
            creaturesCounter++;
        }
        if(!it->second.isAlive()){
            itdw.push_back(it->second);
        }else{
            it->second.step();
        }
    }        
    std::list<Rabbit> itdr;
    for(auto it = rabbitsMap.begin(); it != rabbitsMap.end(); ++it){
        if(getCreatureCell(it->second.getCoords()).getRabbitIndexes().size() > 1 && rand() % 2){
           it->second.makePregnant(); 
        }
        if(it->second.timeToGiveBirth()){
            Rabbit r;
            r.setParentId(it->first);
            r.setField(this);
            r.setCoords(it->second.getCoords());
            rabbitsMap[r.getId()] = r;
            getCreatureCell(r.getCoords()).addVictim(r.getId());
            creaturesCounter++;
        }
        if(!it->second.isAlive()){
            itdr.push_back(it->second);
        }else{
            it->second.step();
        }
    }  
    cleanDead(itdw, itdr);
    fixCoords();
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
