#include <gtest/gtest.h>
#include <../include/VinDecoder.h>

TEST(VinDecoderTest, ValidateGoodVIN) {
    auto decoder = VinDecoder();
    
    auto goodValidation = decoder.validate("WMWMS335X9TY38985");
    ASSERT_TRUE(goodValidation);
};

TEST(VinDecoderTest, DecodeGoodVIN) {
    auto decoder = VinDecoder();

    auto goodResult = *decoder.decode("WMWMS335X9TY38985");
    ASSERT_TRUE(goodResult.country == "Germany");
    ASSERT_TRUE(goodResult.manufacturer == "MINI");
    ASSERT_TRUE(goodResult.serialNumber == "T");
};

TEST(VinDecoderTest, ValidateGoodTooShortVIN) {
    auto decoder = VinDecoder();

    auto tooShortValidation = decoder.validate("WMWMS335X9TY3898");
    ASSERT_FALSE(tooShortValidation);
};

TEST(VinDecoderTest, ValidateMistypedVIN) {
    auto decoder = VinDecoder();

    auto invalidSumValidation = decoder.decode("WMWMS335X8TY38985");
    ASSERT_FALSE(invalidSumValidation);
};

TEST(VinDecoderTest, DecodeBadVIN) {
    auto decoder = VinDecoder();

    auto badResult = decoder.decode("WMWMS335X8TY38985");
    ASSERT_TRUE(badResult == nullptr);
};