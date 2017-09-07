/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _EXAMPLE_ENGINE_H_
#define _EXAMPLE_ENGINE_H_

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

#include "BooleanType.h"
#include "Booster.h"
#include "BoostType.h"

using namespace sbe;

namespace example {

class Engine
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t m_offset;
    std::uint64_t m_actingVersion;

    inline void reset(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT(((offset + 10) > bufferLength), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        m_buffer = buffer;
        m_bufferLength = bufferLength;
        m_offset = offset;
        m_actingVersion = actingVersion;
    }

public:
    Engine(void) : m_buffer(nullptr), m_offset(0) {}

    Engine(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    Engine(const Engine& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

#if __cplusplus >= 201103L
    Engine(Engine&& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

    Engine& operator=(Engine&& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

#endif

    Engine& operator=(const Engine& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

    Engine &wrap(char *buffer, const std::uint64_t offset, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const std::uint64_t encodedLength(void)
    {
        return 10;
    }


    static const std::uint16_t capacityNullValue()
    {
        return SBE_NULLVALUE_UINT16;
    }

    static const std::uint16_t capacityMinValue()
    {
        return (std::uint16_t)0;
    }

    static const std::uint16_t capacityMaxValue()
    {
        return (std::uint16_t)65534;
    }

    std::uint16_t capacity(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(m_buffer + m_offset + 0)));
    }

    Engine &capacity(const std::uint16_t value)
    {
        *((std::uint16_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_16(value);
        return *this;
    }

    static const std::uint8_t numCylindersNullValue()
    {
        return SBE_NULLVALUE_UINT8;
    }

    static const std::uint8_t numCylindersMinValue()
    {
        return (std::uint8_t)0;
    }

    static const std::uint8_t numCylindersMaxValue()
    {
        return (std::uint8_t)254;
    }

    std::uint8_t numCylinders(void) const
    {
        return (*((std::uint8_t *)(m_buffer + m_offset + 2)));
    }

    Engine &numCylinders(const std::uint8_t value)
    {
        *((std::uint8_t *)(m_buffer + m_offset + 2)) = (value);
        return *this;
    }

    static const std::uint16_t maxRpmNullValue()
    {
        return SBE_NULLVALUE_UINT16;
    }

    static const std::uint16_t maxRpmMinValue()
    {
        return (std::uint16_t)0;
    }

    static const std::uint16_t maxRpmMaxValue()
    {
        return (std::uint16_t)65534;
    }

    std::uint16_t maxRpm(void) const
    {
        return (std::uint16_t)9000;
    }

    static const char manufacturerCodeNullValue()
    {
        return (char)0;
    }

    static const char manufacturerCodeMinValue()
    {
        return (char)32;
    }

    static const char manufacturerCodeMaxValue()
    {
        return (char)126;
    }

    static const std::uint64_t manufacturerCodeLength(void)
    {
        return 3;
    }

    const char *manufacturerCode(void) const
    {
        return (m_buffer + m_offset + 3);
    }

    char manufacturerCode(const std::uint64_t index) const
    {
        if (index >= 3)
        {
            throw std::runtime_error("index out of range for manufacturerCode [E104]");
        }

        return (*((char *)(m_buffer + m_offset + 3 + (index * 1))));
    }

    void manufacturerCode(const std::uint64_t index, const char value)
    {
        if (index >= 3)
        {
            throw std::runtime_error("index out of range for manufacturerCode [E105]");
        }

        *((char *)(m_buffer + m_offset + 3 + (index * 1))) = (value);
    }

    std::uint64_t getManufacturerCode(char *dst, const std::uint64_t length) const
    {
        if (length > 3)
        {
             throw std::runtime_error("length too large for getManufacturerCode [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 3, length);
        return length;
    }

    Engine &putManufacturerCode(const char *src)
    {
        std::memcpy(m_buffer + m_offset + 3, src, 3);
        return *this;
    }

    std::string getManufacturerCodeAsString() const
    {
        std::string result(m_buffer + m_offset + 3, 3);
        return std::move(result);
    }

    Engine &putManufacturerCode(const std::string& str)
    {
        std::memcpy(m_buffer + m_offset + 3, str.c_str(), 3);
        return *this;
    }


    static const char fuelNullValue()
    {
        return (char)0;
    }

    static const char fuelMinValue()
    {
        return (char)32;
    }

    static const char fuelMaxValue()
    {
        return (char)126;
    }

    static const std::uint64_t fuelLength(void)
    {
        return 6;
    }

    const char *fuel(void) const
    {
        static sbe_uint8_t fuelValues[] = {80, 101, 116, 114, 111, 108};

        return (const char *)fuelValues;
    }

    char fuel(const std::uint64_t index) const
    {
        static sbe_uint8_t fuelValues[] = {80, 101, 116, 114, 111, 108};

        return fuelValues[index];
    }

    std::uint64_t getFuel(char *dst, const std::uint64_t length) const
    {
        static sbe_uint8_t fuelValues[] = {80, 101, 116, 114, 111, 108};
        std::uint64_t bytesToCopy = (length < sizeof(fuelValues)) ? length : sizeof(fuelValues);

        std::memcpy(dst, fuelValues, bytesToCopy);
        return bytesToCopy;
    }

    static const std::int8_t percentageNullValue()
    {
        return SBE_NULLVALUE_INT8;
    }

    static const std::int8_t percentageMinValue()
    {
        return (std::int8_t)0;
    }

    static const std::int8_t percentageMaxValue()
    {
        return (std::int8_t)100;
    }

    std::int8_t percentage(void) const
    {
        return (*((std::int8_t *)(m_buffer + m_offset + 6)));
    }

    Engine &percentage(const std::int8_t value)
    {
        *((std::int8_t *)(m_buffer + m_offset + 6)) = (value);
        return *this;
    }

    BooleanType::Value booleanType(void) const
    {
        return BooleanType::get((*((std::uint8_t *)(m_buffer + m_offset + 7))));
    }

    Engine &booleanType(const BooleanType::Value value)
    {
        *((std::uint8_t *)(m_buffer + m_offset + 7)) = (value);
        return *this;
    }

private:
    Booster m_booster;

public:

    Booster &booster(void)
    {
        m_booster.wrap(m_buffer, m_offset + 8, m_actingVersion, m_bufferLength);
        return m_booster;
    }
};
}
#endif
