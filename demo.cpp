#include <iostream>
#include <string>
#include <cctype>
#include "rsa.h"
using namespace std;

int main(){
    uint64 p, q, e, n, d;

    // keep asking until generate_keys accepts p, q and e
    bool ok = false;
    while(!ok){
        cout << "Enter p q e: ";
        if(!(cin >> p >> q >> e)){
            if(cin.eof()){
                return 1;
            }
            // the user typed something that is not a number
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Please enter three numbers" << endl;
        }
        else{
            try {
                generate_keys(p, q, e, n, d);
                ok = true;
            }
            catch (const invalid_argument &ex) {
                cout << ex.what() << endl;
            }
        }
    }

    int k = block_size(n);
    cout << "Public key: n = " << n << ", e = " << e << endl;
    cout << "Private key: d = " << d << endl;
    cout << "Block size: " << k << " letters" << endl;

    // read the whole line, so spaces are allowed
    string message;
    cin.ignore(1000, '\n');
    cout << "Message: ";
    getline(cin, message);

    // clean the message: letters only, all capital
    string cleaned = "";
    for(int i = 0; i < message.length(); i++){
        if(isalpha(message[i])){
            cleaned = cleaned + (char)toupper(message[i]);
        }
    }
    cout << "Cleaned: " << cleaned << endl;

    // show the plaintext numbers (same steps as encrypt_text, X is added to fill the last block)
    string padded = cleaned;
    while(padded.length() % k != 0){
        padded = padded + 'X';
    }
    cout << "Plaintext blocks:";
    for(int i = 0; i < padded.length(); i = i + k){
        uint64 M = 0;
        for(int j = 0; j < k; j++){
            M = M * 100 + (padded[i + j] - 'A');
        }
        cout << " " << M;
    }
    cout << endl;

    string cipher = encrypt_text(message, n, e);
    cout << "Ciphertext: " << cipher << endl;
    cout << "Decrypted: " << decrypt_text(cipher, n, d) << endl;
    return 0;
}
