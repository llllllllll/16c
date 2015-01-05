/* proc.h --- Processing loop interface.
   Copyright (c) 2015 Joe Jevnik.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along with
   this program; if not, write to the Free Software Foundation, Inc., 51
   Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef c16_PROC_H
#define c16_PROC_H

#include "c16.h"
#include "mem.h"
#include "regs.h"


// A structure to pass the arguments to the standard in processing thread.
typedef struct{
    c16_mem  *a_mem;   // The system memory.
    c16_regs *a_regs;  // The system registers.
}c16_proc_stdin_args;



// Run one processor tick.
// return: Zero on success, -1 when OP_TERM is read.
int c16_proc_tick(c16_regs*,const c16_mem*);


// Run the processor until termination, executing the current program in RAM.
// The parameter is the minimum length in micro seconds of a single tick.
// If tick_len is <= 0, no time checking shall occur (program runs with
// unbounded speed and ticks will have variable length).
void c16_proc_run(c16_regs*,const c16_mem*,long);


// Process the stdin.
// Argument should be a `c16_proc_stdin_args`.
// Return is always `NULL`.
void *c16_process_stdin(void*);


#endif
