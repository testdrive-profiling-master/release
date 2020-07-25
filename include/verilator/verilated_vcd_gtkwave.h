//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
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
// Title : Verilator plugins
// Rev.  : 11/5/2019 Tue (clonextop@gmail.com)
//================================================================================
#include "verilated_vcd_c.h"
#ifndef __VERILATED_VCD_GTKWAVE_H__
#define __VERILATED_VCD_GTKWAVE_H__
#include <string>
#ifdef WIN32
#include <windows.h>
#endif

class VerilatedVcdGTKWave : public VerilatedVcdFile
{
public:
	VerilatedVcdGTKWave();
	virtual ~VerilatedVcdGTKWave();

	virtual bool open(const std::string& name);
	virtual void close();
	virtual ssize_t write(const char* buf, ssize_t len);

private:
#ifdef _VCD_GTKWAVE_DEBUG_
	int consume(void);
#endif
	void emit(const char *s);
	unsigned int get_8(char *p);
	unsigned int get_32(char *p);
	void put_8(char *p, unsigned int v);
	void put_32(char *p, unsigned int v);

	bool			m_bOpened;
	bool			m_bGtkStarted;
	std::string		m_str;
	HANDLE			m_hMapFile;
	struct{
		char*		buf;
		char*		top;
		char*		cur;
		char*		consume;
	}m_fifo;
};

#endif//__VERILATED_VCD_GTKWAVE_H__
