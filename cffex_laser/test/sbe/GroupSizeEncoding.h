/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _EXAMPLE_GROUPSIZEENCODING_H_
#define _EXAMPLE_GROUPSIZEENCODING_H_

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

class GroupSizeEncoding
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t m_offset;
    std::uint64_t m_actingVersion;

    inline void reset(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT(((offset + 4) > bufferLength), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        m_buffer = buffer;
        m_bufferLength = bufferLength;
        m_offset = offset;
        m_actingVersion = actingVersion;
    }

public:
    GroupSizeEncoding(void) : m_buffer(nullptr), m_offset(0) {}

    GroupSizeEncoding(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    GroupSizeEncoding(const GroupSizeEncoding& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

#if __cplusplus >= 201103L
    GroupSizeEncoding(GroupSizeEncoding&& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

    GroupSizeEncoding& operator=(GroupSizeEncoding&& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

#endif

    GroupSizeEncoding& operator=(const GroupSizeEncoding& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

    GroupSizeEncoding &wrap(char *buffer, const std::uint64_t offset, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const std::uint64_t encodedLength(void)
    {
        return 4;
    }


    static const std::uint16_t blockLengthNullValue()
    {
        return SBE_NULLVALUE_UINT16;
    }

    static const std::uint16_t blockLengthMinValue()
    {
        return (std::uint16_t)0;
    }

    static const std::uint16_t blockLengthMaxValue()
    {
        return (std::uint16_t)65534;
    }

    std::uint16_t blockLength(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + m_offset + 0)));
    }

    GroupSizeEncoding &blockLength(const std::uint16_t value)
    {
        *((std::uint16_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }

    static const std::uint16_t numInGroupNullValue()
    {
        return SBE_NULLVALUE_UINT16;
    }

    static const std::uint16_t numInGroupMinValue()
    {
        return (std::uint16_t)0;
    }

    static const std::uint16_t numInGroupMaxValue()
    {
        return (std::uint16_t)65534;
    }

    std::uint16_t numInGroup(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + m_offset + 2)));
    }

    GroupSizeEncoding &numInGroup(const std::uint16_t value)
    {
        *((std::uint16_t *)(m_buffer + m_offset + 2)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }
};
}
#endif
