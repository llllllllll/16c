/* mem.c --- Memory model.
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

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "c16.h"
#include "mem.h"


const char * const default_filepath  = "/tmp/c16";
const c16_dword    c16_MEM_WIDTH     = 0x10000;
const c16_word     c16_VMEM_START    = 0x8000;
const c16_word     c16_INPUTC_OFFSET = 256;
const c16_word     c16_INPUTB_OFFSET = 257;


// Initializes a `c16_mem` structure. The `filepath` argument should be a
// string that is the filepath to the addressable memory space file. If this
// is NULL, the default file will be used.
// return: A pointer to the initialized memory or NULL.
c16_mem *c16_mem_init(c16_mem *mem,const char *filepath){

    // If the filepath is `NULL`: use the default filepath.
    filepath = (filepath) ? filepath : default_filepath;

    // Set the filepath and open the file.
    mem->m_filepath = filepath;
    if (!(mem->m_fd = open(filepath,O_RDWR | O_CREAT, S_IRUSR | S_IWUSR))){
        perror("c16_init_mem: open");
        return NULL;
    }

    // Allocate the proper space needed for the addressable memory space.
    if (fallocate(mem->m_fd,0,0,c16_MEM_WIDTH)){
        perror("c16_init_mem: fallocate");
        return NULL;
    }

    // Map the memory to the file.
    mem->m_mem = mmap(NULL,
                      c16_MEM_WIDTH,
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED,
                      mem->m_fd,
                      0);

    // Set that there is no input and that stdin is open.
    *((c16_halfword*) c16_mem_inputc(mem)) = 0;
    *((c16_halfword*) c16_mem_inputb(mem)) = c16_true;
}


// Cleanup a `c16_mem` structure that was initialized with `c16_init_mem`.
void c16_mem_destroy(c16_mem *mem){
    munmap(mem->m_mem,c16_MEM_WIDTH);
    close(mem->m_fd);
}



// return: The address of the stdin buffer in memory.
c16_halfword *c16_mem_inputv(const c16_mem *mem){
    return mem->m_mem + c16_VMEM_START;
}


// return: The address of the stdin buffer count value in memory.
c16_halfword *c16_mem_inputc(const c16_mem *mem){
    return mem->m_mem + c16_VMEM_START + c16_INPUTC_OFFSET;
}


// return: The address of the stdin buffer count value in memory.
c16_halfword *c16_mem_inputb(const c16_mem *mem){
    return mem->m_mem + c16_VMEM_START + c16_INPUTB_OFFSET;
}


// Loads `data` into memory. This assigns the stack pointer (`spt`) to the
// next addressable address.
// return: Zero on success, nonzero on failure.
int c16_mem_loaddata(c16_mem *mem,
                     c16_regs *regs,
                     c16_word len,
                     const c16_halfword *data){
    return c16_mem_loaddata_offset(mem,regs,len,data,0);
}


// Loads `data` into memory with an offset of `o`. This assigns the stack
// pointer (`spt`) to the next addressable address.
// return: Zero on success, nonzero on failure.
int c16_mem_loaddata_offset(c16_mem *mem,
                            c16_regs *regs,
                            c16_word len,
                            const c16_halfword *data,
                            c16_word o){
    memcpy(&mem->m_mem[o],data,len);
    regs->spt = mem->m_sptstart = len;
    return 0;
}


// Grabs an immediate word out of memory advancing the instruction pointer.
// return: The next immediate word after `ipt`.
c16_word c16_mem_fillword(const c16_mem *mem,c16_regs *regs){
    c16_word ret;

    ret = c16_mem_getword(mem,regs->ipt);
    regs->ipt += sizeof(c16_word);
    return ret;
}


// Grabs an immediate halfword out of memory advancing the instruction pointer.
// return: The next immediate word after `ipt`.
c16_halfword c16_mem_fillhalfword(const c16_mem *mem,c16_regs *regs){
    c16_halfword ret;

    ret = c16_mem_gethalfword(mem,regs->ipt);
    regs->ipt += sizeof(c16_halfword);
    return ret;
}


// Fills `a` and `b` with the next 2 words.
void c16_mem_fill_lit_lit(const c16_mem *mem,
                          c16_regs *regs,
                          c16_word *a,
                          c16_word *b){
    *a = c16_mem_fillword(mem,regs);
    *b= c16_mem_fillword(mem,regs);
}


// Fills `ac1` with the next word and `ac2` with the next halfword.
void c16_mem_fill_lit_reg(const c16_mem *mem,
                          c16_regs *regs,
                          c16_word *a,
                          c16_word *b){
    *a = c16_mem_fillword(mem,regs);
    *b = (c16_word) c16_mem_fillhalfword(mem,regs);
}


// Fills `ac1` with the next halfword and `ac2` with the next word.
void c16_mem_fill_reg_lit(const c16_mem *mem,
                          c16_regs *regs,
                          c16_word *a,
                          c16_word *b){
    *a = (c16_word) c16_mem_fillhalfword(mem,regs);
    *b = c16_mem_fillword(mem,regs);
}



// Fills `ac1` and `ac2` with the next two halfwords.
void c16_mem_fill_reg_reg(const c16_mem *mem,
                          c16_regs *regs,
                          c16_word *a,
                          c16_word *b){
    *a = (c16_word) c16_mem_fillhalfword(mem,regs);
    *b = (c16_word) c16_mem_fillhalfword(mem,regs);
}


// Loads `w` into memory at `addr`.
// return: The value loaded into memory.
c16_word c16_mem_loadword(const c16_mem *mem,c16_word w,c16_word addr){
    return *((c16_word*) (mem->m_mem + addr)) = w;
}


// Loads `h` into memory at `addr`.
// return: The value loaded into memory.
c16_word c16_mem_loadhalfword(const c16_mem *mem,c16_halfword h,c16_word addr){
    return mem->m_mem[addr] = h;
}


// return: The `c16_word` in memory at `addr`.
c16_word c16_mem_getword(const c16_mem *mem,c16_word addr){
    return *((c16_word*) (mem->m_mem + addr));
}


// return: The `c16_halfword` in memory at `addr`.
c16_halfword c16_mem_gethalfword(const c16_mem *mem,c16_word addr){
    return mem->m_mem[addr];
}
