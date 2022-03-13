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
class roulette_reproductive_strategy : public reproductive_strategy<T>{
public:
std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> operator()(std::vector<std::shared_ptr<individual<T>>> population){
std::size_t size=population.size();
   long sum_adapt_value=0;
    for(auto x: population){
        sum_adapt_value+= x->adapt();
    }
    std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> result;
    std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>> candidate;
    long double chance;
    std::shared_ptr<individual<T>>* candidate_pair = new std::shared_ptr<individual<T>>[2];
    while(population.size()>1){
        for(unsigned int j = 0; j < 2; ++j)
        {
            if(sum_adapt_value == 0)
            {
            chance = rand()%population.size();
            candidate_pair[j]=population[chance];
            population.erase(population.begin()+chance);
            }
            else
            {
                chance=rand()%sum_adapt_value;
                for(int i=0;i<population.size();i++){
                    if(chance<=population[i]->adapt()){
                        candidate_pair[j]=population[i];
                        population.erase(population.begin()+i);
                        sum_adapt_value-=candidate_pair[j]->adapt();
                        break;
                    }else{
                        chance-=population[i]->adapt();
                    }
                }
                chance=0;
            }
        }
        candidate.first=candidate_pair[0];
        candidate.second=candidate_pair[1];
        result.push_back(candidate);
    }
    return result;
}
};

template<typename T>
class ordered_inbreeding_reproductive_strategy : public reproductive_strategy<T>{
public:
ordered_inbreeding_reproductive_strategy(std::size_t distance):distance(distance){}
std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> operator()(std::vector<std::shared_ptr<individual<T>>> population){
    std::vector<unsigned int> first_parent_order;
    std::vector<unsigned int> second_parent_order;
    
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
    std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> result;
    std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>> candidate;
    std::size_t pos;
    while(population.size()>1){
        pos = rand()%population.size();
        candidate.first = population[pos];
        population.erase(population.begin()+pos);
        for(int i=0;i<population.size();i++){
            T max_adapt = std::max(candidate.first->adapt(), population[i]->adapt());
            T min_adapt = std::min(candidate.first->adapt(), population[i]->adapt());
            float chance;
            if(max_adapt == 0)
            {
                chance = 0;
            }
            else
            {
                chance = rand()%max_adapt;
            }
            if(chance >= max_adapt - min_adapt){
                candidate.second = population[i];
                population.erase(population.begin()+i);
                break;
            }
        }
        if(candidate.second == nullptr)
        {
            candidate.second = population[rand()%population.size()];
        }
        result.push_back(candidate);
        candidate.second = nullptr;
    }
    return result;
}
};

template<typename T>
class negative_assotiative_reproductive_sterategy : public reproductive_strategy<T>{
public:
std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> operator()(std::vector<std::shared_ptr<individual<T>>> population){
    std::vector<std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>> result;
    std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>> candidate;
    std::size_t pos;
    while(population.size()>1){
        pos = rand()%population.size();
        candidate.first = population[pos];
        population.erase(population.begin()+pos);
        for(int i=0;i<population.size();i++){
            T max_adapt = std::max(candidate.first->adapt(), population[i]->adapt());
            T min_adapt = std::min(candidate.first->adapt(), population[i]->adapt());
            T chance;
            if(max_adapt == 0)
            {
                chance = 1;
            }
            else
            {
                chance = rand()%max_adapt;
            }
            if(chance <= max_adapt - min_adapt){
                candidate.second = population[i];
                population.erase(population.begin()+i);
                break;
            }
        }
        if(candidate.second == nullptr)
        {
            candidate.second = population[rand()%population.size()];
        }
        result.push_back(candidate);
        candidate.second = nullptr;
    }
    return result;
}
};