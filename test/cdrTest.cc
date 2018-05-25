/*
 * Copyright 2014-2018 Neueda Ltd.
 */
#include <gtest/gtest.h>
#include <iostream>

#include "cdr.h"

using namespace neueda;

TEST (CDR, UPDATE)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setDouble   (2, 100.1);
    d.setInteger  (3, 100);

    cdr x;
    ASSERT_EQ (x.size(), 0);

    x.update( d );
    ASSERT_EQ (x.size(), 3);
}

TEST (CDR, CLEAR)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setDouble   (2, 100.1);
    d.setInteger  (3, 100);

    ASSERT_EQ (d.size(), 3);
    d.clear();
    ASSERT_EQ (d.size(), 0);
}

TEST (CDR, CONTAINS)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setDouble   (2, 100.1);
    d.setInteger  (3, 100);

    ASSERT_TRUE (d.contains (3));
}

TEST (CDR, CONTAINS_FAILURE)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setDouble   (2, 100.1);
    d.setInteger  (3, 100);

    ASSERT_FALSE (d.contains (4));
}

TEST (CDR, IS_TYPE)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setDouble   (2, 100.1);
    d.setInteger  (3, 100);

    ASSERT_TRUE (d.isType (3, CDR_INTEGER, sizeof (CDR_INTEGER)));
}

TEST (CDR, IS_NOT_TYPE)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setDouble   (2, 100.1);
    d.setInteger  (3, 100);

    ASSERT_FALSE (d.isType (3, CDR_DOUBLE, sizeof (CDR_DOUBLE)));
}

TEST (CDR, IS_TYPE_STR)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setString   (2, "100.1");
    d.setInteger  (3, 100);

    ASSERT_TRUE (d.isType (2, CDR_STRING, 5));
}

TEST (CDR, IS_NOT_TYPE_STR)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setString   (2, "101");
    d.setInteger  (3, 100);

    ASSERT_FALSE (d.isType (2, CDR_STRING, 5));
}

TEST (CDR, DELETE_ITEM)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setDouble   (2, 100.1);
    d.setInteger  (3, 100);

    ASSERT_TRUE (d.contains(3));

    d.deleteItem(3);
    ASSERT_FALSE (d.contains(3));
}

TEST (CDR, SIZE)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setDouble   (2, 100.1);
    d.setInteger  (3, 100);

    ASSERT_EQ (d.size(), 3);
}

TEST (CDR, OPERATOR_EQUAL)
{
    cdr d;
    d.setDouble   (1, 100.111111);
    d.setDouble   (2, 100.1);
    d.setInteger  (3, 100);

    cdr newCdr = d;

    double v1;
    double v2;

    d.getDouble( 1, v1 );
    newCdr.getDouble( 1, v2 );

    ASSERT_EQ( v1, v2 );
}

TEST (CDR, GET_DOUBLE_SUCCESS)
{
    cdr d;
    d.setDouble  (1, 100.111111);

    double v1;
    ASSERT_TRUE (d.getDouble (1, v1));
}

TEST (CDR, GET_DOUBLE_STR_SUCCESS)
{
    cdr d;
    d.setString  (1, "100.111111");

    double v1;
    ASSERT_TRUE (d.getDouble (1, v1));
}

TEST (CDR, GET_DOUBLE_EMPTY_STR_FAILURE)
{
    cdr d;
    d.setString  (1, "  ");

    double v1;
    ASSERT_FALSE (d.getDouble (1, v1));
}

TEST (CDR, GET_DOUBLE_NULL_FAILURE)
{
    cdr d;

    double v1;
    ASSERT_FALSE (d.getDouble (1, v1));
}

TEST (CDR, GET_DOUBLE_STR_FAILURE)
{
    cdr d;
    d.setString  (1, "EEE");

    double v1;
    ASSERT_FALSE (d.getDouble (1, v1));
}

