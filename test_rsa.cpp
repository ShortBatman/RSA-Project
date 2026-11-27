#include <iostream>
#include <string>
#include "rsa.h"
using namespace std;

// counts the tests and reports the ones that fail
int testNumber = 0;
bool allPassed = true;
void check(bool ok){
    testNumber++;
    if(!ok){
        cout << "Test number " << testNumber << " FAILED" << endl;
        allPassed = false;
    }
}

int main(){
    // B0 mul_mod
    check(mul_mod(7, 8, 5) == 1);
    check(mul_mod(0, 99, 7) == 0);
    check(mul_mod(123456789012ULL, 987654321098ULL, 1000036000099ULL) == 813538246183ULL);

    // B1 mod_exp
    check(mod_exp(3, 644, 645) == 36);
    check(mod_exp(1819, 13, 2537) == 2081);
    check(mod_exp(5, 0, 7) == 1);
    check(mod_exp(2, 1000000000ULL, 1000000007ULL) == 140625001ULL);
    check(mod_exp(9, 5, 1) == 0);

    // B2 gcd
    check(gcd(252, 198) == 18);
    check(gcd(13, 2436) == 1);
    check(gcd(17, 0) == 17);

    // B3 extended_gcd
    long long s, t;
    long long g = extended_gcd(252, 198, s, t);
    check(g == 18 && s == 4 && t == -5);
    g = extended_gcd(4620, 101, s, t);
    check(g == 1 && s == -35 && t == 1601);
    g = extended_gcd(1769, 551, s, t);   // same numbers as A3
    check(g == 29 && s == 5 && t == -16);

    // B4 mod_inverse
    check(mod_inverse(3, 7) == 5);
    check(mod_inverse(101, 4620) == 1601);
    check(mod_inverse(13, 2436) == 937);
    check(mod_inverse(43, 600) == 307);    // same as A4
    check(mod_inverse(17, 3120) == 2753);  // same as A6
    bool threw = false;
    try {
        mod_inverse(2, 6);
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    check(threw);   // gcd(2, 6) = 2 so there is no inverse

    // B5 is_prime
    check(is_prime(2) && is_prime(2539) && is_prime(1000003));
    check(!is_prime(0) && !is_prime(1) && !is_prime(2537) && !is_prime(561));

    // B6 generate_keys
    uint64 n, d;
    generate_keys(43, 59, 13, n, d);
    check(n == 2537 && d == 937);
    generate_keys(61, 53, 17, n, d);        // same as A6
    check(n == 3233 && d == 2753);

    threw = false;
    try {
        generate_keys(42, 59, 13, n, d);    // 42 is not prime
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    check(threw);

    threw = false;
    try {
        generate_keys(43, 43, 13, n, d);    // p == q
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    check(threw);

    threw = false;
    try {
        generate_keys(43, 59, 2, n, d);     // gcd(2, 2436) = 2
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    check(threw);

    // B7 encrypt and decrypt
    check(encrypt(1819, 2537, 13) == 2081);
    check(encrypt(1415, 2537, 13) == 2182);
    check(decrypt(981, 2537, 937) == 704);
    check(decrypt(461, 2537, 937) == 1115);
    check(encrypt(65, 3233, 17) == 2790);   // A6
    check(decrypt(2790, 3233, 2753) == 65);
    check(encrypt(61, 3233, 17) == 610);    // A6 part 4
    check(decrypt(610, 3233, 2753) == 61);

    threw = false;
    try {
        encrypt(2537, 2537, 13);             // M is not less than n
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    check(threw);

    threw = false;
    try {
        decrypt(2600, 2537, 937);            // C is not less than n
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    check(threw);

    // B8 block_size, encrypt_text, decrypt_text
    check(block_size(2537) == 2);
    check(block_size(1000036000099ULL) == 6);
    check(encrypt_text("STOP", 2537, 13) == "2081 2182");
    check(encrypt_text("Stop!", 2537, 13) == "2081 2182");
    check(encrypt_text("abc", 2537, 13) == "0001 0027");   // padded to ABCX
    check(decrypt_text("0981 0461", 2537, 937) == "HELP");
    check(decrypt_text(encrypt_text("DISCRETE", 2537, 13), 2537, 937) == "DISCRETE");

    threw = false;
    try {
        block_size(25);                      // even one letter does not fit
    }
    catch (const invalid_argument &) {
        threw = true;
    }
    check(threw);

    if(allPassed){
        cout << "All tests passed" << endl;
    }
    return 0;
}
