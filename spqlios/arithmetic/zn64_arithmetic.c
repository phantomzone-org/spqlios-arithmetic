#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../coeffs/coeffs_arithmetic.h"

EXPORT void zn64_normalize_base2k_ref(uint64_t nn,                              // N
                                         uint64_t log2_base2k,                              // output base 2^K
                                         int64_t* res, uint64_t res_size, uint64_t res_sl,  // res
                                         const int64_t* a, uint64_t a_size, uint64_t a_sl,  // a
                                         uint8_t* tmp_space  // scratch space of size >= N
) {

  int64_t* cout = (int64_t*)tmp_space;
  int64_t* cin = 0x0;

  // propagate carry until first limb of res
  int64_t i = a_size - 1;
  for (; i >= res_size; --i) {
    znx_normalize(nn, log2_base2k, 0x0, cout, a + i * a_sl, cin);
    cin = cout;
  }

  // propagate carry and normalize
  for (; i >= 1; --i) {
    znx_normalize(nn, log2_base2k, res + i * res_sl, cout, a + i * a_sl, cin);
    cin = cout;
  }

  // normalize last limb
  znx_normalize(nn, log2_base2k, res, 0x0, a, cin);

  // extend result with zeros
  for (uint64_t i = a_size; i < res_size; ++i) {
    znx_zero_i64_ref(nn, res + i * res_sl);
  }
}