TEST (CDR, GET_INTEGER_STR_SUCCESS)
{
    cdr d;
    d.setString  (1, "100");

    uint8_t v1;
    ASSERT_TRUE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_EMPTY_STR_FAILURE)
{
    cdr d;
    d.setString  (1, "  ");

    uint8_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_NULL_FAILURE)
{
    cdr d;

    uint8_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_STR_FAILURE)
{
    cdr d;
    d.setString  (1, "EEE");

    uint8_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_8_SUCCESS)
{
    cdr d;
    d.setInteger (1, 100);

    uint8_t v1;
    ASSERT_TRUE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_8_TYPE_FAILURE_SMALL)
{
    cdr d;
    d.setInteger (1, (INT16_MIN));

    uint8_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_8_TYPE_FAILURE_LARGE)
{
    cdr d;
    d.setInteger (1, (INT16_MAX));

    uint8_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_8_KEY_FAILURE)
{
    cdr d;
    d.setInteger (1, -100);

    uint8_t v1;
    ASSERT_FALSE (d.getInteger (2, v1));
}

TEST (CDR, GET_INTEGER_8)
{
    cdr d;
    d.setInteger (1, 100);


    int8_t v1;
    d.getInteger (1, v1);
    ASSERT_EQ (v1, 100);
}

TEST (CDR, GET_INTEGER_U8)
{
    cdr d;
    d.setInteger (1, 100);


    uint8_t v2;
    d.getInteger (1, v2);
    ASSERT_EQ (v2, (u_int)100);
}

TEST (CDR, GET_INTEGER_16_SUCCESS)
{
    cdr d;
    d.setInteger (1, 100);

    uint16_t v1;
    ASSERT_TRUE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_16_TYPE_FAILURE)
{
    cdr d;
    d.setInteger (1, -100);

    uint16_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_16_KEY_FAILURE)
{
    cdr d;
    d.setInteger (1, -100);

    uint16_t v1;
    ASSERT_FALSE (d.getInteger (2, v1));
}

TEST (CDR, GET_INTEGER_16_TYPE_FAILURE_SMALL)
{
    cdr d;
    d.setInteger (1, (INT32_MIN));

    uint16_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_16_TYPE_FAILURE_LARGE)
{
    cdr d;
    d.setInteger (1, (INT32_MAX));

    uint16_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_16)
{
    cdr d;
    d.setInteger (1, 100);


    int16_t v1;
    d.getInteger (1, v1);
    ASSERT_EQ (v1, 100);
}

TEST (CDR, GET_INTEGER_U16)
{
    cdr d;
    d.setInteger (1, 100);


    uint16_t v4;
    d.getInteger (1, v4);
    ASSERT_EQ (v4, (u_int)100);
}

TEST (CDR, GET_INTEGER_32_SUCCESS)
{
    cdr d;
    d.setInteger (1, 100);

    uint32_t v1;
    ASSERT_TRUE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_32_TYPE_FAILURE)
{
    cdr d;
    d.setInteger (1, -100);

    uint32_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_32_KEY_FAILURE)
{
    cdr d;
    d.setInteger (1, -100);

    uint32_t v1;
    ASSERT_FALSE  (d.getInteger (2, v1));
}

TEST (CDR, GET_INTEGER_32_TYPE_FAILURE_SMALL)
{
    cdr d;
    d.setInteger (1, (INT64_MIN));

    uint32_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_32_TYPE_FAILURE_LARGE)
{
    cdr d;
    d.setInteger (1, (INT64_MAX));

    uint32_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_32)
{
    cdr d;
    d.setInteger (1, 100);

    int32_t v2;
    d.getInteger (1, v2);
    ASSERT_EQ (v2, 100);
}

TEST (CDR, GET_INTEGER_U32)
{
    cdr d;
    d.setInteger (1, 100);

    int32_t v2;
    d.getInteger (1, v2);
    ASSERT_EQ (v2, 100);

}

TEST (CDR, GET_INTEGER_64_SUCCESS)
{
    cdr d;
    d.setInteger (1, 100);

    uint64_t v1;
    ASSERT_TRUE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_64_TYPE_FAILURE)
{
    cdr d;
    d.setInteger (1, -100);

    uint64_t v1;
    ASSERT_FALSE (d.getInteger (1, v1));
}

