# C++ Service Template

[![Build Status](https://travis-ci.com/AlcheraInc/cpp-service-template.svg?branch=master)](https://travis-ci.com/AlcheraInc/cpp-service-template)

Basic setup for service prototyping in Alchera Inc.

* gRPC Async Server Example (No operation)

### References

* https://grpc.io
* https://github.com/luncliff/cpp20_grpc

## How To

### Build

Simply run with the CMake. Its version must be **3.14 or later**.

```cmake
cmake_minimum_required(VERSION 3.14)
```

#### VcPkg

The project requires [VcPkg](https://github.com/microsoft/vcpkg). Expects 2019.08 release or later

```console
$ vcpkg install --triplet x64-windows grpc catch2 ms-gsl
```

After the installation, run CMake with the `CMAKE_TOOLCHAIN_FILE`.

```console
$ cd /code/build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE="/vcpkg/scripts/buildsystem/vcpkg.cmake"
$ cmake --build . --config debug
```

### Code generation for gRPC 

There are several **CMake targets** to support code generations. Each can be used like the following

```console
$ cd /code/build
$ cmake --build . --target run_protoc_cpp
```

Current codegen target list:
* `run_protoc_cpp`
* `run_protoc_python`
* `run_protoc_nodejs`
* `run_protoc_csharp`
* `run_protoc_go`

Especially, Go requires additional setup to acquire code generator.

```sh
export GOPATH=$(pwd)
go get github.com/golang/protobuf/protoc-gen-go
```

Or, for PowerShell, 

```ps1
$env:GOPATH=Get-Location
go get github.com/golang/protobuf/protoc-gen-go
```

By doing this, 'bin/' folder will contain 'protoc-gen-go' executable.

### Test

CTest is enabled

> TBA

