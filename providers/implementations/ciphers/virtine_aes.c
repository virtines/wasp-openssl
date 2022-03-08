#include <openssl/aes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <xmmintrin.h>
#include <virtine.h>


// This comes from /runtime_do_vpaes_cbc_encrypt.s
// (which is a bit of a hack to get around compiler limitations)
extern void AES_encrypt__ASM(const unsigned char *in, unsigned char *out, const AES_KEY *key);
extern void vpaes_encrypt_ASM(const unsigned char *in, unsigned char *out, const AES_KEY *key);
extern void vpaes_cbc_encrypt_ASM(const unsigned char *in, unsigned char *out, size_t length, const AES_KEY *key,
                                  unsigned char *ivec, int enc);


#define SZ 256

struct virtine_aes_state {
  unsigned char in[SZ];
  unsigned char out[SZ];
  unsigned char iv[16];
  short length;
};



virtine int do_vpaes_cbc_encrypt(struct virtine_aes_state *state, const AES_KEY key, int enc) {
  vpaes_cbc_encrypt_ASM((unsigned char *)state->in, (unsigned char *)state->out, state->length, &key, (unsigned char *)state->iv, enc);
  return 0;
}


void vpaes_cbc_encrypt(const unsigned char *in, unsigned char *out, size_t length, const AES_KEY *key,
                       unsigned char *ivec, int enc) {
  size_t remaining = length;
  struct virtine_aes_state state;
  memcpy(&state.iv, ivec, 16);

  for (int i = 0; i < length; i += SZ) {
    state.length = remaining < SZ ? remaining : SZ;

    memcpy(&state.in, in + i, state.length);
    do_vpaes_cbc_encrypt(&state, *key, enc);

    memcpy(out + i, &state.out, 16);

    remaining -= state.length;
  }
  memcpy(ivec, &state.iv, 16);

  return;

}
