#include <assert.h>
#include <string.h>

#include "zipping.h"


enum ErrorCode zipping2_achtung(FILE* stream_in, FILE* stream_out)
{
    lassert(stream_in);
    lassert(stream_out);


    char prev_symbol = (char)fgetc(stream_in);
    char cur_symbol = EOF;
    unsigned symbol_count = 1;
    while (!feof(stream_in) && !ferror(stream_in))
    {
        cur_symbol = (char)fgetc(stream_in);
        if (cur_symbol != prev_symbol)
        {
            if (fprintf(stream_out, "(%u)%c", symbol_count, prev_symbol) <= 0)
                return ERROR_FAILURE;

            symbol_count = 1;
        }
        else
        {
            ++symbol_count;
        }
        prev_symbol = cur_symbol;
    }
    return ferror(stream_in) ? ERROR_FAILURE : ERROR_SUCCESS;
}

enum ErrorCode zipping(FILE* stream_in, FILE* stream_out)
{
    lassert(stream_in);
    lassert(stream_out);


    char prev_symbol = 0;
    char cur_symbol = (char)fgetc(stream_in);


    unsigned char repeat_count = REPEAT + 1;

    unsigned char norepeat_count = NOREPEAT + 1;
    char norepeat_buf[REPEAT - 1] = {};
    norepeat_buf[0] = cur_symbol;

    while (!feof(stream_in) && !ferror(stream_in))
    {
        prev_symbol = cur_symbol;
        cur_symbol = (char)fgetc(stream_in);

        if (cur_symbol != prev_symbol)
        {
            if (repeat_count - REPEAT > 1
                && fprintf(stream_out, "%c%c", repeat_count - 1, prev_symbol) <= 0)
                return ERROR_FAILURE;

            repeat_count = REPEAT;

            if (norepeat_count == 126)
            { 
                if (fprintf(stream_out, "%c%s", norepeat_count, norepeat_buf) <= 0)
                    return ERROR_FAILURE;
                memset(norepeat_buf, 0, norepeat_count);
                norepeat_count = NOREPEAT;
            }

            norepeat_buf[norepeat_count++] = cur_symbol;
        }
        else
        {
            if (norepeat_count > 1 && fprintf(stream_out, "%c%s", norepeat_count, norepeat_buf) <= 0)
                return ERROR_FAILURE;

            memset(norepeat_buf, 0, norepeat_count);
            norepeat_buf[0] = cur_symbol;
            norepeat_count = NOREPEAT + 1;

            if (repeat_count - REPEAT == 126)
            { 
                if (fprintf(stream_out, "%c%c", repeat_count - 1, prev_symbol) <= 0)
                    return ERROR_FAILURE;
                repeat_count =  REPEAT + 1;
            }

            ++repeat_count;
        }
    }

    //NOTE - last symbols
    norepeat_buf[--norepeat_count] = '\0';
    if (norepeat_count > 0 && fprintf(stream_out, "%c%s", norepeat_count, norepeat_buf) <= 0)
        return ERROR_FAILURE;

    return ferror(stream_in) ? ERROR_FAILURE : ERROR_SUCCESS;
}
