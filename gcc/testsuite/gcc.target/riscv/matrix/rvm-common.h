#include <stdint.h>

#define RVM_ALL_TYPE_TEST(MACRO, ...) \
  MACRO(int8_t, i8, int8)                        \
  MACRO(int16_t, i16, int16)                      \
  MACRO(int32_t, i32, int32)                      \
  MACRO(int64_t, i64, int64)                      \
  MACRO(uint8_t, ui8, uint8)                      \
  MACRO(uint16_t, ui16, uint16)                    \
  MACRO(uint32_t, ui32, uint32)                    \
  MACRO(uint64_t, ui64, uint64)                    \
  MACRO(float16_t, f16, float16)                    \
  MACRO(float32_t, f32, float32)                    \
  MACRO(float64_t, f64, float64)

#define RVM_INT_TYPE_TEST(MACRO, ...) \
  MACRO(int8_t, i8, int8)                        \
  MACRO(int16_t, i16, int16)                      \
  MACRO(int32_t, i32, int32)                      \
  MACRO(int64_t, i64, int64)                      \
  MACRO(uint8_t, ui8, uint8)                      \
  MACRO(uint16_t, ui16, uint16)                    \
  MACRO(uint32_t, ui32, uint32)                    \
  MACRO(uint64_t, ui64, uint64)

#define RVM_FLOAT_TYPE_TEST(MACRO, ...) \
  MACRO(float16_t, f16, float16)                    \
  MACRO(float32_t, f32, float32)                    \
  MACRO(float64_t, f64, float64)

#define RVM_INT_SD_TYPE_TEST(MACRO, ...) \
  MACRO(int32_t, i32, int32)                      \
  MACRO(int64_t, i64, int64)

#define RVM_INT_USD_TYPE_TEST(MACRO, ...) \
  MACRO(uint32_t, ui32, uint32)                      \
  MACRO(uint64_t, ui64, uint64)
