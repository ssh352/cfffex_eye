#ifndef SBE_TEAT_H
#define SBE_TEAT_H

#include <iostream>
#include <string>

#include <stdio.h>
#include <inttypes.h>
#include <iomanip>

#include "sbe/MessageHeader.h"
#include "sbe/Car.h"

using namespace std;
using namespace example;

std::size_t encodeHdrCar(MessageHeader &hdr, Car &car, char *buffer, std::uint64_t offset, std::uint64_t bufferLength);

std::size_t encodeCar(Car &car, char *buffer, std::uint64_t offset, std::uint64_t bufferLength);

/*
    char buffer[2048];
    MessageHeader hdr;
    Car car;

    std::size_t encodeHdrLength = encodeHdr(hdr, car, buffer, 0, sizeof(buffer));
    std::size_t encodeMsgLength = encodeCar(car, buffer, hdr.encodedLength(), sizeof(buffer));

    cout << "Encoded Lengths are " << encodeHdrLength << " + " << encodeMsgLength << endl;
*/

#endif // SBE_TEAT_H
