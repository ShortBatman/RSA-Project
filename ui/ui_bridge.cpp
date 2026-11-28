// ui_bridge.cpp
// Thin layer that lets the web page call the real RSA code from rsa.cpp.
// All the maths is done by the functions in rsa.cpp (compiled to WebAssembly).
// This file only checks the input first and passes numbers and text in and out.
#include "../rsa.h"
#include <string>
using namespace std;

static char inputBuffer[4096];   // the page writes text here
static string outputText;        // the page reads the answer from here
static uint64 savedN = 0;
static uint64 savedD = 0;

extern "C" {

char* w_input_buffer(){ return inputBuffer; }
const char* w_output_text(){ return outputText.c_str(); }
int w_output_length(){ return (int)outputText.size(); }

uint64 w_mul_mod(uint64 a, uint64 b, uint64 m){ return mul_mod(a, b, m); }
uint64 w_mod_exp(uint64 b, uint64 n, uint64 m){ return mod_exp(b, n, m); }
uint64 w_gcd(uint64 a, uint64 b){ return gcd(a, b); }
int w_is_prime(uint64 n){ return is_prime(n) ? 1 : 0; }
int w_block_size(uint64 n){ return block_size(n); }

// returns 0 if the keys were made, otherwise an error code
// 1 p not prime, 2 q not prime, 3 p equals q, 4 bad e, 5 n too big, 6 numbers too small
int w_generate_keys(uint64 p, uint64 q, uint64 e){
    if(p < 2 || q < 2){ return 6; }
    if(!is_prime(p)){ return 1; }
    if(!is_prime(q)){ return 2; }
    if(p == q){ return 3; }
    if(p > 9223372036854775807ULL / q){ return 5; }   // n must stay below 2^63
    uint64 phi = (p - 1) * (q - 1);
    if(e <= 1 || e >= phi || gcd(e, phi) != 1){ return 4; }
    if(p * q <= 25){ return 6; }
    generate_keys(p, q, e, savedN, savedD);
    return 0;
}
uint64 w_saved_n(){ return savedN; }
uint64 w_saved_d(){ return savedD; }

uint64 w_encrypt(uint64 M, uint64 n, uint64 e){
    if(M >= n){ return 0; }
    return encrypt(M, n, e);
}
uint64 w_decrypt(uint64 C, uint64 n, uint64 d){
    if(C >= n){ return 0; }
    return decrypt(C, n, d);
}

// the text is read from the input buffer, the answer goes to the output text
void w_encrypt_text(uint64 n, uint64 e){
    outputText = encrypt_text(string(inputBuffer), n, e);
}
// a wrong key or a bad cipher text makes the code in rsa.cpp stop with an error,
// the page catches that and shows a message
void w_decrypt_text(uint64 n, uint64 d){
    outputText = decrypt_text(string(inputBuffer), n, d);
}

}
