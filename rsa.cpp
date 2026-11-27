#include "rsa.h"
using namespace std;

// B0: (a * b) mod m using repeated doubling, so we never multiply two big numbers
uint64 mul_mod(uint64 a, uint64 b, uint64 m){
    a = a % m;
    b = b % m;
    uint64 result = 0;
    while(b > 0){
        // if the last bit of b is 1, add the current a to the result
        if(b % 2 == 1){
            result = (result + a) % m;
        }
        // double a and drop the last bit of b
        a = (a + a) % m;
        b = b / 2;
    }
    return result;
}

// B1: b^n mod m using fast exponentiation (Rosen Algorithm 5)
uint64 mod_exp(uint64 b, uint64 n, uint64 m){
    uint64 x = 1 % m;      // 1 % m so that m = 1 gives 0
    uint64 power = b % m;  // this is b, b^2, b^4, b^8, ... (mod m)
    while(n > 0){
        // if the last bit of n is 1, multiply the answer by the current power
        if(n % 2 == 1){
            x = mul_mod(x, power, m);
        }
        power = mul_mod(power, power, m);
        n = n / 2;
    }
    return x;
}

// B2: gcd with the Euclidean algorithm
uint64 gcd(uint64 a, uint64 b){
    while(b != 0){
        uint64 r = a % b;
        a = b;
        b = r;
    }
    return a;
}

// B3: returns g = gcd(a, b) and finds s and t so that g = s*a + t*b
long long extended_gcd(long long a, long long b, long long &s, long long &t){
    long long quotients[100];   // an array is enough: Euclid never needs more than about 90 steps for 64-bit numbers
    int count = 0;
    long long r0 = a;
    long long r1 = b;
    // step 1: go forward like the normal Euclidean algorithm and save every quotient
    while(r1 != 0){
        quotients[count] = r0 / r1;
        count++;
        long long r2 = r0 % r1;
        r0 = r1;
        r1 = r2;
    }
    // now r0 is the gcd
    // step 2: go backward through the quotients (this is the back substitution we do by hand)
    long long x = 1;
    long long y = 0;
    for(int i = count - 1; i >= 0; i--){
        long long newX = y;
        long long newY = x - quotients[i] * y;
        x = newX;
        y = newY;
    }
    s = x;
    t = y;
    return r0;
}

// B4: inverse of a mod m, answer is between 0 and m - 1
uint64 mod_inverse(uint64 a, uint64 m){
    long long s, t;
    long long g = extended_gcd(a, m, s, t);
    if(g != 1){
        throw invalid_argument("no inverse");
    }
    // s can be negative, so bring it into the range 0 to m - 1
    // (m must be cast to long long here or the negative number breaks)
    long long r = s % (long long)m;
    if(r < 0){
        r = r + m;
    }
    return r;
}

// B5: check if n is prime using trial division
bool is_prime(uint64 n){
    if(n < 2){
        return false;
    }
    // d <= n / d is the same as d * d <= n but it can not overflow
    for(uint64 d = 2; d <= n / d; d++){
        if(n % d == 0){
            return false;
        }
    }
    return true;
}

// B6: make the keys from p, q and e
void generate_keys(uint64 p, uint64 q, uint64 e, uint64 &n, uint64 &d){
    if(!is_prime(p) || !is_prime(q)){
        throw invalid_argument("p and q must be prime");
    }
    if(p == q){
        throw invalid_argument("p and q must be different");
    }
    uint64 phi = (p - 1) * (q - 1);
    if(e <= 1 || e >= phi || gcd(e, phi) != 1){
        throw invalid_argument("bad value of e");
    }
    n = p * q;
    d = mod_inverse(e, phi);
}

// B7: encrypt one number, C = M^e mod n
uint64 encrypt(uint64 M, uint64 n, uint64 e){
    if(M >= n){
        throw invalid_argument("M must be less than n");
    }
    return mod_exp(M, e, n);
}

// B7: decrypt one number, M = C^d mod n
uint64 decrypt(uint64 C, uint64 n, uint64 d){
    if(C >= n){
        throw invalid_argument("C must be less than n");
    }
    return mod_exp(C, d, n);
}

// B8: how many letters fit in one block
// a block of k letters is the number 2525...25 at its largest (ZZ...Z), and it must be less than n
int block_size(uint64 n){
    if(25 >= n){
        throw invalid_argument("n is too small");
    }
    uint64 z = 25;
    int k = 1;
    while(k < 9){
        uint64 next = z * 100 + 25;  // add one more Z
        if(next >= n){
            break;
        }
        z = next;
        k++;
    }
    return k;
}

// our own versions of isalpha and toupper (static just means only this file uses them)
static bool is_letter(char c){
    if(c >= 'A' && c <= 'Z'){
        return true;
    }
    if(c >= 'a' && c <= 'z'){
        return true;
    }
    return false;
}
static char to_capital(char c){
    if(c >= 'a' && c <= 'z'){
        return c - 'a' + 'A';
    }
    return c;
}

// B8: turn a message into blocks of numbers and encrypt each block
string encrypt_text(const string &text, uint64 n, uint64 e){
    int k = block_size(n);
    // keep only the letters and make them capital
    string letters = "";
    for(int i = 0; i < text.length(); i++){
        if(is_letter(text[i])){
            letters = letters + to_capital(text[i]);
        }
    }
    // add X at the end until the length is a multiple of k
    while(letters.length() % k != 0){
        letters = letters + 'X';
    }
    int width = to_string(n).length();  // every cipher block is written with this many digits
    string result = "";
    for(int i = 0; i < letters.length(); i = i + k){
        // join k letters into one number (A = 00, B = 01, ..., Z = 25)
        uint64 M = 0;
        for(int j = 0; j < k; j++){
            M = M * 100 + (letters[i + j] - 'A');
        }
        uint64 C = encrypt(M, n, e);
        // add zeros in front so the block has the same number of digits as n
        string block = to_string(C);
        while(block.length() < width){
            block = "0" + block;
        }
        if(result != ""){
            result = result + " ";
        }
        result = result + block;
    }
    return result;
}

// helper for decrypt_text: turns one number back into k letters
// we take two digits at a time from the right
string number_to_letters(uint64 M, int k){
    string letters = "";
    for(int j = 0; j < k; j++){
        int value = M % 100;
        if(value > 25){
            throw invalid_argument("wrong key");
        }
        letters = (char)('A' + value) + letters;   // put the new letter at the front
        M = M / 100;
    }
    if(M != 0){
        throw invalid_argument("wrong key");
    }
    return letters;
}

// B8: decrypt blocks like "0981 0461" back into letters
string decrypt_text(const string &blocks, uint64 n, uint64 d){
    int k = block_size(n);
    string result = "";
    uint64 num = 0;
    bool gotDigit = false;
    // go one character at a time, the extra step at i == length is the end of the string
    for(int i = 0; i <= blocks.length(); i++){
        if(i == blocks.length() || blocks[i] == ' '){
            // a number just ended, so decrypt it
            if(gotDigit){
                uint64 M = decrypt(num, n, d);
                result = result + number_to_letters(M, k);
            }
            num = 0;
            gotDigit = false;
        }
        else if(blocks[i] >= '0' && blocks[i] <= '9'){
            num = num * 10 + (blocks[i] - '0');
            gotDigit = true;
        }
        else{
            throw invalid_argument("bad character in cipher text");
        }
    }
    return result;
}
