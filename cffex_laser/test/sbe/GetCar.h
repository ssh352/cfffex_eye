/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _EXAMPLE_GETCAR_H_
#define _EXAMPLE_GETCAR_H_

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

#include "OptionalExtras.h"
#include "Booster.h"
#include "BooleanType.h"
#include "Model.h"
#include "GroupSizeEncoding.h"
#include "VarStringEncoding.h"
#include "Engine.h"
#include "BoostType.h"

using namespace sbe;

namespace example {

class GetCar
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

    GetCar(void) : m_buffer(nullptr), m_bufferLength(0), m_offset(0) {}

    GetCar(char *buffer, const std::uint64_t bufferLength)
    {
        reset(buffer, 0, bufferLength, sbeBlockLength(), sbeSchemaVersion());
    }

    GetCar(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    GetCar(const GetCar& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
    }

#if __cplusplus >= 201103L
    GetCar(GetCar&& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
    }

    GetCar& operator=(GetCar&& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
        return *this;
    }

#endif

    GetCar& operator=(const GetCar& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
        return *this;
    }

    static const std::uint16_t sbeBlockLength(void)
    {
        return (std::uint16_t)6;
    }

    static const std::uint16_t sbeTemplateId(void)
    {
        return (std::uint16_t)2;
    }

    static const std::uint16_t sbeSchemaId(void)
    {
        return (std::uint16_t)1;
    }

    static const std::uint16_t sbeSchemaVersion(void)
    {
        return (std::uint16_t)0;
    }

    static const char *sbeSemanticType(void)
    {
        return "";
    }

    std::uint64_t offset(void) const
    {
        return m_offset;
    }

    GetCar &wrapForEncode(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    GetCar &wrapForDecode(
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

    static const std::uint32_t param1NullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const std::uint32_t param1MinValue()
    {
        return 0;
    }

    static const std::uint32_t param1MaxValue()
    {
        return 4294967293;
    }

    std::uint32_t param1(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 0)));
    }

    GetCar &param1(const std::uint32_t value)
    {
        *((std::uint32_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
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

    static const std::uint16_t param2NullValue()
    {
        return SBE_NULLVALUE_UINT16;
    }

    static const std::uint16_t param2MinValue()
    {
        return (std::uint16_t)0;
    }

    static const std::uint16_t param2MaxValue()
    {
        return (std::uint16_t)65534;
    }

    std::uint16_t param2(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + m_offset + 4)));
    }

    GetCar &param2(const std::uint16_t value)
    {
        *((std::uint16_t *)(m_buffer + m_offset + 4)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
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

    static const char *param3CharacterEncoding()
    {
        return "UTF-8";
    }

    static const std::uint64_t param3SinceVersion(void)
    {
         return 0;
    }

    bool param3InActingVersion(void)
    {
        return (m_actingVersion >= param3SinceVersion()) ? true : false;
    }

    static const std::uint16_t param3Id(void)
    {
        return 3;
    }


    static const std::uint64_t param3HeaderSize()
    {
        return 4;
    }

    std::uint32_t param3Length(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + position())));
    }

    const char *param3(void)
    {
         const char *fieldPtr = (m_buffer + position() + 4);
         position(position() + 4 + *((std::uint32_t *)(m_buffer + position())));
         return fieldPtr;
    }

    std::uint64_t getParam3(char *dst, const std::uint64_t length)
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + lengthPosition)));
        std::uint64_t bytesToCopy = (length < dataLength) ? length : dataLength;
        std::uint64_t pos = position();
        position(position() + dataLength);
        std::memcpy(dst, m_buffer + pos, bytesToCopy);
        return bytesToCopy;
    }

    std::uint64_t putParam3(const char *src, const std::uint64_t length)
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint32_t *)(m_buffer + lengthPosition)) = SBE_LITTLE_ENDIAN_ENCODE_32((std::uint32_t)length);
        std::uint64_t pos = position();
        position(position() + length);
        std::memcpy(m_buffer + pos, src, length);
        return length;
    }

    const std::string getParam3AsString()
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + lengthPosition)));
        std::uint64_t pos = position();
        const std::string result(m_buffer + pos, dataLength);
        position(position() + dataLength);
        return std::move(result);
    }

    GetCar &putParam3(const std::string& str)
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint32_t *)(m_buffer + lengthPosition)) = SBE_LITTLE_ENDIAN_ENCODE_32((std::uint32_t)str.length());
        std::uint64_t pos = position();
        position(position() + str.length());
        std::memcpy(m_buffer + pos, str.c_str(), str.length());
        return *this;
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

    static const char *param4CharacterEncoding()
    {
        return "UTF-8";
    }

    static const std::uint64_t param4SinceVersion(void)
    {
         return 0;
    }

    bool param4InActingVersion(void)
    {
        return (m_actingVersion >= param4SinceVersion()) ? true : false;
    }

    static const std::uint16_t param4Id(void)
    {
        return 4;
    }


    static const std::uint64_t param4HeaderSize()
    {
        return 4;
    }

    std::uint32_t param4Length(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + position())));
    }

    const char *param4(void)
    {
         const char *fieldPtr = (m_buffer + position() + 4);
         position(position() + 4 + *((std::uint32_t *)(m_buffer + position())));
         return fieldPtr;
    }

    std::uint64_t getParam4(char *dst, const std::uint64_t length)
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + lengthPosition)));
        std::uint64_t bytesToCopy = (length < dataLength) ? length : dataLength;
        std::uint64_t pos = position();
        position(position() + dataLength);
        std::memcpy(dst, m_buffer + pos, bytesToCopy);
        return bytesToCopy;
    }

    std::uint64_t putParam4(const char *src, const std::uint64_t length)
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint32_t *)(m_buffer + lengthPosition)) = SBE_LITTLE_ENDIAN_ENCODE_32((std::uint32_t)length);
        std::uint64_t pos = position();
        position(position() + length);
        std::memcpy(m_buffer + pos, src, length);
        return length;
    }

    const std::string getParam4AsString()
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + lengthPosition)));
        std::uint64_t pos = position();
        const std::string result(m_buffer + pos, dataLength);
        position(position() + dataLength);
        return std::move(result);
    }

    GetCar &putParam4(const std::string& str)
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint32_t *)(m_buffer + lengthPosition)) = SBE_LITTLE_ENDIAN_ENCODE_32((std::uint32_t)str.length());
        std::uint64_t pos = position();
        position(position() + str.length());
        std::memcpy(m_buffer + pos, str.c_str(), str.length());
        return *this;
    }
};
}
#endif
