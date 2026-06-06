# learn-boost

A learning project for Boost C++ libraries: cryptography, networking, sorting, JSON, INI, UUID, and unit testing.

## Features

- **Sorting benchmarks** - std::sort, Boost.Sort (pdqsort, spinsort, parallel variants)
- **JSON** - Boost.JSON serialize/parse
- **Networking** - HTTP client and async session
- **Devcore** - Base64, UUID, AES-CBC, SHA3/SHA256/RIPEMD160, INI parsing

## Requirements

- C++20 compiler, CMake >= 3.28
- Boost >= 1.83, OpenSSL >= 3.0
- lcov (optional, for coverage)

## Build & Test

```bash
bash build.sh
```

This script configures, builds, runs Boost.Test unit tests, and generates a coverage report (lcov + gcov required). HTML report is at `build/coverage/html/index.html`.

```bash
bash build.sh clean     # clean build
```

## Run

```bash
./build/learn_boost                 # default
./build/learn_boost sort
./build/learn_boost json
./build/learn_boost net
./build/learn_boost base64
./build/learn_boost uuid
./build/learn_boost aes
./build/learn_boost hash
./build/learn_boost ini
```

## Test

```bash
./build/tests/test_main                              # run all
./build/tests/test_main --run_test=test_aes_cbc_roundtrip   # one case
```

## CI

GitHub Actions: builds and runs tests on Linux, generates coverage. On `v*` tag push, builds for Linux and macOS and creates a GitHub Release.