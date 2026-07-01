#include "Cipher.h"
#include <iostream>

Cipher::Cipher() {
    hLib = LoadLibraryA("cipher.dll");
    if (!hLib) {
        std::cerr << "[Cipher] Failed to load cipher.dll!\n";
        create_vigenere = nullptr;
        return;
    }

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
    return hLib && create_vigenere && do_encrypt && do_decrypt && do_destroy && do_free;
}

std::string Cipher::encrypt(const std::string& text, const std::string& key) {
    if (!isLoaded()) return text;

    cipher_t* c = create_vigenere(key.c_str());
    char* res = do_encrypt(c, text.c_str());
    std::string out = (res ? res : "");
    do_free(res);
    do_destroy(c);
    return out;
}

std::string Cipher::decrypt(const std::string& text, const std::string& key) {
    if (!isLoaded()) return text;

    cipher_t* c = create_vigenere(key.c_str());
    char* res = do_decrypt(c, text.c_str());
    std::string out = (res ? res : "");
    do_free(res);
    do_destroy(c);
    return out;
}
