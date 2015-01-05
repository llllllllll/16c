/* op.c --- Operators.
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
#include "mem.h"
#include "regs.h"


// return: true iff this opcode is a binary operator.
c16_bool c16_op_is_opbin(c16_halfword op){
    return op <= OP_MAX_REG_REG;
}


// return: true iff this opcode is a comparison operator.
c16_bool c16_op_is_opcmp(c16_halfword op){
    return !c16_op_is_opbin(op) && op <= OP_LT_REG_REG;
}


// return: true iff this opcode is a unary operator.
c16_bool c16_op_is_opun(c16_halfword op){
    return !c16_op_is_opcmp(op) && op <= OP_SET_REG;
}


// return: true iff this opcode is a jump operator.
c16_bool c16_is_opjmp(c16_halfword op){
    return !c16_op_is_opun(op) && ((op >> 1) << 1 != OP_PUSH_) && op <= OP_JMPF;
}


// Operator handling:


// Binary operation handling.
void c16_opbin_apply(c16_halfword op,c16_regs *regs,const c16_mem *mem){
    c16_word     a;
    c16_word     b;
    void        *reg;
    c16_halfword return_register_op;
    void        *return_register;
    c16_opbin    f = c16_opbin_parse(op);

    switch(op % 4){
    case LIT_LIT:
        c16_mem_fill_lit_lit(mem,regs,&a,&b);
        break;
    case LIT_REG:
        c16_mem_fill_lit_reg(mem,regs,&a,&b);
        reg = c16_regs_parse(regs,(c16_halfword) a);
        if (c16_regs_issubreg(b)){
            b = *((c16_halfword*) reg);
        }else{
            b = *((c16_word*) reg);
        }
        break;
    case REG_LIT:
        c16_mem_fill_reg_lit(mem,regs,&a,&b);
        reg = c16_regs_parse(regs,(c16_halfword) a);
        if (c16_regs_issubreg(a)){
            a = *((c16_halfword*) reg);
        }else{
            a = *((c16_word*) reg);
        }
        break;
    case REG_REG:
        c16_mem_fill_reg_reg(mem,regs,&a,&b);
        reg = c16_regs_parse(regs,(c16_halfword) a);
        if (c16_regs_issubreg(a)){
            a = *((c16_halfword*) reg);
        }else{
            a = *((c16_word*) reg);
        }

        reg = c16_regs_parse(regs,(c16_halfword) a);
        if (c16_regs_issubreg(b)){
            b = *((c16_halfword*) reg);
        }else{
            b = *((c16_word*) reg);
        }
        break;
    }

    return_register_op = c16_mem_fillhalfword(mem,regs);
    return_register    = c16_regs_parse(regs,return_register_op);

    if (c16_regs_issubreg(return_register_op)){
        *((c16_halfword*) return_register) = (c16_halfword) f(a,b);
    }else{
        *((c16_word*) return_register) = f(a,b);
    }
}


// Comparison operator handling.
void c16_opcmp_apply(c16_halfword op,c16_regs *regs,const c16_mem *mem){
    c16_word     a;
    c16_word     b;

    switch(op % 4){
    case LIT_LIT:
        c16_mem_fill_lit_lit(mem,regs,&a,&b);
        break;
    case LIT_REG:
        c16_mem_fill_lit_reg(mem,regs,&a,&b);
        break;
    case REG_LIT:
        c16_mem_fill_reg_lit(mem,regs,&a,&b);
        break;
    case REG_REG:
        c16_mem_fill_reg_reg(mem,regs,&a,&b);
        break;
    }

    regs->tst = c16_opcmp_parse(op)(a,b);
}


// Unary operator handling.
void c16_opun_apply(c16_halfword op,c16_regs *regs,const c16_mem *mem){
    c16_word     a;
    c16_halfword return_register_op;
    void        *return_register;
    c16_opun     f = c16_opun_parse(op);

    switch(op % 2){
    case LIT:
        a = c16_mem_fillword(mem,regs);
        break;
    case REG:
        a = (c16_word) c16_mem_fillhalfword(mem,regs);
        break;
    }

    return_register_op = c16_mem_fillhalfword(mem,regs);
    return_register    = c16_regs_parse(regs,return_register_op);

    if (c16_regs_issubreg(return_register_op)){
        *((c16_halfword*) return_register) = (c16_halfword) f(a);
    }else{
        *((c16_word*) return_register) = f(a);
    }
}


// Jump operator handling.
void c16_opjmp_apply(c16_halfword op,c16_regs *regs,const c16_mem *mem){
    c16_word dest = c16_mem_fillword(mem,regs);

    switch(op){
    case OP_JMP:
        regs->ipt = dest;
        return;
    case OP_JMPT:
        if (regs->tst){
            regs->ipt = dest;
        }
        return;
    case OP_JMPF:
        if (!regs->tst){
            regs->ipt = dest;
        }
        return;
    }
}
