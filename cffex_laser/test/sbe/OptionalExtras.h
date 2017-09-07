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
#  include <functional>
#  include <string>
#  include <cstring>
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
    OptionalExtras(void) : m_buffer(nullptr), m_offset(0) {}

    OptionalExtras(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    OptionalExtras(const OptionalExtras& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

#if __cplusplus >= 201103L
    OptionalExtras(OptionalExtras&& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

    OptionalExtras& operator=(OptionalExtras&& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

#endif

    OptionalExtras& operator=(const OptionalExtras& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

    OptionalExtras &wrap(char *buffer, const std::uint64_t offset, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const std::uint64_t encodedLength(void)
    {
        return 1;
    }


    OptionalExtras &clear(void)
    {
        *((std::uint8_t *)(m_buffer + m_offset)) = 0;
        return *this;
    }


    bool sunRoof(void) const
    {
        return ((*((std::uint8_t *)(m_buffer + m_offset))) & (0x1L << 0)) ? true : false;
    }

    OptionalExtras &sunRoof(const bool value)
    {
        std::uint8_t bits = (*((std::uint8_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 0)) : (bits & ~(0x1L << 0));
        *((std::uint8_t *)(m_buffer + m_offset)) = (bits);
        return *this;
    }

    bool sportsPack(void) const
    {
        return ((*((std::uint8_t *)(m_buffer + m_offset))) & (0x1L << 1)) ? true : false;
    }

    OptionalExtras &sportsPack(const bool value)
    {
        std::uint8_t bits = (*((std::uint8_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 1)) : (bits & ~(0x1L << 1));
        *((std::uint8_t *)(m_buffer + m_offset)) = (bits);
        return *this;
    }

    bool cruiseControl(void) const
    {
        return ((*((std::uint8_t *)(m_buffer + m_offset))) & (0x1L << 2)) ? true : false;
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
