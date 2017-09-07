/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _EXAMPLE_OPTIONALEXTRAS_H_
#define _EXAMPLE_OPTIONALEXTRAS_H_

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

class OptionalExtras
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t m_offset;
    std::uint64_t m_actingVersion;

    inline void reset(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT(((offset + 1) > bufferLength), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        m_buffer = buffer;
        m_bufferLength = bufferLength;
        m_offset = offset;
        m_actingVersion = actingVersion;
    }

public:
    OptionalExtras() : m_buffer(nullptr), m_offset(0) {}

    OptionalExtras(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    OptionalExtras &wrap(char *buffer, const std::uint64_t offset, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static SBE_CONSTEXPR std::uint64_t encodedLength()
    {
        return 1;
    }

    std::uint64_t offset() const
    {
        return m_offset;
    }

    char *buffer()
    {
        return m_buffer;
    }


    OptionalExtras &clear()
    {
        *((std::uint8_t *)(m_buffer + m_offset)) = 0;
        return *this;
    }


    bool sunRoof() const
    {
        return (*((std::uint8_t *)(m_buffer + m_offset))) & (0x1L << 0);
    }

    OptionalExtras &sunRoof(const bool value)
    {
        std::uint8_t bits = (*((std::uint8_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 0)) : (bits & ~(0x1L << 0));
        *((std::uint8_t *)(m_buffer + m_offset)) = (bits);
        return *this;
    }

    bool sportsPack() const
    {
        return (*((std::uint8_t *)(m_buffer + m_offset))) & (0x1L << 1);
    }

    OptionalExtras &sportsPack(const bool value)
    {
        std::uint8_t bits = (*((std::uint8_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 1)) : (bits & ~(0x1L << 1));
        *((std::uint8_t *)(m_buffer + m_offset)) = (bits);
        return *this;
    }

    bool cruiseControl() const
    {
        return (*((std::uint8_t *)(m_buffer + m_offset))) & (0x1L << 2);
    }

    OptionalExtras &cruiseControl(const bool value)
    {
        std::uint8_t bits = (*((std::uint8_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 2)) : (bits & ~(0x1L << 2));
        *((std::uint8_t *)(m_buffer + m_offset)) = (bits);
        return *this;
    }
};
}
#endif
