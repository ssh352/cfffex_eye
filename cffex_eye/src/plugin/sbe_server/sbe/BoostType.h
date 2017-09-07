/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _EXAMPLE_BOOSTTYPE_H_
#define _EXAMPLE_BOOSTTYPE_H_

#if defined(SBE_HAVE_CMATH)
/* cmath needed for std::numeric_limits<double>::quiet_NaN() */
#  include <cmath>
#  define SBE_FLOAT_NAN std::numeric_limits<float>::quiet_NaN()
#  define SBE_DOUBLE_NAN std::numeric_limits<double>::quiet_NaN()
#else
/* math.h needed for NAN */
#  include <math.h>
#  define SBE_FLOAT_NAN NAN
#  define SBE_DOUBLE_NAN NAN
#endif

#if __cplusplus >= 201103L
#  include <cstdint>
#  include <string>
#  include <cstring>
#endif

#if __cplusplus >= 201103L
#  define SBE_CONSTEXPR constexpr
#else
#  define SBE_CONSTEXPR
#endif

#include <sbe/sbe.h>

using namespace sbe;

namespace example {

class BoostType
{
public:

    enum Value 
    {
        TURBO = (char)84,
        SUPERCHARGER = (char)83,
        NITROUS = (char)78,
        KERS = (char)75,
        NULL_VALUE = (char)0
    };

    static BoostType::Value get(const char value)
    {
        switch (value)
        {
            case 84: return TURBO;
            case 83: return SUPERCHARGER;
            case 78: return NITROUS;
            case 75: return KERS;
            case 0: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum BoostType [E103]");
    }
};
}
#endif
