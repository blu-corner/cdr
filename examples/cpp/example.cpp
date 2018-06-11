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
        cout << "integer: " << intVal << endl;

    string val;
    if (d.getString (2, val))
        cout << "string: " << val << endl;

    cout << "cdr: " << d.toString () << endl;
}
