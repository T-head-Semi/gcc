#include <thead_matrix.h>
#include <stdio.h>

#define __DEBUG_IDENTITY(...) __VA_ARGS__

#define __DEBUG_FUNCTION(TYPE, SUFFIX, FMT, LEN, TAIL, OTHER)   \
void debug_data_##SUFFIX (const char *fmt, TYPE *value) __attribute__((noinline));  \
void debug_data_##SUFFIX (const char *fmt, TYPE *value)   \
{   \
  printf("%-20s: ", fmt);   \
  for (int i = 0; i < LEN; i++)   \
  {   \
    printf(FMT, (TYPE)value[i]);   \
    value[i]=0; \
  }   \
  printf("\n");   \
}

#define __DEBUG_DTAT_ITERATOR(MACRO, ...) \
  MACRO(int8_t,    i8,  "%-4d",  4, __DEBUG_IDENTITY(,), __VA_ARGS__)  \
  MACRO(int16_t,   i16, "%-4d",  4, __DEBUG_IDENTITY(,), __VA_ARGS__)  \
  MACRO(int32_t,   i32, "%-4d",  4, __DEBUG_IDENTITY(,), __VA_ARGS__)  \
  MACRO(int64_t,   i64, "%-4lld",  4, __DEBUG_IDENTITY(,), __VA_ARGS__)  \
  MACRO(uint8_t,   u8,  "%-4d",  4, __DEBUG_IDENTITY(,), __VA_ARGS__)  \
  MACRO(uint16_t,  u16, "%-4d",  4, __DEBUG_IDENTITY(,), __VA_ARGS__)  \
  MACRO(uint32_t,  u32, "%-4d",  4, __DEBUG_IDENTITY(,), __VA_ARGS__)  \
  MACRO(uint64_t,  u64, "%-4lld",  4, __DEBUG_IDENTITY(,), __VA_ARGS__)  \
  MACRO(float16_t, f16, "%-12f", 4, __DEBUG_IDENTITY(,), __VA_ARGS__)  \
  MACRO(float32_t, f32, "%-12f", 4, __DEBUG_IDENTITY(,), __VA_ARGS__)  \
  MACRO(float64_t, f64, "%-12f", 4, __DEBUG_IDENTITY() , __VA_ARGS__)      

__DEBUG_DTAT_ITERATOR(__DEBUG_FUNCTION,)


#ifdef __cplusplus

#define __DEBUG_GENERIC_BY_TYPE(TYPE, SUFFIX, FMT, LEN, TAIL, OTHER)	\
  void debug_data (const char *fmt, TYPE *value){ debug_data_##SUFFIX (fmt, value); }

__DEBUG_DTAT_ITERATOR(__DEBUG_GENERIC_BY_TYPE, )

#else
#define __DEBUG_GENERIC_BY_TYPE(TYPE, SUFFIX, FMT, LEN, TAIL, OTHER)	\
  TYPE* : debug_data_##SUFFIX						\
  TAIL

#define debug_data(fmt, value) (_Generic((value), __DEBUG_DTAT_ITERATOR(__DEBUG_GENERIC_BY_TYPE, )) (fmt, value))
#endif
