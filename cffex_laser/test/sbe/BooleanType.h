/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _EXAMPLE_BOOLEANTYPE_H_
#define _EXAMPLE_BOOLEANTYPE_H_

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
#  include <functional>
#  include <string>
#  include <cstring>
#endif

#include <sbe/sbe.h>

using namespace sbe;

namespace example {

class BooleanType
{
public:

    enum Value 
    {
        F = (std::uint8_t)0,
        T = (std::uint8_t)1,
        NULL_VALUE = (std::uint8_t)255
    };

    static BooleanType::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case 0: return F;
            case 1: return T;
            case 255: return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum BooleanType [E103]");
    }
};
}
#endif
