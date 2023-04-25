//
// Created by Yosef on 24/04/2023.
//

#ifndef EX2_OS_HANDLE_H
#define EX2_OS_HANDLE_H
#define LIBRARY_ERROR "thread library error: %s\n"
#define SYSTEM_CALL_ERROR "system error: %s\n"
#define EXIT_CODE_FAILURE 1
#define FAILURE_ERROR -1
#define EXIT_FORCED 0

#include <iostream>

int handleErrorLibrary (const char * errorMsg);
void handleErrorSystemCall (const char * errorMsg);
void handleForcedExit();


#endif //EX2_OS_HANDLE_H
