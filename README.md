# Common Data Representation
Data structure used within Neueda Front-office-SDK as a common message format for all protocols.

## Compilation

Uses Cmake

```bash
$ git submodule update --init --recurisve
$ mkdir build
$ cd build
$ cmake -DTESTS=ON -DCOVERAGE=ON ../
$ make VERBOSE=1 install |& tee compiler.log
$ make package
```

## Profiling

Requires Valgrind. To install using Ubunutu: 

```bash
$ sudo apt-get update
$ sudo apt-get install valgrind
```

Valgrind docs can be found here: http://valgrind.org/docs/
Once installed use the following commands to create

```bash
$ cd build
$ export LD_LIBRARY_PATH=`pwd`/install/lib
$ valgrind --xml=yes --xml-file=valgrind_report.xml -- ./install/bin/unittest --gtest_output=xml:../test.xml
```

## Static Analysis

Requires Vera++ and cppcheck. To install using Ubunutu: 

```bash
$ sudo apt-get update
$ sudo apt-get install vera++
$ sudo apt-get install cppcheck
```

Once installed use the following commands to retrieve detailed reports highlighting likely bugs, code smells and code coverage.
If you have your own vera profile rememeber to add "-p profile-name" to the command. 

```bash
$ cppcheck -f --enable=all --xml --suppress=missingIncludeSystem ./src/ 2> ./cppcheck_report.xml
$ vera++ -c vera.xml `find ./src/ -regextype posix-egrep -regex ".*\\.(cc|cpp|h|hpp)"`
$ gcovr -r ./ -x > coverage_report.xml
```

## Build with Bindings for Java, C# and Python

Requires swig, python-dev, java, mono-mcs:

```bash
$ cmake -DTESTS=ON -DSWIG=ON -DCOVERAGE=ON -DPYTHON=ON -DJAVA=ON -DCSHARP=ON ../
$ make VERBOSE=1 install |& tee compiler.log
```

### Test Python Bindings

Run example:

```bash
$ cd build/install/lib/python
$ LD_LIBRARY_PATH=../:$LD_LIBRARY_PATH
$ python example.py
```

### Test Java Bindings:

Run example:

```bash
$ cd build/install/lib/java
$ sh build.sh
$ sh run.sh
```

### Test C# bindings:

```bash
$ cd install/lib/csharp
$ sh build.sh
$ sh run.sh
```
