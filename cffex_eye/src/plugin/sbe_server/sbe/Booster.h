/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _EXAMPLE_BOOSTER_H_
#define _EXAMPLE_BOOSTER_H_

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

#include "BoostType.h"

using namespace sbe;

namespace example {

class Booster
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t m_offset;
    std::uint64_t m_actingVersion;

    inline void reset(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT(((offset + 2) > bufferLength), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        m_buffer = buffer;
        m_bufferLength = bufferLength;
        m_offset = offset;
        m_actingVersion = actingVersion;
    }

public:
    Booster() : m_buffer(nullptr), m_offset(0) {}

    Booster(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    Booster &wrap(char *buffer, const std::uint64_t offset, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static SBE_CONSTEXPR std::uint64_t encodedLength()
    {
        return 2;
    }

    std::uint64_t offset() const
    {
        return m_offset;
    }

    char *buffer()
    {
        return m_buffer;
    }


    BoostType::Value boostType() const
    {
        return BoostType::get((*((char *)(m_buffer + m_offset + 0))));
    }

    Booster &boostType(const BoostType::Value value)
    {
        *((char *)(m_buffer + m_offset + 0)) = (value);
        return *this;
    }
    static SBE_CONSTEXPR std::size_t boostTypeEncodingLength()
    {
        return 1;
    }

    static SBE_CONSTEXPR std::uint8_t horsePowerNullValue()
    {
        return SBE_NULLVALUE_UINT8;
    }

    static SBE_CONSTEXPR std::uint8_t horsePowerMinValue()
    {
        return (std::uint8_t)0;
    }

    static SBE_CONSTEXPR std::uint8_t horsePowerMaxValue()
    {
        return (std::uint8_t)254;
    }

    static SBE_CONSTEXPR std::size_t horsePowerEncodingLength()
    {
        return 1;
    }

    std::uint8_t horsePower() const
    {
        return (*((std::uint8_t *)(m_buffer + m_offset + 1)));
    }

    Booster &horsePower(const std::uint8_t value)
    {
        *((std::uint8_t *)(m_buffer + m_offset + 1)) = (value);
        return *this;
    }
};
}
#endif
