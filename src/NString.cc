#include "NString.h"

NString::NString(const char* cstr, unsigned int length) {
    buffer = NULL;
    capacity = len = 0;
    if (cstr && length) {
        if (!reserve(length)) {
            invalidate();
            return;
        }
        len = length;
        memcpy(buffer, cstr, length);
    }
}
