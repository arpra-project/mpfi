/* random.c -- Handle random seed in tests.

Copyright (C) 2009
                     Spaces project, Inria Lorraine
                     Arenaire project, Inria Rhone-Alpes, France
                     and Lab. ANO, USTL (Univ. of Lille),  France


This file is part of the MPFI Library, based on the MPFR Library.

The MPFI Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

The MPFI Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
License for more details.

You should have received a copy of the GNU Library General Public License
along with the MPFR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA 02110-1301, USA. */

/* Put test_start at the beginning of your test function and
   test_end at the end.
   These are an adaptation of those of MPFR. */

#include <stdlib.h>

#include "mpfi-tests.h"

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

gmp_randstate_t  rands;
char             rands_initialized;

void
test_start (void)
{
  char *environment_seed;
  unsigned long seed;

  if (rands_initialized)
    {
      fprintf (stderr,
               "Put test_start at the beginning of your test function.\n");
      exit (1);
    }

  gmp_randinit_default (rands);
  rands_initialized = 1;

  environment_seed = getenv ("MPFI_CHECK_RANDOMIZE");
  if (environment_seed == NULL)
      gmp_randseed_ui (rands, 0xfac11e);
  else
    {
      seed = atoi (environment_seed);
      if (seed == 0 || seed == 1)
        {
#if HAVE_GETTIMEOFDAY
          struct timeval  tv;
          gettimeofday (&tv, NULL);
          seed = tv.tv_sec + tv.tv_usec;
#else
          time_t  tv;
          time (&tv);
          seed = tv;
#endif
          gmp_randseed_ui (rands, seed);
          printf ("Seed MPFI_CHECK_RANDOMIZE=%lu "
                  "(include this in bug reports)\n", seed);
        }
      else
        {
          printf ("Re-seeding with MPFI_CHECK_RANDOMIZE=%lu\n", seed);
          gmp_randseed_ui (rands, seed);
        }
    }
}

void
test_end (void)
{
  if (rands_initialized)
    {
      rands_initialized = 0;
      gmp_randclear (rands);
    }
  mpfr_free_cache ();
}
