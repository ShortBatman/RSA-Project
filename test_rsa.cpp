#include <iostream>
#include <string>
#include <cassert>
#include "rsa.h"
using namespace std;

int main(){
    // B0 mul_mod
    assert(mul_mod(7, 8, 5) == 1);
    assert(mul_mod(0, 99, 7) == 0);
    assert(mul_mod(123456789012ULL, 987654321098ULL, 1000036000099ULL) == 813538246183ULL);

    // B1 mod_exp
    assert(mod_exp(3, 644, 645) == 36);
    assert(mod_exp(1819, 13, 2537) == 2081);
    assert(mod_exp(5, 0, 7) == 1);
    assert(mod_exp(2, 1000000000ULL, 1000000007ULL) == 140625001ULL);
    assert(mod_exp(9, 5, 1) == 0);

    // B2 gcd
    assert(gcd(252, 198) == 18);
    assert(gcd(13, 2436) == 1);
    assert(gcd(17, 0) == 17);

    // B3 extended_gcd
    long long s, t;
    long long g = extended_gcd(252, 198, s, t);
    assert(g == 18 && s == 4 && t == -5);
    g = extended_gcd(4620, 101, s, t);
    assert(g == 1 && s == -35 && t == 1601);
    g = extended_gcd(1769, 551, s, t);   // same numbers as A3
    assert(g == 29 && s == 5 && t == -16);

    // B4 mod_inverse
    assert(mod_inverse(3, 7) == 5);
    assert(mod_inverse(101, 4620) == 1601);
    assert(mod_inverse(13, 2436) == 937);
    assert(mod_inverse(43, 600) == 307);    // same as A4
    assert(mod_inverse(17, 3120) == 2753);  // same as A6
    bool threw = false;
    try {
        mod_inverse(2, 6);
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    assert(threw);   // gcd(2, 6) = 2 so there is no inverse

    // B5 is_prime
    assert(is_prime(2) && is_prime(2539) && is_prime(1000003));
    assert(!is_prime(0) && !is_prime(1) && !is_prime(2537) && !is_prime(561));

    // B6 generate_keys
    uint64 n, d;
    generate_keys(43, 59, 13, n, d);
    assert(n == 2537 && d == 937);
    generate_keys(61, 53, 17, n, d);        // same as A6
    assert(n == 3233 && d == 2753);

    threw = false;
    try {
        generate_keys(42, 59, 13, n, d);    // 42 is not prime
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        generate_keys(43, 43, 13, n, d);    // p == q
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        generate_keys(43, 59, 2, n, d);     // gcd(2, 2436) = 2
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    assert(threw);

    // B7 encrypt and decrypt
    assert(encrypt(1819, 2537, 13) == 2081);
    assert(encrypt(1415, 2537, 13) == 2182);
    assert(decrypt(981, 2537, 937) == 704);
    assert(decrypt(461, 2537, 937) == 1115);
    assert(encrypt(65, 3233, 17) == 2790);   // A6
    assert(decrypt(2790, 3233, 2753) == 65);
    assert(encrypt(61, 3233, 17) == 610);    // A6 part 4
    assert(decrypt(610, 3233, 2753) == 61);

    threw = false;
    try {
        encrypt(2537, 2537, 13);             // M is not less than n
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        decrypt(2600, 2537, 937);            // C is not less than n
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    assert(threw);

    // B8 block_size, encrypt_text, decrypt_text
    assert(block_size(2537) == 2);
    assert(block_size(1000036000099ULL) == 6);
    assert(encrypt_text("STOP", 2537, 13) == "2081 2182");
    assert(encrypt_text("Stop!", 2537, 13) == "2081 2182");
    assert(encrypt_text("abc", 2537, 13) == "0001 0027");   // padded to ABCX
    assert(decrypt_text("0981 0461", 2537, 937) == "HELP");
    assert(decrypt_text(encrypt_text("DISCRETE", 2537, 13), 2537, 937) == "DISCRETE");

    threw = false;
    try {
        block_size(25);                      // even one letter does not fit
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    assert(threw);

    cout << "All tests passed" << endl;
    return 0;
}
