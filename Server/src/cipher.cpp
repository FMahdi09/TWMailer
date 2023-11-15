#include "cipher.h"

// constructor
Cipher::Cipher()
{
    exePath = std::string(dirname((char*)getExePath().c_str()));;

    // RSA
    RSA_keyPair = nullptr;
    otherPublicKey = nullptr;
}

// destructor
Cipher::~Cipher()
{
    EVP_cleanup();
    ERR_free_strings();
}

// public methods
bool Cipher::readKeyFromFile()
{
    std::string privatePath = exePath + "/private.pem";
    std::string publicPath = exePath + "/public.pem";

    // check if files exist
    if(!std::filesystem::exists(privatePath) ||
        !std::filesystem::exists(publicPath))
    {
        return false;
    }

    // read public key
    BIO *publicBio = BIO_new_file(publicPath.c_str(), "r");
    RSA_keyPair = PEM_read_bio_PUBKEY(publicBio, nullptr, nullptr, nullptr);

    if(RSA_keyPair == nullptr)
        throw std::runtime_error("failed to read public key from file");

    BIO_free(publicBio);

    // read private key
    BIO *privateBio = BIO_new_file(privatePath.c_str(), "r");
    RSA_keyPair = PEM_read_bio_PrivateKey(privateBio, nullptr, nullptr, nullptr);

    if(RSA_keyPair == nullptr)
        throw std::runtime_error("failed to read public key from file");

    BIO_free(privateBio);

    return true;
}

void Cipher::generateKeyPair()
{
    std::string privatePath = exePath + "/private.pem";
    std::string publicPath = exePath + "/public.pem";

    // generate 2048 bit RSA key pair
    EVP_PKEY_CTX *ctx;

    // set context
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

    if(!ctx)
        throw std::runtime_error("ctx (context) generation failed");

    if(EVP_PKEY_keygen_init(ctx) <= 0)
        throw std::runtime_error("ctx keygen_init failed");

    if(EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0)
        throw std::runtime_error("ctx set_rsa_keygen_bits failed");

    // generate keys
    if(EVP_PKEY_keygen(ctx, &RSA_keyPair) <= 0)
        throw std::runtime_error("failed to generate RSA keypair");

    EVP_PKEY_CTX_free(ctx);

    // write keys to files

    // public key
    FILE* publicKeyFile = fopen(publicPath.c_str(), "w");

    if(PEM_write_PUBKEY(publicKeyFile, RSA_keyPair) <= 0)
        throw std::runtime_error("failed to write public key to file");

    fclose(publicKeyFile);

    // private key
    FILE* privateKeyFile = fopen(privatePath.c_str(), "w");

    if(PEM_write_PrivateKey(privateKeyFile, RSA_keyPair, nullptr, nullptr , 0, nullptr, nullptr) <= 0)
        throw std::runtime_error("failed to write private key to file");

    fclose(privateKeyFile);
}

void Cipher::readOtherPublicKey(std::string pKey)
{
    const char* pbkByteArray = pKey.c_str();
    BIO* bpKey = BIO_new_mem_buf(pbkByteArray, -1);

    otherPublicKey = PEM_read_bio_PUBKEY(bpKey, nullptr, nullptr, nullptr);

    if(otherPublicKey == nullptr)
        throw std::runtime_error("failed to read public key from file");

    BIO_free(bpKey);
}

std::string Cipher::generateRandomString(int length)
{
    unsigned char toReturn[length];
    RAND_bytes(toReturn, length);
    return std::string((char*)toReturn, length);
}

// RSA

