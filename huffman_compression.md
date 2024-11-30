# Huffman Compression in Password Manager

## Overview
Huffman Compression is an algorithm used for lossless data compression. In this password manager, it is used to reduce the size of stored user credentials, ensuring efficient storage and transfer. The program compresses the `user_credentials.csv` file into `user_credentials.huff` when the user exits the application.

## How It Works
1. **Compression on Exit**:
   - When you exit the password manager, the program reads the `user_credentials.csv` file and compresses it into `user_credentials.huff`.
   - The original file is overwritten to save disk space.

2. **Decompression on Login**:
   - On login, if the `user_credentials.huff` file exists, the program decompresses it to restore the `user_credentials.csv` file.
   - This ensures a seamless user experience while maintaining an optimized storage format.

3. **Fallback**:
   - If no compressed file exists, the program directly reads the plaintext database (`user_credentials.csv`).

## Installation Process
1. Ensure you have the following installed:
   - A C++17-compatible compiler.
   - `CMake` build system.

2. Add Huffman Encoding as a submodule:
   ```bash
   git submodule add https://github.com/BaptisteEtroy/Huffman-Encoding.git

3. Build the project
    ```
    mkdir build && cd build
    cmake ..
    make

4. Run the program
    ```
    ./password_manager
