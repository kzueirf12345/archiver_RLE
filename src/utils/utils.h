#ifndef SRC_UTILS_H
#define SRC_UTILS_H

#include "../logger/logger.h"

#define REPEAT   0b10000000
#define NOREPEAT 0b00000000

enum ErrorCode
{
    ERROR_SUCCESS,
    ERROR_FAILURE,
    ERROR_INCORRECT
};


#endif /* SRC_UTILS_H */