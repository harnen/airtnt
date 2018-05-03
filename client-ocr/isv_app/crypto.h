#ifndef CRYPTO_H
#define CRYPTO_H

int decrypt(uint8_t *key, uint8_t *ciphertext, int ciphertext_len, uint8_t *plaintext, uint8_t *iv, int iv_len, uint8_t *aad,
    int aad_len, uint8_t *tag);

int encrypt(uint8_t *key, uint8_t *plaintext, int plaintext_len, uint8_t *ciphertext, uint8_t *iv, int iv_len, uint8_t *aad,
    int aad_len, uint8_t *tag);

void handleErrors();
#endif /* CRYPTO_H */