std::string Cipher::getPublicKey()
{
    std::string publicPath = exePath + "/public.pem";

    // read public key from file
    std::ifstream file(publicPath);
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

std::string Cipher::RSAencrypt(std::string toEncrypt)
{
    size_t outLength;

    // create context
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(otherPublicKey, NULL);

    if(!ctx)
        throw std::runtime_error("ctx (context) generation failed");

    if(EVP_PKEY_encrypt_init(ctx) <= 0)
        throw std::runtime_error("ctx encrypt_init failed");

    // perform encryption

    // determine length
    if(EVP_PKEY_encrypt(ctx, NULL, &outLength, (unsigned char *)toEncrypt.c_str(), toEncrypt.size()) <= 0)
        throw std::runtime_error("EVP_PKEY_encrypt(nullptr) error");

    unsigned char result[outLength];

    // encrypt
    if(EVP_PKEY_encrypt(ctx, result, &outLength, (unsigned char *)toEncrypt.c_str(), toEncrypt.size()) <= 0)
        throw std::runtime_error("EVP_PKEY_encrypt() error");

    EVP_PKEY_CTX_free(ctx);

    return std::string((char *)result, outLength);
}

std::string Cipher::RSAdecrypt(std::string toDecrypt)
{
    size_t outLength;

    // create context
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(RSA_keyPair, nullptr);

    if(!ctx)
        throw std::runtime_error("ctx (context) generation failed");

    if(EVP_PKEY_decrypt_init(ctx) <= 0)
        throw std::runtime_error("ctx decrypt init failed");

    // perform decryption

    // determine length
    if(EVP_PKEY_decrypt(ctx, nullptr, &outLength, (unsigned char*)toDecrypt.c_str(), toDecrypt.size()) <= 0)
        throw std::runtime_error("EVP_PKEY_decrypt(nullptr) failed");

    unsigned char result[outLength];

    // decrypt
    if(EVP_PKEY_decrypt(ctx, result, &outLength, (unsigned char*)toDecrypt.c_str(), toDecrypt.size()) <= 0)
        throw std::runtime_error("EVP_PKEY_decrypt() failed");

    return std::string((char *)result, outLength);
}

// AES

void Cipher::AESinit(std::string passphrase, std::string salt)
{
    int rounds = 5;

    // generate key & iv for AES 256 CBC mode
    if(EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), (const unsigned char*)salt.c_str(),
                       (const unsigned char*)passphrase.c_str(), passphrase.length(),
                       rounds, key, iv) != 32)
        throw std::runtime_error("failed to generate AES key and iv");
}

std::string Cipher::AESencrypt(std::string toEncrypt)
{
    // cipherlength is maximum plaintextsize + AES_BLOCK_SIZE
    int cipherLength = toEncrypt.size() + AES_BLOCK_SIZE;
    int finalLength = 0;

    // buffer for ciphertext
    unsigned char result[cipherLength];

    // create context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if(EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != ERR_LIB_NONE)
        throw std::runtime_error("ctx AES encrypt init failed");

    // perform encryption

    // generate ciphertext
    if(EVP_EncryptUpdate(ctx, result, &cipherLength, (unsigned char*)toEncrypt.c_str(), toEncrypt.size())
        != ERR_LIB_NONE)
        throw std::runtime_error("AES encryption update failed");

    finalLength = cipherLength;

    // update Cipher with remaining bytes
    EVP_EncryptFinal(ctx, result + cipherLength, &cipherLength);

    // calculate total bytes of ciphertext
    finalLength += cipherLength;

    EVP_CIPHER_CTX_free(ctx);

    return std::string((char *)result, finalLength);
}

std::string Cipher::AESdecrypt(std::string toDecrypt)
{
    int plainTextLength = toDecrypt.size();
    int finalLength = 0;

    // buffer for plaintext
    unsigned char result[plainTextLength];

    // create context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if(EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != ERR_LIB_NONE)
        throw std::runtime_error("ctx AES decrypt init failed");

    // perfrom decryption

    if(EVP_DecryptUpdate(ctx, result, &plainTextLength, (unsigned char*)toDecrypt.c_str(), toDecrypt.size())
        != ERR_LIB_NONE)
        throw std::runtime_error("AES decryption update failed");

    finalLength = plainTextLength;

    if(EVP_DecryptFinal_ex(ctx, result + plainTextLength, &plainTextLength) != ERR_LIB_NONE)
        throw std::runtime_error("AES decryption final failed");

    finalLength += plainTextLength;

    EVP_CIPHER_CTX_free(ctx);

    return std::string((char *)result, finalLength);
}

// private methods
std::string Cipher::getExePath()
{
    char result[ PATH_MAX ];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    return std::string( result, (count > 0) ? count : 0 );
}
