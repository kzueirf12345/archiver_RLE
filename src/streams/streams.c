#include <assert.h>
#include <string.h>

#include "streams.h"

enum ErrorCode open_streams(FILE** const stream_in, FILE** const stream_out)
{
    logg(LOG_LEVEL_DETAILS_INFO, "Open function. stream_in:%p, stream_out:%p", 
         stream_in, stream_out);
         
    lassert(stream_in);
    lassert(stream_out);

    printf("Enter the input file name: ");

#define MAX_FILENAME_LEN 256

    char filename_in[MAX_FILENAME_LEN] = {};
    if (scanf("%s", filename_in) != 1)
        return ERROR_FAILURE;
    
    if (!(*stream_in = fopen(filename_in, "rb"))) // TODO perror
        return ERROR_FAILURE;


    printf("Enter the output file name: ");

    char filename_out[MAX_FILENAME_LEN] = {};
    if (scanf("%s", filename_out) != 1)
        return ERROR_FAILURE;

    if (!(*stream_out = fopen(filename_out, "wb")))
        return ERROR_FAILURE;
    

    return ERROR_SUCCESS;
}

enum ErrorCode close_streams(FILE** const stream_in, FILE** const stream_out)
{
    if (*stream_in && fclose(*stream_in))
        return ERROR_FAILURE;
    *stream_in = NULL;
    
    if (*stream_out && fclose(*stream_out))
        return ERROR_FAILURE;
    *stream_out = NULL;
    
    return ERROR_SUCCESS;
}