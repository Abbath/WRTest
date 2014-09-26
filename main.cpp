#include <iostream>
#include <vector>
#include <vector>
#include <random>
#include <functional>
#include <thread>
#include <deque>
#include <algorithm>
#include <ctime>

using namespace std;
constexpr int FIELD_SIZE = 1000;
constexpr int WOLF_MAX_AGE = 2000;
constexpr int RABBIT_MAX_AGE = 50;
constexpr int WOLF_START_HUNGRY = 500;
constexpr int RABBIT_START_HUNGRY = 35;
constexpr int WOLF_PREGNANCY_TIME = 30;
constexpr int RABBIT_PREGNANCY_TIME = 7;
constexpr int WOLF_CHILDREN_NUM = 3;
constexpr int RABBIT_CHILDREN_NUM = 5;
constexpr int WOLF_NUMBER = 10;
constexpr int RABBIT_NUMBER = 100;

class Field;
class Creature{
protected:
    int idx;
    int parentId;
    std::pair<int, int> coords;
    bool isMale;
    int hungry;
    int age;
    int pregnancy;
    Field *field;
    int getid(){static int id = 0; return id++;}
public:
    Creature(int _hungry) : idx(getid()), isMale(rand() % 2), hungry(_hungry), age(0), pregnancy(0) {} 
    Creature() : Creature(1) {}
    virtual ~Creature(){}
    virtual std::pair<int, int> getCoords() const;
    virtual void setCoords(const std::pair<int, int> &value);
    virtual bool getIsMale() const;
    virtual void setIsMale(bool value);
    virtual bool timeToBorn(){ return pregnancy == 1; }
    virtual void eat(){ hungry++; }
    virtual int getIdx() const { return idx; }
    virtual void makePregnant() = 0;
    virtual bool isAlive() = 0;
    virtual int getParentId() const;
    virtual void setParentIdx(int value);
    virtual Field *getField() const;
    virtual void setField(Field *value);
    virtual void step() = 0;
};

class Wolf : public Creature{
public:
    Wolf() : Creature(WOLF_START_HUNGRY){}
    void makePregnant(){ pregnancy = WOLF_PREGNANCY_TIME; }   
    void step(){ 
        if(pregnancy)pregnancy--;
        hungry--;
        age++; 
        coords.first += rand() % 3 - 1;
        coords.second += rand() % 3 - 1;
        if(coords.first < 0)
            coords.first = 0;
        if(coords.second < 0)
            coords.second = 0;
        if(coords.first > FIELD_SIZE - 1){
            coords.first = FIELD_SIZE - 1;
        }
        if(coords.second > FIELD_SIZE - 1){
            coords.second = FIELD_SIZE - 1;
        }
    }
    bool isAlive() {return hungry != 0 && age != WOLF_MAX_AGE; }  
    bool operator ==(const Wolf& w){ return this->idx == w.idx; }    
    bool operator !=(const Wolf& w){ return this->idx != w.idx; }
};

class Rabbit : public Creature{   
public:
    Rabbit() : Creature(RABBIT_START_HUNGRY){}
    void makePregnant(){ pregnancy = RABBIT_PREGNANCY_TIME; }
    void step(){ 
        if(pregnancy)pregnancy--; 
        hungry--; 
        age++;
        int newx = coords.first + rand() % 3 - 1;
        int newy = coords.second + rand() % 3 - 1;
//        if(field->getCell(newx, newy).getWolfWasHere()){
//            newx = coords.first + rand() % 3 - 1;
//            newy = coords.second + rand() % 3 - 1;
//            return;
//        }
        coords.first += newx; //rand() % 3 - 1;
        coords.second += newy; //rand() % 3 - 1;
        if(coords.first < 0)
            coords.first = 0;
        if(coords.second < 0)
            coords.second = 0;
        if(coords.first > FIELD_SIZE - 1){
            coords.first = FIELD_SIZE - 1;
        }
        if(coords.second > FIELD_SIZE - 1){
            coords.second = FIELD_SIZE - 1;
        }
    }
    bool isAlive() {return hungry != 0 && age != RABBIT_MAX_AGE; }   
    bool operator ==(const Rabbit& r){ return this->idx == r.idx; }
    bool operator !=(const Rabbit& r){ return this->idx != r.idx; }
};

class Cell {
    std::vector<int> wolfIndexes;
    std::vector<int> rabbitIndexes;
    bool isThereGrass;
    bool wolfWasHere;
public:
    Cell(): isThereGrass(true){
        
    }
    void addPredator(int index){ wolfIndexes.push_back(index); }
    void addVictim(int index){ rabbitIndexes.push_back(index); }
    bool getIsThereGrass() const;
    void setIsThereGrass(bool value);
    
