#ifndef RANDOM_ENGINE_MAPPER_MISC_HPP
#define RANDOM_ENGINE_MAPPER_MISC_HPP

#include "../random.hpp"

namespace hadoken {

namespace impl{

class abstract_engine{
public:
    abstract_engine(){};
    virtual ~abstract_engine(){};

    virtual void map_seed()= 0;
    virtual void map_seed(random_engine_mapper::result_type seed) =0;

    virtual random_engine_mapper::result_type generate() =0;
private:
};

template< typename Engine >
class map_engine_intern: public abstract_engine{
public:
    map_engine_intern(const Engine & e) : _e(e) {}

    virtual void map_seed(){
        _e.seed();
    }

    virtual void map_seed(random_engine_mapper::result_type seed){
        _e.seed(seed);
    }

    virtual random_engine_mapper::result_type generate(){
        return _e();
    }

private:
    Engine _e;
};


}

template<typename Engine>
random_engine_mapper::random_engine_mapper(const Engine & e) : _engine(new impl::map_engine_intern<Engine>(e)){

}

void random_engine_mapper::seed(){
    _engine->map_seed();
}

void random_engine_mapper::seed(random_engine_mapper::result_type seed){
    _engine->map_seed(seed);
}

random_engine_mapper::result_type random_engine_mapper::operator ()(){
    return _engine->generate();
}




}

#endif // RANDOM_ENGINE_MAPPER_MISC_HPP
