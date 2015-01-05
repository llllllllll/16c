/* bin.c --- Binary operators.
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

#include "c16.h"
#include "op.h"


// Parses a binary operator from an opcode.
// return: The proper operator or NULL if `op` is not an `c16_opbin`.
c16_opbin c16_opbin_parse(c16_halfword op){
    switch((op / 4) * 4){
    case OP_AND_:
        return c16_opbin_and;
    case OP_OR_:
        return c16_opbin_or;
    case OP_XAND_:
        return c16_opbin_xand;
    case OP_XOR_:
        return c16_opbin_xor;
    case OP_LSHIFT_:
        return c16_opbin_lshift;
    case OP_RSHIFT_:
        return c16_opbin_rshift;
    case OP_ADD_:
        return c16_opbin_add;
    case OP_SUB_:
        return c16_opbin_sub;
    case OP_MUL_:
        return c16_opbin_mul;
    case OP_DIV_:
        return c16_opbin_div;
    case OP_MOD_:
        return c16_opbin_mod;
    case OP_MIN_:
        return c16_opbin_min;
    case OP_MAX_:
        return c16_opbin_max;
    }
    return 0;
}


c16_word c16_opbin_and(c16_word a,c16_word b){
    return a & b;
}


c16_word c16_opbin_or(c16_word a,c16_word b){
    return a | b;
}


c16_word c16_opbin_xand(c16_word a,c16_word b){
    return ~(a ^ b);
}


c16_word c16_opbin_xor(c16_word a,c16_word b){
    return a ^ b;
}


c16_word c16_opbin_lshift(c16_word a,c16_word b){
    return a << b;
}


c16_word c16_opbin_rshift(c16_word a,c16_word b){
    return a >> b;
}


c16_word c16_opbin_add(c16_word a,c16_word b){
    return a + b;
}


c16_word c16_opbin_sub(c16_word a,c16_word b){
    return a - b;
}


c16_word c16_opbin_mul(c16_word a,c16_word b){
    return a * b;
}


c16_word c16_opbin_div(c16_word a,c16_word b){
    return a / b;
}


c16_word c16_opbin_mod(c16_word a,c16_word b){
    return a % b;
}


c16_word c16_opbin_min(c16_word a,c16_word b){
    return (a < b) ? a : b;
}


c16_word c16_opbin_max(c16_word a,c16_word b){
    return (a > b) ? a : b;
}
