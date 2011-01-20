// Copyright (c) 2011 Nils Schneider
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

class CMiner;

void ThreadFMI(void* parg);
void ThreadFMI2(void* parg);

class CMiner
{
	public:
		SOCKET hSocket;
		bool fDisconnect;
		CBlock* pblock;
}
