#pragma once
#include "includes.h"


template<typename T, typename Adapt_func>
class crossover{
public:
    virtual std::vector<std::shared_ptr<individual<T>>> operator()(const std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>& parents, const Adapt_func& func, std::size_t offspring_size=2)=0;
};

template<typename T, typename Adapt_func>
class npoint_crossover : public crossover<T,Adapt_func>{
public:
    npoint_crossover(std::vector<unsigned int>& positions):positions(positions){}
    std::vector<std::shared_ptr<individual<T>>> operator()(const std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>& parents, const Adapt_func& func, std::size_t offspring_size=2){
    std::vector<std::shared_ptr<individual<T>>> offspring;
    std::size_t size=parents.first->size();
    std::vector<T> fgenotype(size);
    std::vector<T> sgenotype(size);
    std::size_t index=0;
    bool changed=false;
    for(std::size_t i=0;i<size;i++){
        if(i==positions[index]&&index<positions.size()){
            changed=!changed;
            ++index;
        }
        if(changed){
            fgenotype[i]=parents.second->at(i);
            sgenotype[i]=parents.first->at(i);
        }else{
            fgenotype[i]=parents.first->at(i);
            sgenotype[i]=parents.second->at(i);
        }
    }
    offspring.push_back(std::make_shared<individual<T>>(fgenotype,func));
    offspring.push_back(std::make_shared<individual<T>>(sgenotype,func));
    return offspring;
}
std::vector<unsigned int> positions;
};

template<typename T, typename Adapt_func>
class homogen_equal_crossover : public crossover<T,Adapt_func>{
public:
    std::vector<std::shared_ptr<individual<T>>> operator()(const std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>& parents, const Adapt_func& func, std::size_t offspring_size=2){
    std::vector<std::shared_ptr<individual<T>>> offspring;
    std::size_t size=parents.first->size();
    std::vector<std::vector<T>> genotypes(offspring_size,std::vector<T>(size));
    int chance;
    for(std::size_t i=0;i<offspring_size;i++){
        for(int j=0;j<size;j++){
            chance=rand()%2;
            if(chance==0){
                genotypes[i][j]=parents.first->at(j);
            }else{
                genotypes[i][j]=parents.second->at(j);
            }
        }
    }
    for(auto& x:genotypes){
        offspring.push_back(std::make_shared<individual<T>>(x,func));
    }
    return offspring;
}
};

//todo: take a look at this algo, seems like it's incorrect
template<typename T, typename Adapt_func>
class classic_crossover : public crossover<T,Adapt_func>{
public:
    std::vector<std::shared_ptr<individual<T>>> operator()(const std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>& parents, const Adapt_func& func, std::size_t offspring_size=2){
    std::vector<unsigned int> first_parent_order=order_code(*(parents.first));
    std::vector<unsigned int> second_parent_order=order_code(*(parents.second));
    std::vector<std::shared_ptr<individual<T>>> offspring;
    std::size_t size=parents.first->size();
    std::vector<std::vector<T>> genotypes(offspring_size,std::vector<T>(size));
    int chance;
    for(std::size_t i=0;i<offspring_size;i++){
        for(int j=0;j<size;j++){
            chance=rand()%2;
            if(chance==0){
                genotypes[i][j]=first_parent_order.at(j);
            }else{
                genotypes[i][j]=second_parent_order.at(j);
            }
        }
    }
    for(auto& x:genotypes){
        offspring.push_back(std::make_shared<individual<T>>(order_decode(*(parents.first),x),func));
    }
    return offspring;
}
};

template<typename T, typename Adapt_func>
class npoint_ordered_crossover : public crossover<T,Adapt_func>{
public:
    npoint_ordered_crossover(std::vector<unsigned int>& positions):positions(positions), point_count(positions.size()), positions_initialized(true){}
    npoint_ordered_crossover(std::size_t point_count):point_count(point_count), positions_initialized(false){}
    std::vector<std::shared_ptr<individual<T>>> operator()(const std::pair<std::shared_ptr<individual<T>>,std::shared_ptr<individual<T>>>& parents, const Adapt_func& func, std::size_t offspring_size=2){
        std::vector<unsigned int> first_parent_order=order_code(*(parents.first));
        std::vector<unsigned int> second_parent_order=order_code(*(parents.second));
        std::vector<std::shared_ptr<individual<T>>> offspring;
        std::size_t size=first_parent_order.size();
        std::size_t index=0;
        bool changed=false;
        std::vector<T> fgenotype(size);
        std::vector<T> sgenotype(size);

        if(!positions_initialized)
        {
            positions.clear();
            unsigned int last_point = 0;
            for(std::size_t i = 0; i < point_count; ++i)
            {
                last_point = rand()%(size - last_point) + last_point;
                positions.push_back(last_point);
            }
        }
        for(std::size_t i=0;i<size;i++){
            if(i==positions[index]&&index<positions.size()){
                changed=!changed;
                ++index;
            }
            if(changed){
                fgenotype[i]=second_parent_order.at(i);
                sgenotype[i]=first_parent_order.at(i);
            }else{
                fgenotype[i]=first_parent_order.at(i);
                sgenotype[i]=second_parent_order.at(i);
            }
        }
        offspring.push_back(std::make_shared<individual<T>>(order_decode(*(parents.first),fgenotype),func));
        offspring.push_back(std::make_shared<individual<T>>(order_decode(*(parents.second),sgenotype),func));
        return offspring;
    }
    std::size_t point_count;
    bool positions_initialized;
    std::vector<unsigned int> positions;
};

