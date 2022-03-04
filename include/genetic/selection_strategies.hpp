#pragma once
#include "includes.h"


template<typename T, typename Comparator>
class selection_strategy{
public:
    virtual std::vector<std::shared_ptr<individual<T>>> operator()(std::vector<std::shared_ptr<individual<T>>>& old_gen, std::vector<std::shared_ptr<individual<T>>>& new_gen, const Comparator& func)=0;
};

template<typename T, typename Comparator>
class stable_select_strategy : public selection_strategy<T,Comparator>{
public:
stable_select_strategy(int new_gen_percent):percentage(new_gen_percent){}
std::vector<std::shared_ptr<individual<T>>> operator()(std::vector<std::shared_ptr<individual<T>>>& old_gen, std::vector<std::shared_ptr<individual<T>>>& new_gen, const Comparator& func){
    std::vector<std::shared_ptr<individual<T>>> result_gen;

    std::size_t new_gen_size=(old_gen.size()*percentage)/100;
    new_gen_size=std::min(new_gen_size,new_gen.size());
    std::size_t old_gen_size=old_gen.size()-new_gen_size;

    std::size_t sum_old_adapt_value=0;
    long chance;
    for(auto x: old_gen){
        sum_old_adapt_value+= x->adapt();
    }

    std::size_t sum_new_adapt_value=0;
    for(auto x: new_gen){
        sum_new_adapt_value+= x->adapt();
    }

    bool selector=false;
    while(old_gen_size>0 || new_gen_size>0){
        if(selector && old_gen_size>0 ||new_gen_size==0){
            if(sum_old_adapt_value>0){
                chance=rand()%sum_old_adapt_value;
            }else{
                chance=rand();
            }
            for(int i=0;i<old_gen.size();i++){
                if(func(chance,old_gen[i]->adapt())){
                    sum_old_adapt_value-=old_gen[i]->adapt();
                    result_gen.push_back(old_gen[i]);
                    old_gen.erase(old_gen.begin()+i);
                    --old_gen_size;
                    selector=false;
                    break;
                }
            }
        }else{
            if(sum_new_adapt_value>0){
                chance=rand()%sum_new_adapt_value;
            }else{
                chance=rand();
            }
            for(int i=0;i<new_gen.size();i++){
                if(func(chance,new_gen[i]->adapt())){
                    sum_new_adapt_value-=new_gen[i]->adapt();
                    result_gen.push_back(new_gen[i]);
                    new_gen.erase(new_gen.begin()+i);
                    --new_gen_size;
                    selector=true;
                    break;
                }
            }
        }
    }
    return result_gen;
}
int percentage;
};



template<typename T, typename Comparator>
class beta_tournament : public selection_strategy<T,Comparator>{
public:
beta_tournament(std::size_t tournament_size):tournament_size(tournament_size){}
    std::vector<std::shared_ptr<individual<T>>> operator()(std::vector<std::shared_ptr<individual<T>>>& old_gen, std::vector<std::shared_ptr<individual<T>>>& new_gen, const Comparator& func){
        std::size_t size=old_gen.size();
        for(auto& x:new_gen){
            old_gen.push_back(x);
        }
        std::vector<std::shared_ptr<individual<T>>> result(size);
        std::vector<int> positions(tournament_size);
        std::shared_ptr<individual<T>> best;
        std::size_t pos;
        for(std::size_t i=0;i<size;i++){
            for(std::size_t j=0;j<tournament_size;j++){
                positions[j]=rand()%old_gen.size();
            }
            best=old_gen[positions[0]];
            pos=positions[0];
            for(std::size_t j=1;j<tournament_size;j++){
                if(func(old_gen[positions[j]]->adapt(),best->adapt())){
                    best=old_gen[positions[j]];
                    pos=positions[j];
                }
            }
            result[i]=best;
            old_gen.erase(old_gen.begin()+pos);
        }
        return result;
    }

std::size_t tournament_size;
};