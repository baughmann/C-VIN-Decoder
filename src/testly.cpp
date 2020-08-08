//
// Created by Nick Baughman on 8/8/20.
//
#include "Decoder.h"

int main() {
    auto dec = Decoder();
    auto thing = dec.decode("1C3CCCAB7FN515728");

//    thing->isValid;
    std::cout << thing->manufacturer;
}