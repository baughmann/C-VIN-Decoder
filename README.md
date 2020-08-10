# 🚗 C++ VIN Decoder / Validator 🚙
A simple C++ library for validation and basic decoding of Vehicle Identification Numbers

### Features
- Easily validate VIN numbers using the "security" check digit
- Quickly get basic information about a vehicle"
    - Country of Origin
    - Name of Manufacturer
    - Serial Number


### Installation
The easiest way to use this library is by simply including it as a Git submodule in your project.
```
git submodule add https://github.com/baughmann/C-Vin-Decoder ThirdParty/vindec
git submodule init
git isubmodule sync
git submodule update
```
Then, in your `CMakeLists.txt` file:
```
add_subdirectory(ThirdParty/vindec)
include_directories(ThirdParty/vindec/include)
```

### Usage
#### Decoding a VIN
Returns a `VinDecodeResponse`. The `isValid` property will tell you if teh VIN is valid and if the `manufacturer`, `country`, and `serialNumber` fields are populated.
```
#include <VinDecoder.h>

int main() {
    auto decoder = VinDecoder();
    auto vehicleInfo = decoder.decode("WMWMS335X9TY38985");

    if(vehicleInfo->isValid) { // true
        std::cout << vehicleInfo->country << std::endl; // Germany
        std::cout << vehicleInfo->manufacturer << std::endl; // MINI
        std::cout << vehicleInfo->serialNumber << std::endl; // T
    }
}
```
`VinDecodeResponse` looks like this:
```
struct VinDecodeResponse {
    std::string manufacturer;
    std::string country;
    std::string serialNumber;
    bool isValid;
};
```
### Validating a VIN
Returns `true` if the VIN is valid and `false` if it isn't.
```
#include <VinDecoder.h>

int main() {
    auto decoder = VinDecoder();
    auto isValid = decoder.validate("WMWMS335X9TY38985");
    std::cout << isValid << std::endl; // true
}
```

### Purpose
This is my very first completed C++ project. As such, this was done primarily to assist in learning C++ as well as the CMake build system.

That said, ***please, Please, PLEASE*** give me some feedback! I know I have probably implemented a lot of stuff *very* poorly, and I would really like to learn to do it better.

Please be gentle, and thank you in advance! 👍