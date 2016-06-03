/**
 * Copyright (C) 2016 Adrien Devresse
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
*/
#ifndef _HADOKEN_SHA1_HPP_
#define _HADOKEN_SHA1_HPP_

#include <boost/integer.hpp>
#include <boost/array.hpp>


#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <algorithm>

// endianness functions
#include <netinet/in.h>

//
// SHA1 cryptographic hash implementation
// in the same model of boost::uid::sha1
// and TinySHA1
//

namespace hadoken
{


    ///
    /// sha1 processing object
    ///
    class sha1
    {
    public:
        typedef boost::array<boost::uint32_t, 5> digest32_t;
        typedef boost::array<boost::uint8_t, 20> digest8_t;

        inline sha1(){ reset(); }

        inline ~sha1() {}

        inline void reset() {
            m_digest[0] = 0x67452301;
            m_digest[1] = 0xEFCDAB89;
            m_digest[2] = 0x98BADCFE;
            m_digest[3] = 0x10325476;
            m_digest[4] = 0xC3D2E1F0;
            m_blockByteIndex = 0;
            m_byteCount = 0;
        }


        inline void process(boost::uint8_t b) {
            process_byte(b);
        }

        inline void process(boost::uint32_t value){
            boost::uint32_t big_endian_value = htonl(value);
            process_block(&big_endian_value, sizeof(boost::uint32_t));
        }

        template<typename Iterator>
        inline void process_block(const Iterator start, const Iterator end) {
            const boost::uint8_t* begin = static_cast<const boost::uint8_t*>(&(*start));
            const boost::uint8_t* finish = static_cast<const boost::uint8_t*>(&(*end));
            while(begin != finish) {
                process_byte(*begin);
                begin++;
            }
        }

        inline void process_block(const void* const data, size_t len) {
            const boost::uint8_t* block = static_cast<const boost::uint8_t*>(data);
            process_block(block, block + len);
        }







        inline std::string to_string(){
            std::ostringstream ss;

            digest8_t hash =  get_digest_bytes();

            ss << std::hex << std::setfill('0');

            for(digest8_t::iterator it = hash.begin(); it < hash.end(); ++it){
                ss << std::setw(2) << static_cast<unsigned int>(*it);
            }

            return ss.str();
        }


        inline digest32_t get_digest() {
            digest32_t digest;
            finalize();

            std::copy(m_digest.begin(), m_digest.end(), digest.begin());
            return digest;
        }


        inline digest8_t get_digest_bytes() {
            digest8_t digest;
            digest32_t d32 =get_digest();

            // big endian to little endiant
            std::transform(d32.begin(), d32.end(), d32.begin(), ntohl);
            ::memcpy(&digest[0], &(d32[0]), digest.size());

            return digest;
        }

    private:
        digest32_t m_digest;
        boost::array<boost::uint8_t, 64> m_block;
        size_t m_blockByteIndex;
        size_t m_byteCount;
        std::bitset<8> m_flags;

        enum{
            completed = 0x00
        };

        inline static boost::uint32_t left_rotate(boost::uint32_t value, size_t count) {
            return (value << count) ^ (value >> (32-count));
        }


        inline void process_byte(boost::uint8_t b){
            this->m_block[this->m_blockByteIndex++] = b;
            ++this->m_byteCount;
            if(m_blockByteIndex == 64) {
                this->m_blockByteIndex = 0;
                process_internal();
            }
        }


        inline void finalize(){
            if(m_flags[completed] == false){
                size_t bitCount = this->m_byteCount * 8;

                process_byte(0x80);
                if (this->m_blockByteIndex > 56) {
                    while (m_blockByteIndex != 0) {
                        process_byte(0);
                    }
                    while (m_blockByteIndex < 56) {
                        process_byte(0);
                    }
                } else {
                    while (m_blockByteIndex < 56) {
                        process_byte(0);
                    }
                }
                process_byte(0);
                process_byte(0);
                process_byte(0);
                process_byte(0);
                process_byte( static_cast<unsigned char>((bitCount>>24) & 0xFF));
                process_byte( static_cast<unsigned char>((bitCount>>16) & 0xFF));
                process_byte( static_cast<unsigned char>((bitCount>>8 ) & 0xFF));
                process_byte( static_cast<unsigned char>((bitCount)     & 0xFF));

                m_flags[completed] = true;
            }
        }


        inline void process_internal() {
            boost::uint32_t w[80];
            for (size_t i = 0; i < 16; i++) {
                w[i]  = (m_block[i*4 + 0] << 24);
                w[i] |= (m_block[i*4 + 1] << 16);
                w[i] |= (m_block[i*4 + 2] << 8);
                w[i] |= (m_block[i*4 + 3]);
            }
            for (size_t i = 16; i < 80; i++) {
                w[i] = left_rotate((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1);
            }

            boost::uint32_t a = m_digest[0];
            boost::uint32_t b = m_digest[1];
            boost::uint32_t c = m_digest[2];
            boost::uint32_t d = m_digest[3];
            boost::uint32_t e = m_digest[4];

            for (std::size_t i=0; i<80; ++i) {
                boost::uint32_t f = 0;
                boost::uint32_t k = 0;

                if (i<20) {
                    f = (b & c) | (~b & d);
                    k = 0x5A827999;
                } else if (i<40) {
                    f = b ^ c ^ d;
                    k = 0x6ED9EBA1;
                } else if (i<60) {
                    f = (b & c) | (b & d) | (c & d);
                    k = 0x8F1BBCDC;
                } else {
                    f = b ^ c ^ d;
                    k = 0xCA62C1D6;
                }
                boost::uint32_t temp = left_rotate(a, 5) + f + e + k + w[i];
                e = d;
                d = c;
                c = left_rotate(b, 30);
                b = a;
                a = temp;
            }

            m_digest[0] += a;
            m_digest[1] += b;
            m_digest[2] += c;
            m_digest[3] += d;
            m_digest[4] += e;
        }

    };



}


#endif
