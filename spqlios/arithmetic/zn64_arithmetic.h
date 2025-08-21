#include <stdint.h>

EXPORT void zn64_normalize_base2k_ref(uint64_t nn,                              // N
                                         uint64_t log2_base2k,                              // output base 2^K
                                         int64_t* res, uint64_t res_size, uint64_t res_sl,  // res
                                         const int64_t* a, uint64_t a_size, uint64_t a_sl,  // a
                                         uint8_t* tmp_space  // scratch space of size >= N
);