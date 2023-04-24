//
// Created by Yosef on 24/04/2023.
//

#include "Handle.h"

int handleErrorLibrary(const std::string &errorMsg) {
        std::cerr << LIBRARY_ERROR << errorMsg << std::endl;
        return FAILURE_ERROR;
}

void handleErrorSystemCall(const std::string &errorMsg) {
    std::cerr << SYSTEM_CALL_ERROR << errorMsg << std::endl;
    exit (EXIT_CODE_FAILURE);
}

void handleForcedExit() {
    exit (EXIT_FORCED);
}