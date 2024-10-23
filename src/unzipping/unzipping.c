#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>

#include "unzipping.h"

enum UnzippingState
{
    UNZIPPING_STATE_START,
    UNZIPPING_STATE_OPEN_BRAKET,
    UNZIPPING_STATE_SYMBOL_COUNT,
    UNZIPPING_STATE_CLOSE_BRAKET,
    UNZIPPING_STATE_SYMBOL
};

#define MAX_SYMBOL_COUNT_LEN_ 10


enum ErrorCode unzipping2_achtung(FILE* stream_in, FILE* stream_out)
{
    lassert(stream_in);
    lassert(stream_out);

    size_t count_symbol_size = 0;
    char count_symbol_buf[MAX_SYMBOL_COUNT_LEN_] = {};
    enum UnzippingState unzipping_state = UNZIPPING_STATE_START;
    char prev_symbol = EOF;
    char cur_symbol = (char)fgetc(stream_in);

    while (!feof(stream_in) && !ferror(stream_in))
    {
        switch (unzipping_state)
        {
        case UNZIPPING_STATE_START:
        {
            if (cur_symbol != '(')
                return ERROR_INCORRECT;

            unzipping_state = UNZIPPING_STATE_OPEN_BRAKET;
            break;
        }

        case UNZIPPING_STATE_OPEN_BRAKET:
        {
            if (!isdigit(cur_symbol))
                return ERROR_INCORRECT;

            unzipping_state = UNZIPPING_STATE_SYMBOL_COUNT;
            break;
        }

        case UNZIPPING_STATE_SYMBOL_COUNT:
        {
            if (cur_symbol == ')')
                unzipping_state = UNZIPPING_STATE_CLOSE_BRAKET;
            else if (!isdigit(cur_symbol))
                return ERROR_INCORRECT;
                
            if (count_symbol_size > MAX_SYMBOL_COUNT_LEN_)
                return ERROR_INCORRECT;
            count_symbol_buf[count_symbol_size++] = prev_symbol;

            break;
        }

        case UNZIPPING_STATE_CLOSE_BRAKET:
        {
            unzipping_state = UNZIPPING_STATE_SYMBOL;

            break;
        }

        case UNZIPPING_STATE_SYMBOL:
        {
            unzipping_state = UNZIPPING_STATE_OPEN_BRAKET;
            
            char symbol = prev_symbol;

            size_t count_symbol = (size_t)atol(count_symbol_buf);
            if (count_symbol == 0)
                return ERROR_FAILURE;
            
            memset(count_symbol_buf, '\0', count_symbol_size);
            count_symbol_size = 0;
            
            for (size_t i = 0; i < count_symbol; ++i)
            {
                if (fprintf(stream_out, "%c", symbol) <= 0)
                    return ERROR_FAILURE;
            }
            break;
        }
        
        default:
            lassert(!"Unknown error");
            break;
        }

        prev_symbol = cur_symbol;
        cur_symbol = (char)fgetc(stream_in);
    }

    //TODO - обработка последнего символа

    return ERROR_SUCCESS;
}
#undef MAX_SYMBOL_COUNT_LEN_


enum ErrorCode unzipping(FILE* stream_in, FILE* stream_out) 
{
    lassert(stream_in);
    lassert(stream_out);

    unsigned char count = (unsigned char)fgetc(stream_in);
    char symbol = 0;

    while (!feof(stream_in) && !ferror(stream_in))
    {
        if (count & REPEAT)
        {
            symbol = (char)fgetc(stream_in);
            for (unsigned char i = 0; i < count - (unsigned char)REPEAT; ++i)
            {
                if (putc(symbol, stream_out) <= 0)
                    return ERROR_FAILURE;
            }
        }
        else
        {
            for (unsigned char i = 0; i < count; ++i)
            {
                symbol = (char)fgetc(stream_in);
                if (putc(symbol, stream_out) <= 0)
                    return ERROR_FAILURE;
            }
        }

        count = (unsigned char)fgetc(stream_in);
    }

    return ferror(stream_in) ? ERROR_FAILURE : ERROR_SUCCESS;
}
