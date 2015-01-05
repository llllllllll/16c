/* regs.c --- Register modeling.
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
#include <string.h>

#include "c16.h"
#include "regs.h"


// Initializes a `c16_regs` structure.
// return: A pointer to the initialized registers or NULL.
c16_regs *c16_regs_init(c16_regs *regs){
    return memset(regs,0,sizeof(c16_regs));
}


// Cleans up a `c16_regs` structure that was initialized with
// `c16_regs_init`
void c16_regs_destory(c16_regs *regs){
    // Placeholder in case `c16_regs_init` ever changes to need cleanup.
}


// return: true iff this word is a subreg opcode.
c16_bool c16_regs_issubreg(c16_halfword op){
    return op > OP_r9;
}


// Returns the register from the given halfword.
// return: A pointer to the reg or subreg that the opcode describes.
// If this `c16_halfword` does not describe a register, then this returns NULL.
void *c16_regs_parse(c16_regs *regs,c16_halfword reg){
    switch(reg){
    case OP_ipt:   return &regs->ipt;
    case OP_spt:   return &regs->spt;
    case OP_ac1:   return &regs->ac1;
    case OP_ac2:   return &regs->ac2;
    case OP_tst:   return &regs->tst;
    case OP_inp:   return &regs->inp;
    case OP_inp_r: return &regs->inp_r;
    case OP_inp_w: return &regs->inp_w;
    case OP_r0:    return &regs->r0;
    case OP_r0_f:  return &regs->r0_f;
    case OP_r0_b:  return &regs->r0_b;
    case OP_r1:    return &regs->r1;
    case OP_r1_f:  return &regs->r1_f;
    case OP_r1_b:  return &regs->r1_b;
    case OP_r2:    return &regs->r2;
    case OP_r2_f:  return &regs->r2_f;
    case OP_r2_b:  return &regs->r2_b;
    case OP_r3:    return &regs->r3;
    case OP_r3_f:  return &regs->r3_f;
    case OP_r3_b:  return &regs->r3_b;
    case OP_r4:    return &regs->r4;
    case OP_r4_f:  return &regs->r4_f;
    case OP_r4_b:  return &regs->r4_b;
    case OP_r5:    return &regs->r5;
    case OP_r5_f:  return &regs->r5_f;
    case OP_r5_b:  return &regs->r5_b;
    case OP_r6:    return &regs->r6;
    case OP_r6_f:  return &regs->r6_f;
    case OP_r6_b:  return &regs->r6_b;
    case OP_r7:    return &regs->r7;
    case OP_r7_f:  return &regs->r7_f;
    case OP_r7_b:  return &regs->r7_b;
    case OP_r8:    return &regs->r8;
    case OP_r8_f:  return &regs->r8_f;
    case OP_r8_b:  return &regs->r8_b;
    case OP_r9:    return &regs->r9;
    case OP_r9_f:  return &regs->r9_f;
    case OP_r9_b:  return &regs->r9_b;
    default:       return NULL;
    }
}


// Prints the state of registers to stdout.
void c16_regs_dump(c16_regs *regs){
    printf(
        "ipt: 0x%04x\n"
        "spt: 0x%04x\n"
        "ac1: 0x%04x\n"
        "ac2: 0x%04x\n"
        "tst: 0x%04x\n"
        "inp: 0x%04x: inp_w: 0x%02x, inp_r: 0x%02x\n"
        "r0:  0x%04x: r0_f:  0x%02x, r0_b:  0x%02x\n"
        "r1:  0x%04x: r1_f:  0x%02x, r1_b:  0x%02x\n"
        "r2:  0x%04x: r2_f:  0x%02x, r2_b:  0x%02x\n"
        "r3:  0x%04x: r3_f:  0x%02x, r3_b:  0x%02x\n"
        "r4:  0x%04x: r4_f:  0x%02x, r4_b:  0x%02x\n"
        "r5:  0x%04x: r5_f:  0x%02x, r5_b:  0x%02x\n"
        "r6:  0x%04x: r6_f:  0x%02x, r6_b:  0x%02x\n"
        "r7:  0x%04x: r7_f:  0x%02x, r7_b:  0x%02x\n"
        "r8:  0x%04x: r8_f:  0x%02x, r8_b:  0x%02x\n"
        "r9:  0x%04x: r9_f:  0x%02x, r9_b:  0x%02x\n",
        regs->ipt,
        regs->spt,
        regs->ac1,
        regs->ac2,
        regs->tst,
        regs->inp,
        regs->inp_w,
        regs->inp_r,
        regs->r0,
        regs->r0_f,
        regs->r0_b,
        regs->r1,
        regs->r1_f,
        regs->r1_b,
        regs->r2,
        regs->r2_f,
        regs->r2_b,
        regs->r3,
        regs->r3_f,
        regs->r3_b,
        regs->r4,
        regs->r4_f,
        regs->r4_b,
        regs->r5,
        regs->r5_f,
        regs->r5_b,
        regs->r6,
        regs->r6_f,
        regs->r6_b,
        regs->r7,
        regs->r7_f,
        regs->r7_b,
        regs->r8,
        regs->r8_f,
        regs->r8_b,
        regs->r9,
        regs->r9_f,
        regs->r9_b);
}