TEST (CDR, GET_INTEGER_64_KEY_FAILURE)
{
    cdr d;
    d.setInteger (1, -100);

    uint64_t v1;
    ASSERT_FALSE (d.getInteger (2, v1));
}

TEST (CDR, GET_INTEGER_64)
{
    cdr d;
    d.setInteger (1, 100);

    int64_t v3;
    d.getInteger (1, v3);
    ASSERT_EQ (v3, 100);
}

TEST (CDR, GET_INTEGER_U64)
{
    cdr d;
    d.setInteger (1, 100);


    uint64_t v6;
    d.getInteger (1, v6);
    ASSERT_EQ (v6, (u_int)100);

    //TODO split into seperate cases for specific byte sizes
}

TEST (CDR, COPY)
{
    cdr d;
    d.setString (1, "someData");

    cdr e (d);
    std::string val;
    e.getString (1, val);
    ASSERT_EQ (val, "someData");
}

TEST (CDR, COPY_NESTED)
{
    //cdr* d = new cdr ();
    //cdr* e = new cdr ();
    cdr d;
    cdr e;

    d.setString (1, "someData");
    e.setString (2, "nestedData");

    d.appendArray (2, e);

    cdr f (d);

    const cdrArray* arr;
    ASSERT_TRUE (f.getArray (2, &arr));

    std::string val;
    arr->at (0).getString (2, val);
    ASSERT_EQ (val, "nestedData");

    //switched to using the stack rather than the heap for memory efficency
}


TEST (CDR, TO_STRING)
{
    cdr d;
    d.setString (1, "SomeData");
    std::string val = d.toString ();
    ASSERT_STREQ (val.c_str (), "1=SomeData");
}

TEST (CDR, TO_STRING_2)
{
    cdr d;
    d.setString (1, "SomeData");
    d.setString (2, "SomeData2");

    std::string val = d.toString ();
    ASSERT_STREQ (val.c_str (), "1=SomeData,2=SomeData2");
}


//TODO MORE TO_STRING METHODS

TEST (CDR, FIND_IN_ARRAY)
{
    cdr d;
    d.setString (1, "someData");
    d.setString (2, "someData2");


    std::vector<const cdrItem*> items = d.findAll (2);
    ASSERT_EQ (items.size (), 1);

    std::string value;
    bool ok = items[0]->asString (value);
    ASSERT_TRUE (ok);
    ASSERT_STREQ (value.c_str (), "someData2");

}

TEST (CDR, FIND_IN_ARRAY_INT)
{
    cdr d;
    d.setInteger (1, 10);
    d.setInteger (2, 20);

    std::vector<const cdrItem *> items = d.findAll (1);
    ASSERT_EQ (items.size (), 1);

    std::string value;
    bool ok = items[0]->asString (value);
    ASSERT_TRUE (ok);
    ASSERT_EQ (value, "10");

}

TEST (CDR, FIND_IN_ARRAY_DOUBLE)
{
    cdr d;
    d.setDouble (1, 10.23);
    d.setDouble (2, 10.0);

    std::vector<const cdrItem *> items = d.findAll (2);
    ASSERT_EQ (items.size (), 1);

    std::string value;
    bool ok = items[0]->asString (value);
    ASSERT_TRUE (ok);
    ASSERT_EQ (value, "10.000000");

}

TEST (CDR, FIND_IN_ARRAY_DATETIME)
{
    cdrDateTime dt;
    dt.mHour = 1;
    dt.mMinute = 25;
    dt.mSecond = 21;
    dt.mDay = 1;
    dt.mMonth = 2;
    dt.mYear = 2021;

    cdr d;
    d.setDateTime (1, dt );
    std::vector<const cdrItem *> items = d.findAll(1);

    ASSERT_EQ (items.size (), 1);

    std::string value;
    bool ok = items[0]->asString (value);
    ASSERT_TRUE (ok);
    ASSERT_EQ (value, "2021-02-01 01:25:21.0000");
}
