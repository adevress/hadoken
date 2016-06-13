#ifndef RANDOM_ENGINE_MAPPER_MISC_HPP
#define RANDOM_ENGINE_MAPPER_MISC_HPP


#include "../random.hpp"

#include <cassert>
#include <hadoken/crypto/sha/sha1.hpp>

namespace hadoken {

inline hadoken::sha1::digest32_t generate_deterministic_seed_160(boost::uint32_t origin_seed, boost::uint32_t key);

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

    virtual abstract_engine* derivate(result_type key) =0;

private:
};


template< typename Uint, typename Engine >
class map_engine_intern: public abstract_engine<Uint>{
public:
    typedef Uint result_type;

    map_engine_intern(const Engine & e, result_type seed = 0 ) : _e(e), _seed(seed) {
        if(_seed != 0){
            map_seed(_seed);
        }
    }

    virtual void map_seed(){
        _e.seed();
    }

    virtual void map_seed(result_type s){
        _e.seed(s);
        _seed = s;
    }

    virtual result_type generate(){
        return _e();
    }

    virtual abstract_engine<result_type>* clone(){
           return new map_engine_intern(_e, _seed);
    }

    virtual abstract_engine<result_type>* derivate(result_type key){
        map_engine_intern<result_type, Engine>* ret = new map_engine_intern<result_type, Engine>(_e);

        // compute a new  seed determinitically
        hadoken::sha1::digest32_t digest = generate_deterministic_seed_160(_seed, key);
        result_type new_seed;
        std::memcpy(&new_seed, &(digest[0]), sizeof(result_type));

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
    result_type _seed;
};


}

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
random_engine_mapper<Uint> random_engine_mapper<Uint>::derivate(result_type key){
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



boost::uint64_t generate_deterministic_seed(boost::uint64_t origin_seed, boost::uint64_t key){
    boost::uint64_t res_64;
    const hadoken::sha1::digest32_t array = generate_deterministic_seed_160(origin_seed, key);
    std::memcpy(&res_64, &(array[0]), sizeof(res_64));
    return res_64;
}





}

#endif // RANDOM_ENGINE_MAPPER_MISC_HPP
