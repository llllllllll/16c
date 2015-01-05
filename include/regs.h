/* regs.c --- Register modeling interface.
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

#ifndef c16_REGISTER_H
#define c16_REGISTER_H

#include "c16.h"


// The registers of the 16candles runtime.
// The union lets us say: regs.ipt OR regs.ipt_f without a second level of
// attribute access. This is done for sugar and to make sure the front and back
// halves of the register are pointing to the proper place.
typedef struct{
    // Instruction pointer.
    union{
        struct{
            c16_halfword ipt_f;
            c16_halfword ipt_b;
        };
        c16_word ipt;
    };

    // Stack pointer.
    union{
        struct{
            c16_halfword spt_f;
            c16_halfword spt_b;
        };
        c16_word spt;
    };

    // Accumulator 1.
    union{
        struct{
            c16_halfword ac1_f;
            c16_halfword ac1_b;
        };
        c16_word ac1;
    };

    // Accumulator 2.
    union{
        struct{
            c16_halfword ac2_f;
            c16_halfword ac2_b;
        };
        c16_word ac2;
    };

    // Test register.
    union{
        struct{
            c16_halfword tst_f;
            c16_halfword tst_b;
        };
        c16_word tst;
    };

    // Input register.
    union{
        struct{
            c16_halfword inp_r;  // read
            c16_halfword inp_w;  // write
        };
        c16_word inp;
    };

    // Unused registers 0-9. These are for the programmer.
    union{
        struct{
            c16_halfword r0_f;
            c16_halfword r0_b;
        };
        c16_word r0;
    };
    union{
        struct{
            c16_halfword r1_f;
            c16_halfword r1_b;
        };
        c16_word r1;
    };
    union{
        struct{
            c16_halfword r2_f;
            c16_halfword r2_b;
        };
        c16_word r2;
    };
    union{
        struct{
            c16_halfword r3_f;
            c16_halfword r3_b;
        };
        c16_word r3;
    };
    union{
        struct{
            c16_halfword r4_f;
            c16_halfword r4_b;
        };
        c16_word r4;
    };
    union{
        struct{
            c16_halfword r5_f;
            c16_halfword r5_b;
        };
        c16_word r5;
    };
    union{
        struct{
            c16_halfword r6_f;
            c16_halfword r6_b;
        };
        c16_word r6;
    };
    union{
        struct{
            c16_halfword r7_f;
            c16_halfword r7_b;
        };
        c16_word r7;
    };
    union{
        struct{
            c16_halfword r8_f;
            c16_halfword r8_b;
        };
        c16_word r8;
    };
    union{
        struct{
            c16_halfword r9_f;
            c16_halfword r9_b;
        };
        c16_word r9;
    };
}c16_regs;


// Initializes a `c16_regs` structure.
// return: A pointer to the initialized registers or NULL.
c16_regs *c16_regs_init(c16_regs*);


// Cleans up a `c16_regs` structure that was initialized with
// `c16_regs_init`
void c16_regs_destory(c16_regs*);


// return: true iff this word is a subreg opcode.
c16_bool c16_regs_issubreg(c16_halfword);


// Returns the register from the given halfword.
// return: A pointer to the reg or subreg that the opcode describes.
// If this `c16_halfword` does not describe a register, then this returns NULL.
void *c16_regs_parse(c16_regs*,c16_halfword);


// Prints the state of registers to stdout.
void c16_regs_dump(c16_regs*);


#endif
