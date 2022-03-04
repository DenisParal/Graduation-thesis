#pragma once
#include "includes.h"


template<typename T>
class mutation{
public:
    virtual void operator()(std::shared_ptr<individual<T>>& individ)=0;
};

template<typename T>
class point_mut : public mutation<T>{
public:
void operator()(std::shared_ptr<individual<T>>& individ){
    std::size_t pos=rand()%individ->size();
    (*individ)[pos]=1-(*individ)[pos];
}
};

template<typename T>
class saltation_mut : public mutation<T>{
public:
void operator()(std::shared_ptr<individual<T>>& individ){
    std::size_t pos1=rand()%individ->size();
    std::size_t pos2=rand()%individ->size();
    std::swap((*individ)[pos1],(*individ)[pos2]);
}
};

template<typename T>
class inversion_mut : public mutation<T>{
public:
void operator()(std::shared_ptr<individual<T>>& individ){
    std::size_t pos1=rand()%individ->size();
    std::size_t pos2=rand()%individ->size();
    for(std::size_t i=pos1;i<=pos2;i++){
        (*individ)[i]=1-(*individ)[i];
    }
}
};

template<typename T>
class point_ordered_mut : public mutation<T>{
public:
void operator()(std::shared_ptr<individual<T>>& individ){
    std::size_t pos=rand()%(individ->size()-1);
    std::swap((*individ)[pos],(*individ)[pos+1]);
}
};