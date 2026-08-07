#include "Crypto.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <openssl/buffer.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

using namespace std;

string AES::KEY = [] {
    const char* envSecret = getenv("AES_KEY");

    if (!envSecret || string(envSecret).empty())
    {
        cerr << "AES_KEY no configurada\n";
        exit(EXIT_FAILURE);
        
    } else {
        cout << "AES_KEY cargada Correctamente\n";
    }

    return string(envSecret);
}();

string AES::encrypt(const string& dato) {
    vector<unsigned char> IV(12);
    long longitud = 0;
    unsigned char* buffer = nullptr;
    const EVP_CIPHER* cipher = nullptr;
    EVP_CIPHER_CTX* ctx = nullptr;
    BIO* base64 = nullptr;
    BIO* mem = nullptr;
    BIO* push = nullptr;

    try {
        
        if (KEY.empty( ) || KEY.size() != 64)
            throw runtime_error("La key no debe estar vacia");
        
        if (dato.empty())
            throw logic_error("El dato no puede estar vacio");

        ctx = EVP_CIPHER_CTX_new();

        if (ctx == nullptr)
            throw runtime_error("No se pudo generar el contexto");

        buffer = OPENSSL_hexstr2buf(KEY.c_str(), &longitud);

        if (buffer == nullptr || longitud != 32)
            throw runtime_error("No se pudo obtener el buffer");

        int result = RAND_bytes(IV.data(), 12);

        if (result != 1) 
            throw runtime_error("No se pudo generer el RAND_bytes");

        cipher = EVP_aes_256_gcm();
        result = EVP_EncryptInit_ex(ctx, cipher, nullptr, buffer, IV.data());

        if (result != 1)
            throw runtime_error("No se pudo iniciar el encryptado");
        
        vector<unsigned char> ciphertext(dato.size() + EVP_CIPHER_block_size(cipher));
        int outUpdate = 0;
        int in = dato.size();
        
        result = EVP_EncryptUpdate(ctx, ciphertext.data(), &outUpdate, reinterpret_cast<const unsigned char*>(dato.c_str()), in);

        if (result != 1)
            throw runtime_error("No se pudo actualizar el Encrypt");

        int outFinal = 0;
        result = EVP_EncryptFinal_ex(ctx, ciphertext.data() + outUpdate, &outFinal);

        if (result != 1)
            throw runtime_error("No se pudo finalizar el encryptado");

        ciphertext.resize(outUpdate + outFinal);
    
        vector<unsigned char> tag_buffer(16);
        result = EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag_buffer.data());

        if (result != 1)
            throw runtime_error("No se pudo generar la firma");

        vector <unsigned char> resultado;
        resultado.insert(resultado.begin(), IV.begin(), IV.end());
        resultado.insert(resultado.end(), ciphertext.begin(), ciphertext.end());
        resultado.insert(resultado.end(), tag_buffer.begin(), tag_buffer.end());

        base64 = BIO_new(BIO_f_base64());

        if (base64 == nullptr)
            throw runtime_error("no se pudo generar el BIO");

        BIO_set_flags(base64, BIO_FLAGS_BASE64_NO_NL);
        mem = BIO_new(BIO_s_mem());

        if (mem == nullptr)
            throw runtime_error("No se pudo asignar memoria");

        push = BIO_push(base64, mem);

        if (push == nullptr)
            throw runtime_error("No se pudo generar el push");

        result = BIO_write(push, resultado.data(), resultado.size());

        if (result <= 0)
            throw runtime_error("No se pudo convertir a base64");

        result = BIO_flush(push);

        if (result != 1)
            throw runtime_error("No se pudo finalizar Base64");

        BUF_MEM* ptr = nullptr;

        BIO_get_mem_ptr(mem, &ptr);

        if (ptr == nullptr)
            throw runtime_error("No se pudo obtener el buffer Base64");

        string resultadoFinal(ptr->data, ptr->length);

        OPENSSL_free(buffer);
        EVP_CIPHER_CTX_free(ctx);
        BIO_free_all(push);

        return resultadoFinal;
    } catch (const exception& e) {
        OPENSSL_free(buffer);
        EVP_CIPHER_CTX_free(ctx);
        if (push != nullptr) 
            BIO_free_all(push);
        cout << e.what() << endl;
        throw runtime_error("Ocurrio un error inesperado");
    }
}

