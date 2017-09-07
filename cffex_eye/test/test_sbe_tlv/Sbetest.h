/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _SBETEST_SBETEST_H_
#define _SBETEST_SBETEST_H_

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

#include "sbe.h"

#include "VarDataEncoding.h"

using namespace sbe;

namespace sbetest {

class Sbetest
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t *m_positionPtr;
    std::uint64_t m_offset;
    std::uint64_t m_position;
    std::uint64_t m_actingBlockLength;
    std::uint64_t m_actingVersion;

    inline void reset(
        char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength,
        const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        m_buffer = buffer;
        m_offset = offset;
        m_bufferLength = bufferLength;
        m_actingBlockLength = actingBlockLength;
        m_actingVersion = actingVersion;
        m_positionPtr = &m_position;
        position(offset + m_actingBlockLength);
    }

public:

    Sbetest(void) : m_buffer(nullptr), m_bufferLength(0), m_offset(0) {}

    Sbetest(char *buffer, const std::uint64_t bufferLength)
    {
        reset(buffer, 0, bufferLength, sbeBlockLength(), sbeSchemaVersion());
    }

    Sbetest(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    Sbetest(const Sbetest& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
    }

#if __cplusplus >= 201103L
    Sbetest(Sbetest&& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
    }

    Sbetest& operator=(Sbetest&& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
        return *this;
    }

#endif

    Sbetest& operator=(const Sbetest& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
        return *this;
    }

    static const std::uint16_t sbeBlockLength(void)
    {
        return (std::uint16_t)24;
    }

    static const std::uint16_t sbeTemplateId(void)
    {
        return (std::uint16_t)1;
    }

    static const std::uint16_t sbeSchemaId(void)
    {
        return (std::uint16_t)1;
    }

    static const std::uint16_t sbeSchemaVersion(void)
    {
        return (std::uint16_t)1;
    }

    static const char *sbeSemanticType(void)
    {
        return "";
    }

    std::uint64_t offset(void) const
    {
        return m_offset;
    }

    Sbetest &wrapForEncode(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    Sbetest &wrapForDecode(
         char *buffer, const std::uint64_t offset, const std::uint64_t actingBlockLength,
         const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingBlockLength, actingVersion);
        return *this;
    }

    std::uint64_t position(void) const
    {
        return m_position;
    }

    void position(const std::uint64_t position)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false))
        {
            throw std::runtime_error("buffer too short [E100]");
        }
        m_position = position;
    }

    std::uint64_t encodedLength(void) const
    {
        return position() - m_offset;
    }

    char *buffer(void)
    {
        return m_buffer;
    }

    std::uint64_t actingVersion(void) const
    {
        return m_actingVersion;
    }

    static const std::uint16_t param1Id(void)
    {
        return 1;
    }

    static const std::uint64_t param1SinceVersion(void)
    {
         return 0;
    }

    bool param1InActingVersion(void)
    {
        return (m_actingVersion >= param1SinceVersion()) ? true : false;
    }


    static const char *param1MetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const std::int32_t param1NullValue()
    {
        return SBE_NULLVALUE_INT32;
    }

    static const std::int32_t param1MinValue()
    {
        return -2147483647;
    }

    static const std::int32_t param1MaxValue()
    {
        return 2147483647;
    }

    std::int32_t param1(void) const
    {
        return SBE_BIG_ENDIAN_ENCODE_32(*((std::int32_t *)(m_buffer + m_offset + 0)));
    }

    Sbetest &param1(const std::int32_t value)
    {
        *((std::int32_t *)(m_buffer + m_offset + 0)) = SBE_BIG_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const std::uint16_t param2Id(void)
    {
        return 2;
    }

    static const std::uint64_t param2SinceVersion(void)
    {
         return 0;
    }

    bool param2InActingVersion(void)
    {
        return (m_actingVersion >= param2SinceVersion()) ? true : false;
    }


    static const char *param2MetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const std::int32_t param2NullValue()
    {
        return SBE_NULLVALUE_INT32;
    }

    static const std::int32_t param2MinValue()
    {
        return -2147483647;
    }

    static const std::int32_t param2MaxValue()
    {
        return 2147483647;
    }

    std::int32_t param2(void) const
    {
        return SBE_BIG_ENDIAN_ENCODE_32(*((std::int32_t *)(m_buffer + m_offset + 4)));
    }

    Sbetest &param2(const std::int32_t value)
    {
        *((std::int32_t *)(m_buffer + m_offset + 4)) = SBE_BIG_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const std::uint16_t param3Id(void)
    {
        return 3;
    }

    static const std::uint64_t param3SinceVersion(void)
    {
         return 0;
    }

    bool param3InActingVersion(void)
    {
        return (m_actingVersion >= param3SinceVersion()) ? true : false;
    }


    static const char *param3MetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const std::int32_t param3NullValue()
    {
        return SBE_NULLVALUE_INT32;
    }

    static const std::int32_t param3MinValue()
    {
        return -2147483647;
    }

    static const std::int32_t param3MaxValue()
    {
        return 2147483647;
    }

    std::int32_t param3(void) const
    {
        return SBE_BIG_ENDIAN_ENCODE_32(*((std::int32_t *)(m_buffer + m_offset + 8)));
    }

    Sbetest &param3(const std::int32_t value)
    {
        *((std::int32_t *)(m_buffer + m_offset + 8)) = SBE_BIG_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const std::uint16_t param4Id(void)
    {
        return 4;
    }

    static const std::uint64_t param4SinceVersion(void)
    {
         return 0;
    }

    bool param4InActingVersion(void)
    {
        return (m_actingVersion >= param4SinceVersion()) ? true : false;
    }


    static const char *param4MetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const std::int32_t param4NullValue()
    {
        return SBE_NULLVALUE_INT32;
    }

    static const std::int32_t param4MinValue()
    {
        return -2147483647;
    }

    static const std::int32_t param4MaxValue()
    {
        return 2147483647;
    }

    std::int32_t param4(void) const
    {
        return SBE_BIG_ENDIAN_ENCODE_32(*((std::int32_t *)(m_buffer + m_offset + 12)));
    }

    Sbetest &param4(const std::int32_t value)
    {
        *((std::int32_t *)(m_buffer + m_offset + 12)) = SBE_BIG_ENDIAN_ENCODE_32(value);
        return *this;
    }

    static const std::uint16_t param5Id(void)
    {
        return 5;
    }

    static const std::uint64_t param5SinceVersion(void)
    {
         return 0;
    }

    bool param5InActingVersion(void)
    {
        return (m_actingVersion >= param5SinceVersion()) ? true : false;
    }


    static const char *param5MetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const double param5NullValue()
    {
        return SBE_DOUBLE_NAN;
    }

    static const double param5MinValue()
    {
        return 4.9E-324;
    }

    static const double param5MaxValue()
    {
        return 1.7976931348623157E308;
    }

    double param5(void) const
    {
        return SBE_BIG_ENDIAN_ENCODE_64(*((double *)(m_buffer + m_offset + 16)));
    }

    Sbetest &param5(const double value)
    {
        *((double *)(m_buffer + m_offset + 16)) = SBE_BIG_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const char *param6MetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const char *param6CharacterEncoding()
    {
        return "UTF-8";
    }

    static const std::uint64_t param6SinceVersion(void)
    {
         return 0;
    }

    bool param6InActingVersion(void)
    {
        return (m_actingVersion >= param6SinceVersion()) ? true : false;
    }

    static const std::uint16_t param6Id(void)
    {
        return 6;
    }


    static const std::uint64_t param6HeaderSize()
    {
        return 2;
    }

    std::uint16_t param6Length(void) const
    {
        return SBE_BIG_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + position())));
    }

    const char *param6(void)
    {
         const char *fieldPtr = (m_buffer + position() + 2);
         position(position() + 2 + *((std::uint16_t *)(m_buffer + position())));
         return fieldPtr;
    }

    std::uint64_t getParam6(char *dst, const std::uint64_t length)
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_BIG_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + lengthPosition)));
        std::uint64_t bytesToCopy = (length < dataLength) ? length : dataLength;
        std::uint64_t pos = position();
        position(position() + dataLength);
        std::memcpy(dst, m_buffer + pos, bytesToCopy);
        return bytesToCopy;
    }

    std::uint64_t putParam6(const char *src, const std::uint64_t length)
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint16_t *)(m_buffer + lengthPosition)) = SBE_BIG_ENDIAN_ENCODE_16((std::uint16_t)length);
        std::uint64_t pos = position();
        position(position() + length);
        std::memcpy(m_buffer + pos, src, length);
        return length;
    }

    const std::string getParam6AsString()
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_BIG_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + lengthPosition)));
        std::uint64_t pos = position();
        const std::string result(m_buffer + pos, dataLength);
        position(position() + dataLength);
        return std::move(result);
    }

    Sbetest &putParam6(const std::string& str)
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint16_t *)(m_buffer + lengthPosition)) = SBE_BIG_ENDIAN_ENCODE_16((std::uint16_t)str.length());
        std::uint64_t pos = position();
        position(position() + str.length());
        std::memcpy(m_buffer + pos, str.c_str(), str.length());
        return *this;
    }

    static const char *param7MetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const char *param7CharacterEncoding()
    {
        return "UTF-8";
    }

    static const std::uint64_t param7SinceVersion(void)
    {
         return 0;
    }

    bool param7InActingVersion(void)
    {
        return (m_actingVersion >= param7SinceVersion()) ? true : false;
    }

    static const std::uint16_t param7Id(void)
    {
        return 7;
    }


    static const std::uint64_t param7HeaderSize()
    {
        return 2;
    }

    std::uint16_t param7Length(void) const
    {
        return SBE_BIG_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + position())));
    }

    const char *param7(void)
    {
         const char *fieldPtr = (m_buffer + position() + 2);
         position(position() + 2 + *((std::uint16_t *)(m_buffer + position())));
         return fieldPtr;
    }

    std::uint64_t getParam7(char *dst, const std::uint64_t length)
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_BIG_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + lengthPosition)));
        std::uint64_t bytesToCopy = (length < dataLength) ? length : dataLength;
        std::uint64_t pos = position();
        position(position() + dataLength);
        std::memcpy(dst, m_buffer + pos, bytesToCopy);
        return bytesToCopy;
    }

    std::uint64_t putParam7(const char *src, const std::uint64_t length)
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint16_t *)(m_buffer + lengthPosition)) = SBE_BIG_ENDIAN_ENCODE_16((std::uint16_t)length);
        std::uint64_t pos = position();
        position(position() + length);
        std::memcpy(m_buffer + pos, src, length);
        return length;
    }

    const std::string getParam7AsString()
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_BIG_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + lengthPosition)));
        std::uint64_t pos = position();
        const std::string result(m_buffer + pos, dataLength);
        position(position() + dataLength);
        return std::move(result);
    }

    Sbetest &putParam7(const std::string& str)
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint16_t *)(m_buffer + lengthPosition)) = SBE_BIG_ENDIAN_ENCODE_16((std::uint16_t)str.length());
        std::uint64_t pos = position();
        position(position() + str.length());
        std::memcpy(m_buffer + pos, str.c_str(), str.length());
        return *this;
    }

    static const char *param8MetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const char *param8CharacterEncoding()
    {
        return "UTF-8";
    }

    static const std::uint64_t param8SinceVersion(void)
    {
         return 0;
    }

    bool param8InActingVersion(void)
    {
        return (m_actingVersion >= param8SinceVersion()) ? true : false;
    }

    static const std::uint16_t param8Id(void)
    {
        return 8;
    }


    static const std::uint64_t param8HeaderSize()
    {
        return 2;
    }

    std::uint16_t param8Length(void) const
    {
        return SBE_BIG_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + position())));
    }

    const char *param8(void)
    {
         const char *fieldPtr = (m_buffer + position() + 2);
         position(position() + 2 + *((std::uint16_t *)(m_buffer + position())));
         return fieldPtr;
    }

    std::uint64_t getParam8(char *dst, const std::uint64_t length)
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_BIG_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + lengthPosition)));
        std::uint64_t bytesToCopy = (length < dataLength) ? length : dataLength;
        std::uint64_t pos = position();
        position(position() + dataLength);
        std::memcpy(dst, m_buffer + pos, bytesToCopy);
        return bytesToCopy;
    }

    std::uint64_t putParam8(const char *src, const std::uint64_t length)
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint16_t *)(m_buffer + lengthPosition)) = SBE_BIG_ENDIAN_ENCODE_16((std::uint16_t)length);
        std::uint64_t pos = position();
        position(position() + length);
        std::memcpy(m_buffer + pos, src, length);
        return length;
    }

    const std::string getParam8AsString()
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_BIG_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + lengthPosition)));
        std::uint64_t pos = position();
        const std::string result(m_buffer + pos, dataLength);
        position(position() + dataLength);
        return std::move(result);
    }

    Sbetest &putParam8(const std::string& str)
    {
        std::uint64_t lengthOfLengthField = 2;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint16_t *)(m_buffer + lengthPosition)) = SBE_BIG_ENDIAN_ENCODE_16((std::uint16_t)str.length());
        std::uint64_t pos = position();
        position(position() + str.length());
        std::memcpy(m_buffer + pos, str.c_str(), str.length());
        return *this;
    }
};
}
#endif
