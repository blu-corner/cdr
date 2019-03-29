# cdr (Common Data Representation) [![Build Status](https://travis-ci.com/blu-corner/cdr.svg?branch=master)](https://travis-ci.com/blu-corner/cdr) [![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=blu-corner_cdr&metric=alert_status)](https://sonarcloud.io/dashboard?id=blu-corner_cdr)


cdr is a data structure for storage of heterogenous values:

* String
* Double
* Integer
* Datetime
* Array of CDRs (nested CDRs)

It is used within the Front-OfficeSDK as a common object format for all protocols.

## Getting Started

To compile the installation:

```bash
$ git submodule update --init --recursive
$ mkdir build
$ cd build
$ cmake -DTESTS=ON ../
$ make
$ make install
```

Language bindings can be enabled by passing -DJAVA=on, -DPYTHON=on
to CMake. It is possible to build all bindings in the same build.

### Dependencies

The only external dependency is SWIG, and is only required when building the
Java, C# or Python bindings. For information on installing SWIG please visit the
[SWIG website](http://www.swig.org). All other dependencies are managed through 
git submodules.

### Example Usage

The following code displays example usage of the CDR.

```cpp
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
```

Examples have been provided in each language within the [examples folder](./examples/).

* [C++](examples/cpp/README.md)
* [Java](examples/java/README.md)
* [Python](examples/python/README.md)

## Running the Tests

To run the unit tests:

```bash
$ make test
```
