# Adhan C

Adhan C is written to be compatible with embedded systems and microcontrollers.

## Usage

### Compile course

To compile this project you need access to internet since it needs to download googletest sources.

```
cd C/adhan/
./build.sh
```

This will build the library in the C/adhan/build directory.

### Run unit tests

```
./C/adhan/build/test/runUnitTests
```

### Run example

```
./C/adhan/build/src/example
````

### Include the library in your project

You need to add "C/adhan/build/src/libadhan.a" to your project. See example in "C/adhan/src/example.c".