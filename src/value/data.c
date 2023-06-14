#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <value.h>

/* memcasecmp from gnulib */

static int memcasecmp(const void *vs1, const void *vs2, size_t n)
{
  size_t i;
  char const *s1 = vs1;
  char const *s2 = vs2;

  for (i = 0; i < n; i++)
  {
    unsigned char u1 = s1[i];
    unsigned char u2 = s2[i];
    int U1 = toupper(u1);
    int U2 = toupper(u2);
    int diff = (UCHAR_MAX <= INT_MAX ? U1 - U2 : U1 < U2 ? -1 : U2 < U1);
    if (diff)
      return diff;
  }
  return 0;
}

data_t data_null(void)
{
  return (data_t) {0};
}

data_t data_define(const void *base, size_t size)
{
  return (data_t) {.iov.iov_base = (void *) base, .iov.iov_len = size};
}

data_t data_string(const char *chars)
{
  return data_define(chars, strlen(chars));
}

data_t data_offset(const data_t data, size_t offset)
{
  return data_define((char *) data_base(data) + offset, data_size(data) - offset);
}

data_t data_select(const data_t data, size_t size)
{
  return data_define(data_base(data), size);
}

data_t data_copy(const data_t from)
{
  data_t to = data_define(malloc(data_size(from)), data_size(from));
  memcpy(data_base(to), data_base(from), data_size(from));
  return to;
}

data_t data_copyz(const data_t from)
{
  data_t to = data_define(malloc(data_size(from) + 1), data_size(from));
  memcpy(data_base(to), data_base(from), data_size(from));
  *(char *) data_end(to) = 0;
  return to;
}

void data_release(data_t data)
{
  free(data_base(data));
}

/* capacity */

size_t data_size(const data_t data)
{
  return data.iov.iov_len;
}

bool data_empty(const data_t data)
{
  return data_size(data) == 0;
}

bool data_nullp(const data_t data)
{
  return data.iov.iov_base == NULL;
}

/* element access */

void *data_base(const data_t data)
{
  return data.iov.iov_base;
}

void *data_end(const data_t data)
{
  return (char *) data_base(data) + data_size(data);
}

/* operations */

bool data_equal(const data_t data1, const data_t data2)
{
  return data_size(data1) == data_size(data2) && memcmp(data_base(data1), data_base(data2), data_size(data1)) == 0;
}

bool data_equal_case(const data_t data1, const data_t data2)
{
  return data_size(data1) == data_size(data2) && memcasecmp(data_base(data1), data_base(data2), data_size(data1)) == 0;
}