    std::vector<int> getWolfIndexes() const;
    void setWolfIndexes(const std::vector<int> &value);
    std::vector<int> getRabbitIndexes() const;
    std::vector<int>& getRabbitIndexesRef() { return rabbitIndexes; }  
    std::vector<int>& getWolfIndexesRef() { return wolfIndexes; }        
    void setRabbitIndexes(const std::vector<int> &value);
    void removeWolfIndex(int index){
        wolfIndexes.erase(std::find(wolfIndexes.begin(), wolfIndexes.end(), index));
    }
    void removeRabbitIndex(int index){
        rabbitIndexes.erase(std::find(rabbitIndexes.begin(), rabbitIndexes.end(), index));
    }
    int getLastRabbitIndex() const { return *(rabbitIndexes.end() - 1); }
    void removeLastRabbitIndex() { rabbitIndexes.erase(rabbitIndexes.end() - 1); }
    bool getWolfWasHere() const;
    void setWolfWasHere(bool value);
};

class Field {
    std::vector<std::vector<Cell>> cells;
    std::vector<Wolf> wolfs;
    std::vector<Rabbit> rabbits;
    int creaturesCounter;    
    static constexpr int prCount = WOLF_NUMBER;
    static constexpr int viCount = RABBIT_NUMBER;
    
    std::function<int()> dice;
public:
    Field() : creaturesCounter(0){  
        cells.resize(FIELD_SIZE);
        for(auto& x : cells) {
            x.resize(FIELD_SIZE);
        }
        std::default_random_engine engine;
        std::uniform_int_distribution<int> distribution(0,FIELD_SIZE-1);
        dice = std::bind(distribution, engine);
    }
    Cell getCell(int x, int y) { return cells[x][y]; }
    bool isEmpty(){ return creaturesCounter == 0; } 
    void generatePopulations(){
        for (int i = 0; i < prCount; ++i) {
            Wolf w;
            int x = dice();
            int y = dice();
            w.setField(this);
            w.setCoords(std::make_pair(x,y));
            wolfs.push_back(w);
            cells[x][y].addPredator(i);
            creaturesCounter++;
        }
        for (int i = 0; i < viCount; ++i) {
            Rabbit r;
            int x = dice();
            int y = dice();
            r.setField(this);
            r.setCoords(std::make_pair(x,y));
            rabbits.push_back(r);
            cells[x][y].addVictim(i);
            creaturesCounter++;            
        }
    }
    void out(){
        std::cout << "W: " << wolfs.size() << " R: " << rabbits.size() << std::endl;
    }
    void feedRabbits(Cell& c){
        for (auto x : c.getRabbitIndexes()) {
            rabbits[x].eat();
        }     
    }
    void feedWolfs(Cell& c){
        for (unsigned i = 0; i < c.getWolfIndexes().size() ; ++i) {
            wolfs[c.getWolfIndexes()[i]].eat();
            if(!c.getRabbitIndexes().empty()){
                std::cout << "Rabbit " <<  rabbits[c.getLastRabbitIndex()].getIdx() << " was eaten!" << std::endl;
                rabbits.erase(rabbits.begin() + c.getLastRabbitIndex());
                c.removeLastRabbitIndex();
                creaturesCounter--;
            }else{
                break;
            }
        }
    }
    void fixCoords(){
        std::vector<int> idmw, idmr;
        for(int i = 0; i < int(cells.size()); ++i){
            for (int j = 0; j < int(cells[i].size()); ++j) {
                Cell& c =  cells[i][j];
                for(int x : c.getWolfIndexes()){
                    if(wolfs[x].getCoords() != std::make_pair(i,j)){
                        cells[wolfs[x].getCoords().first][wolfs[x].getCoords().second].addPredator(x);
                    }
                    //std::cout << "Wolf " << wolfs[x].getIdx() << " was moved!" << std::endl;                    
                    c.removeWolfIndex(x);
                }
                for(int x : c.getRabbitIndexes()){
                    if(rabbits[x].getCoords() != std::make_pair(i,j)){
                        cells[rabbits[x].getCoords().first][rabbits[x].getCoords().second].addVictim(x);
                    }
                    //std::cout << "Rabbit " << rabbits[x].getIdx() << " was moved!" << std::endl;
                    c.removeRabbitIndex(x);
                }
            }
        }
    }
    void cleanDead(std::vector<Wolf> itdw, std::vector<Rabbit> itdr){
        for (Wolf& x : itdw) {
            Cell& c = cells[x.getCoords().first][x.getCoords().second];
            std::vector<int> idx;
            for(auto y : c.getWolfIndexes()){
                if(wolfs[y] != x){
                    idx.push_back(y);
                }
            }
            c.setWolfIndexes(idx);
        }
        for (Rabbit& x : itdr) {
            //std::cout << "Rabbit " << x.getIdx() << " is dead!" << std::endl;
            Cell& c = cells[x.getCoords().first][x.getCoords().second];
            std::vector<int> idx;
            for(auto y : c.getRabbitIndexes()){
                if(rabbits[y] != x){
                    idx.push_back(y);
                }
            }
            c.setRabbitIndexes(idx);
        }         
        for (Wolf& x : itdw) {
            wolfs.erase(std::find(wolfs.begin(), wolfs.end(), x));
            creaturesCounter--;
        }
        for (Rabbit& x : itdr) {
            rabbits.erase(std::find(rabbits.begin(), rabbits.end(), x));
            creaturesCounter--;
        }
    }
    
