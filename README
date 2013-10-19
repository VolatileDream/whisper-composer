# Whisper-Composer #


Whisper Composer is a dynamic audio arrangement system based on the idea that it won't be any good at composing pieces of music, and instead intends to excel at combining many small compositions into a larger narrative.


### Getting Started

Whisper Composer is written mostly in python, with some c/c++ to interface with audio systems ( using [Port Audio][1] ).

#### Building

First checkout Whisper Composer
> git clone https://github.com/VolatileDream/whisper-composer.git

##### Dependencies

Then you'll need to make sure (at minimum) that you have [Port Audio][1] and [Python][2] installed. They are the minimum required to use Whisper Composer, however to build it you will also need [GCC][4] and [Make][5]. Depending on your system installing these dependencies will be different.

On debian based systems (this should also install all dependancies as well):
> sudo apt-get install portaudio19-dev python gcc g++ make

We know what you need, but we're not sure how you do it on other systems. [Please help us fix that][3].

##### Actually Building

After you've installed all the dependencies, to do a full build and unit test run, execute the follwing in the project root:
> make all

##### Running

Currently, there's not much to look at. All of the output files are in the `lib` folder.

Files of interest:
 * `main-cpp` - demos simple sine wave audio playback
 * `main-c` - demos simple sine wave audio playback
 * `test-cpp` - runs all unit tests for the C++ code

Alternatively, with [Python][2] installed, you can run the simple python demo, which does exactly what `main-cpp` does, but in python.
> src/python> python main.py

[1]: http://portaudio.com
[2]: http://www.python.org
[3]: https://github.com/VolatileDream/whisper-composer/issues/11
[4]: http://gcc.gnu.org/
[5]: https://www.gnu.org/software/make/