/*
	Command.hpp: Part of the WDM Video Capture Driver for Bt848 based video capture cards
	Copyright (C) 2000 - 2002 by Eduardo Jos?Tagle.
 
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the Free
	Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef __COMMAND_H
#define __COMMAND_H

/* Type: Instruction
 * Purpose: enumerates all RISC commands
 */
typedef enum
{
   WRIT    = 0x01, SKIP = 0x02,
   JUMP    = 0x07, SYNC = 0x08
} Instruction;

/* Type: SyncCode
 * Purpose: enumerates all sync codes coming out of decoder
*/
typedef enum
{
   SC_FM1 = 0x6, SC_VRE = 0x4,	SC_VRO = 0xC, SC_EOL4 = 0x1, SC_SOL = 0x02
} SyncCode;

// Macros to generate the RISC instructions as fast as possible!
#define COUNT_MSK & 0x3FFF
#define REP_MSK & 0x3FF
#define OP_MSK & 0xF
#define STAT_MSK & 0xF

// The following sequence forces the compiler to create the RISC instruction and write it at once as the last step. 
// Compiler is not allowed to write operands as soon they are calculated: It calculates all the required bytes
// and then writes as a single operation all the instructions
static DWORD __r_buff[2];

#define UPD(ptr)			((volatile DWORD*)ptr)[0] = __r_buff[0]; \
							((volatile DWORD*)ptr)[1] = __r_buff[1]; \
							ptr += 2

#define WDW(addr,off,val) __r_buff[(off)] = (DWORD)(val)

// RISC instruction core
#define RISC_CORE(count,repeat,opcode) \
					(count COUNT_MSK) | \
					((opcode OP_MSK) << 28 ) | \
					((repeat REP_MSK) << 14 )





// Useful macros
#define RISC_WRITE(instrptr,physaddr,count, sol, eol, irq, ldclr, repeat) \
		{WDW(instrptr,0,RISC_CORE(count,repeat, 0x1) | (sol?0x08000000:0) | (eol?0x04000000:0) | (irq?0x01000000:0) | (ldclr?0x02000000:0) ); \
		WDW(instrptr,1,physaddr); \
		UPD(instrptr); }

#define RISC_REGW(instrptr, irq, ldclr, addr, data) \
		{ WDW(instrptr,0,RISC_CORE(addr, 0, 0x4) | (irq?0x01000000:0) | (ldclr?0x02000000:0) ); \
		WDW(instrptr,1,data); \
		UPD(instrptr); }

#define RISC_JUMP(instrptr,physaddr, irq) \
		{ WDW(instrptr,0,RISC_CORE(0,0,0x7) | (irq?0x01000000:0) ); \
		WDW(instrptr,1,physaddr); \
		UPD(instrptr); }

#define RISC_SYNC(instrptr,status, irq, ldclr)	\
		{ WDW(instrptr,0,RISC_CORE(status STAT_MSK,0,0x8)| (irq?0x01000000:0) | (ldclr?0x02000000:0) ); \
		WDW(instrptr,1,0); \
		UPD(instrptr); }

#endif __COMMAND_H
