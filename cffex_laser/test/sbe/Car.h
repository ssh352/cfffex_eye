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

    Car(void) : m_buffer(nullptr), m_bufferLength(0), m_offset(0) {}

    Car(char *buffer, const std::uint64_t bufferLength)
    {
        reset(buffer, 0, bufferLength, sbeBlockLength(), sbeSchemaVersion());
    }

    Car(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    Car(const Car& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
    }

#if __cplusplus >= 201103L
    Car(Car&& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
    }

    Car& operator=(Car&& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
        return *this;
    }

#endif

    Car& operator=(const Car& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
        return *this;
    }

    static const std::uint16_t sbeBlockLength(void)
    {
        return (std::uint16_t)49;
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

    static const std::uint16_t serialNumberId(void)
    {
        return 1;
    }

    static const std::uint64_t serialNumberSinceVersion(void)
    {
         return 0;
    }

    bool serialNumberInActingVersion(void)
    {
        return (m_actingVersion >= serialNumberSinceVersion()) ? true : false;
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

    static const std::uint64_t serialNumberNullValue()
    {
        return SBE_NULLVALUE_UINT64;
    }

    static const std::uint64_t serialNumberMinValue()
    {
        return 0x0L;
    }

    static const std::uint64_t serialNumberMaxValue()
    {
        return 0xfffffffffffffffeL;
    }

    std::uint64_t serialNumber(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((std::uint64_t *)(m_buffer + m_offset + 0)));
    }

    Car &serialNumber(const std::uint64_t value)
    {
        *((std::uint64_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const std::uint16_t modelYearId(void)
    {
        return 2;
    }

    static const std::uint64_t modelYearSinceVersion(void)
    {
         return 0;
    }

    bool modelYearInActingVersion(void)
    {
        return (m_actingVersion >= modelYearSinceVersion()) ? true : false;
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

    static const std::uint16_t modelYearNullValue()
    {
        return SBE_NULLVALUE_UINT16;
    }

    static const std::uint16_t modelYearMinValue()
    {
        return (std::uint16_t)0;
    }

    static const std::uint16_t modelYearMaxValue()
    {
        return (std::uint16_t)65534;
    }

    std::uint16_t modelYear(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + m_offset + 8)));
    }

    Car &modelYear(const std::uint16_t value)
    {
        *((std::uint16_t *)(m_buffer + m_offset + 8)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }

    static const std::uint16_t availableId(void)
    {
        return 3;
    }

    static const std::uint64_t availableSinceVersion(void)
    {
         return 0;
    }

    bool availableInActingVersion(void)
    {
        return (m_actingVersion >= availableSinceVersion()) ? true : false;
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

    BooleanType::Value available(void) const
    {
        return BooleanType::get((*((std::uint8_t *)(m_buffer + m_offset + 10))));
    }

    Car &available(const BooleanType::Value value)
    {
        *((std::uint8_t *)(m_buffer + m_offset + 10)) = (value);
        return *this;
    }

    static const std::uint16_t codeId(void)
    {
        return 4;
    }

    static const std::uint64_t codeSinceVersion(void)
    {
         return 0;
    }

    bool codeInActingVersion(void)
    {
        return (m_actingVersion >= codeSinceVersion()) ? true : false;
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

    Model::Value code(void) const
    {
        return Model::get((*((char *)(m_buffer + m_offset + 11))));
    }

    Car &code(const Model::Value value)
    {
        *((char *)(m_buffer + m_offset + 11)) = (value);
        return *this;
    }

    static const std::uint16_t someNumbersId(void)
    {
        return 5;
    }

    static const std::uint64_t someNumbersSinceVersion(void)
    {
         return 0;
    }

    bool someNumbersInActingVersion(void)
    {
        return (m_actingVersion >= someNumbersSinceVersion()) ? true : false;
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

    static const std::uint32_t someNumbersNullValue()
    {
        return SBE_NULLVALUE_UINT32;
    }

    static const std::uint32_t someNumbersMinValue()
    {
        return 0;
    }

    static const std::uint32_t someNumbersMaxValue()
    {
        return 4294967293;
    }

    static const std::uint64_t someNumbersLength(void)
    {
        return 5;
    }

    const char *someNumbers(void) const
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


    static const std::uint16_t vehicleCodeId(void)
    {
        return 6;
    }

    static const std::uint64_t vehicleCodeSinceVersion(void)
    {
         return 0;
    }

    bool vehicleCodeInActingVersion(void)
    {
        return (m_actingVersion >= vehicleCodeSinceVersion()) ? true : false;
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

    static const char vehicleCodeNullValue()
    {
        return (char)0;
    }

    static const char vehicleCodeMinValue()
    {
        return (char)32;
    }

    static const char vehicleCodeMaxValue()
    {
        return (char)126;
    }

    static const std::uint64_t vehicleCodeLength(void)
    {
        return 6;
    }

    const char *vehicleCode(void) const
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
        return std::move(result);
    }

    Car &putVehicleCode(const std::string& str)
    {
        std::memcpy(m_buffer + m_offset + 32, str.c_str(), 6);
        return *this;
    }


    static const std::uint16_t extrasId(void)
    {
        return 7;
    }

    static const std::uint64_t extrasSinceVersion(void)
    {
         return 0;
    }

    bool extrasInActingVersion(void)
    {
        return (m_actingVersion >= extrasSinceVersion()) ? true : false;
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

    static const std::uint16_t discountedModelId(void)
    {
        return 8;
    }

    static const std::uint64_t discountedModelSinceVersion(void)
    {
         return 0;
    }

    bool discountedModelInActingVersion(void)
    {
        return (m_actingVersion >= discountedModelSinceVersion()) ? true : false;
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

    Model::Value discountedModel(void) const
    {
        return Model::Value::C;
    }


    static const std::uint16_t engineId(void)
    {
        return 9;
    }

    static const std::uint64_t engineSinceVersion(void)
    {
         return 0;
    }

    bool engineInActingVersion(void)
    {
        return (m_actingVersion >= engineSinceVersion()) ? true : false;
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

    Engine &engine(void)
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

        static const std::uint64_t sbeHeaderSize()
        {
            return 4;
        }

        static const std::uint64_t sbeBlockLength()
        {
            return 6;
        }

        std::uint64_t position(void) const
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

        inline std::uint64_t count(void) const
        {
            return m_count;
        }

        inline bool hasNext(void) const
        {
            return m_index + 1 < m_count;
        }

        inline FuelFigures &next(void)
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
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        inline void forEach(std::function<void(FuelFigures&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static const std::uint16_t speedId(void)
        {
            return 11;
        }

        static const std::uint64_t speedSinceVersion(void)
        {
             return 0;
        }

        bool speedInActingVersion(void)
        {
            return (m_actingVersion >= speedSinceVersion()) ? true : false;
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

        static const std::uint16_t speedNullValue()
        {
            return SBE_NULLVALUE_UINT16;
        }

        static const std::uint16_t speedMinValue()
        {
            return (std::uint16_t)0;
        }

        static const std::uint16_t speedMaxValue()
        {
            return (std::uint16_t)65534;
        }

        std::uint16_t speed(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + m_offset + 0)));
        }

        FuelFigures &speed(const std::uint16_t value)
        {
            *((std::uint16_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
            return *this;
        }

        static const std::uint16_t mpgId(void)
        {
            return 12;
        }

        static const std::uint64_t mpgSinceVersion(void)
        {
             return 0;
        }

        bool mpgInActingVersion(void)
        {
            return (m_actingVersion >= mpgSinceVersion()) ? true : false;
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

        static const float mpgNullValue()
        {
            return SBE_FLOAT_NAN;
        }

        static const float mpgMinValue()
        {
            return 1.401298464324817E-45f;
        }

        static const float mpgMaxValue()
        {
            return 3.4028234663852886E38f;
        }

        float mpg(void) const
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

        static const std::uint64_t usageDescriptionSinceVersion(void)
        {
             return 0;
        }

        bool usageDescriptionInActingVersion(void)
        {
            return (m_actingVersion >= usageDescriptionSinceVersion()) ? true : false;
        }

        static const std::uint16_t usageDescriptionId(void)
        {
            return 200;
        }


        static const std::uint64_t usageDescriptionHeaderSize()
        {
            return 4;
        }

        std::uint32_t usageDescriptionLength(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + position())));
        }

        const char *usageDescription(void)
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

        std::uint64_t putUsageDescription(const char *src, const std::uint64_t length)
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

        const std::string getUsageDescriptionAsString()
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

        FuelFigures &putUsageDescription(const std::string& str)
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

private:
    FuelFigures m_fuelFigures;

public:

    static const std::uint16_t fuelFiguresId(void)
    {
        return 10;
    }


    inline FuelFigures &fuelFigures(void)
    {
        m_fuelFigures.wrapForDecode(m_buffer, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_fuelFigures;
    }

    FuelFigures &fuelFiguresCount(const std::uint16_t count)
    {
        m_fuelFigures.wrapForEncode(m_buffer, count, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_fuelFigures;
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

        static const std::uint64_t sbeHeaderSize()
        {
            return 4;
        }

        static const std::uint64_t sbeBlockLength()
        {
            return 1;
        }

        std::uint64_t position(void) const
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

        inline std::uint64_t count(void) const
        {
            return m_count;
        }

        inline bool hasNext(void) const
        {
            return m_index + 1 < m_count;
        }

        inline PerformanceFigures &next(void)
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
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        inline void forEach(std::function<void(PerformanceFigures&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static const std::uint16_t octaneRatingId(void)
        {
            return 14;
        }

        static const std::uint64_t octaneRatingSinceVersion(void)
        {
             return 0;
        }

        bool octaneRatingInActingVersion(void)
        {
            return (m_actingVersion >= octaneRatingSinceVersion()) ? true : false;
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

        static const std::uint8_t octaneRatingNullValue()
        {
            return SBE_NULLVALUE_UINT8;
        }

        static const std::uint8_t octaneRatingMinValue()
        {
            return (std::uint8_t)90;
        }

        static const std::uint8_t octaneRatingMaxValue()
        {
            return (std::uint8_t)110;
        }

        std::uint8_t octaneRating(void) const
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

            static const std::uint64_t sbeHeaderSize()
            {
                return 4;
            }

            static const std::uint64_t sbeBlockLength()
            {
                return 6;
            }

            std::uint64_t position(void) const
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

            inline std::uint64_t count(void) const
            {
                return m_count;
            }

            inline bool hasNext(void) const
            {
                return m_index + 1 < m_count;
            }

            inline Acceleration &next(void)
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
                while(hasNext())
                {
                    next(); func(*this);
                }
            }

        #else
            inline void forEach(std::function<void(Acceleration&)> func)
            {
                while(hasNext())
                {
                    next(); func(*this);
                }
            }

        #endif


            static const std::uint16_t mphId(void)
            {
                return 16;
            }

            static const std::uint64_t mphSinceVersion(void)
            {
                 return 0;
            }

            bool mphInActingVersion(void)
            {
                return (m_actingVersion >= mphSinceVersion()) ? true : false;
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

            static const std::uint16_t mphNullValue()
            {
                return SBE_NULLVALUE_UINT16;
            }

            static const std::uint16_t mphMinValue()
            {
                return (std::uint16_t)0;
            }

            static const std::uint16_t mphMaxValue()
            {
                return (std::uint16_t)65534;
            }

            std::uint16_t mph(void) const
            {
                return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + m_offset + 0)));
            }

            Acceleration &mph(const std::uint16_t value)
            {
                *((std::uint16_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
                return *this;
            }

            static const std::uint16_t secondsId(void)
            {
                return 17;
            }

            static const std::uint64_t secondsSinceVersion(void)
            {
                 return 0;
            }

            bool secondsInActingVersion(void)
            {
                return (m_actingVersion >= secondsSinceVersion()) ? true : false;
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

            static const float secondsNullValue()
            {
                return SBE_FLOAT_NAN;
            }

            static const float secondsMinValue()
            {
                return 1.401298464324817E-45f;
            }

            static const float secondsMaxValue()
            {
                return 3.4028234663852886E38f;
            }

            float seconds(void) const
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

        static const std::uint16_t accelerationId(void)
        {
            return 15;
        }


        inline Acceleration &acceleration(void)
        {
            m_acceleration.wrapForDecode(m_buffer, m_positionPtr, m_actingVersion, m_bufferLength);
            return m_acceleration;
        }

        Acceleration &accelerationCount(const std::uint16_t count)
        {
            m_acceleration.wrapForEncode(m_buffer, count, m_positionPtr, m_actingVersion, m_bufferLength);
            return m_acceleration;
        }
    };

private:
    PerformanceFigures m_performanceFigures;

public:

    static const std::uint16_t performanceFiguresId(void)
    {
        return 13;
    }


    inline PerformanceFigures &performanceFigures(void)
    {
        m_performanceFigures.wrapForDecode(m_buffer, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_performanceFigures;
    }

    PerformanceFigures &performanceFiguresCount(const std::uint16_t count)
    {
        m_performanceFigures.wrapForEncode(m_buffer, count, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_performanceFigures;
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

    static const std::uint64_t manufacturerSinceVersion(void)
    {
         return 0;
    }

    bool manufacturerInActingVersion(void)
    {
        return (m_actingVersion >= manufacturerSinceVersion()) ? true : false;
    }

    static const std::uint16_t manufacturerId(void)
    {
        return 18;
    }


    static const std::uint64_t manufacturerHeaderSize()
    {
        return 4;
    }

    std::uint32_t manufacturerLength(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + position())));
    }

    const char *manufacturer(void)
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

    std::uint64_t putManufacturer(const char *src, const std::uint64_t length)
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

    const std::string getManufacturerAsString()
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

    Car &putManufacturer(const std::string& str)
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

    static const std::uint64_t modelSinceVersion(void)
    {
         return 0;
    }

    bool modelInActingVersion(void)
    {
        return (m_actingVersion >= modelSinceVersion()) ? true : false;
    }

    static const std::uint16_t modelId(void)
    {
        return 19;
    }


    static const std::uint64_t modelHeaderSize()
    {
        return 4;
    }

    std::uint32_t modelLength(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + position())));
    }

    const char *model(void)
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

    std::uint64_t putModel(const char *src, const std::uint64_t length)
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

    const std::string getModelAsString()
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

    Car &putModel(const std::string& str)
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

    static const std::uint64_t activationCodeSinceVersion(void)
    {
         return 0;
    }

    bool activationCodeInActingVersion(void)
    {
        return (m_actingVersion >= activationCodeSinceVersion()) ? true : false;
    }

    static const std::uint16_t activationCodeId(void)
    {
        return 20;
    }


    static const std::uint64_t activationCodeHeaderSize()
    {
        return 4;
    }

    std::uint32_t activationCodeLength(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + position())));
    }

    const char *activationCode(void)
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

    std::uint64_t putActivationCode(const char *src, const std::uint64_t length)
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

    const std::string getActivationCodeAsString()
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

    Car &putActivationCode(const std::string& str)
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
