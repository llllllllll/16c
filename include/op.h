/* op.h --- Operator interface.
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

#ifndef c16_OP_H
#define c16_OP_H

#include "c16.h"
#include "regs.h"
#include "mem.h"


// Typedefs for the operation implementation functions.
typedef c16_word (*c16_opbin)(c16_word,c16_word);
typedef c16_opbin c16_opcmp;
typedef c16_word (*c16_opun)(c16_word);


// return: true iff this word is a subreg opcode.
c16_bool c16_op_is_subreg(c16_halfword);


// return: true iff this opcode is a binary operator.
c16_bool c16_op_is_opbin(c16_halfword);


// return: true iff this opcode is a comparison operator.
c16_bool c16_op_is_opcmp(c16_halfword);


// return: true iff this opcode is a unary operator.
c16_bool c16_op_is_opun(c16_halfword);


// return: true iff this opcode is a jump operator.
c16_bool c16_is_opjmp(c16_halfword);


// Binary operation handling.
void c16_opbin_apply(c16_halfword,c16_regs*,const c16_mem*);

// Parses a binary operator from an opcode.
// return: The proper operator or NULL if `op` is not an `c16_opbin`.
c16_opbin c16_opbin_parse(c16_halfword);


// Binary operator implementations.
c16_word c16_opbin_and(c16_word,c16_word);
c16_word c16_opbin_or(c16_word,c16_word);
c16_word c16_opbin_xand(c16_word,c16_word);
c16_word c16_opbin_xor(c16_word,c16_word);
c16_word c16_opbin_lshift(c16_word,c16_word);
c16_word c16_opbin_rshift(c16_word,c16_word);
c16_word c16_opbin_add(c16_word,c16_word);
c16_word c16_opbin_sub(c16_word,c16_word);
c16_word c16_opbin_mul(c16_word,c16_word);
c16_word c16_opbin_div(c16_word,c16_word);
c16_word c16_opbin_mod(c16_word,c16_word);
c16_word c16_opbin_min(c16_word,c16_word);
c16_word c16_opbin_max(c16_word,c16_word);


// Parses a comparison operator from an opcode.
// return: The proper operator or NULL if `op` is not an `c16_opcmp`.
c16_opcmp c16_opcmp_parse(c16_halfword);


// Comparison operator handling.
void c16_opcmp_apply(c16_halfword,c16_regs*,const c16_mem*);


// Comparison operator implementations.
c16_word c16_opcmp_gte(c16_word,c16_word);
c16_word c16_opcmp_lte(c16_word,c16_word);
c16_word c16_opcmp_eq(c16_word,c16_word);
c16_word c16_opcmp_neq(c16_word,c16_word);
c16_word c16_opcmp_gt(c16_word,c16_word);
c16_word c16_opcmp_lt(c16_word,c16_word);


// Parses a unary operator from an opcode.
// return: The proper operator or NULL if `op` is not an `c16_opun`.
c16_opun c16_opun_parse(c16_halfword);


// Unary operator handling.
void c16_opun_apply(c16_halfword,c16_regs*,const c16_mem*);


// Jump operations handler.
void c16_opjmp_apply(c16_halfword,c16_regs*,const c16_mem*);


// Unary operator implementations.
c16_word c16_opun_inv(c16_word);
c16_word c16_opun_inc(c16_word);
c16_word c16_opun_dec(c16_word);
c16_word c16_opun_set(c16_word);


// Misc handlers.
void c16_oppush_apply(c16_halfword,c16_regs*,const c16_mem*);
void c16_opwrite_apply(c16_halfword,c16_regs*,const c16_mem*);
void c16_opmset_apply(c16_halfword,c16_regs*,const c16_mem*);
void c16_opswap_apply(c16_regs*,const c16_mem*);
void c16_oppop_apply(c16_regs*,const c16_mem*);
void c16_oppeek_apply(c16_regs*,const c16_mem*);
void c16_opflush_apply(c16_regs*,const c16_mem*);
void c16_opread_apply(c16_regs*,const c16_mem*);


#endif
