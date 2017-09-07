#include "sbe_test.h"



#if defined(WIN32) || defined(_WIN32)
#    define snprintf _snprintf
#endif /* WIN32 */

char VEHICLE_CODE[] = {'a', 'b', 'c', 'd', 'e', 'f'};
char MANUFACTURER_CODE[] = {'1', '2', '3'};
const char *MANUFACTURER = "Honda";
const char *MODEL = "Civic VTi";
const int messageHeaderVersion = 0;

std::size_t encodeHdrCar(MessageHeader &hdr, Car &car, char *buffer, std::uint64_t offset, std::uint64_t bufferLength)
{
    // encode the header
    hdr.wrap(buffer, offset, messageHeaderVersion, bufferLength)
       .blockLength(Car::sbeBlockLength())
       .templateId(Car::sbeTemplateId())
       .schemaId(Car::sbeSchemaId())
       .version(Car::sbeSchemaVersion());

    return hdr.encodedLength();
}

std::size_t encodeCar(Car &car, char *buffer, std::uint64_t offset, std::uint64_t bufferLength)
{
    car.wrapForEncode(buffer, offset, bufferLength)
       .serialNumber(1234)
       .modelYear(65535)
       .available(BooleanType::T)
       .code(Model::A)
       .putVehicleCode(VEHICLE_CODE);

    for (std::uint64_t i = 0, size = car.someNumbersLength(); i < size; i++)
    {
        car.someNumbers(i, static_cast<std::int32_t>(i));
    }

    car.extras().clear()
       .cruiseControl(true)
       .sportsPack(true)
       .sunRoof(false);

    car.engine()
       .capacity(2000)
       .numCylinders((short)4)
       .putManufacturerCode(MANUFACTURER_CODE)
       .booster().boostType(BoostType::NITROUS).horsePower(200);

    Car::FuelFigures& fuelFigures = car.fuelFiguresCount(3);

    fuelFigures
        .next().speed(30).mpg(35.9f)
        .putUsageDescription("Urban Cycle", 11);

    fuelFigures
        .next().speed(55).mpg(49.0f)
        .putUsageDescription("Combined Cycle", 14);

    fuelFigures
        .next().speed(75).mpg(40.0f)
        .putUsageDescription("Highway Cycle", 13);

    Car::PerformanceFigures &performanceFigures = car.performanceFiguresCount(2);

    performanceFigures.next()
        .octaneRating((short)95)
        .accelerationCount(3)
            .next().mph(30).seconds(4.0f)
            .next().mph(60).seconds(7.5f)
            .next().mph(100).seconds(12.2f);

    performanceFigures.next()
        .octaneRating((short)99)
        .accelerationCount(3)
            .next().mph(30).seconds(3.8f)
            .next().mph(60).seconds(7.1f)
            .next().mph(100).seconds(11.8f);

    car.putManufacturer(MANUFACTURER)
        .putModel(MODEL)
        .putActivationCode("deadbeef");

    return car.encodedLength();
}
