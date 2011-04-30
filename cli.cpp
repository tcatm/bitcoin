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
string JSONRPCRequest(const string& strMethod, const Array& params, const Value& id);

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

string ExecuteCommand(const string &strRequest)
{
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

void StartCLI(void)
{
    bool fInteractive;
    bool fRawOutput;
    char *pszInput = NULL;
    string strRequest;
    
    fInteractive = isatty(fileno(stdin));

    if (fInteractive) {
        printf("Welcome to Bitcoin, the digital crypto currency. (%s)\n\n", FormatFullVersion().c_str());
        printf("Type:  'help' for help with commands\n");
        printf("       'stop' to quit\n\n");
    }

    while((pszInput = GetLine(fInteractive)) != NULL) {
        fRawOutput = true;

        if (pszInput[0] != '{') {
            string strInput(pszInput);
            istringstream iss(strInput);
            vector<string> vTokens;

            vTokens.clear();

            copy(istream_iterator<string>(iss),
                     istream_iterator<string>(),
                     back_inserter<vector<string> >(vTokens));

            Array params;
            string strMethod;

            fRawOutput = false;

            params.clear();

            for (int i = 0; i < vTokens.size(); i++) {
                if (i == 0)
                    strMethod = vTokens[i];
                else
                    params.push_back(vTokens[i]);
            }

            // if user enters stop, break loop to call Shutdown()
            if (strMethod == "stop")
                break;

            strRequest = JSONRPCRequest(strMethod, params, 1);
        } else {
            strRequest = string(pszInput);
        }

        string strResult = ExecuteCommand(strRequest);

        printf("%s\n", strResult.c_str());

    }

    if (fInteractive) {
        printf("\nShutdown initiated. Thank you for using Bitcoin!\n");
    }

    Shutdown(NULL);
}
