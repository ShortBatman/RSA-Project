#include <iostream>
#include <string>
#include "rsa.h"
using namespace std;

int main(){
    // public key and ciphertext from the instructor
    uint64 n = 1000036000099ULL;
    uint64 e = 65537;
    string ciphertext = "0614157364221 0502078796304 0031583460315";

    // step 1: factor n by trial division, the first number that divides n is p
    uint64 p = 2;
    while(n % p != 0){
        p++;
    }
    uint64 q = n / p;

    // step 2: now we know p and q, so we can find phi and the private key d
    uint64 phi = (p - 1) * (q - 1);
    uint64 d = mod_inverse(e, phi);

    // step 3: decrypt the message with our own decrypt_text
    string message = decrypt_text(ciphertext, n, d);

    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    cout << "Private key d = " << d << endl;
    cout << "Message: " << message << endl;
    return 0;
}
