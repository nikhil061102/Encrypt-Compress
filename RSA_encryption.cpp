#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

// Function to find the greatest common divisor (GCD)
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Function to generate the public and private keys
void generateKeys(int& publicKey, int& privateKey, int& modulus) {
    int p, q;
    
    // Choose two prime numbers p and q
    do {
        p = rand() % 100 + 1;
    } while (!isPrime(p));
    do {
        q = rand() % 100 + 1;
    } while (!isPrime(q));
    
    // Calculate modulus
    modulus = p * q;
    
    // Calculate Euler's totient function
    int phi = (p - 1) * (q - 1);
    
    // Choose public key
    do {
        publicKey = rand() % (phi - 2) + 2;
    } while (gcd(publicKey, phi) != 1);
    
    // Calculate private key
    int k = 1;
    while ((phi * k + 1) % publicKey != 0 || (phi * k + 1) / publicKey >= modulus)
        k++;
    privateKey = (phi * k + 1) / publicKey;
}

// Function to perform modular exponentiation
int modularExponentiation(int base, int exponent, int modulus) {
    int result = 1;
    base %= modulus;
    while (exponent > 0) {
        if (exponent & 1)
            result = (result * base) % modulus;
        exponent >>= 1;
        base = (base * base) % modulus;
    }
    return result;
}

// Function to encrypt a string
std::string encrypt(const std::string& message, int publicKey, int modulus) {
    std::stringstream encryptedStream;
    for (char c : message) {
        int encryptedChar = modularExponentiation(c, publicKey, modulus);
        encryptedStream << encryptedChar << " ";
    }
    return encryptedStream.str();
}

// Function to decrypt a string
std::string decrypt(const std::string& encryptedMessage, int privateKey, int modulus) {
    std::stringstream decryptedStream(encryptedMessage);
    std::string decryptedString;
    std::string encryptedChar;
    while (std::getline(decryptedStream, encryptedChar, ' ')) {
        int decryptedChar = modularExponentiation(std::stoi(encryptedChar), privateKey, modulus);
        decryptedString += static_cast<char>(decryptedChar);
    }
    return decryptedString;
}

int main() {
    int publicKey, privateKey, modulus;
    std::string message;
    
    // Generate keys
    generateKeys(publicKey, privateKey, modulus);
    
    // Input the message
    std::cout << "Enter the message to encrypt: ";
    std::getline(std::cin, message);
    
    // Encrypt the message
    std::string encryptedMessage = encrypt(message, publicKey, modulus);
    std::cout << "Encrypted message: " << encryptedMessage << std::endl;
    
    // Decrypt the message
    std::string decryptedMessage = decrypt(encryptedMessage, privateKey, modulus);
    std::cout << "Decrypted message: " << decryptedMessage << std::endl;
    
    return 0;
}
