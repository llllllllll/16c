/* proc.c --- Processing loop.
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "c16.h"
#include "proc.h"
#include "op.h"
#include "mem.h"
#include "regs.h"


// Run one processor tick.
// return: Zero on success, -1 when OP_TERM is read.
int c16_proc_tick(c16_regs *regs,const c16_mem *mem){
    c16_halfword op = c16_mem_fillhalfword(mem,regs);

    if (op == OP_TERM){                     // exit case
        return -1;
    }
    if (c16_op_is_opbin(op)){               // binary operators
        c16_opbin_apply(op,regs,mem);
    }else if (c16_op_is_opcmp(op)){         // comparison operators
        c16_opcmp_apply(op,regs,mem);
    }else if (c16_op_is_opun(op)){          // unary operators
        c16_opun_apply(op,regs,mem);
    }else if ((op >> 1) << 1 == OP_PUSH_){  // op push
        c16_oppush_apply(op,regs,mem);
    }else if (c16_is_opjmp(op)){            // jump operations
        c16_opjmp_apply(op,regs,mem);
    }else if (op <= OP_WRITE_REG){          // escaping write
        c16_opwrite_apply(op,regs,mem);
    }else if (op <= OP_MSET_MEMREG){        // memset operations
        c16_opmset_apply(op,regs,mem);
    }else if (op == OP_SWAP){               // swap operator
        c16_opswap_apply(regs,mem);
    }else if (op == OP_POP){                // pop operator
        c16_oppop_apply(regs,mem);
    }else if (op == OP_PEEK){               // peek operator
        c16_oppeek_apply(regs,mem);
    }else if (op == OP_FLUSH){              // flush the stack operator
        c16_opflush_apply(regs,mem);
    }else if (op == OP_READ){               // escaping read
        c16_opread_apply(regs,mem);
    }

    return 0;
}

// Subtracts two timeval structures storing the result in the result struct.
// Returns 1 if result is negative.
// Source: http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
static int timeval_subtract(struct timeval *result,
                            struct timeval *x,
                            struct timeval *y){
    int nsec;
    if (x->tv_usec < y->tv_usec){
        nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000){
        nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;
    return x->tv_sec < y->tv_sec;
}


// Run the processor until termination, executing the current program in RAM.
// The parameter is the minimum length in micro seconds of a single tick.
// If tick_len is <= 0, no time checking shall occur (program runs with
// unbounded speed and ticks will have variable length).
void c16_proc_run(c16_regs *regs,const c16_mem *mem,long tick_len){
    // Difference, Before, After.
    struct timeval d;
    struct timeval b;
    struct timeval a;
    int            pt;

    if (tick_len <= 0){
        while(c16_proc_tick(regs,mem) != -1);
    }else{
        do{
            gettimeofday(&b,NULL);
            pt = c16_proc_tick(regs,mem);
            for (gettimeofday(&a,NULL),timeval_subtract(&d,&a,&b);
                 d.tv_usec < tick_len;
                 gettimeofday(&a,NULL),timeval_subtract(&d,&a,&b));
        }while(pt != -1);
    }
}


// Process the stdin.
// Argument should be a `c16_proc_stdin_args`.
// Return is always `NULL`.
void *c16_process_stdin(void *args){
    c16_mem      *mem  = ((c16_proc_stdin_args*) args)->a_mem;
    c16_regs     *regs = ((c16_proc_stdin_args*) args)->a_regs;
    c16_halfword *inputc;
    int           c;

    while ((c = getchar()) != EOF){
        c16_mem_inputv(mem)[regs->inp_w++] = (c16_halfword) c;
        if (*(inputc = c16_mem_inputc(mem)) < 256){
            ++(*inputc);
        }
    }
    *c16_mem_inputb(mem) = c16_false;
    return NULL;
}
