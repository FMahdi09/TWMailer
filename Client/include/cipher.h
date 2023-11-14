#ifndef CIPHER_H
#define CIPHER_H

#include <unistd.h>
#include <libgen.h>

#include <filesystem>
#include <exception>
#include <string>

// openssl includes
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

class Cipher
{
public:
    // constructor
    Cipher();

    // destructor
    ~Cipher();

    // public methods
    bool readKeyFromFile();
    void generateKeyPair();
    void readOtherPublicKey(std::string pKey);

    // RSA
    std::string RSAencrypt(std::string toEncrypt);
    std::string RSAdecrypt(std::string toDecrypt);

    // AES
    void AESinit(std::string passphrase, std::string salt);
    std::string AESencrypt(std::string toEncrypt);
    std::string AESdecrypt(std::string toDecrypt);

private:
    // private variables
    std::string exePath;

    // RSA
    EVP_PKEY *RSA_keyPair;
    EVP_PKEY *otherPublicKey;

    // AES
    unsigned char key[32];
    unsigned char iv[32];

    // private methods
    std::string getExePath();
};

#endif // CIPHER_H
