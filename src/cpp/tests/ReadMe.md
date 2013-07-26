## For the impatient

Run `make test` in the Whisper/src/cpp directory

## Getting started

* Read up on [CATCH](https://github.com/philsquared/Catch/)
* Tests are organized as `${filename}.test.cpp`
* Tests run twice, once with valgrind, once without
* Everything must pass, and valgrind can't detect memory issues
* Try to use CHECK over REQUIRE (for valgrind)