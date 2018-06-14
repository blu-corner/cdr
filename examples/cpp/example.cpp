/*
 * Copyright 2014-2018 Neueda Ltd.
 *
 * C++ Example displaying numerous features within the CDR. To build the example
 * run 'make' and execute with './example'. 
 *
 * If the install directory is not at
 * the standard location change the INSTALL_PREFIX value within the Makefile to
 * point at the install location.
 *
 */
#include "cdr.h"
#include <stdint.h>
#include <iostream>

using namespace std;
using namespace neueda;

int main ()
{
    cdr d;
    d.setInteger (1, 100);
    d.setString (2, "hello");

    int intVal;
    if (d.getInteger (1, intVal))
        cout << "get integer: " << intVal << endl;

    string val;
    if (d.getString (1, val))
        cout << "get integer as string: " << val << endl;

    if (d.getString (2, val))
        cout << "get string: " << val << endl;

    if (!d.getInteger (2, intVal))
        cout << "retrieve integer from non-numeric string" << endl;

    if (!d.getString (3, val))
        cout << "retrieve non existant key" << endl;

    cdr e;
    e.setString (1, "nested");
    e.setString (2, "cdr");
    d.appendArray (3, e);

    cdrArray* f;
    d.getArray (3, (const cdrArray**)&f);
    cout << "nested-cdr: " << f->at (0).toString () << endl;

    cout << "cdr.toString(): " << d.toString () << endl;
}
