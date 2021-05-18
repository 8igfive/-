#ifndef __RSA_H__
#define __RSA_H__
#include <gmp.h>
struct public_key{
    mpz_t modulus;
    mpz_t exponent;
};
struct private_key{
    mpz_t modulus;
    mpz_t exponent;
};
//
void RSA_gen_keys(struct public_key* pub,struct private_key* priv);
//
void RSA_encrypt(mpz_t outseed,mpz_t seed,const struct public_key* pub);
//
void RSA_decrypt(mpz_t outseed,mpz_t seed,const struct private_key* priv);
#endif