string AES::decrypt(const string& dato) {

    long longitud = 0;
    unsigned char* buffer = nullptr;
    EVP_CIPHER_CTX* ctx = nullptr;
    BIO* base64 = nullptr;
    BIO* mem = nullptr;
    BIO* push = nullptr;
    const EVP_CIPHER* cipher = nullptr;
    vector<unsigned char> uncifrated(dato.size());

    try {

    if (KEY.empty( ) || KEY.size() != 64)
            throw runtime_error("La key no debe estar vacia");
        
        if (dato.empty())
            throw logic_error("El dato no puede estar vacio");

        ctx = EVP_CIPHER_CTX_new();

        if (ctx == nullptr)
            throw runtime_error("No se pudo generar el contexto");

        buffer = OPENSSL_hexstr2buf(KEY.c_str(), &longitud);

        if (buffer == nullptr || longitud != 32)
            throw runtime_error("No se pudo obtener el buffer");

        base64 = BIO_new(BIO_f_base64());

        if (base64 == nullptr)
            throw runtime_error("no se pudo generar el BIO");

        BIO_set_flags(base64, BIO_FLAGS_BASE64_NO_NL);
        
        mem = BIO_new_mem_buf(dato.c_str(), dato.size());

        if (mem == nullptr)
            throw runtime_error("No se pudo asignar memoria");

        push = BIO_push(base64, mem);

        if (push == nullptr)
            throw runtime_error("No se pudo generar el push");

        int result = BIO_read(push, uncifrated.data(), uncifrated.size());

        if (result <= 0)
            throw runtime_error("No se pudo leer el buffer");
        
        uncifrated.resize(result);

        if (uncifrated.size() < 28)
            throw runtime_error("El mensaje cifrado es inválido");

        vector<unsigned char> IV(12);
        copy(uncifrated.begin(), uncifrated.begin() + IV.size(), IV.begin());

        vector<unsigned char> tag(16);
        copy(uncifrated.end()- tag.size(), uncifrated.end(),  tag.begin());

        vector<unsigned char> ciphertext(uncifrated.size() - IV.size() - tag.size());
        copy(uncifrated.begin() + IV.size(), uncifrated.end() - tag.size(), ciphertext.begin());

        cipher = EVP_aes_256_gcm();

        result = EVP_DecryptInit_ex(ctx, cipher, nullptr, buffer, IV.data());

        if (result != 1)
            throw runtime_error("No se pudo inicializar el desencryptado");

        vector<unsigned char> plaintext(ciphertext.size());

        int outUpdate = 0;

        result = EVP_DecryptUpdate(ctx, plaintext.data(), &outUpdate, ciphertext.data(), ciphertext.size());

        if (result != 1)
            throw runtime_error("No se pudo actualizar el desencryptado");

        result = EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, tag.size(), tag.data());

        if (result != 1)
            throw runtime_error("No se pudo configurar el tag");

        int outFinal = 0;
        result = EVP_DecryptFinal_ex(ctx, plaintext.data() + outUpdate, &outFinal);
        
        if (result != 1)
            throw runtime_error("No se terminar de decifrar el texto");

        plaintext.resize(outUpdate + outFinal);

        string resultadoFinal(plaintext.begin(), plaintext.end());

        OPENSSL_free(buffer);
        EVP_CIPHER_CTX_free(ctx);
        BIO_free_all(push);

        return resultadoFinal;
    } catch (const exception& e) {
        EVP_CIPHER_CTX_free(ctx);
        OPENSSL_free(buffer);
        if (push != nullptr) 
            BIO_free_all(push);
        cout << e.what() << endl;
        throw runtime_error("No se pudo decifrar");
        
    }
}