    void step(){
        for(int i = 0; i < int(cells.size()); ++i){
            for (int j = 0; j < int(cells[i].size()); ++j) {
                Cell& c =  cells[i][j];
                if(c.getRabbitIndexes().size()){
                    if(c.getIsThereGrass()){
                        feedRabbits(c);
                    }
                    c.setIsThereGrass(false);
                    if(c.getWolfIndexes().size()){
                        feedWolfs(c);
                    }
                }else{
                    c.setIsThereGrass(true);
                }
            }
        }
        std::vector<Wolf> itdw;
        for (int i = 0; i < int(wolfs.size()); ++i) {
            if(rand() % 2){
                wolfs[i].makePregnant();
            }
            if(wolfs[i].timeToBorn()){
                for (int j = 0; j < WOLF_CHILDREN_NUM; ++j) {
                    Wolf w;
                    w.setParentIdx(wolfs[i].getIdx());
                    w.setField(this);
                    w.setCoords(wolfs[i].getCoords());
                    wolfs.push_back(w);
                    cells[wolfs[i].getCoords().first][wolfs[i].getCoords().second].addPredator(wolfs.size()-1);
                    creaturesCounter++;
                }
            }
            if(!wolfs[i].isAlive()){
                itdw.push_back(wolfs[i]);
            }else{
                wolfs[i].step();
            }
        }
        std::vector<Rabbit> itdr;
        for (int i = 0; i < int(rabbits.size()); ++i) {
            if(rand() % 2){
                rabbits[i].makePregnant();
            }
            if(rabbits[i].timeToBorn()){
                for (int j = 0; j < RABBIT_CHILDREN_NUM; ++j) {
                    Rabbit r;
                    r.setParentIdx(rabbits[i].getIdx());
                    r.setField(this);
                    r.setCoords(rabbits[i].getCoords());
                    rabbits.push_back(r);
                    cells[rabbits[i].getCoords().first][rabbits[i].getCoords().second].addVictim(rabbits.size()-1);
                    creaturesCounter++;                    
                }
            }
            if(!rabbits[i].isAlive()){
                itdr.push_back(rabbits[i]);                
            }else{
                rabbits[i].step();
            }
        }
        cleanDead(itdw, itdr);
        fixCoords();
    }
};

int main()
{
    srand(time(0));
    Field field;
    field.generatePopulations();
    while(!field.isEmpty()){
        field.step();
        field.out();
        //this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    field.out();
    return 0;
}



std::vector<int> Cell::getWolfIndexes() const
{
    return wolfIndexes;
}

void Cell::setWolfIndexes(const std::vector<int> &value)
{
    wolfIndexes = value;
}

std::vector<int> Cell::getRabbitIndexes() const
{
    return rabbitIndexes;
}

void Cell::setRabbitIndexes(const std::vector<int> &value)
{
    rabbitIndexes = value;
}

bool Cell::getWolfWasHere() const
{
return wolfWasHere;
}

void Cell::setWolfWasHere(bool value)
{
wolfWasHere = value;
}
bool Cell::getIsThereGrass() const
{
    return isThereGrass;
}

void Cell::setIsThereGrass(bool value)
{
    isThereGrass = value;
}

bool Creature::getIsMale() const
{
return isMale;
}

void Creature::setIsMale(bool value)
{
isMale = value;
}

int Creature::getParentId() const
{
return parentId;
}

void Creature::setParentIdx(int value)
{
parentId = value;
}

Field *Creature::getField() const
{
return field;
}

void Creature::setField(Field *value)
{
field = value;
}
std::pair<int, int> Creature::getCoords() const
{
return coords;
}

void Creature::setCoords(const std::pair<int, int> &value)
{
coords = value;
}
