#pragma once
#include "includes.h"

template<typename Adapt_func>
class forming_algorithm{
public:
virtual std::vector<std::shared_ptr<individual<int>>> operator()(std::size_t individ_size, std::size_t pop_size,const Adapt_func& func)=0;
};

template<typename Adapt_func>
class random_forming_algorithm : public forming_algorithm<Adapt_func>{
public:
std::vector<std::shared_ptr<individual<int>>> operator()(std::size_t individ_size, std::size_t pop_size,const Adapt_func& func){
    std::vector<std::shared_ptr<individual<int>>> result;
    std::vector<int> base;
    std::vector<int> order(individ_size);
    int pos;
    for(std::size_t i=0;i<pop_size;i++){
        for(int j=0;j<individ_size;j++){
            base.push_back(j);
            order[j]=rand()%individ_size;
        }
        for(int j=0;j<individ_size;j++){
            pos=rand()%base.size();
            order[j]=base[pos];
            base.erase(base.begin()+pos);
        }
        result.push_back(std::make_shared<individual<int>>(order,func));
    }
    return result;
}
};


template<typename T>
int find_min(const std::vector<T>& range, int post){
    T min;
    int pos;
    for(int i=0;i<range.size();i++){
        if(range[i]==-1||i==post){
            continue;
        }
        min=range[i];
        pos=i;
        break;
    }

    for(int i=pos+1;i<range.size();i++){
        if(range[i]==-1||i==post){
            continue;
        }
        if(range[i]<min){
            min=range[i];
            pos=i;
        }
    }
    return pos;
}

template<typename T>
void erase_position(std::vector<std::vector<T>>& matr, int pos){
    for(int i=0;i<matr.size();i++){
        matr[i][pos]=-1;
    }
    for(auto& x:matr[pos]){
        x=-1;
    }
}
template<typename T>
T path_sum(std::vector<std::vector<T>>& matr, std::vector<int> path){
    T sum=0;
    for(int i=0;i<path.size()-1;i++){
        sum+=matr[path[i]][path[i+1]];
    }
    return sum;
}


template<typename Adapt_func>
class greedy_forming_algorithm : public forming_algorithm<Adapt_func>{
public:
greedy_forming_algorithm(const std::vector<std::vector<float>>& matr):matr(matr){}
std::vector<std::shared_ptr<individual<int>>> operator()(std::size_t individ_size, std::size_t pop_size, const Adapt_func& func){
    std::vector<std::shared_ptr<individual<int>>> result;
    std::vector<std::vector<float>> temp_matr=matr;
    std::vector<int> path;
    std::size_t size=matr.size();
    int start;
    float sum_distance=0;
    for(std::size_t j=0;j<pop_size;j++){
        start=rand()%size;
        path.push_back(start);
        for(int i=1;i<size;i++){
            start=find_min(temp_matr[path[i-1]],path[i-1]);
            path.push_back(start);
            sum_distance=path_sum(matr,path);
            erase_position(temp_matr,path[i-1]);
        }
        result.push_back(std::make_shared<individual<int>>(path,func));
        path.clear();
        sum_distance=0;
        temp_matr=matr;
    }
    return result;
}
std::vector<std::vector<float>> matr;
};