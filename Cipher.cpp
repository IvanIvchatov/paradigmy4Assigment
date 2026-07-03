#include "Cipher.h"
#include <iostream>
#include <cstdlib>

Cipher::Cipher() {
    hLib = LoadLibraryA("cipher.dll");
    if (!hLib) {
        std::cerr << "[Cipher] Failed to load cipher.dll!\n";
        create_caesar = nullptr;
        create_vigenere = nullptr;
        return;
    }

    create_caesar   = (CreateCaesarPtr)   GetProcAddress(hLib, "cipher_create_caesar");
    create_vigenere = (CreateVigenerePtr) GetProcAddress(hLib, "cipher_create_vigenere");
    do_encrypt = (EncryptPtr) GetProcAddress(hLib, "cipher_encrypt");
    do_decrypt = (DecryptPtr) GetProcAddress(hLib, "cipher_decrypt");
    do_destroy = (DestroyPtr) GetProcAddress(hLib, "cipher_destroy");
    do_free    = (FreePtr)    GetProcAddress(hLib, "cipher_free");
}

Cipher::~Cipher() {
    if (hLib) {
        FreeLibrary(hLib);
    }
}

bool Cipher::isLoaded() const {
    return hLib && create_caesar && create_vigenere && do_encrypt && do_decrypt && do_destroy && do_free;
}

Cipher::cipher_t* Cipher::createCipher(CipherType type, const std::string& key) {
    if (type == CipherType::Caesar) {
        int shift = std::stoi(key);
        return create_caesar(shift);
    }
    return create_vigenere(key.c_str());
}

std::string Cipher::encrypt(const std::string& text, const std::string& key, CipherType type) {
    if (!isLoaded()) return text;

    cipher_t* c = createCipher(type, key);
    char* res = do_encrypt(c, text.c_str());
    std::string out = (res ? res : "");
    do_free(res);
    do_destroy(c);
    return out;
}

std::string Cipher::decrypt(const std::string& text, const std::string& key, CipherType type) {
    if (!isLoaded()) return text;

    cipher_t* c = createCipher(type, key);
    char* res = do_decrypt(c, text.c_str());
    std::string out = (res ? res : "");
    do_free(res);
    do_destroy(c);
    return out;
}
