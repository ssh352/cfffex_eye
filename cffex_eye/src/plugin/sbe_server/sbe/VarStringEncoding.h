/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _EXAMPLE_VARSTRINGENCODING_H_
#define _EXAMPLE_VARSTRINGENCODING_H_

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

class VarStringEncoding
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t m_offset;
    std::uint64_t m_actingVersion;

    inline void reset(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT(((offset + -1) > bufferLength), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        m_buffer = buffer;
        m_bufferLength = bufferLength;
        m_offset = offset;
        m_actingVersion = actingVersion;
    }

public:
    VarStringEncoding() : m_buffer(nullptr), m_offset(0) {}

    VarStringEncoding(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    VarStringEncoding &wrap(char *buffer, const std::uint64_t offset, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static SBE_CONSTEXPR std::uint64_t encodedLength()
    {
        return -1;
    }

    std::uint64_t offset() const
    {
        return m_offset;
    }

    char *buffer()
    {
        return m_buffer;
    }


    static SBE_CONSTEXPR std::uint32_t lengthNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static SBE_CONSTEXPR std::uint32_t lengthMinValue()
    {
        return 0;
    }

    static SBE_CONSTEXPR std::uint32_t lengthMaxValue()
    {
        return 1073741824;
    }

    static SBE_CONSTEXPR std::size_t lengthEncodingLength()
    {
        return 4;
    }

    std::uint32_t length() const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 0)));
    }

    VarStringEncoding &length(const std::uint32_t value)
    {
        *((std::uint32_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static SBE_CONSTEXPR std::uint8_t varDataNullValue()
    {
        return SBE_NULLVALUE_UINT8;
    }

    static SBE_CONSTEXPR std::uint8_t varDataMinValue()
    {
        return (std::uint8_t)0;
    }

    static SBE_CONSTEXPR std::uint8_t varDataMaxValue()
    {
        return (std::uint8_t)254;
    }

    static SBE_CONSTEXPR std::size_t varDataEncodingLength()
    {
        return 1;
    }
};
}
#endif
