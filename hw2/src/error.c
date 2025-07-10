/*
 * Error handling routines
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "error.h"

int errors;
int warnings;
int dbflag = 1;

void fatal(char *fmt, ...)
{
        va_list ap;
        char *p, *sval;
        int ival;
        double dval;

        fprintf(stderr, "\nFatal error: ");
        va_start(ap, fmt);
        for (p = fmt; *p; p++)
        {
                if (*p != '%')
                {
                        putchar(*p);
                        continue;
                }
                switch (*++p)
                {
                case 'd':
                        ival = va_arg(ap, int);
                        printf("%d", ival);
                        break;
                case 'f':
                        dval = va_arg(ap, double);
                        printf("%f", dval);
                        break;
                case 's':
                        for (sval = va_arg(ap, char *); *sval; sval++)
                                putchar(*sval);
                        break;
                default:
                        putchar(*p);
                        break;
                }
        }
        fprintf(stderr, "\n");

        va_end(ap);
        exit(1);
}

void error(char *fmt, ...)
{
        va_list ap;
        char *p, *sval;
        int ival;
        double dval;

        fprintf(stderr, "\nError: ");
        va_start(ap, fmt);
        for (p = fmt; *p; p++)
        {
                if (*p != '%')
                {
                        putchar(*p);
                        continue;
                }
                switch (*++p)
                {
                case 'd':
                        ival = va_arg(ap, int);
                        printf("%d", ival);
                        break;
                case 'f':
                        dval = va_arg(ap, double);
                        printf("%f", dval);
                        break;
                case 's':
                        for (sval = va_arg(ap, char *); *sval; sval++)
                                putchar(*sval);
                        break;
                default:
                        putchar(*p);
                        break;
                }
        }
        fprintf(stderr, "\n");
        errors++;

        va_end(ap);
}

void warning(char *fmt, ...)
{
        va_list ap;
        char *p, *sval;
        int ival;
        double dval;

        fprintf(stderr, "\nWarning: ");
        va_start(ap, fmt);
        for (p = fmt; *p; p++)
        {
                if (*p != '%')
                {
                        putchar(*p);
                        continue;
                }
                switch (*++p)
                {
                case 'd':
                        ival = va_arg(ap, int);
                        printf("%d", ival);
                        break;
                case 'f':
                        dval = va_arg(ap, double);
                        printf("%f", dval);
                        break;
                case 's':
                        for (sval = va_arg(ap, char *); *sval; sval++)
                                putchar(*sval);
                        break;
                default:
                        putchar(*p);
                        break;
                }
        }
        fprintf(stderr, "\n");
        warnings++;

        va_end(ap);
}

void debug(char *fmt, ...)
{
        if(!dbflag) return;

        va_list ap;
        char *p, *sval;
        int ival;
        double dval;

        fprintf(stderr, "\nDebug: ");
        va_start(ap, fmt);
        for (p = fmt; *p; p++)
        {
                if (*p != '%')
                {
                        putchar(*p);
                        continue;
                }
                switch (*++p)
                {
                case 'd':
                        ival = va_arg(ap, int);
                        printf("%d", ival);
                        break;
                case 'f':
                        dval = va_arg(ap, double);
                        printf("%f", dval);
                        break;
                case 's':
                        for (sval = va_arg(ap, char *); *sval; sval++)
                                putchar(*sval);
                        break;
                default:
                        putchar(*p);
                        break;
                }
        }
        fprintf(stderr, "\n");

        va_end(ap);
}
