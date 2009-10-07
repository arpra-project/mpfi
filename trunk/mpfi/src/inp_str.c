/* inp_str.c -- Set an interval from an input stream.

Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005,
                     Spaces project, Inria Lorraine
                     and Salsa project, INRIA Rocquencourt,
                     and Arenaire project, Inria Rhone-Alpes, France
                     and Lab. ANO, USTL (Univ. of Lille),  France

This file is part of the MPFI Library.

The MPFI Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPFI Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPFI Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA 02110-1301, USA. */


#include "mpfi_io.h"
#include "mpfi-impl.h"

size_t
mpfi_inp_str (mpfi_ptr x, FILE *s, int base)
{
  size_t t;
  int size_s = 100, left = 0, right = 0, i;
  char *str;
  mpfr_t tmp;
  int c = fgetc (s);

  while (MPFI_ISSPACE (c)) c=fgetc (s);

  if (c!='[') {  /* one single number defining an interval */
    fputc (c, s);
    mpfr_init2 (tmp, mpfi_get_prec (x));
    t = mpfr_inp_str (tmp, s, base, GMP_RNDD);
    left = mpfr_set (&(x->left), tmp, MPFI_RNDD);
    right = mpfr_set (&(x->right), tmp, MPFI_RNDD);
    if (mpfr_cmp_ui (&(x->right), 0) >= 0)
      mpfr_add_one_ulp (&(x->right), MPFI_RNDU);
    else
      mpfr_sub_one_ulp (&(x->right), MPFI_RNDU);
    mpfr_clear (tmp);

    return t;
  }
  else { /* interval given by two endpoints between square brackets */
    /* The interval is copied into a string and handled by mpfi_set_str */
    str = (char *)(*__gmp_allocate_func)(size_s * sizeof(char));
    c = fgetc (s);
    while (MPFI_ISSPACE (c)) c = fgetc (s);
    str[0] = '[';
    str[1] = c;
    i = 2;
    while (c != ']') {
      c = fgetc (s);
      str[i] = c;
      i++;
      if (i == size_s) {
        /* size_s *= 2;
	   str = (char *) realloc (str, size_s * sizeof (char));*/
	str = (char *)(*__gmp_reallocate_func)(str, size_s * sizeof(char), 2 * size_s * sizeof(char));
        size_s *= 2;
      }
      str[i]='\0';
    }
    t = mpfi_set_str (x, str, base);
    /* free (str);*/
    (*__gmp_free_func)(str, size_s * sizeof(char));
    return t;
  }
}
