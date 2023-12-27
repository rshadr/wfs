#ifndef _unicode_h
#define _unicode_h

#include <stdint.h>

static inline int
ascii_is_lower_alpha(uint32_t c)
{
  return (c >= 'a' && c <= 'z');
}

static inline int
ascii_is_upper_alpha(uint32_t c)
{
  return (c >= 'A' && c <= 'Z');
}

static inline int
ascii_is_alpha(uint32_t c)
{
  return (ascii_is_lower_alpha(c) || ascii_is_upper_alpha(c));
}

#endif /* _unicode_h */
