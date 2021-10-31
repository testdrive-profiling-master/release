//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
// 
// Redistribution and use in source and binary forms,
// with or without modification, are permitted provided
// that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
// 
// Title : Simulation HDL module
// Rev.  : 10/31/2021 Sun (clonextop@gmail.com)
//================================================================================
#include "verilated_vcd_gtkwave.h"
// VerilatedVcdGtkWave
#include <process.h>
#define WAVE_PARTIAL_VCD_RING_BUFFER_SIZE (1024*1024)

VerilatedVcdGTKWave::VerilatedVcdGTKWave()
{
	m_hMapFile		= NULL;
	m_bOpened		= false;
	m_bGtkStarted	= false;
	memset(&m_fifo, 0, sizeof(m_fifo));
}

VerilatedVcdGTKWave::~VerilatedVcdGTKWave()
{
	if(m_hMapFile) {
		if(m_fifo.top == m_fifo.buf) 	// wait once consume at least...
			for(;;) {
				while(!*m_fifo.top) {
					int blksiz;

					if((blksiz = get_32(m_fifo.top + 1))) {
						m_fifo.top += 1 + 4 + blksiz;

						if(m_fifo.top >= (m_fifo.buf + WAVE_PARTIAL_VCD_RING_BUFFER_SIZE))
							m_fifo.top -= WAVE_PARTIAL_VCD_RING_BUFFER_SIZE;
					} else {
						break;
					}
				}

				if(m_fifo.top == m_fifo.cur) break;

				Sleep(10);
			}

		if(m_fifo.buf) UnmapViewOfFile(m_fifo.buf);

		CloseHandle(m_hMapFile);
		m_hMapFile	= NULL;
	}
}

bool VerilatedVcdGTKWave::open(const std::string& name)
{
	if(!m_bOpened) {
		// run gtkwave
		char sTestDrivePath[MAX_PATH];
		int pid	= _getpid();
		{
			// check TestDrive's 'GTKWave' path
			if(!GetEnvironmentVariableA("TESTDRIVE_DIR", sTestDrivePath, MAX_PATH)) {
				MessageBox(NULL, TEXT("TestDrive is not installed normally."), TEXT("Warning"), MB_OK | MB_ICONHAND);
				return false;
			}
		}
		{
			// create map file
			char mapName[65];
			sprintf(mapName, "shmidcat%d", pid);
			m_hMapFile	= CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, WAVE_PARTIAL_VCD_RING_BUFFER_SIZE, mapName);

			if(m_hMapFile) {
				m_fifo.consume	=
					m_fifo.buf		=
						m_fifo.cur		=
							m_fifo.top		= (char*)MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, WAVE_PARTIAL_VCD_RING_BUFFER_SIZE);
				memset(m_fifo.buf, 0, WAVE_PARTIAL_VCD_RING_BUFFER_SIZE);
			} else {
				return false;
			}
		}
		{
			// set gtkwave's 'SHMID' environment
			char sID[16];
			sprintf(sID, "%08X", pid);
			SetEnvironmentVariableA("SHMID", sID);
		}
		{
			std::string sCmd("\"");
			sCmd	+= sTestDrivePath;
			sCmd	+= "bin\\msys64\\ucrt64\\bin\\gtkwave\" -r \"";
			sCmd	+= sTestDrivePath;
			sCmd	+= "bin\\gtkwave.ini\" -v -I ";

			if(!name.empty()) {
				sCmd	+= "\"";
				sCmd	+= name;
				sCmd	+= "\"";
			}

			{
				STARTUPINFO si;
				SECURITY_ATTRIBUTES sa;
				PROCESS_INFORMATION pi;
				sa.nLength				= sizeof(SECURITY_ATTRIBUTES);
				sa.lpSecurityDescriptor	= NULL;
				sa.bInheritHandle		= TRUE;
				ZeroMemory(&si, sizeof(STARTUPINFO));
				si.cb			= sizeof(STARTUPINFO);
				si.dwFlags		= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
				si.hStdOutput	= NULL;
				si.hStdInput	= NULL;
				si.hStdError	= NULL;
				si.wShowWindow	= SW_SHOWMAXIMIZED;
				CreateProcess(NULL, (LPSTR)sCmd.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			}
		}
		m_bOpened	= true;
	}

	return true;
}

void VerilatedVcdGTKWave::close()
{
	if(m_fifo.consume && !m_bGtkStarted) {
		int iTimeOut	= 20;

		while(*((volatile char*)m_fifo.consume) && iTimeOut) {
			Sleep(100);	// wait last consume.
			iTimeOut--;
		}
	}
}

