// You can refer this video for better understanding :- https://www.youtube.com/watch?v=orIgy2MjqrA 
// Reference - https://helix.stormhub.org/papers/SHA-256.pdf

#include <bits/stdc++.h>
using namespace std;

const long long H[] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

const long long K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Circular Right Shift of n bits
long long RotR(long long X, int n) {
    return (X >> n) | (X << (32 - n));
}

// Right Shift of n bits
long long ShR(long long X, int n) {
    return X >> n;
}

// Σ0(X) = RotR(X, 2) ⊕ RotR(X, 13) ⊕ RotR(X, 22)
long long E0(long long X) {
    return RotR(X, 2) ^ RotR(X, 13) ^ RotR(X, 22);
}

// Σ1(X) = RotR(X, 6) ⊕ RotR(X, 11) ⊕ RotR(X, 25)
long long E1(long long X) {
    return RotR(X, 6) ^ RotR(X, 11) ^ RotR(X, 25);
}

// σ0(X) = RotR(X, 7) ⊕ RotR(X, 18) ⊕ ShR(X, 3)
long long o0(long long X) {
    return RotR(X, 7) ^ RotR(X, 18) ^ ShR(X, 3);
}

// σ1(X) = RotR(X, 17) ⊕ RotR(X, 19) ⊕ ShR(X, 10)
long long o1(long long X) {
    return RotR(X, 17) ^ RotR(X, 19) ^ ShR(X, 10);
}

// Ch(X, Y, Z) = (X ∧ Y ) ⊕ (¬X ∧ Z)
long long Ch(long long X, long long Y, long long Z) {
    return (X & Y) ^ (~X & Z);
}

// Maj(X, Y, Z) = (X ∧ Y ) ⊕ (X ∧ Z) ⊕ (Y ∧ Z)
long long Maj(long long X, long long Y, long long Z) {
    return (X & Y) ^ (X & Z) ^ (Y & Z);
}

long long mod2power32(long long num){
    long long modulus = 1LL << 32; // 2^32
    return (num % modulus);
}

vector<string> splitString(const string& input, int chunkSize) {
    vector<string> chunks;
    int startIndex = 0;

    while (startIndex < input.length()) {
        string chunk = input.substr(startIndex, chunkSize);
        chunks.push_back(chunk);
        startIndex += chunkSize;
    }

    return chunks;
}

string intToHex(long long value) {
    const char hexDigits[] = "0123456789abcdef";
    string hexString;

    do {
        int digit = value & 0xF;
        hexString = hexDigits[digit] + hexString; 
        value >>= 4; 
    } while (value > 0);

    // Pad with zeroes if needed
    while (hexString.length() < 8) {
        hexString = "0" + hexString;
    }

    return hexString;
}

string stringToBinary(const string& input) {
    string binaryString;

    for (char c : input) {
        binaryString += bitset<8>(c).to_string();
    }

    return binaryString;
}

int main() {
    cout << "Give your string : ";
    string input; getline(cin, input);

    /* 
        PADDING OF INPUT STRING 
        1. Convert the input string (std::string) to binary form (std::string)
           Let l be length of string
        2. Concatenate "1" to binary string
        3. Concatenate the require no. of "0" to binary string 
            No of zeros be addded be k then k is given by this eqn :- 
                l+1+k = 448 mod 512
            where k is smallest positive no. solution to this eqn
        4. Convert l (lenght of starting binary string) to 64-bit binary string and concatenate this to binary string too 

        Now this binary string has length that is multiple of 512
    */
    string binaryString = stringToBinary(input);
    int l = binaryString.length();
    binaryString = binaryString+"1";   
    int k = (((448 - (l+1) + 512) % 512)+512)%512;
    for(int i = 0 ; i < k ; i++){binaryString = binaryString+"0";}
    string lengthInBinary = bitset<64>(l).to_string();
    binaryString = binaryString + lengthInBinary;
    
    /* 
        SPLIT THE BINARY STRING
        We split the binary string into blocks of 512 bits
    */
    vector<string> blocks = splitString(binaryString,512);
    
    /*  
        1. Initialize a set of eight 32-bit (sha256digest) initial hash values based on H[0] to H[7]
        2. Now for each block, we do the following operation
            1. Create a array W[] of 64 32-bit words by breaking down each chunk into 16 words and expanding the rest (W[16] to W[63]) using a specific formula.
                Formula :- W[i] = σ1(W[i−2]) + W[i−7] + σ0([Wi−15]) + W[i−16]
            2. For each W array (64 rounds), update the working variables using a series of logical functions and bitwise operations, involving the message schedule, the current block's words, and the constant values.
            3. Update the working variables based on these logical functions and the message schedule.
            4. Update sha256digest by adding the working variables to the current values modulo 2^32
    */
    vector<long long> sha256digest;
    for(int i = 0 ; i < 8 ; i++){
        sha256digest.push_back(H[i]);
    }
    for(auto block : blocks){
        vector<string> words = splitString(block,32);
        vector<long long> W;
        for (const string& word : words) {
            long long w_t = bitset<32>(word).to_ullong();
            W.push_back(w_t);
        }
        int t = 16;
        while(t <= 63){
            long long w_t = mod2power32(o1(W[t-2]) + W[t-7] + o0(W[t-15]) + W[t-16]);
            W.push_back(w_t);
            t++;
        }

        long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7];
        for (int t = 0 ; t < 64 ; t++) {
            long long T1 = h + E1(e) + Ch(e, f, g) + K[t] + W[t];
            long long T2 = E0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = mod2power32((d + T1));
            d = c;
            c = b;
            b = a;
            a = mod2power32((T1 + T2));
        }

        sha256digest[0] = mod2power32(sha256digest[0]+a);
        sha256digest[1] = mod2power32(sha256digest[1]+b);
        sha256digest[2] = mod2power32(sha256digest[2]+c);
        sha256digest[3] = mod2power32(sha256digest[3]+d);
        sha256digest[4] = mod2power32(sha256digest[4]+e);
        sha256digest[5] = mod2power32(sha256digest[5]+f);
        sha256digest[6] = mod2power32(sha256digest[6]+g);
        sha256digest[7] = mod2power32(sha256digest[7]+h);
    }

    /*
        PRINTING THE SHA256 DIGEST
        Print each value in sha256digest after converting to hexadecimal values
    */
    cout << "SHA256 digest : ";
    for(int i = 0 ; i < 8 ; i++){
        cout << intToHex(sha256digest[i]);
    }
}
