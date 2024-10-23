#include "streams/streams.h"
#include "zipping/zipping.h"
#include "unzipping/unzipping.h"
#include "utils/utils.h"
#include "mode/mode.h"

int main()
{
    init_logger();
    set_level_details(LOG_LEVEL_DETAILS_ALL);

    FILE* stream_in = NULL, *stream_out = NULL;
    if (open_streams(&stream_in, &stream_out) == ERROR_FAILURE)
    {
        fprintf(stderr, "OPEN FAILURE\n");
        return -1;
    }
    logg(LOG_LEVEL_DETAILS_INFO, "Open (in-)output files: stream_in(%p), stream_out(%p)", 
         stream_in, stream_out);


    enum Mode mode = change_mode();
    if (mode == MODE_ZIP)
    {
        if (zipping(stream_in, stream_out) == ERROR_FAILURE)
        {
            fprintf(stderr, "ZIPPING FAILURE\n");
            return -1;
        }
    }
    else if (mode == MODE_UNZIP)
    {
        enum ErrorCode unzipping_code = unzipping(stream_in, stream_out);
        if (unzipping_code == ERROR_FAILURE)
        {
            fprintf(stderr, "ZIPPING FAILURE\n");
            return -1;
        }
        if (unzipping_code == ERROR_INCORRECT)
        {
            fprintf(stderr, "File for unzip is INCORRECT\n");
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "CHANGE MODE FAILURE\n");
        return -1;
    }

    if (close_streams(&stream_in, &stream_out) == ERROR_FAILURE)
    {
        fprintf(stderr, "CLOSE FAILURE\n");
        return -1;
    }

    destroy_logger();

    return 0;
}