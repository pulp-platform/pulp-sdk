/******************************************************************************
 * Copyright (C) 2011-2018 Cadence Design Systems, Inc.
 * All rights reserved worldwide.
 ******************************************************************************
 * cdn_print.c
 * Print debug logs
 *
 * This is the example implementation of DbgPrint used in debug build
 *****************************************************************************/

#include <stdio.h>
#include <stdarg.h>

#ifdef DEBUG
// parasoft-begin-suppress MISRA2012-RULE-17_1_a-2 "Example implementation of DbgPrint"
// parasoft-begin-suppress MISRA2012-RULE-17_1_b-2 "Example implementation of DbgPrint"

// DbgPrint is required for DEBUG build
void DbgPrint(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    (void)vprintf(fmt, args);
    va_end(args);
}

// parasoft-end-suppress MISRA2012-RULE-17_1_a-2 "Example implementation of DbgPrint"
// parasoft-end-suppress MISRA2012-RULE-17_1_b-2 "Example implementation of DbgPrint"#endif
#endif
