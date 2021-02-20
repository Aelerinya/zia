# Zia

## Prerequisites

Conan, cmake, make, gcc 10

### Conan

``` sh
pip install conan
conan remote add epitech https://api.bintray.com/conan/epitech/public-conan
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

## Building

``` sh
mkdir build
cd build
conan install .. --build=missing
cmake ..
cmake --build .
```

