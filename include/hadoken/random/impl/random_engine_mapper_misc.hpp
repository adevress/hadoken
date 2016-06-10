#ifndef RANDOM_ENGINE_MAPPER_MISC_HPP
#define RANDOM_ENGINE_MAPPER_MISC_HPP


#include "../random.hpp"

#include <cassert>
#include <hadoken/crypto/sha/sha1.hpp>

namespace hadoken {

inline hadoken::sha1::digest32_t generate_deterministic_seed_160(boost::uint32_t origin_seed, boost::uint32_t key);

namespace impl{




class abstract_engine{
public:
    abstract_engine(){};
    virtual ~abstract_engine(){};

    virtual void map_seed()= 0;
    virtual void map_seed(random_engine_mapper::result_type seed) =0;

    virtual random_engine_mapper::result_type generate() =0;

    virtual abstract_engine* clone() =0;

    virtual abstract_engine* derivate(random_engine_mapper::result_type key) =0;

private:
};

template< typename Engine >
class map_engine_intern: public abstract_engine{
public:
    map_engine_intern(const Engine & e, random_engine_mapper::result_type seed = 0 ) : _e(e), _seed(seed) {
        if(_seed != 0){
            map_seed(_seed);
        }
    }

    virtual void map_seed(){
        _e.seed();
    }

    virtual void map_seed(random_engine_mapper::result_type s){
        _e.seed(s);
        _seed = s;
    }

    virtual random_engine_mapper::result_type generate(){
        return _e();
    }

    virtual abstract_engine* clone(){
           return new map_engine_intern(_e, _seed);
    }

    virtual abstract_engine* derivate(random_engine_mapper::result_type key){
        map_engine_intern<Engine>* ret = new map_engine_intern<Engine>(_e);

        // compute a new  seed determinitically
        hadoken::sha1::digest32_t digest = generate_deterministic_seed_160(_seed, key);
        const random_engine_mapper::result_type new_seed = digest[0];

        // use it
        ret->map_seed(new_seed);

        // use the digest to increase entropy even more
        // we get the number of 1 bits in our digest and
        // initialize our random engine from 0 to n
        for(hadoken::sha1::digest32_t::iterator it = digest.begin();
            it < digest.end(); ++it){

            std::bitset<32> bits(*it);
            const std::size_t iteration = bits.count();
            for(std::size_t i =0; i <iteration; ++i){
                ret->generate();
            }
        }

        return ret;
    }

private:
    Engine _e;
    random_engine_mapper::result_type _seed;
};


}

template<typename Engine>
random_engine_mapper::random_engine_mapper(const Engine & e) : _engine(new impl::map_engine_intern<Engine>(e)){

}

random_engine_mapper::random_engine_mapper() : _engine() {

}

random_engine_mapper::random_engine_mapper(const random_engine_mapper & other) : _engine(NULL){
    if(other._engine.get() != NULL){
        _engine.reset(other._engine->clone());
    }
}

void random_engine_mapper::seed(){
    assert(_engine.get());
    _engine->map_seed();
}

void random_engine_mapper::seed(random_engine_mapper::result_type seed){
    assert(_engine.get());
    _engine->map_seed(seed);
}

random_engine_mapper::result_type random_engine_mapper::operator ()(){
    assert(_engine.get());
    return _engine->generate();
}


random_engine_mapper random_engine_mapper::derivate(result_type key){
    assert(_engine.get());
    random_engine_mapper res;
    res._engine.reset(_engine->derivate(key));

    return res;
}



inline hadoken::sha1::digest32_t generate_deterministic_seed_160(boost::uint32_t origin_seed, boost::uint32_t key){
    // let's add a bit of salt to our meal today
    boost::array<boost::uint8_t, 16> salt = { { 0x6c, 0x77, 0xad, 0xb8, 0x3e, 0xf8, 0x21, 0x61, 0xc3, 0xd8, 0x2e, 0x4c, 0x13, 0xfd, 0x75, 0xd3 } };

    hadoken::sha1 sha_compute;
    //  we want to generate a new seed determinitically 'seed = f(old_seed, key)'
    //  the new seed need to be free or any  statistics correlation from old_seed and key
    //
    // we do
    //   new_seed = sha1( concat(salt, key, origin_seed) )
    //
    // we return then the first 32 bits from the generations
    sha_compute.process_block(salt.begin(), salt.end());
    sha_compute.process(key);
    sha_compute.process(origin_seed);
    return sha_compute.get_digest();
}



inline boost::uint32_t generate_deterministic_seed(boost::uint32_t origin_seed, boost::uint32_t key){
    return generate_deterministic_seed_160(origin_seed, key)[0];
}





}

#endif // RANDOM_ENGINE_MAPPER_MISC_HPP
