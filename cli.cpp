// Copyright (c) 2011 Nils Schneider
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#include "headers.h"

#define printf(...) fprintf(stdout, __VA_ARGS__)

char *GetLine(bool fInteractive) {
    static size_t nInput = 0;
    static char *pszInput = NULL;

    bool fResult;

    if (fInteractive) {
        printf("> ");
    }

    fResult = getline(&pszInput, &nInput, stdin) != -1;

    if (fResult)
        return pszInput;
    else 
        return NULL;
}

void StartCLI(void) {
    bool fInteractive;
    char *pszInput = NULL;
    
    fInteractive = isatty(fileno(stdin));

    if (fInteractive) {
        printf("Welcome to Bitcoin, the digital crypto currency. (%s)\n\n", FormatFullVersion().c_str());
        printf("Type:  'help' for help with commands\n");
        printf("       'stop' to quit\n\n");
    }

    while((pszInput = GetLine(fInteractive)) != NULL) {
        printf("got %s", pszInput);
    }

    if (fInteractive) {
        printf("Shutdown initiated. Thank you for using Bitcoin!\n");
    }

    Shutdown(NULL);
}
