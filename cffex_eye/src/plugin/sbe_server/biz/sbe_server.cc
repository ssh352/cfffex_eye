#include "sbe_server.h"
//sbe header
#include "MessageHeader.h"
#include "GetCar.h"
#include "Car.h"
using namespace example;

char VEHICLE_CODE[] = {'a', 'b', 'c', 'd', 'e', 'f'};
char MANUFACTURER_CODE[] = {'1', '2', '3'};
const char *MANUFACTURER = "Honda";
const char *MODEL = "Civic VTi";
const int messageHeaderVersion = 0;

std::size_t encodeHdr(MessageHeader &hdr, Car &car, char *buffer, std::uint64_t offset, std::uint64_t bufferLength)
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


int sbe_server::assign_request(request_info *req_info)
{
	XLOG(XLOG_DEBUG, "sbe_server::%s \n", __FUNCTION__);
	xp_decoder de;
	de.decode(req_info->data.buff, req_info->data.len);
	de.dump();

	if (de.get_serviceid() == SERVICE_ID_GETCAR)
	{
		MessageHeader hdr;
		GetCar getcar;

		hdr.wrap((char*)de.get_body(), 0, messageHeaderVersion, de.get_body_len());
	    // decode the header
	    XLOG(XLOG_DEBUG, "messageHeader.blockLength=%d\n", hdr.blockLength());
	    XLOG(XLOG_DEBUG, "messageHeader.templateId=%d\n", hdr.templateId());
	    XLOG(XLOG_DEBUG, "messageHeader.schemaId=%d\n", hdr.schemaId());
	    XLOG(XLOG_DEBUG, "messageHeader.schemaVersion=%d\n", hdr.version());
	    XLOG(XLOG_DEBUG, "messageHeader.encodedLength=%lld\n", hdr.encodedLength());

	    // decode getcar
	    getcar.wrapForDecode((char*)de.get_body(), hdr.encodedLength(), hdr.blockLength(), hdr.version(), de.get_body_len());
	    XLOG(XLOG_DEBUG, "getcar.param1=%u\n", getcar.param1());
	    XLOG(XLOG_DEBUG, "getcar.param2=%u\n", getcar.param2());
	    XLOG(XLOG_DEBUG, "getcar.param3=%s\n", getcar.getParam3AsString().c_str());
	    XLOG(XLOG_DEBUG, "getcar.param4=%s\n", getcar.getParam3AsString().c_str());

		en_.init(XP_RESPONSE);
		en_.set_seqno(de.get_seqno());
		en_.set_messageid(1);
		en_.set_resultcode(0);
		en_.set_serviceid(SERVICE_ID_GETCAR);
		en_.set_version(de.get_version());
		en_.set_proto_type(de.get_proto_type());
	    //
	    Car car;
	    en_.get_buffer()->reserve(2048);
		std::size_t encodeHdrLength = encodeHdr(hdr, car, en_.get_buffer()->top(), 0, en_.get_buffer()->capacity());
		std::size_t encodeMsgLength = encodeCar(car, en_.get_buffer()->top(), hdr.encodedLength(), en_.get_buffer()->capacity());
en_.get_buffer()->inc_loc(encodeHdrLength + encodeMsgLength);
		//返回应答
		en_.set_len();  //少一次copy
//		en_.set_buf(buf_car, encodeHdrLength + encodeMsgLength);

		buffer_trans data;
		char *p = new char[en_.get_len()];
		memcpy(p, en_.get_buf(), en_.get_len());
		data.buff = p;
		data.len = en_.get_len();
		data.seqno = de.get_seqno();
		host_->post_respond(data, req_info->session_info);
	}

}


