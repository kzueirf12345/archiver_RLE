#include "zipping.h"

#include <assert.h>
#include <string.h>

enum ErrorCode zipping(FILE* stream_in, FILE* stream_out)
{
    assert(stream_in);
    assert(stream_out);


    char prev_symbol = (char)fgetc(stream_in);
    char symbol = EOF;
    unsigned symbol_count = 1;
    while (!feof(stream_in) && !ferror(stream_in))
    {
        symbol = (char)fgetc(stream_in);
        if (symbol != prev_symbol)
        {
            if (fprintf(stream_out, "(%u)%c", symbol_count, prev_symbol) <= 0)
                return FAILURE;

            symbol_count = 1;
        }
        else
        {
            ++symbol_count;
        }
        prev_symbol = symbol;
    }
    return ferror(stream_in) ? FAILURE : SUCCESS;
}