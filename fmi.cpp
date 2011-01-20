// Copyright (c) 2011 Nils Schneider
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#include "headers.h"

void ThreadFMI(void* parg)
{
    IMPLEMENT_RANDOMIZE_STACK(ThreadFMI(parg));
    try
    {
        vnThreadsRunning[5]++;
        ThreadFMI2(parg);
        vnThreadsRunning[5]--;
    }
    catch (std::exception& e) {
        vnThreadsRunning[5]--;
        PrintException(&e, "ThreadFMI()");
    } catch (...) {
        vnThreadsRunning[5]--;
        PrintException(NULL, "ThreadFMI()");
    }
    printf("ThreadFMI exiting\n");
}

int BindSocket(SOCKET *hSocket) {
	int nOne = 1;

	*hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*hSocket == INVALID_SOCKET)
	{
		printf("Error: Couldn't open socket for incoming connections (socket returned error %d)", WSAGetLastError());
		return false;
	}

	setsockopt(*hSocket, SOL_SOCKET, SO_REUSEADDR, (void*)&nOne, sizeof(int));

	if (fcntl(*hSocket, F_SETFL, O_NONBLOCK) == SOCKET_ERROR)
	{
		printf("Error: Couldn't set properties on socket for incoming connections (error %d)", WSAGetLastError());
		return false;
	}

    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY; // bind to all IPs on this computer
    sockaddr.sin_port = 8334;
    if (::bind(*hSocket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
    {
        int nErr = WSAGetLastError();
        if (nErr == WSAEADDRINUSE)
            printf("Unable to bind to port %d on this computer.  Bitcoin is probably already running.", ntohs(sockaddr.sin_port));
        else
            printf("Error: Unable to bind to port %d on this computer (bind returned error %d)", ntohs(sockaddr.sin_port), nErr);
        return false;
    }
    printf("Bound to port %d\n", ntohs(sockaddr.sin_port));

    // Listen for incoming connections
    if (listen(*hSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("Error: Listening for incoming connections failed (listen returned error %d)", WSAGetLastError());
        return false;
	}

	return true;
}

void ThreadFMI2(void* parg)
{
	SOCKET hSocket;
	vector<CMiner*> vMiners;

    printf("ThreadFMI started\n");

	if (!BindSocket(&hSocket))
	{
		printf("Error: BindSocket() failed\n");
		return;
	}

	loop
	{
		if (fShutdown)
			break;

		/*
		 * CreateNewBlock
		 * update extranonce (IncrementExtraNonce)
		 * store block in miner
		 */

		/* update from AcceptBlock or SetBestChain */

		/* miner acks every message */

		
	}

	/* FIXME: cleanup sockets */
}
