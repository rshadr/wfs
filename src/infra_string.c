/* 
 * This file is part of the wfs distribution (https://github.com/lauch788/wfs).
 * Copyright (c) 2023 Adrien Ricciardi.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

#include <grapheme.h>

#include <wfs/infra_string.h>

static void maybe_grow(InfraString *string, size_t need);
static const uint32_t k_string_grow_step = 4;

InfraString *
infra_string_create(void)
{
  InfraString *string = malloc(sizeof (*string));
  memset(string, 0, sizeof (*string));

  uint32_t init_cap = k_string_grow_step;
  char *data = malloc(init_cap);
  memset(data, 0, init_cap);

  string->data = data;
  string->cap  = init_cap;

  return infra_string_ref(string);
}

void
infra_string_free(InfraString *string)
{
  free(string->data);
  free(string);
}

static void
maybe_grow(InfraString *string, size_t need)
{
  size_t newsize = string->size + need;

  if (newsize >= string->cap) {
    size_t newcap = string->cap + k_string_grow_step * 4; /* lazy */
    char *newdata = malloc(newcap);
    memset(newdata, 0, newcap);

    memcpy(newdata, string->data, string->cap);
    free(string->data);

    string->data = newdata;
    string->cap = newcap;
  }

}

void
infra_string_put_char(InfraString *string, char c)
{
  maybe_grow(string, 1);
  string->data[string->size++] = c;
}

void
infra_string_put_codepoint(InfraString *string, uint32_t c)
{
  char enc[10] = { 0 };
  size_t written;

  written = grapheme_encode_utf8(c, enc, sizeof (enc));
  maybe_grow(string, written);

  memcpy(&string->data[string->size], enc, written);
  string->size += written;
}
