# Evolution Simulator

![Showcase](docs/images/showcase.png)




## How to build

### Using conan
```
mkdir build
cd build

conan install . --build=missing
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```