//
// Created by Yosef on 24/04/2023.
//

#include "Handle.h"

int handleErrorLibrary(const char * errorMsg) {
    fprintf(stderr, LIBRARY_ERROR, errorMsg);
    return FAILURE_ERROR;
}

void handleErrorSystemCall(const char * errorMsg) {
    fprintf(stderr, SYSTEM_CALL_ERROR, errorMsg);
    exit (EXIT_CODE_FAILURE);
}

void handleForcedExit() {
    exit (EXIT_FORCED);
}