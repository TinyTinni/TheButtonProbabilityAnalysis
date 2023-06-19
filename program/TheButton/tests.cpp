#include "catch.hpp"
#include "NumberExtractor.hpp"

#define TEST_IMAGE_PATH SOURCE_DIR"/images/"
#define CMP_IMGS_PATH SOURCE_DIR"/cmp/"

TEST_CASE("Images")
{
    Analyzer a(CMP_IMGS_PATH);
    SECTION("0")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_0.png") == 0);
    }
    SECTION("1")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_1.png") == 1);
    }
    SECTION("2")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_2.png") == 2);
    }
    SECTION("3")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_3.png") == 3);
    }
    SECTION("4")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_4.png") == 4);
    }
    SECTION("5")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_5.png") == 5);
    }
    SECTION("6")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_6.png") == 6);
    }
    SECTION("7")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_7.png") == 7);
    }
    SECTION("8")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_8.png") == 8);
    }
    SECTION("9")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_9.png") == 9);
    }
    SECTION("10")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_10.png") == 10);
    }
    SECTION("12")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_12.png") == 12);
    }
    SECTION("17")
    {
        REQUIRE(a.number(TEST_IMAGE_PATH "example_17.png") == 17);
    }
}
