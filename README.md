# 3D Online Chess + Chess Engine
This project is my first large scale C++ project that attempts to incorporate various aspects from a number of my current and previous classes (graphics, algorithms, statistics, etc.). The goal is to write a robust chess application, design an OpenGL renderer, and create chess engines using various statistical and machine learning models.

# Goals
- Style: Ensure that I conform to C++ best practices and make good stylistic decisions when writing the code.
- Efficiency: Utilize efficient algorithms and rendering techniques.
- Testability: Learn C++ testing frameworks and thoroughly test code.
- Documentation: Learn how to properly document C++ code.
- Design: Create a well designed object-oriented program.

# Build (Mac OSX)
Building project requires [Homebrew](http://brew.sh/).

## Graphics Dependencies
```
brew install glfw3
brew install glew
```

## Testing Dependencies
### GoogleMock and GoogleTest
```
brew install autoconf
git clone https://github.com/google/googletest.git
cd googletest/googlemock

autoreconf -fvi
./configure

g++ -I../googletest/include -I../googletest/ -Iinclude -I. -c ../googletest/src/gtest-all.cc 
g++ -I../googletest/include -I../googletest/ -Iinclude -I. -c src/gmock-all.cc 
ar -rv libgmock.a gtest-all.o gmock-all.o

mv libgmock.a lib
make
make check
```

Next, move the libgmock.a file into the lib/ directory of this project. Note that building against googlemock *also builds against googletest*.
