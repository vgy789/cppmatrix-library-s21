[На русском 🇷🇺](README_RUS.md)

# C++ Matrix Library
An implementation of a C++ matrix library using an object-based approach. This project is a C++ version of a [previously created library](https://github.com/vgy789/matrix-library-s21) written in C.

## Key Features & Concepts

- **Abstract Data Type (ADT)**
- **Encapsulation**
- **RAII**
- **The Rule of Five**
- **Operator Overloading:** Arithmetic (`+`, `-`, `*`), assignment (`+=`, `-=`, `*=`), comparison (`==`), and element access (`()`) operators.
- **Const-correctness**
- **Exception Safety Guarantees**
- **Google Test** for unit testing.

## How to Build and Run

- To build the library `s21_matrix_oop.a`:
  ```sh
  make all
  ```
- To build and run the tests:
  ```sh
  make test
  ```

- For a list of all available commands, run:
  ```sh
  make help
  ```

## License
This project is licensed under the Apache License, Version 2.0. See LICENSE file for details.
