#include <gtest/gtest.h>
#include <iostream>

#include "cdr/cdr.h"

using namespace CommonDataRepresentation;

TEST(CDR, GET_INTEGER)
{
    cdr* d = new cdr ();
    d->setInteger (1, 100);

    int16_t v1;
    d->getInteger (1, v1);
    ASSERT_EQ (v1, 100);

    int32_t v2;
    d->getInteger (1, v2);
    ASSERT_EQ (v2, 100);

    int64_t v3;
    d->getInteger (1, v3);
    ASSERT_EQ (v3, 100);

    uint16_t v4;
    d->getInteger (1, v4);
    ASSERT_EQ (v4, (u_int)100);

    uint32_t v5;
    d->getInteger (1, v5);
    ASSERT_EQ (v5, (u_int)100);

    uint64_t v6;
    d->getInteger (1, v6);
    ASSERT_EQ (v6, (u_int)100);
}

TEST(CDR, COPY)
{
    cdr* d = new cdr ();
    d->setString (1, "someData");

    cdr* e = new cdr (*d);
    std::string val;
    e->getString (1, val);
    ASSERT_EQ (val, "someData");
}

TEST(CDR, COPY_NESTED)
{
    cdr* d = new cdr ();
    cdr* e = new cdr ();

    d->setString (1, "someData");
    e->setString (2, "nestedData");

    d->appendArray(2, *e);

    cdr* f = new cdr (*d);

    const cdrArray* arr;
    ASSERT_TRUE (f->getArray (2, &arr));

    std::string val;
    arr->at(0).getString (2, val);
    ASSERT_EQ (val, "nestedData");
}
