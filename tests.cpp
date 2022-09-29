#include "catch.hpp"
#include <iostream>
#include "DShashtable.h"
#include <vector>
#include <string>

TEST_CASE("HashTable class", "[DShashtable]") {
    DShashtable<int, std::string> map1;
    DShashtable<int, std::string> map2(map1);

    SECTION("Insert method") {
        map1.insert(0, "zero"); //adds a node to an empty table
        REQUIRE(map1.getSize() == 1);
        map1.insert(1, "one"); //adds a node to a non-empty table
        REQUIRE(map1.getSize() == 2);
        map1.insert(0, "0"); //adds data to a current key
        REQUIRE(map1.getSize() == 2);
        map1.insert(1001, "1001"); //adds data to a non-empty bucket
        REQUIRE(map1.getSize() == 3);
    }

    SECTION("Clear method") {
        map2.insert(0, "zero");
        map2.insert(1, "one");
        map2.clear(); //tests clear of a non-empty map
        REQUIRE(map2.getSize() == 0);

        map2.insert(0, "zero");
        map2.insert(0, "0");
        map2.clear(); //tests clear of a non-empty map with a chained bucket
        REQUIRE(map2.getSize() == 0);

        map1.clear(); //tests clear of an empty map
        REQUIRE(map1.getSize() == 0);
    }

    SECTION("[] operator") {
        std::vector<std::string> testVec = {"zero", "0"};

        map2.insert(0, "zero");
        map2.insert(0, "0");
        REQUIRE(map2[0] == testVec);
        map2.insert(1, "one");
        testVec = {"one"};
        REQUIRE(map2[1] == testVec);
        REQUIRE(map2[5].empty()); //confirms that searching for a key not in map will return an empty vector
    }

    SECTION("= operator") {
        map1.insert(5, "five");
        map1.insert(7, "seven");
        map2 = map1; //tests with left-hand map empty
        REQUIRE(map1.getSize() == map2.getSize());
        REQUIRE(map1[5] == map2[5]);

        map1.clear();
        map2 = map1; //tests with right-hand map empty
        REQUIRE(map2[7] == map1[7]);
        REQUIRE(map2[5] == map1[5]);

        DShashtable<int, std::string> cappedMap(15);
        cappedMap = map1; //tests with maps of different capacities
        REQUIRE(cappedMap.getSize() == map1.getSize());
    }
}