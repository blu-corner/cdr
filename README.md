# Common Data Representation
Data structure used within Neueda Front-office-SDK as a common message format for all protocols.

## Compilation

Uses Cmake

```bash
$ git submodule update --init --recurisve
$ mkdir build
$ cd build
$ cmake -DTESTS=ON ./
$ make install
```

## Bindings

Requires swig, python-dev, java, mono-mcs:

```bash
$ cmake -DTESTS=ON -DSWIG=ON ../
$ make install
```

### Python

Run example:

```bash
$ cd build/install/lib/python
$ LD_LIBRARY_PATH=../:$LD_LIBRARY_PATH python example.py
```

### Java

Run example:

```bash
$ cd build/install/lib/java
$ sh build.sh
$ sh run.sh
```

