#include <gtest/gtest.h>
#include <../include/VinDecoder.h>

// try to validate a valid VIN
TEST(VinDecoderTest, ValidateGoodVIN) {
    auto decoder = VinDecoder();
    
    auto goodValidation = decoder.validate("WMWMS335X9TY38985");
    ASSERT_TRUE(goodValidation);
};

// Try to decode a valid VIN
TEST(VinDecoderTest, DecodeGoodVIN) {
    auto decoder = VinDecoder();

    auto goodResult = decoder.decode("WMWMS335X9TY38985");
    ASSERT_TRUE(goodResult->country == "Germany");
    ASSERT_TRUE(goodResult->manufacturer == "MINI");
    ASSERT_TRUE(goodResult->serialNumber == "T");
};

// Try a vin that is shorter than 17 characters
TEST(VinDecoderTest, ValidateTooShortVIN) {
    auto decoder = VinDecoder();

    auto tooShortValidation = decoder.validate("WMWMS335X9TY3898");
    ASSERT_FALSE(tooShortValidation);
};

// Try a vin that is longer than 17 characters
TEST(VinDecoderTest, ValidateTooLongVIN) {
    auto decoder = VinDecoder();

    auto tooShortValidation = decoder.validate("WMWMS335X9TY389850");
    ASSERT_FALSE(tooShortValidation);
};

// Try a VIN whose sum is off
TEST(VinDecoderTest, ValidateMistypedVIN) {
    auto decoder = VinDecoder();

    auto invalidSumValidation = decoder.validate("WMWMS335X8TY38985");
    ASSERT_FALSE(invalidSumValidation);
};

// Try to decode a VIN whose sum is off
TEST(VinDecoderTest, DecodeBadVIN) {
    auto decoder = VinDecoder();

    auto badResult = decoder.decode("WMWMS335X8TY38985");
    ASSERT_FALSE(badResult->isValid);

    ASSERT_TRUE(badResult->country == "");
    ASSERT_TRUE(badResult->manufacturer == "");
    ASSERT_TRUE(badResult->serialNumber == "");
};