#pragma once
#include <openssl/evp.h>

static void md5(uint8_t* buf, uint32_t len, uint8_t* md)
{
    EVP_MD_CTX *ctx;
    unsigned char *md5_digest;
    unsigned int digest_len = EVP_MD_size(EVP_md5()); //16
    
    ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
    EVP_DigestUpdate(ctx, buf, len);
    EVP_DigestFinal_ex(ctx, md, &digest_len);
    EVP_MD_CTX_free(ctx);
}