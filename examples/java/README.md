# cdr Java Example

An example app showing use of the various features within the Cdr from Java. To
build the application:

```bash
$ mvn -Dinstall.dir=$INSTALL_PREFIX package
```

Where INSTALL_PREFIX points to the absolute path of the installation. To
execute the jar:

```bash
$ java -cp $INSTALL_PREFIX/lib/java/CdrJNI.jar:`pwd`/target/example-1.0-SNAPSHOT.jar com.neueda.cdrexample.App
```

View the source [here](src/main/java/com/neueda/cdrexample/App.java).
