/* cmp.c --- Comparison operators.
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


// Parses a comparison operator from an opcode.
// return: The proper operator or NULL if `op` is not an `c16_opcmp`.
c16_opcmp c16_opcmp_parse(c16_halfword op){
    switch((op / 4) * 4){
    case OP_GTE_:
        return c16_opcmp_gte;
    case OP_LTE_:
        return c16_opcmp_lte;
    case OP_EQ_:
        return c16_opcmp_eq;
    case OP_NEQ_:
        return c16_opcmp_neq;
    case OP_GT_:
        return c16_opcmp_gt;
    case OP_LT_:
        return c16_opcmp_lt;
    }
    return 0;
}


c16_word c16_opcmp_gte(c16_word a,c16_word b){
    return a >= b;
}


c16_word c16_opcmp_lte(c16_word a,c16_word b){
    return a <= b;
}


c16_word c16_opcmp_eq(c16_word a,c16_word b){
    return a == b;
}


c16_word c16_opcmp_neq(c16_word a,c16_word b){
    return a != b;
}


c16_word c16_opcmp_gt(c16_word a,c16_word b){
    return a > b;
}


c16_word c16_opcmp_lt(c16_word a,c16_word b){
    return a < b;
}
