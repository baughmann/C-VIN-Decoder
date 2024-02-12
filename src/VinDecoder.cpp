#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <Resources.h>

struct VinDecodeResponse {
    std::string manufacturer;
    std::string country;
    std::string serialNumber;
    bool isValid;
};

struct VinRawInfo {
    std::string country;
    std::string manufacturer;
    std::string details;
    std::string checkDigit;
    std::string year;
    std::string assemblyPlant;
    std::string serialNumber;
};

class VinDecoder {
private:

    // TODO: we can definitely do more here for US VINS, but EU VINS are the wild west. should we do more?
    template<typename T>
    VinRawInfo getData(const T &_vin) {
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

        auto rawInfo = VinRawInfo();
        rawInfo.country = vin.substr(MADE_IN_START, MADE_IN_SIZE);
        rawInfo.manufacturer = vin.substr(MANUFATURER_START, MANUFACTURER_SIZE);
        rawInfo.details = vin.substr(DETAILS_START, DETAILS_SIZE);
        rawInfo.checkDigit = vin.substr(CHECK_DIGIT, 1);
        rawInfo.year = vin.substr(YEAR, 1);
        rawInfo.assemblyPlant = vin.substr(ASSEMBLY_PLANT, 1);
        rawInfo.serialNumber = vin.substr(SERIAL_NUMBER, 1);

        return rawInfo;
    }

    template<typename T>
    std::string getValue(const char *key, const T& lines) {
        for (std::string line : lines) {
            auto eqPos = line.find('=');
            auto k = line.substr(0, eqPos);
            if(k == key) {
                return line.substr(eqPos + 1);
            }
        }
        return std::string();
    }

    std::string getCountry(const char *countryCode) {
        return getValue(countryCode, countries);
    }

    std::string getManufaturer(const char *manufacturerCode) {
        return getValue(manufacturerCode, manufacturers);
    }

    // TODO: This doesn't appear to be working right....
    int getYear(const char *_vin) {
        static const int years[] ={
            1980, 1981, 1982, 1983, 1984, 1985, 1986, 1987, 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996,
            1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013,
            2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026, 2027, 2028, 2029, 2030,
            2031, 2032, 2033, 2034, 2035, 2036, 2037, 2038, 2039
        };
        std::string values = "ABCDEFGHJKLMNPRSTVWXY123456789ABCDEFGHJKLMNPRSTVWXY123456789";

        auto pos7AsNum = _vin[7] - 48;
        auto pos7IsNum = pos7AsNum > -1 && pos7AsNum < 10;

        std::cout << "what is it? " << _vin[10] << " is " << values.find_first_of(_vin[10]) << "\n";

        size_t i = 0;
        // check
        if (!pos7IsNum) {
            i = values.find_last_of(_vin[10]);
        }
        else {
            i = values.find_first_of(_vin[10]);
        }

        return years[i];
    }

    // TODO: well, there's DEFINITELY a better way to do this crap...
    static int translitChar(const char character) {
        // handle converting numeric chars to ints
        int charAsInt = character - 48;
        if (charAsInt > -1 && charAsInt < 10) {
            return charAsInt;
        }

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
        // well, we tried
        return -1;
    }


public:
    /**
     * Gets some basic vehicle information about the vehicle whose VIN you're checking.
     *
     * @param vin The VIN you wish to check.
     * @return A pointer to `VinDecodeResponse` containing the properties `isValid`, `manufacturer`, `serialNumber` and `country`
     */
    template<typename T>
    VinDecodeResponse *decode(const T &vin) {
        // TODO: Do we *really* want to heap alloc this?
        auto result = new VinDecodeResponse();

        if (!validate(vin)) {
            result->isValid = false;
            return result;
        }

        VinRawInfo data = getData(vin);
        result->serialNumber = data.serialNumber;
        result->manufacturer = getManufaturer(data.manufacturer.data());
        result->country = getCountry(data.country.data());
        result->isValid = true;
        //        result->year = getYear(vin); // not reliable for now

        return result;
    }

    // Implementation of: https://en.wikibooks.org/wiki/Vehicle_Identification_Numbers_(VIN_codes)/Check_digit
    // Inspired by: https://gist.github.com/ubergesundheit/5626679
    //  and https://github.com/frankely/vin-decoder/blob/master/index.js
    /**
     * Checks whether or not the VIN appears to be a valid VIN.
     *
     * @param _vin The VIN you wish to check.
     * @return A boolean indicating whether or not the vin is valid.
     */
    template<typename T>
    bool validate(const T &_vin) {
        auto vin = std::string(_vin);

        if (vin.size() != 17) {
            return false;
        }
        // use static to avoid redefining
        static const int values[] ={ 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 0, 7, 0, 9, 2, 3, 4, 5, 6, 7, 8, 9 };
        static const int weights[] ={ 8, 7, 6, 5, 4, 3, 2, 10, 0, 9, 8, 7, 6, 5, 4, 3, 2 };

        int sum = 0;
        for (int i = 0; i < 17; i++) {
            int value;
            auto character = vin[i];

            // handle letters
            if (character >('A' - 1) && character < ('Z' + 1)) {
                value = values[character - 'A'];
                // don't allow 0
                if (!value) {
                    return false;
                }
            }
            // handle numbers
            else if (character >('0' - 1) && character < ('9' + 1)) {
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