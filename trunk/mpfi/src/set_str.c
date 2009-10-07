/* set_str.c -- Set an interval to the value of a string.

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

int
mpfi_set_str (mpfi_ptr x, char *s, int base)
{
  int i, start1, start2;
  int overflow_left = 0;
  int overflow_right = 0;
  int slen;
  char tmp[1000];

  /* bzero (tmp, 1000); */
  memset (tmp, 0, 1000);

  slen= (int)strlen (s);
  if (slen >= 1000) return -1;

  i=0;
  /* read the blanks */
  while ( (((unsigned int) i) < slen) && MPFI_ISSPACE (s[i]) ) i++;
  /* Now s[i] is the first non blank character if any or '\0' otherwise */
  if ( ((unsigned int) i) == slen ) {
    fprintf (stderr, "Error: blank string in mpfi_set_str: %s\n", s);
    return (1);
  }

  if (s[i] == '[') {
    i++;

    /* read the blanks between '[' and the number itself */
    while ( (((unsigned int) i) < slen) && MPFI_ISSPACE (s[i]) ) i++;
    if ( ((unsigned int) i) == slen ) {
      fprintf (stderr, "Error: no number in string in mpfi_set_str: %s\n", s);
      return (1);
    }

    /* Copy the first number in the string tmp and then in x->left */
    start1 = i; /* s[start1] is the first digit of the first number */
    i++;

    /* determine the end of the first number in s */
    while ( (((unsigned int)i) < slen) && !MPFI_ISSPACE (s[i]) && ((s[i])!=',') ) i++;
    if ( ((unsigned int) i) == slen ) {
      fprintf (stderr, "Error: only one number in string in mpfi_set_str: %s\n", s);
      return (1);
    }

    /* Now s[i] is the first character after the first number */
    /* Note that, for now, in a string such as " [ 123foo321 , bar ] */
    /* 123foo321 is considered as the first number: */
    /* the error will be handled by mpfr_set_str */

    strncpy (tmp, &(s[start1]), i-start1);  /* s[start1]...s[i-1] */
    tmp[i-start1]='\0';
    overflow_left = mpfr_set_str (&(x->left), tmp, base, MPFI_RNDD);

    /* Read (possibly) blank characters between the first number and the comma */
    while ( (((unsigned int) i) < slen) && MPFI_ISSPACE (s[i]) ) i++;
    if ( ((unsigned int) i) == slen ) {
      fprintf (stderr, "Error: only one number in string in mpfi_set_str: %s\n", s);
      return (1);
    }

    if ( s[i] != ',' ) {
      fprintf (stderr, "Error: missing comma in mpfi_set_str:: %s\n", s);
      return (1);
    }
    /* Now s[i]==',' */
    i++;

    /* read (possibly) blank characters between the comma and the 2nd number */
    while ( (((unsigned int)i) < slen) && MPFI_ISSPACE (s[i]) ) i++;
    if ( ((unsigned int) i) == slen ) {
      fprintf (stderr, "Error: only one number in string in mpfi_set_str: %s\n", s);
      return (1);
    }

    /* Now s[i] is the first character of the 2nd number */
    /* Note that in the previous example, "bar" will be considered as */
    /* this number. Once again, the error will be handled by mpfr_set_str */
    start2 = i;
    i++;

    /* determine the end of the first number in s */
    while ( (((unsigned int) i) < slen) && !MPFI_ISSPACE (s[i]) && ((s[i])!=']') ) i++;
    if ( ((unsigned int) i) == slen ) {
      fprintf (stderr, "Error: missing closing square bracket in mpfi_set_str:: %s\n", s);
      return (1);
    }

    /* Now s[i] is the first character after the second number */
    strncpy (tmp, &(s[start2]), i-start2);  /* s[start2]...s[i-1] */
    tmp[i-start2] = '\0';
    overflow_right = mpfr_set_str (&(x->right), tmp, base, MPFI_RNDU);

    /* Read (possibly) blank characters between the 2nd number and */
    /* closing square bracket */
    while ( (((unsigned int)i) < slen) && MPFI_ISSPACE (s[i]) ) i++;
    if ( ((unsigned int) i) == slen ) {
      fprintf (stderr, "Error: missing closing square bracket in mpfi_set_str: %s\n", s);
      return (1);
    }

    if (s[i] != ']') { /* The closing square bracket is missing */
      fprintf (stderr, "Missing closing square bracket in mpfi_set_str: %s \n", s);
      return (1);
    }

    /* Note that the string may contain any character after the */
    /* closing square bracket: they will be ignored */
    if (overflow_left || overflow_right)
      return (-1);
    else
      return (0);
  }

  else {
    /* Only one number to store as an interval */
    /* s[i] is the first non blank character and is not an */
    /* opening square bracket */

    /* Copy the number in the string tmp and then in x->left */
    start1 = i; /* s[start1] is the first digit of the number */
    i++;

    /* determine the end of the number in s */
    while ( (((unsigned int) i) < slen) && !MPFI_ISSPACE (s[i]) ) i++;

    /* Now s[i] is the first character after the number */
    /* Note that, for now, in a string such as " 123foo321 bar" */
    /* 123foo321 is considered as the number: */
    /* the error will be handled by mpfr_set_str */
    /* "bar" will be ignored */

    strncpy (tmp, &(s[start1]), i-start1);
    tmp[i-start1] = '\0';
    overflow_left = mpfr_set_str (&(x->left), tmp, base, MPFI_RNDD);
    overflow_right = mpfr_set_str (&(x->right), tmp, base, MPFI_RNDU);

    /*
      overflow_right = mpfr_set (&(x->right), &(x->left), GMP_RNDU);
      if (mpfr_number_p (&(x->right))) {
      if (mpfr_cmp_ui (&(x->right), 0) >= 0)
      mpfr_add_one_ulp (&(x->right), GMP_RNDU);
      else
      mpfr_sub_one_ulp (&(x->right), GMP_RNDU);
      }
    */

    if ( (overflow_left > 0) || (overflow_right > 0) )
      return (1);
    else if (overflow_left || overflow_right)
      return (-1);
    else
      return (0);
  }
  return (0);
}

int
mpfi_init_set_str (mpfi_ptr x, char *s, int base)
{
  mpfi_init (x);
  return (mpfi_set_str (x, s, base));
}
