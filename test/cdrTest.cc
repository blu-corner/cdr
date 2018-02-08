#include <gtest/gtest.h>
#include <iostream>

#include "cdr/cdr.h"

using namespace CommonDataRepresentation;

TEST(CDR, GET_INTEGER_16)
{
    cdr d;
    d.setInteger (1, 100);


    int16_t v1;
    d.getInteger (1, v1);
    ASSERT_EQ (v1, 100);


    uint16_t v4;
    d.getInteger (1, v4);
    ASSERT_EQ (v4, (u_int)100);
}

TEST(CDR, GET_INTEGER_32)
{
    cdr d;
    d.setInteger (1, 100);

    int32_t v2;
    d.getInteger (1, v2);
    ASSERT_EQ (v2, 100);

    uint32_t v5;
    d.getInteger (1, v5);
    ASSERT_EQ (v5, (u_int)100);

}
TEST(CDR, GET_INTEGER_64)
{
    cdr d;
    d.setInteger (1, 100);

    int64_t v3;
    d.getInteger (1, v3);
    ASSERT_EQ (v3, 100);

    uint64_t v6;
    d.getInteger (1, v6);
    ASSERT_EQ (v6, (u_int)100);

    //TODO split into seperate cases for specific byte sizes
}

TEST(CDR, COPY)
{
    cdr d;
    d.setString (1, "someData");

    cdr e (d);
    std::string val;
    e.getString (1, val);
    ASSERT_EQ (val, "someData");
}

TEST(CDR, COPY_NESTED)
{
    //cdr* d = new cdr ();
    //cdr* e = new cdr ();
    cdr d;
    cdr e;

    d.setString (1, "someData");
    e.setString (2, "nestedData");

    d.appendArray(2, e);

    cdr f(d);

    const cdrArray* arr;
    ASSERT_TRUE (f.getArray (2, &arr));

    std::string val;
    arr->at(0).getString (2, val);
    ASSERT_EQ (val, "nestedData");

    //switched to using the stack rather than the heap for memory efficency
}


TEST(CDR, TO_STRING)
{
    cdr d;
    d.setString(1, "SomeData");
    std::string val = d.toString ();
    ASSERT_STREQ(val.c_str (), "1=SomeData");
}

TEST(CDR, TO_STRING_2)
{
    cdr d;
    d.setString(1, "SomeData");
    d.setString(2, "SomeData2");

    std::string val = d.toString ();
    ASSERT_STREQ(val.c_str (), "1=SomeData,2=SomeData2");
}


//TODO MORE TO_STRING METHODS

TEST(CDR, FIND_IN_ARRAY)
{
    cdr d;
    d.setString(1, "someData");
    d.setString(2, "someData2");


    std::vector<const cdrItem*> items = d.findAll(2);
    ASSERT_EQ(items.size(), 1);

    std::string value;
    bool ok = items[0]->asString(value);
    ASSERT_TRUE(ok);
    ASSERT_STREQ(value.c_str (), "someData2");

}