ssize_t VerilatedVcdGTKWave::write(const char* buf, ssize_t len)
{
	m_str.append(buf, len);
	{
		int pos;
		std::string sub_string;

		while((pos = m_str.find('\n')) != std::string::npos) {
			sub_string	= m_str.substr(0, pos + 1);
			emit(sub_string.c_str());
			m_str.erase(0, pos + 1);
		}
	}
	return len;
}

unsigned int VerilatedVcdGTKWave::get_8(char* p)
{
	if(p >= (m_fifo.buf + WAVE_PARTIAL_VCD_RING_BUFFER_SIZE))
		p -= WAVE_PARTIAL_VCD_RING_BUFFER_SIZE;

	return((unsigned int)((unsigned char) * p));
}

unsigned int VerilatedVcdGTKWave::get_32(char* p)
{
	unsigned int rc;
	rc =	(get_8(p++) << 24);
	rc |=	(get_8(p++) << 16);
	rc |=	(get_8(p++) <<  8);
	rc |=	(get_8(p)   <<  0);
	return(rc);
}

void VerilatedVcdGTKWave::put_8(char* p, unsigned int v)
{
	if(p >= (m_fifo.buf + WAVE_PARTIAL_VCD_RING_BUFFER_SIZE))
		p -= WAVE_PARTIAL_VCD_RING_BUFFER_SIZE;

	*p = (unsigned char)v;
}

void VerilatedVcdGTKWave::put_32(char* p, unsigned int v)
{
	put_8(p++, (v >> 24));
	put_8(p++, (v >> 16));
	put_8(p++, (v >> 8));
	put_8(p, (v >> 0));
}

#ifdef _VCD_GTKWAVE_DEBUG_
int VerilatedVcdGTKWave::consume(void) 	// for testing only...similar code also is on the receiving end in gtkwave
{
	char mybuff[32769];
	int rc;

	if((rc = *m_fifo.consume)) {
		unsigned int len = get_32(m_fifo.consume + 1);
		unsigned int i;

		for(i = 0; i < len; i++) {
			mybuff[i] = get_8(m_fifo.consume + i + 5);
		}

		mybuff[i] = 0;
		printf("%s", mybuff);
		*m_fifo.consume		= 0;	// 소비 했음.
		m_fifo.consume		= m_fifo.consume + i + 5;

		if(m_fifo.consume >= (m_fifo.buf + WAVE_PARTIAL_VCD_RING_BUFFER_SIZE)) {
			m_fifo.consume -= WAVE_PARTIAL_VCD_RING_BUFFER_SIZE;
		}
	}

	return(rc);
}
#endif

void VerilatedVcdGTKWave::emit(const char* s)
{
	int len = strlen(s);
	ssize_t l_top, l_curr;
	int consumed;
	int blksiz;

	for(;;) {
		while(!*m_fifo.top) {
			if((blksiz = get_32(m_fifo.top + 1))) {
				m_fifo.top += 1 + 4 + blksiz;

				if(m_fifo.top >= (m_fifo.buf + WAVE_PARTIAL_VCD_RING_BUFFER_SIZE))
					m_fifo.top -= WAVE_PARTIAL_VCD_RING_BUFFER_SIZE;

				m_bGtkStarted	= true;
			} else {
				break;
			}
		}

		l_top = (ssize_t)m_fifo.top;
		l_curr = (ssize_t)m_fifo.cur;

		if(l_curr >= l_top)
			consumed = (int)(l_curr - l_top);
		else
			consumed = (int)((l_curr + WAVE_PARTIAL_VCD_RING_BUFFER_SIZE) - l_top);

		if((consumed + len + 16) > WAVE_PARTIAL_VCD_RING_BUFFER_SIZE) { // just a guardband, it's oversized
			Sleep(10);
			continue;
		} else {
			const char* ss;
			char* sd;
			put_32(m_fifo.cur + 1, len);
			sd = m_fifo.cur + 1 + 4;
			ss = s;

			while(*ss) {
				put_8(sd, *ss);
				ss++;
				sd++;
			}

			put_8(sd, 0);		// next valid
			put_32(sd + 1, 0);	// next len
			put_8(m_fifo.cur, 1); // current valid
			m_fifo.cur += 1 + 4 + len;

			if(m_fifo.cur >= (m_fifo.buf + WAVE_PARTIAL_VCD_RING_BUFFER_SIZE))
				m_fifo.cur -= WAVE_PARTIAL_VCD_RING_BUFFER_SIZE;

			break;
		}
	}
}
