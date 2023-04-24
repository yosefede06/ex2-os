//
// Created by Yosef on 24/04/2023.
//

#ifndef EX2_OS_HANDLE_H
#define EX2_OS_HANDLE_H
#define LIBRARY_ERROR "thread library error: "
#define SYSTEM_CALL_ERROR "system error: "
#define EXIT_CODE_FAILURE 1
#define FAILURE_ERROR -1
#define EXIT_FORCED 0

#include <iostream>

int handleErrorLibrary (const std::string &errorMsg);
void handleErrorSystemCall (const std::string &errorMsg);
void handleForcedExit();


#endif //EX2_OS_HANDLE_H
