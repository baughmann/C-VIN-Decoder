#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>

struct DecodeResponse {
    std::string manufacturer;
    std::string country;
    std::string serialNumber;
};

struct RawInfo {
    std::string country;
    std::string manufacturer;
    std::string details;
    std::string checkDigit;
    std::string year;
    std::string assemblyPlant;
    std::string serialNumber;
};

class Decoder {
  private:

    // TODO: we can definitely do more here for US VINS, but EU VINS are the wild west. should we do more?
    template<typename T>
    RawInfo getData(const T &_vin) {
        static const int MADE_IN_START = 0;
        static const int MADE_IN_SIZE = 2;
        static const int MANUFATURER_START = 0;
        static const int MANUFACTURER_SIZE = 3;
        static const int DETAILS_START = 3;
        static const int DETAILS_SIZE = 5;
        static const int CHECK_DIGIT = 8;
        static const int YEAR = 9;
        static const int ASSEMBLY_PLANT = 10;
        static const int SERIAL_NUMBER = 10;

        auto vin = std::string(_vin);

        auto rawInfo = RawInfo();
        rawInfo.country = vin.substr(MADE_IN_START, MADE_IN_SIZE);
        rawInfo.manufacturer = vin.substr(MANUFATURER_START, MANUFACTURER_SIZE);
        rawInfo.details = vin.substr(DETAILS_START, DETAILS_SIZE);
        rawInfo.checkDigit = vin.substr(CHECK_DIGIT, 1);
        rawInfo.year = vin.substr(YEAR, 1);
        rawInfo.assemblyPlant = vin.substr(ASSEMBLY_PLANT, 1);
        rawInfo.serialNumber = vin.substr(SERIAL_NUMBER, 1);

        return rawInfo;
    }

    std::string getValue(const char *key, const char *filename) {
        std::ifstream infile(filename);
        std::string line;
        while (std::getline(infile, line)) {
            auto eqPos = line.find('=');
            auto k = line.substr(0, eqPos);
            if (key == k) {
                return line.substr(eqPos + 1);
            }
        }

        std::cout << "Failed to find value for " << key << " in file " << filename <<std::endl;

        return nullptr;
    }

    std::string getCountry(const char *countryCode) {
        return getValue(countryCode, "../resources/countries.txt");
    }

    std::string getManufaturer(const char *manufacturerCode) {
        return getValue(manufacturerCode, "../resources/manufacturers.txt");
    }

    // TODO: well, there's DEFINITELY a better way to do this crap...
    static int translitChar(const char character) {
        static const std::array<std::string, 9> letterGroups{
                "AB", "BKS", "CTL",
                "DMU", "ENV", "FW",
                "GPX", "HY", "RZ"
        };

        // handle converting letter chars to ints
        int i = 1;
        for (const auto &letterGroup :letterGroups) {
            if (letterGroup.find(character) != std::string::npos) {
                return i;
            }
            i++;
        }
        // handle converting numeric chars to ints
        int charAsInt = character - 48;
        if (charAsInt > -1 && charAsInt < 10)
            return charAsInt;
        // well, we tried
        return -1;
    }


  public:
    /**
     * Gets some basic vehicle information about the vehicle whose VIN you're checking.
     *
     * @param vin The VIN you wish to check.
     * @return A `DecodeResponse` if the VIN is valid, and a `nullptr` if the VIN is invalid.
     */
    template<typename T>
    DecodeResponse *decode(const T &vin) {
        if (!validate(vin)) {
            return nullptr;
        }

        RawInfo data = getData(vin);
        // TODO: Do we *really* want to heap alloc this?
        auto result = new DecodeResponse();
        result->serialNumber = data.serialNumber;
        result->manufacturer = getManufaturer(data.manufacturer.data());
        result->country = getCountry(data.country.data());

        return result;
    }

    /**
     * Checks whether or not the VIN appears to be a valid VIN.
     *
     * @param _vin The VIN you wish to check.
     * @return A boolean indicating whether or not the vin is valid.
     */
    // Implementation of: https://en.wikibooks.org/wiki/Vehicle_Identification_Numbers_(VIN_codes)/Check_digit
    // Inspired by: https://gist.github.com/ubergesundheit/5626679
    //  and https://github.com/frankely/vin-decoder/blob/master/index.js
    template<typename T>
    bool validate(const T &_vin) {
        auto vin = std::string(_vin);

        if (vin.size() != 17) {
            return false;
        }
        // use static to avoid redefining
        static int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 0, 7, 0, 9, 2, 3, 4, 5, 6, 7, 8, 9};
        static int weights[] = {8, 7, 6, 5, 4, 3, 2, 10, 0, 9, 8, 7, 6, 5, 4, 3, 2};

        int sum = 0;
        for (int i = 0; i < 17; i++) {
            int value;
            auto character = vin[i];

            // handle letters
            if (character > ('A' - 1) && character < ('Z' + 1)) {
                value = values[character - 'A'];
                // don't allow 0
                if (!value) {
                    return false;
                }
            }
                // handle numbers
            else if (character > ('0' - 1) && character < ('9' + 1)) {
                value = character - '0';
            }
                // handle bad characters
            else {
                return false;
            }
            sum = sum + weights[i] * value;
        }

        // find the divisor
        sum = sum % 11;
        auto checkDigit = vin[8];

        if ((sum == 10 && checkDigit == 'X')
            || (sum == translitChar(checkDigit)))
            return true;
        else
            return false;
    }
};