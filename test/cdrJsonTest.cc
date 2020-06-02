/*
 * Copyright 2014-2018 Neueda Ltd.
 */
#include <gtest/gtest.h>
#include <iostream>

#include "cdr.h"

using namespace neueda;

TEST (CDR_JSON, TO_JSON)
{
    cdr d;
    d.setInteger (1, 100);
    d.setDouble (2, 100.1);
    d.setString (3, "100");

    string j;
    string err;
    ASSERT_TRUE (d.toJson (j, err));

    Json::Reader parser;
    Json::Value  obj;

    ASSERT_TRUE (parser.parse (j, obj));

    ASSERT_TRUE (obj["1"].isInt ());
    ASSERT_TRUE (obj["2"].isDouble ());
    ASSERT_TRUE (obj["3"].isString ());

    ASSERT_EQ (obj["1"], 100);
    ASSERT_EQ (obj["2"], 100.1);
    ASSERT_EQ (obj["3"], "100");
}

TEST (CDR_JSON, FROM_JSON)
{
    string json = "{\"1\":[{\"11\": \"CP\",\"12\":\"YOURE\"}]}";

    cdr d;
    string err;
    ASSERT_TRUE (d.fromJson (json, err));

    string j;
    d.toJson (j, err);

    Json::Reader parser;
    Json::Value  orig;
    Json::Value  gen;

    ASSERT_TRUE (parser.parse (json, orig));
    ASSERT_TRUE (parser.parse (j, gen));

    ASSERT_EQ (gen, orig);
}
