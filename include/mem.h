/* mem.h --- Memory model interface.
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

#ifndef c16_MEMORY_H
#define c16_MEMORY_H

#include <stdio.h>

#include "c16.h"
#include "regs.h"


// The addressable memory space of the 16candles runtime.
typedef struct{
    int           m_fd;        // The file this memory is mapped to.
    const char *  m_filepath;  // The name of the file that this is mapped to.
    c16_halfword *m_mem;       // The memory segment.
    c16_word      m_sptstart;  // The initial stackpointer start.
}c16_mem;


// Initializes a `c16_mem` structure. The `filepath` argument should be a
// string that is the filepath to the addressable memory space file. If this
// is NULL, the default file will be used.
// return: A pointer to the initialized memory or NULL.
c16_mem *c16_mem_init(c16_mem*,const char*);


// Cleanup a `c16_mem` structure that was initialized with `c16_init_mem`.
void c16_mem_destroy(c16_mem*);


// return: The address of the stdin buffer in memory.
c16_halfword *c16_mem_inputv(const c16_mem*);


// return: The address of the stdin buffer count value in memory.
c16_halfword *c16_mem_inputc(const c16_mem*);


// return: The address of the stdin buffer count value in memory.
c16_halfword *c16_mem_inputb(const c16_mem*);


// Loads `data` into memory. This assigns the stack pointer (`spt`) to the
// next addressable address.
// return: Zero on success, nonzero on failure.
int c16_mem_loaddata(c16_mem*,c16_regs*,c16_word,const c16_halfword*);


// Loads `data` into memory with an offset of `o`. This assigns the stack
// pointer (`spt`) to the next addressable address.
// return: Zero on success, nonzero on failure.
int c16_mem_loaddata_offset(c16_mem*,
                            c16_regs*,
                            c16_word,
                            const c16_halfword*,
                            c16_word);


// Loads `f` into memory. This assigns the stack pointer (`spt`) to the next
// addressable address.
// return: Zero on success, nonzero on failure.
int c16_mem_loadfile(c16_mem*,c16_regs*,FILE*);


// Loads `f` into memory with an offset of `o`. This assigns the stack pointer
// (`spt`) to the next addressable address.
// return: Zero on success, nonzero on failure.
int c16_mem_loadfile_offset(c16_mem*,c16_regs*,FILE*,c16_word);


// Grabs an immediate word out of memory advancing the instruction pointer.
// return: The next immediate word after `ipt`.
c16_word c16_mem_fillword(const c16_mem*,c16_regs*);


// Grabs an immediate halfword out of memory advancing the instruction pointer.
// return: The next immediate word after `ipt`.
c16_halfword c16_mem_fillhalfword(const c16_mem*,c16_regs*);


// Fills `a` and `b` with the next 2 words.
void c16_mem_fill_lit_lit(const c16_mem*,c16_regs*,c16_word*,c16_word*);


// Fills `a` with the next word and `b` with the next halfword.
void c16_mem_fill_lit_reg(const c16_mem*,c16_regs*,c16_word*,c16_word*);


// Fills `a` with the next halfword and `b` with the next word.
void c16_mem_fill_reg_lit(const c16_mem*,c16_regs*,c16_word*,c16_word*);


// Fills `a` and `b` with the next two halfwords.
void c16_mem_fill_reg_reg(const c16_mem*,c16_regs*,c16_word*,c16_word*);


// Loads `w` into memory at `addr`.
// return: The value loaded into memory.
c16_word c16_mem_loadword(const c16_mem*,c16_word,c16_word);


// Loads `h` into memory at `addr`.
// return: The value loaded into memory.
c16_word c16_mem_loadhalfword(const c16_mem*,c16_halfword,c16_word);


// return: The `c16_word` in memory at `addr`.
c16_word c16_mem_getword(const c16_mem*,c16_word);


// return: The `c16_halfword` in memory at `addr`.
c16_halfword c16_mem_gethalfword(const c16_mem*,c16_word);


#endif
