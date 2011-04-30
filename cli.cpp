// Copyright (c) 2011 Nils Schneider
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#include "headers.h"
#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_writer_template.h"
#include "json/json_spirit_utils.h"

using namespace json_spirit;

#define printf(...) fprintf(stdout, __VA_ARGS__)

typedef Value(*rpcfn_type)(const Array& params, bool fHelp);
tuple<rpcfn_type, Value, Array> ParseJSON(const string& strRequest);

Object JSONRPCError(int code, const string& message);
string JSONRPCReply(const Value& result, const Value& error, const Value& id);

char *GetLine(bool fInteractive)
{
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

string RPCError(const Object& objError, const Value& id)
{
    return JSONRPCReply(Value::null, objError, id);
}

string ExecuteCommand(const string &strRequest) {
    Value id = Value::null;
    try
    {
        // Parse request
        rpcfn_type func;
        Value id;
        Array params;

        tie(func, id, params) = ParseJSON(strRequest);

        try
        {
            // Execute
            Value result = (*func)(params, false);

            // Send reply
            return JSONRPCReply(result, Value::null, id);
        }
        catch (std::exception& e)
        {
            return RPCError(JSONRPCError(-1, e.what()), id);
        }
    }
    catch (Object& objError)
    {
        return RPCError(objError, id);
    }
    catch (std::exception& e)
    {
        return RPCError(JSONRPCError(-32700, e.what()), id);
    }
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
        string foo = ExecuteCommand(string(pszInput));

        printf("%s\n", foo.c_str());

    }

    if (fInteractive) {
        printf("\nShutdown initiated. Thank you for using Bitcoin!\n");
    }

    Shutdown(NULL);
}
