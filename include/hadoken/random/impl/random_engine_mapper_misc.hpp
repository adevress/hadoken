#ifndef RANDOM_ENGINE_MAPPER_MISC_HPP
#define RANDOM_ENGINE_MAPPER_MISC_HPP




#include <cassert>

#include <hadoken/random/random_engine_mapper.hpp>
#include <hadoken/random/random_derivate.hpp>

namespace hadoken {

namespace impl{



template< typename Uint >
class abstract_engine {
public:

    typedef Uint result_type;

    abstract_engine(){};
    virtual ~abstract_engine(){};

    virtual void map_seed()= 0;
    virtual void map_seed(result_type seed) =0;

    virtual result_type generate() =0;

    virtual abstract_engine* clone() =0;

    virtual abstract_engine* derivate(result_type key) const =0;

private:
};


template< typename Uint, typename Engine >
class map_engine_intern: public abstract_engine<Uint>{
public:
    typedef Uint result_type;

    map_engine_intern(const Engine & e) : _e(e){
    }

    virtual void map_seed(){
        _e.seed();
    }

    virtual void map_seed(result_type s){
        _e.seed(s);
    }

    virtual result_type generate(){
        return _e();
    }

    virtual abstract_engine<result_type>* clone(){
           return new map_engine_intern(_e);
    }

    virtual abstract_engine<result_type>* derivate(result_type key) const{
        Engine derivated_engine = random_engine_derivate(_e, key);
        map_engine_intern<result_type, Engine>* ret = new map_engine_intern<result_type, Engine>(derivated_engine);
        return ret;
    }

private:
    Engine _e;
};


} // impl

template< typename Uint >
template< typename Engine >
random_engine_mapper<Uint>::random_engine_mapper(const Engine & e) : _engine(new impl::map_engine_intern<Uint, Engine>(e)){

}


template< typename Uint >
random_engine_mapper<Uint>::random_engine_mapper() : _engine() {

}

template< typename Uint >
random_engine_mapper<Uint>::random_engine_mapper(const random_engine_mapper<Uint> & other) : _engine(NULL){
    if(other._engine.get() != NULL){
        _engine.reset(other._engine->clone());
    }
}

template< typename Uint >
void random_engine_mapper<Uint>::seed(){
    assert(_engine.get());
    _engine->map_seed();
}

template< typename Uint >
void random_engine_mapper<Uint>::seed(result_type seed){
    assert(_engine.get());
    _engine->map_seed(seed);
}

template< typename Uint >
typename random_engine_mapper<Uint>::result_type random_engine_mapper<Uint>::operator ()(){
    assert(_engine.get());
    return _engine->generate();
}

template< typename Uint >
random_engine_mapper<Uint> random_engine_mapper<Uint>::derivate(result_type key) const{
    assert(_engine.get());
    random_engine_mapper res;
    res._engine.reset(_engine->derivate(key));

    return res;
}








}

#endif // RANDOM_ENGINE_MAPPER_MISC_HPP
