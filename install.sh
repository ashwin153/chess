#!/bin/bash
# Install Boost
brew install boost

# Install TensorFlow
sudo easy_install pip
sudo easy_install --upgrade six
sudo pip install --upgrade https://storage.googleapis.com/tensorflow/mac/tensorflow-0.8.0-py2-none-any.whl

# Install GLFW3
brew install glfw3

# Install GLEW
brew install glew

# Install Valgrind
brew install valgrind

# Install GoogleTest/GoogleMock
brew install autoconf
git clone https://github.com/google/googletest.git
cd googletest/googlemock
autoreconf -fvi
./configure
g++ -I../googletest/include -I../googletest/ -Iinclude -I. -c ../googletest/src/gtest-all.cc 
g++ -I../googletest/include -I../googletest/ -Iinclude -I. -c src/gmock-all.cc 
ar -rv libgmock.a gtest-all.o gmock-all.o
make
make check
cd ../..
mkdir lib
mv googletest/googlemock/libgmock.a ./lib
rm -rf googletest

# Install Git Large File Storage (LFS)
brew install git-lfs
git lfs install
