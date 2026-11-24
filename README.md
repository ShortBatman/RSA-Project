# RSA Project (Discrete Structures, Fall 2026)

RSA built from scratch in C++17 for the Discrete Structures course. It has modular arithmetic, the extended Euclidean algorithm, key generation, text encryption in blocks, and an attack that breaks a small key. No big-integer or crypto libraries are used.

Team: Umer and Hamiz

## Files

- computations.pdf: Part A, handwritten and scanned
- rsa.h, rsa.cpp: functions B0 to B8
- test_rsa.cpp: tests for every function (B9)
- demo.cpp: interactive demo (B9)
- break_key.cpp: breaks the toy key (B10)

## Compile and run

Tests:

    g++ -std=c++17 -O2 rsa.cpp test_rsa.cpp -o test_rsa
    ./test_rsa

This prints "All tests passed".

Demo:

    g++ -std=c++17 -O2 rsa.cpp demo.cpp -o demo
    ./demo

Sample run:

    Enter p q e: 43 59 13
    Public key: n = 2537, e = 13
    Private key: d = 937
    Block size: 2 letters
    Message: Stop!
    Cleaned: STOP
    Plaintext blocks: 1819 1415
    Ciphertext: 2081 2182
    Decrypted: STOP

Break the key:

    g++ -std=c++17 -O2 rsa.cpp break_key.cpp -o break_key
    ./break_key

## Output of break_key

    p = 1000003
    q = 1000033
    Private key d = 983264276609
    Message: NUMBERTHEORYISFUNX

The final X is padding added by encrypt_text. The message is "NUMBER THEORY IS FUN".

## Sources

- Rosen, Discrete Mathematics and Its Applications, sections 4.1 to 4.4 and 4.6
- Lecture slides on number theory for RSA
- Code was written with help from an AI assistant (Claude) with the TA's permission, and then read and traced by both of us.
