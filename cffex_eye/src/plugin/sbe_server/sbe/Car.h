/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _EXAMPLE_CAR_H_
#define _EXAMPLE_CAR_H_

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

#include "OptionalExtras.h"
#include "MessageHeader.h"
#include "BooleanType.h"
#include "Model.h"
#include "GroupSizeEncoding.h"
#include "VarStringEncoding.h"
#include "Engine.h"
#include "BooleanType.h"
#include "Booster.h"
#include "BoostType.h"

using namespace sbe;

namespace example {

class Car
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

    Car() : m_buffer(nullptr), m_bufferLength(0), m_offset(0) {}

    Car(char *buffer, const std::uint64_t bufferLength)
    {
        reset(buffer, 0, bufferLength, sbeBlockLength(), sbeSchemaVersion());
    }

    Car(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    static SBE_CONSTEXPR std::uint16_t sbeBlockLength()
    {
        return (std::uint16_t)49;
    }

    static SBE_CONSTEXPR std::uint16_t sbeTemplateId()
    {
        return (std::uint16_t)1;
    }

    static SBE_CONSTEXPR std::uint16_t sbeSchemaId()
    {
        return (std::uint16_t)1;
    }

    static SBE_CONSTEXPR std::uint16_t sbeSchemaVersion()
    {
        return (std::uint16_t)0;
    }

    static SBE_CONSTEXPR const char * sbeSemanticType()
    {
        return "";
    }

    std::uint64_t offset() const
    {
        return m_offset;
    }

    Car &wrapForEncode(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    Car &wrapForDecode(
         char *buffer, const std::uint64_t offset, const std::uint64_t actingBlockLength,
         const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingBlockLength, actingVersion);
        return *this;
    }

    std::uint64_t position() const
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

    std::uint64_t encodedLength() const
    {
        return position() - m_offset;
    }

    char *buffer()
    {
        return m_buffer;
    }

    std::uint64_t actingVersion() const
    {
        return m_actingVersion;
    }

    static SBE_CONSTEXPR std::uint16_t serialNumberId()
    {
        return 1;
    }

    static SBE_CONSTEXPR std::uint64_t serialNumberSinceVersion()
    {
         return 0;
    }

    bool serialNumberInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= serialNumberSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t serialNumberEncodingOffset()
    {
         return 0;
    }


    static const char *serialNumberMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static SBE_CONSTEXPR std::uint64_t serialNumberNullValue()
    {
        return SBE_NULLVALUE_UINT64;
    }

    static SBE_CONSTEXPR std::uint64_t serialNumberMinValue()
    {
        return 0x0L;
    }

    static SBE_CONSTEXPR std::uint64_t serialNumberMaxValue()
    {
        return 0xfffffffffffffffeL;
    }

    static SBE_CONSTEXPR std::size_t serialNumberEncodingLength()
    {
        return 8;
    }

    std::uint64_t serialNumber() const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((std::uint64_t *)(m_buffer + m_offset + 0)));
    }

    Car &serialNumber(const std::uint64_t value)
    {
        *((std::uint64_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static SBE_CONSTEXPR std::uint16_t modelYearId()
    {
        return 2;
    }

    static SBE_CONSTEXPR std::uint64_t modelYearSinceVersion()
    {
         return 0;
    }

    bool modelYearInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= modelYearSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t modelYearEncodingOffset()
    {
         return 8;
    }


    static const char *modelYearMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static SBE_CONSTEXPR std::uint16_t modelYearNullValue()
    {
        return SBE_NULLVALUE_UINT16;
    }

    static SBE_CONSTEXPR std::uint16_t modelYearMinValue()
    {
        return (std::uint16_t)0;
    }

    static SBE_CONSTEXPR std::uint16_t modelYearMaxValue()
    {
        return (std::uint16_t)65534;
    }

    static SBE_CONSTEXPR std::size_t modelYearEncodingLength()
    {
        return 2;
    }

    std::uint16_t modelYear() const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + m_offset + 8)));
    }

    Car &modelYear(const std::uint16_t value)
    {
        *((std::uint16_t *)(m_buffer + m_offset + 8)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }

    static SBE_CONSTEXPR std::uint16_t availableId()
    {
        return 3;
    }

    static SBE_CONSTEXPR std::uint64_t availableSinceVersion()
    {
         return 0;
    }

    bool availableInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= availableSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t availableEncodingOffset()
    {
         return 10;
    }


    static const char *availableMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    BooleanType::Value available() const
    {
        return BooleanType::get((*((std::uint8_t *)(m_buffer + m_offset + 10))));
    }

    Car &available(const BooleanType::Value value)
    {
        *((std::uint8_t *)(m_buffer + m_offset + 10)) = (value);
        return *this;
    }
    static SBE_CONSTEXPR std::size_t availableEncodingLength()
    {
        return 1;
    }

    static SBE_CONSTEXPR std::uint16_t codeId()
    {
        return 4;
    }

    static SBE_CONSTEXPR std::uint64_t codeSinceVersion()
    {
         return 0;
    }

    bool codeInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= codeSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t codeEncodingOffset()
    {
         return 11;
    }


    static const char *codeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    Model::Value code() const
    {
        return Model::get((*((char *)(m_buffer + m_offset + 11))));
    }

    Car &code(const Model::Value value)
    {
        *((char *)(m_buffer + m_offset + 11)) = (value);
        return *this;
    }
    static SBE_CONSTEXPR std::size_t codeEncodingLength()
    {
        return 1;
    }

    static SBE_CONSTEXPR std::uint16_t someNumbersId()
    {
        return 5;
    }

    static SBE_CONSTEXPR std::uint64_t someNumbersSinceVersion()
    {
         return 0;
    }

    bool someNumbersInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= someNumbersSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t someNumbersEncodingOffset()
    {
         return 12;
    }


    static const char *someNumbersMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static SBE_CONSTEXPR std::uint32_t someNumbersNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static SBE_CONSTEXPR std::uint32_t someNumbersMinValue()
    {
        return 0;
    }

    static SBE_CONSTEXPR std::uint32_t someNumbersMaxValue()
    {
        return 4294967293;
    }

    static SBE_CONSTEXPR std::size_t someNumbersEncodingLength()
    {
        return 4;
    }

    static SBE_CONSTEXPR std::uint64_t someNumbersLength()
    {
        return 5;
    }

    const char *someNumbers() const
    {
        return (m_buffer + m_offset + 12);
    }

    std::uint32_t someNumbers(const std::uint64_t index) const
    {
        if (index >= 5)
        {
            throw std::runtime_error("index out of range for someNumbers [E104]");
        }

        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 12 + (index * 4))));
    }

    void someNumbers(const std::uint64_t index, const std::uint32_t value)
    {
        if (index >= 5)
        {
            throw std::runtime_error("index out of range for someNumbers [E105]");
        }

        *((std::uint32_t *)(m_buffer + m_offset + 12 + (index * 4))) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
    }

    std::uint64_t getSomeNumbers(char *dst, const std::uint64_t length) const
    {
        if (length > 5)
        {
             throw std::runtime_error("length too large for getSomeNumbers [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 12, length);
        return length;
    }

    Car &putSomeNumbers(const char *src)
    {
        std::memcpy(m_buffer + m_offset + 12, src, 5);
        return *this;
    }


    static SBE_CONSTEXPR std::uint16_t vehicleCodeId()
    {
        return 6;
    }

    static SBE_CONSTEXPR std::uint64_t vehicleCodeSinceVersion()
    {
         return 0;
    }

    bool vehicleCodeInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= vehicleCodeSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t vehicleCodeEncodingOffset()
    {
         return 32;
    }


    static const char *vehicleCodeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static SBE_CONSTEXPR char vehicleCodeNullValue()
    {
        return (char)0;
    }

    static SBE_CONSTEXPR char vehicleCodeMinValue()
    {
        return (char)32;
    }

    static SBE_CONSTEXPR char vehicleCodeMaxValue()
    {
        return (char)126;
    }

    static SBE_CONSTEXPR std::size_t vehicleCodeEncodingLength()
    {
        return 1;
    }

    static SBE_CONSTEXPR std::uint64_t vehicleCodeLength()
    {
        return 6;
    }

    const char *vehicleCode() const
    {
        return (m_buffer + m_offset + 32);
    }

    char vehicleCode(const std::uint64_t index) const
    {
        if (index >= 6)
        {
            throw std::runtime_error("index out of range for vehicleCode [E104]");
        }

        return (*((char *)(m_buffer + m_offset + 32 + (index * 1))));
    }

    void vehicleCode(const std::uint64_t index, const char value)
    {
        if (index >= 6)
        {
            throw std::runtime_error("index out of range for vehicleCode [E105]");
        }

        *((char *)(m_buffer + m_offset + 32 + (index * 1))) = (value);
    }

    std::uint64_t getVehicleCode(char *dst, const std::uint64_t length) const
    {
        if (length > 6)
        {
             throw std::runtime_error("length too large for getVehicleCode [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 32, length);
        return length;
    }

    Car &putVehicleCode(const char *src)
    {
        std::memcpy(m_buffer + m_offset + 32, src, 6);
        return *this;
    }

    std::string getVehicleCodeAsString() const
    {
        std::string result(m_buffer + m_offset + 32, 6);
        return result;
    }

    Car &putVehicleCode(const std::string& str)
    {
        std::memcpy(m_buffer + m_offset + 32, str.c_str(), 6);
        return *this;
    }


    static SBE_CONSTEXPR std::uint16_t extrasId()
    {
        return 7;
    }

    static SBE_CONSTEXPR std::uint64_t extrasSinceVersion()
    {
         return 0;
    }

    bool extrasInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= extrasSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t extrasEncodingOffset()
    {
         return 38;
    }


    static const char *extrasMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

private:
    OptionalExtras m_extras;

public:

    OptionalExtras &extras()
    {
        m_extras.wrap(m_buffer, m_offset + 38, m_actingVersion, m_bufferLength);
        return m_extras;
    }


    static SBE_CONSTEXPR std::size_t extrasEncodingLength()
    {
        return 1;
    }

    static SBE_CONSTEXPR std::uint16_t discountedModelId()
    {
        return 8;
    }

    static SBE_CONSTEXPR std::uint64_t discountedModelSinceVersion()
    {
         return 0;
    }

    bool discountedModelInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= discountedModelSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t discountedModelEncodingOffset()
    {
         return 39;
    }


    static const char *discountedModelMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    Model::Value discountedModel() const
    {
        return Model::Value::C;
    }


    static SBE_CONSTEXPR std::uint16_t engineId()
    {
        return 9;
    }

    static SBE_CONSTEXPR std::uint64_t engineSinceVersion()
    {
         return 0;
    }

    bool engineInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= engineSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::size_t engineEncodingOffset()
    {
         return 39;
    }


    static const char *engineMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

private:
    Engine m_engine;

public:

    Engine &engine()
    {
        m_engine.wrap(m_buffer, m_offset + 39, m_actingVersion, m_bufferLength);
        return m_engine;
    }

    class FuelFigures
    {
    private:
        char *m_buffer;
        std::uint64_t m_bufferLength;
        std::uint64_t *m_positionPtr;
        std::uint64_t m_blockLength;
        std::uint64_t m_count;
        std::uint64_t m_index;
        std::uint64_t m_offset;
        std::uint64_t m_actingVersion;
        GroupSizeEncoding m_dimensions;

    public:

        inline void wrapForDecode(char *buffer, std::uint64_t *pos, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
        {
            m_buffer = buffer;
            m_bufferLength = bufferLength;
            m_dimensions.wrap(m_buffer, *pos, actingVersion, bufferLength);
            m_blockLength = m_dimensions.blockLength();
            m_count = m_dimensions.numInGroup();
            m_index = -1;
            m_actingVersion = actingVersion;
            m_positionPtr = pos;
            *m_positionPtr = *m_positionPtr + 4;
        }

        inline void wrapForEncode(char *buffer, const std::uint16_t count, std::uint64_t *pos, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
        {
    #if defined(__GNUG__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wtype-limits"
    #endif
            if (count < 0 || count > 65534)
            {
                throw std::runtime_error("count outside of allowed range [E110]");
            }
    #if defined(__GNUG__) && !defined(__clang__)
    #pragma GCC diagnostic pop
    #endif
            m_buffer = buffer;
            m_bufferLength = bufferLength;
            m_dimensions.wrap(m_buffer, *pos, actingVersion, bufferLength);
            m_dimensions.blockLength((std::uint16_t)6);
            m_dimensions.numInGroup((std::uint16_t)count);
            m_index = -1;
            m_count = count;
            m_blockLength = 6;
            m_actingVersion = actingVersion;
            m_positionPtr = pos;
            *m_positionPtr = *m_positionPtr + 4;
        }

        static SBE_CONSTEXPR std::uint64_t sbeHeaderSize()
        {
            return 4;
        }

        static SBE_CONSTEXPR std::uint64_t sbeBlockLength()
        {
            return 6;
        }

        std::uint64_t position() const
        {
            return *m_positionPtr;
        }

        void position(const std::uint64_t position)
        {
            if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false))
            {
                 throw std::runtime_error("buffer too short [E100]");
            }
            *m_positionPtr = position;
        }

        inline std::uint64_t count() const
        {
            return m_count;
        }

        inline bool hasNext() const
        {
            return m_index + 1 < m_count;
        }

        inline FuelFigures &next()
        {
            m_offset = *m_positionPtr;
            if (SBE_BOUNDS_CHECK_EXPECT(( (m_offset + m_blockLength) > m_bufferLength ), false))
            {
                throw std::runtime_error("buffer too short to support next group index [E108]");
            }
            *m_positionPtr = m_offset + m_blockLength;
            ++m_index;

            return *this;
        }

    #if __cplusplus < 201103L
        template<class Func> inline void forEach(Func& func)
        {
            while (hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        template<class Func> inline void forEach(Func&& func)
        {
            while (hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static SBE_CONSTEXPR std::uint16_t speedId()
        {
            return 11;
        }

        static SBE_CONSTEXPR std::uint64_t speedSinceVersion()
        {
             return 0;
        }

        bool speedInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= speedSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t speedEncodingOffset()
        {
             return 0;
        }


        static const char *speedMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint16_t speedNullValue()
        {
            return SBE_NULLVALUE_UINT16;
        }

        static SBE_CONSTEXPR std::uint16_t speedMinValue()
        {
            return (std::uint16_t)0;
        }

        static SBE_CONSTEXPR std::uint16_t speedMaxValue()
        {
            return (std::uint16_t)65534;
        }

        static SBE_CONSTEXPR std::size_t speedEncodingLength()
        {
            return 2;
        }

        std::uint16_t speed() const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + m_offset + 0)));
        }

        FuelFigures &speed(const std::uint16_t value)
        {
            *((std::uint16_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
            return *this;
        }

        static SBE_CONSTEXPR std::uint16_t mpgId()
        {
            return 12;
        }

        static SBE_CONSTEXPR std::uint64_t mpgSinceVersion()
        {
             return 0;
        }

        bool mpgInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= mpgSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t mpgEncodingOffset()
        {
             return 2;
        }


        static const char *mpgMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR float mpgNullValue()
        {
            return SBE_FLOAT_NAN;
        }

        static SBE_CONSTEXPR float mpgMinValue()
        {
            return 1.401298464324817E-45f;
        }

        static SBE_CONSTEXPR float mpgMaxValue()
        {
            return 3.4028234663852886E38f;
        }

        static SBE_CONSTEXPR std::size_t mpgEncodingLength()
        {
            return 4;
        }

        float mpg() const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((float *)(m_buffer + m_offset + 2)));
        }

        FuelFigures &mpg(const float value)
        {
            *((float *)(m_buffer + m_offset + 2)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static const char *usageDescriptionMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static const char *usageDescriptionCharacterEncoding()
        {
            return "UTF-8";
        }

        static SBE_CONSTEXPR std::uint64_t usageDescriptionSinceVersion()
        {
             return 0;
        }

        bool usageDescriptionInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= usageDescriptionSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::uint16_t usageDescriptionId()
        {
            return 200;
        }


        static SBE_CONSTEXPR std::uint64_t usageDescriptionHeaderLength()
        {
            return 4;
        }

        std::uint32_t usageDescriptionLength() const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + position())));
        }

        const char *usageDescription()
        {
             const char *fieldPtr = (m_buffer + position() + 4);
             position(position() + 4 + *((std::uint32_t *)(m_buffer + position())));
             return fieldPtr;
        }

        std::uint64_t getUsageDescription(char *dst, const std::uint64_t length)
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

        FuelFigures &putUsageDescription(const char *src, const std::uint32_t length)
        {
            std::uint64_t lengthOfLengthField = 4;
            std::uint64_t lengthPosition = position();
            position(lengthPosition + lengthOfLengthField);
            *((std::uint32_t *)(m_buffer + lengthPosition)) = SBE_LITTLE_ENDIAN_ENCODE_32(length);
            std::uint64_t pos = position();
            position(position() + length);
            std::memcpy(m_buffer + pos, src, length);
            return *this;
        }

        const std::string getUsageDescriptionAsString()
        {
            std::uint64_t lengthOfLengthField = 4;
            std::uint64_t lengthPosition = position();
            position(lengthPosition + lengthOfLengthField);
            std::uint64_t dataLength = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + lengthPosition)));
            std::uint64_t pos = position();
            const std::string result(m_buffer + pos, dataLength);
            position(position() + dataLength);
            return result;
        }

        FuelFigures &putUsageDescription(const std::string& str)
        {
            if (str.length() > 1073741824)
            {
                 throw std::runtime_error("std::string too long for length type [E109]");
            }
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

private:
    FuelFigures m_fuelFigures;

public:

    static SBE_CONSTEXPR std::uint16_t fuelFiguresId()
    {
        return 10;
    }


    inline FuelFigures &fuelFigures()
    {
        m_fuelFigures.wrapForDecode(m_buffer, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_fuelFigures;
    }

    FuelFigures &fuelFiguresCount(const std::uint16_t count)
    {
        m_fuelFigures.wrapForEncode(m_buffer, count, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_fuelFigures;
    }

    static SBE_CONSTEXPR std::uint64_t fuelFiguresSinceVersion()
    {
         return 0;
    }

    bool fuelFiguresInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= fuelFiguresSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    class PerformanceFigures
    {
    private:
        char *m_buffer;
        std::uint64_t m_bufferLength;
        std::uint64_t *m_positionPtr;
        std::uint64_t m_blockLength;
        std::uint64_t m_count;
        std::uint64_t m_index;
        std::uint64_t m_offset;
        std::uint64_t m_actingVersion;
        GroupSizeEncoding m_dimensions;

    public:

        inline void wrapForDecode(char *buffer, std::uint64_t *pos, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
        {
            m_buffer = buffer;
            m_bufferLength = bufferLength;
            m_dimensions.wrap(m_buffer, *pos, actingVersion, bufferLength);
            m_blockLength = m_dimensions.blockLength();
            m_count = m_dimensions.numInGroup();
            m_index = -1;
            m_actingVersion = actingVersion;
            m_positionPtr = pos;
            *m_positionPtr = *m_positionPtr + 4;
        }

        inline void wrapForEncode(char *buffer, const std::uint16_t count, std::uint64_t *pos, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
        {
    #if defined(__GNUG__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wtype-limits"
    #endif
            if (count < 0 || count > 65534)
            {
                throw std::runtime_error("count outside of allowed range [E110]");
            }
    #if defined(__GNUG__) && !defined(__clang__)
    #pragma GCC diagnostic pop
    #endif
            m_buffer = buffer;
            m_bufferLength = bufferLength;
            m_dimensions.wrap(m_buffer, *pos, actingVersion, bufferLength);
            m_dimensions.blockLength((std::uint16_t)1);
            m_dimensions.numInGroup((std::uint16_t)count);
            m_index = -1;
            m_count = count;
            m_blockLength = 1;
            m_actingVersion = actingVersion;
            m_positionPtr = pos;
            *m_positionPtr = *m_positionPtr + 4;
        }

        static SBE_CONSTEXPR std::uint64_t sbeHeaderSize()
        {
            return 4;
        }

        static SBE_CONSTEXPR std::uint64_t sbeBlockLength()
        {
            return 1;
        }

        std::uint64_t position() const
        {
            return *m_positionPtr;
        }

        void position(const std::uint64_t position)
        {
            if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false))
            {
                 throw std::runtime_error("buffer too short [E100]");
            }
            *m_positionPtr = position;
        }

        inline std::uint64_t count() const
        {
            return m_count;
        }

        inline bool hasNext() const
        {
            return m_index + 1 < m_count;
        }

        inline PerformanceFigures &next()
        {
            m_offset = *m_positionPtr;
            if (SBE_BOUNDS_CHECK_EXPECT(( (m_offset + m_blockLength) > m_bufferLength ), false))
            {
                throw std::runtime_error("buffer too short to support next group index [E108]");
            }
            *m_positionPtr = m_offset + m_blockLength;
            ++m_index;

            return *this;
        }

    #if __cplusplus < 201103L
        template<class Func> inline void forEach(Func& func)
        {
            while (hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        template<class Func> inline void forEach(Func&& func)
        {
            while (hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static SBE_CONSTEXPR std::uint16_t octaneRatingId()
        {
            return 14;
        }

        static SBE_CONSTEXPR std::uint64_t octaneRatingSinceVersion()
        {
             return 0;
        }

        bool octaneRatingInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= octaneRatingSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }

        static SBE_CONSTEXPR std::size_t octaneRatingEncodingOffset()
        {
             return 0;
        }


        static const char *octaneRatingMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "";
            }

            return "";
        }

        static SBE_CONSTEXPR std::uint8_t octaneRatingNullValue()
        {
            return SBE_NULLVALUE_UINT8;
        }

        static SBE_CONSTEXPR std::uint8_t octaneRatingMinValue()
        {
            return (std::uint8_t)90;
        }

        static SBE_CONSTEXPR std::uint8_t octaneRatingMaxValue()
        {
            return (std::uint8_t)110;
        }

        static SBE_CONSTEXPR std::size_t octaneRatingEncodingLength()
        {
            return 1;
        }

        std::uint8_t octaneRating() const
        {
            return (*((std::uint8_t *)(m_buffer + m_offset + 0)));
        }

        PerformanceFigures &octaneRating(const std::uint8_t value)
        {
            *((std::uint8_t *)(m_buffer + m_offset + 0)) = (value);
            return *this;
        }

        class Acceleration
        {
        private:
            char *m_buffer;
            std::uint64_t m_bufferLength;
            std::uint64_t *m_positionPtr;
            std::uint64_t m_blockLength;
            std::uint64_t m_count;
            std::uint64_t m_index;
            std::uint64_t m_offset;
            std::uint64_t m_actingVersion;
            GroupSizeEncoding m_dimensions;

        public:

            inline void wrapForDecode(char *buffer, std::uint64_t *pos, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
            {
                m_buffer = buffer;
                m_bufferLength = bufferLength;
                m_dimensions.wrap(m_buffer, *pos, actingVersion, bufferLength);
                m_blockLength = m_dimensions.blockLength();
                m_count = m_dimensions.numInGroup();
                m_index = -1;
                m_actingVersion = actingVersion;
                m_positionPtr = pos;
                *m_positionPtr = *m_positionPtr + 4;
            }

            inline void wrapForEncode(char *buffer, const std::uint16_t count, std::uint64_t *pos, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
            {
        #if defined(__GNUG__) && !defined(__clang__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wtype-limits"
        #endif
                if (count < 0 || count > 65534)
                {
                    throw std::runtime_error("count outside of allowed range [E110]");
                }
        #if defined(__GNUG__) && !defined(__clang__)
        #pragma GCC diagnostic pop
        #endif
                m_buffer = buffer;
                m_bufferLength = bufferLength;
                m_dimensions.wrap(m_buffer, *pos, actingVersion, bufferLength);
                m_dimensions.blockLength((std::uint16_t)6);
                m_dimensions.numInGroup((std::uint16_t)count);
                m_index = -1;
                m_count = count;
                m_blockLength = 6;
                m_actingVersion = actingVersion;
                m_positionPtr = pos;
                *m_positionPtr = *m_positionPtr + 4;
            }

            static SBE_CONSTEXPR std::uint64_t sbeHeaderSize()
            {
                return 4;
            }

            static SBE_CONSTEXPR std::uint64_t sbeBlockLength()
            {
                return 6;
            }

            std::uint64_t position() const
            {
                return *m_positionPtr;
            }

            void position(const std::uint64_t position)
            {
                if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false))
                {
                     throw std::runtime_error("buffer too short [E100]");
                }
                *m_positionPtr = position;
            }

            inline std::uint64_t count() const
            {
                return m_count;
            }

            inline bool hasNext() const
            {
                return m_index + 1 < m_count;
            }

            inline Acceleration &next()
            {
                m_offset = *m_positionPtr;
                if (SBE_BOUNDS_CHECK_EXPECT(( (m_offset + m_blockLength) > m_bufferLength ), false))
                {
                    throw std::runtime_error("buffer too short to support next group index [E108]");
                }
                *m_positionPtr = m_offset + m_blockLength;
                ++m_index;

                return *this;
            }

        #if __cplusplus < 201103L
            template<class Func> inline void forEach(Func& func)
            {
                while (hasNext())
                {
                    next(); func(*this);
                }
            }

        #else
            template<class Func> inline void forEach(Func&& func)
            {
                while (hasNext())
                {
                    next(); func(*this);
                }
            }

        #endif


            static SBE_CONSTEXPR std::uint16_t mphId()
            {
                return 16;
            }

            static SBE_CONSTEXPR std::uint64_t mphSinceVersion()
            {
                 return 0;
            }

            bool mphInActingVersion()
            {
        #if defined(__clang__)
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wtautological-compare"
        #endif
                return m_actingVersion >= mphSinceVersion();
        #if defined(__clang__)
        #pragma clang diagnostic pop
        #endif
            }

            static SBE_CONSTEXPR std::size_t mphEncodingOffset()
            {
                 return 0;
            }


            static const char *mphMetaAttribute(const MetaAttribute::Attribute metaAttribute)
            {
                switch (metaAttribute)
                {
                    case MetaAttribute::EPOCH: return "unix";
                    case MetaAttribute::TIME_UNIT: return "nanosecond";
                    case MetaAttribute::SEMANTIC_TYPE: return "";
                }

                return "";
            }

            static SBE_CONSTEXPR std::uint16_t mphNullValue()
            {
                return SBE_NULLVALUE_UINT16;
            }

            static SBE_CONSTEXPR std::uint16_t mphMinValue()
            {
                return (std::uint16_t)0;
            }

            static SBE_CONSTEXPR std::uint16_t mphMaxValue()
            {
                return (std::uint16_t)65534;
            }

            static SBE_CONSTEXPR std::size_t mphEncodingLength()
            {
                return 2;
            }

            std::uint16_t mph() const
            {
                return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + m_offset + 0)));
            }

            Acceleration &mph(const std::uint16_t value)
            {
                *((std::uint16_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
                return *this;
            }

            static SBE_CONSTEXPR std::uint16_t secondsId()
            {
                return 17;
            }

            static SBE_CONSTEXPR std::uint64_t secondsSinceVersion()
            {
                 return 0;
            }

            bool secondsInActingVersion()
            {
        #if defined(__clang__)
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wtautological-compare"
        #endif
                return m_actingVersion >= secondsSinceVersion();
        #if defined(__clang__)
        #pragma clang diagnostic pop
        #endif
            }

            static SBE_CONSTEXPR std::size_t secondsEncodingOffset()
            {
                 return 2;
            }


            static const char *secondsMetaAttribute(const MetaAttribute::Attribute metaAttribute)
            {
                switch (metaAttribute)
                {
                    case MetaAttribute::EPOCH: return "unix";
                    case MetaAttribute::TIME_UNIT: return "nanosecond";
                    case MetaAttribute::SEMANTIC_TYPE: return "";
                }

                return "";
            }

            static SBE_CONSTEXPR float secondsNullValue()
            {
                return SBE_FLOAT_NAN;
            }

            static SBE_CONSTEXPR float secondsMinValue()
            {
                return 1.401298464324817E-45f;
            }

            static SBE_CONSTEXPR float secondsMaxValue()
            {
                return 3.4028234663852886E38f;
            }

            static SBE_CONSTEXPR std::size_t secondsEncodingLength()
            {
                return 4;
            }

            float seconds() const
            {
                return SBE_LITTLE_ENDIAN_ENCODE_32(*((float *)(m_buffer + m_offset + 2)));
            }

            Acceleration &seconds(const float value)
            {
                *((float *)(m_buffer + m_offset + 2)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
                return *this;
            }
        };

private:
        Acceleration m_acceleration;

public:

        static SBE_CONSTEXPR std::uint16_t accelerationId()
        {
            return 15;
        }


        inline Acceleration &acceleration()
        {
            m_acceleration.wrapForDecode(m_buffer, m_positionPtr, m_actingVersion, m_bufferLength);
            return m_acceleration;
        }

        Acceleration &accelerationCount(const std::uint16_t count)
        {
            m_acceleration.wrapForEncode(m_buffer, count, m_positionPtr, m_actingVersion, m_bufferLength);
            return m_acceleration;
        }

        static SBE_CONSTEXPR std::uint64_t accelerationSinceVersion()
        {
             return 0;
        }

        bool accelerationInActingVersion()
        {
    #if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wtautological-compare"
    #endif
            return m_actingVersion >= accelerationSinceVersion();
    #if defined(__clang__)
    #pragma clang diagnostic pop
    #endif
        }
    };

private:
    PerformanceFigures m_performanceFigures;

public:

    static SBE_CONSTEXPR std::uint16_t performanceFiguresId()
    {
        return 13;
    }


    inline PerformanceFigures &performanceFigures()
    {
        m_performanceFigures.wrapForDecode(m_buffer, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_performanceFigures;
    }

    PerformanceFigures &performanceFiguresCount(const std::uint16_t count)
    {
        m_performanceFigures.wrapForEncode(m_buffer, count, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_performanceFigures;
    }

    static SBE_CONSTEXPR std::uint64_t performanceFiguresSinceVersion()
    {
         return 0;
    }

    bool performanceFiguresInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= performanceFiguresSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static const char *manufacturerMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const char *manufacturerCharacterEncoding()
    {
        return "UTF-8";
    }

    static SBE_CONSTEXPR std::uint64_t manufacturerSinceVersion()
    {
         return 0;
    }

    bool manufacturerInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= manufacturerSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::uint16_t manufacturerId()
    {
        return 18;
    }


    static SBE_CONSTEXPR std::uint64_t manufacturerHeaderLength()
    {
        return 4;
    }

    std::uint32_t manufacturerLength() const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + position())));
    }

    const char *manufacturer()
    {
         const char *fieldPtr = (m_buffer + position() + 4);
         position(position() + 4 + *((std::uint32_t *)(m_buffer + position())));
         return fieldPtr;
    }

    std::uint64_t getManufacturer(char *dst, const std::uint64_t length)
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

    Car &putManufacturer(const char *src, const std::uint32_t length)
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint32_t *)(m_buffer + lengthPosition)) = SBE_LITTLE_ENDIAN_ENCODE_32(length);
        std::uint64_t pos = position();
        position(position() + length);
        std::memcpy(m_buffer + pos, src, length);
        return *this;
    }

    const std::string getManufacturerAsString()
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + lengthPosition)));
        std::uint64_t pos = position();
        const std::string result(m_buffer + pos, dataLength);
        position(position() + dataLength);
        return result;
    }

    Car &putManufacturer(const std::string& str)
    {
        if (str.length() > 1073741824)
        {
             throw std::runtime_error("std::string too long for length type [E109]");
        }
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint32_t *)(m_buffer + lengthPosition)) = SBE_LITTLE_ENDIAN_ENCODE_32((std::uint32_t)str.length());
        std::uint64_t pos = position();
        position(position() + str.length());
        std::memcpy(m_buffer + pos, str.c_str(), str.length());
        return *this;
    }

    static const char *modelMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const char *modelCharacterEncoding()
    {
        return "UTF-8";
    }

    static SBE_CONSTEXPR std::uint64_t modelSinceVersion()
    {
         return 0;
    }

    bool modelInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= modelSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::uint16_t modelId()
    {
        return 19;
    }


    static SBE_CONSTEXPR std::uint64_t modelHeaderLength()
    {
        return 4;
    }

    std::uint32_t modelLength() const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + position())));
    }

    const char *model()
    {
         const char *fieldPtr = (m_buffer + position() + 4);
         position(position() + 4 + *((std::uint32_t *)(m_buffer + position())));
         return fieldPtr;
    }

    std::uint64_t getModel(char *dst, const std::uint64_t length)
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

    Car &putModel(const char *src, const std::uint32_t length)
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint32_t *)(m_buffer + lengthPosition)) = SBE_LITTLE_ENDIAN_ENCODE_32(length);
        std::uint64_t pos = position();
        position(position() + length);
        std::memcpy(m_buffer + pos, src, length);
        return *this;
    }

    const std::string getModelAsString()
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + lengthPosition)));
        std::uint64_t pos = position();
        const std::string result(m_buffer + pos, dataLength);
        position(position() + dataLength);
        return result;
    }

    Car &putModel(const std::string& str)
    {
        if (str.length() > 1073741824)
        {
             throw std::runtime_error("std::string too long for length type [E109]");
        }
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint32_t *)(m_buffer + lengthPosition)) = SBE_LITTLE_ENDIAN_ENCODE_32((std::uint32_t)str.length());
        std::uint64_t pos = position();
        position(position() + str.length());
        std::memcpy(m_buffer + pos, str.c_str(), str.length());
        return *this;
    }

    static const char *activationCodeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "";
        }

        return "";
    }

    static const char *activationCodeCharacterEncoding()
    {
        return "UTF-8";
    }

    static SBE_CONSTEXPR std::uint64_t activationCodeSinceVersion()
    {
         return 0;
    }

    bool activationCodeInActingVersion()
    {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif
        return m_actingVersion >= activationCodeSinceVersion();
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    }

    static SBE_CONSTEXPR std::uint16_t activationCodeId()
    {
        return 20;
    }


    static SBE_CONSTEXPR std::uint64_t activationCodeHeaderLength()
    {
        return 4;
    }

    std::uint32_t activationCodeLength() const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + position())));
    }

    const char *activationCode()
    {
         const char *fieldPtr = (m_buffer + position() + 4);
         position(position() + 4 + *((std::uint32_t *)(m_buffer + position())));
         return fieldPtr;
    }

    std::uint64_t getActivationCode(char *dst, const std::uint64_t length)
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

    Car &putActivationCode(const char *src, const std::uint32_t length)
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        *((std::uint32_t *)(m_buffer + lengthPosition)) = SBE_LITTLE_ENDIAN_ENCODE_32(length);
        std::uint64_t pos = position();
        position(position() + length);
        std::memcpy(m_buffer + pos, src, length);
        return *this;
    }

    const std::string getActivationCodeAsString()
    {
        std::uint64_t lengthOfLengthField = 4;
        std::uint64_t lengthPosition = position();
        position(lengthPosition + lengthOfLengthField);
        std::uint64_t dataLength = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + lengthPosition)));
        std::uint64_t pos = position();
        const std::string result(m_buffer + pos, dataLength);
        position(position() + dataLength);
        return result;
    }

    Car &putActivationCode(const std::string& str)
    {
        if (str.length() > 1073741824)
        {
             throw std::runtime_error("std::string too long for length type [E109]");
        }
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
