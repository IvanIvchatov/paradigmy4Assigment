#pragma once
#include <string>
#include <windows.h>

enum class CipherType {
    Caesar,
    Vigenere
};

class Cipher {
    typedef void* cipher_t;

    typedef cipher_t* (*CreateCaesarPtr)(int);
    typedef cipher_t* (*CreateVigenerePtr)(const char*);
    typedef char* (*EncryptPtr)(cipher_t*, const char*);
    typedef char* (*DecryptPtr)(cipher_t*, const char*);
    typedef void  (*DestroyPtr)(cipher_t*);
    typedef void  (*FreePtr)(char*);

    HMODULE hLib;
    CreateCaesarPtr   create_caesar;
    CreateVigenerePtr create_vigenere;
    EncryptPtr  do_encrypt;
    DecryptPtr  do_decrypt;
    DestroyPtr  do_destroy;
    FreePtr     do_free;

    cipher_t* createCipher(CipherType type, const std::string& key);

public:
    Cipher();
    ~Cipher();

    bool isLoaded() const;

    std::string encrypt(const std::string& text, const std::string& key, CipherType type);
    std::string decrypt(const std::string& text, const std::string& key, CipherType type);
};
