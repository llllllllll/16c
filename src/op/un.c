/* un.c --- Unary operators.
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


// Parses a unary operator from an opcode.
// return: The proper operator or NULL if `op` is not an `c16_opun`.
c16_opun c16_opun_parse(c16_halfword op){
    switch((op / 2) * 2){
    case OP_INV_:
        return c16_opun_inv;
    case OP_INC_:
        return c16_opun_inc;
    case OP_DEC_:
        return c16_opun_dec;
    case OP_SET_:
        return c16_opun_set;
    }
    return 0;
}


c16_word c16_opun_inv(c16_word a){
    return ~a;
}


c16_word c16_opun_inc(c16_word a){
    return ++a;
}


c16_word c16_opun_dec(c16_word a){
    return --a;
}


c16_word c16_opun_set(c16_word a){
    return a;
}
