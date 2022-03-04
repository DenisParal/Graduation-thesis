#include "includes.h"
#include <random>
#include <algorithm>

template<typename T>
class reproductive_strategy{
public:
    virtual std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> operator()(std::vector<std::shared_ptr<individual<T>>> population)=0;
};


template<typename T>
class random_reproductive_strategy : public reproductive_strategy<T>{
public:
std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> operator()(std::vector<std::shared_ptr<individual<T>>> population){
    std::size_t size=population.size();
    std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> result;
    std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>> candidate;
    std::size_t pos1,pos2;
    while(size>1){
        pos1=rand()%size;
        candidate.first=population[pos1];
        population.erase(population.begin()+pos1);
        --size;
        pos2=rand()%size;
        candidate.first=population[pos2];
        population.erase(population.begin()+pos2);
        result.push_back(candidate);
        --size;
    }
    return result;
}
};
 
template<typename T>
class ordered_inbreeding_reproductive_strategy : public reproductive_strategy<T>{
public:
ordered_inbreeding_reproductive_strategy(std::size_t distance):distance(distance){}
std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> operator()(std::vector<std::shared_ptr<individual<T>>> population){
    std::vector<int> first_parent_order;
    std::vector<int> second_parent_order;
    
    std::size_t size=population.size();
    std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> result;
    std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>> candidate;
    std::size_t pos1,pos2;
    for(std::size_t j=0;j<population.size();j++){
        pos1=rand()%population.size();
        candidate.first=population[pos1];
        first_parent_order=order_code(*(candidate.first));
        population.erase(population.begin()+pos1);

        for(int i=0;i<population.size();i++){
            second_parent_order=order_code(*(population[i]));
            if(get_distance(first_parent_order,second_parent_order)<=distance){
                candidate.second=population[i];
                population.erase(population.begin()+i);
                result.push_back(candidate);
                break;
            }
        }
    }
    return result;
}
private:
std::size_t distance;
};

template<typename T>
class outobreeding_reproductive_strategy : public reproductive_strategy<T>{
public:
outobreeding_reproductive_strategy(std::size_t distance):distance(distance){}
std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> operator()(std::vector<std::shared_ptr<individual<T>>> population){
    std::size_t size=population.size();
    std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> result;
    std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>> candidate;
    std::size_t pos1,pos2;
    while(size>1){
        pos1=rand()%size;
        candidate.first=population[pos1];
        population.erase(population.begin()+pos1);
        --size;
        for(int i=0;i<size;i++){
            if(get_distance(*population[i],*(candidate.first))>=distance){
                candidate.second=population[i];
                population.erase(population.begin()+i);
                --size;
                result.push_back(candidate);
                break;
            }
        }
    }
    return result;
}
std::size_t distance;
};

template<typename T>
class positive_assotiative_reproductive_sterategy : public reproductive_strategy<T>{
public:
std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> operator()(std::vector<std::shared_ptr<individual<T>>> population){
    std::size_t size=population.size();
    long sum_adapt_value=0;
    for(auto x: population){
        sum_adapt_value+= x->adapt();
    }
    std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> result;
    std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>> candidate;
    std::size_t pos1,pos2;
    long double chance;
    while(population.size()>1){
        if(sum_adapt_value>0){
            chance=rand()%sum_adapt_value;
        }else{
            chance=rand();
        }
        for(int i=0;i<population.size();i++){
            if(chance<=population[i]->adapt()){
                candidate.first=population[i];
                population.erase(population.begin()+i);
                sum_adapt_value-=candidate.first->adapt();
                --size;
                break;
            }else{
                chance-=population[i]->adapt();
            }
        }
        if(sum_adapt_value>0){
            chance=rand()%sum_adapt_value;
        }else{
            chance=rand();
        }
        for(int i=0;i<population.size();i++){
            if(chance<=population[i]->adapt()){
                candidate.second=population[i];
                population.erase(population.begin()+i);
                sum_adapt_value-=candidate.second->adapt();
                --size;
                result.push_back(candidate);
                candidate.first=std::shared_ptr<individual<T>>();
                candidate.second=std::shared_ptr<individual<T>>();
                break;
            }else{
                chance-=population[i]->adapt();
            }
        }
    }
    return result;
}
};

template<typename T>
class negative_assotiative_reproductive_sterategy : public reproductive_strategy<T>{
public:
std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> operator()(std::vector<std::shared_ptr<individual<T>>> population){
    std::size_t size=population.size();
    long sum_adapt_value=0;
    for(auto x: population){
        sum_adapt_value+= x->adapt();
    }
    std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> result;
    std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>> candidate;
    std::size_t pos1,pos2;
    long double chance;
    while(population.size()>1){
        if(sum_adapt_value>0){
            chance=rand()%sum_adapt_value;
        }else{
            chance=rand();
        }
        for(int i=0;i<population.size();i++){
            if(chance<=population[i]->adapt()){
                candidate.first=population[i];
                population.erase(population.begin()+i);
                sum_adapt_value-=candidate.first->adapt();
                --size;
                break;
            }else{
                chance-=population[i]->adapt();
            }
        }
        if(sum_adapt_value>0){
            chance=rand()%sum_adapt_value;
        }else{
            chance=rand();
        }
        for(int i=0;i<population.size();i++){
            if(1.0/chance<=1.0/(population[i]->adapt())){
                candidate.second=population[i];
                population.erase(population.begin()+i);
                sum_adapt_value-=candidate.second->adapt();
                --size;
                result.push_back(candidate);
                candidate.first=std::shared_ptr<individual<T>>();
                candidate.second=std::shared_ptr<individual<T>>();
                break;
            }else{
                chance-=population[i]->adapt();
            }
        }
    }
    return result;
}
};