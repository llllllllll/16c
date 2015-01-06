/* misc.c --- Misc operations.
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

#include "c16.h"
#include "op.h"
#include "regs.h"
#include "mem.h"


// The push instruction.
void c16_oppush_apply(c16_halfword op,c16_regs *regs,const c16_mem *mem){
    c16_halfword regop;
    void        *reg;

    switch(op % 2){
    case LIT:
        c16_mem_loadword(mem,c16_mem_fillword(mem,regs),regs->spt);
        regs->spt += sizeof(c16_word);
        return;
    case REG:
        regop = c16_mem_fillhalfword(mem,regs);
        reg   = c16_regs_parse(regs,regop);
        if (c16_regs_issubreg(regop)){
            c16_mem_loadhalfword(mem,*((c16_halfword*) reg),regs->spt);
            regs->spt += sizeof(c16_halfword);
        }else{
            c16_mem_loadword(mem,*((c16_word*) reg),regs->spt);
            regs->spt += sizeof(c16_word);
        }
        return;
    }
}


// The write instruction.
void c16_opwrite_apply(c16_halfword op,c16_regs *regs,const c16_mem *mem){
    c16_word     c;
    c16_halfword regop;
    void        *reg;

    switch(op % 2){
    case LIT:
        c = c16_mem_fillword(mem,regs);
        break;
    case REG:
        regop = c16_mem_fillhalfword(mem,regs);
        reg   = c16_regs_parse(regs,regop);

        if (c16_regs_issubreg(regop)){
            c = (c16_word) *((c16_halfword*) reg);
        }else{
            c = *((c16_word*) reg);
        }
        break;
    }

    putchar((int) c);
}


// The mset instruction.
void c16_opmset_apply(c16_halfword op,c16_regs *regs,const c16_mem *mem){
    c16_word tmpreg_op;
    void    *tmpreg;
    c16_word addr;    // Address we will be working with.
    c16_word value;   // The value, may be a halfword.
    c16_bool isread;  // Is this a read or write mset.
    c16_bool ishalf;  // If this is a write, what type of data to write.

    switch(op){
    case OP_MSET_LIT_MEMREG:  // Write a lit into a dereferenced reg.
        isread = c16_false;
        ishalf = c16_false;
        c16_mem_fill_lit_reg(mem,regs,&value,&addr);
        tmpreg = c16_regs_parse(regs,addr);
        if (ishalf = c16_regs_issubreg((c16_halfword) addr)){
            addr = (c16_word) *((c16_halfword*) tmpreg);
        }else{
            addr = *((c16_word*) tmpreg);
        }
        break;
    case OP_MSET_REG_MEMREG:  // Write the value in a reg to a dereferenced reg.
        isread = c16_false;
        c16_mem_fill_reg_reg(mem,regs,&tmpreg_op,&addr);
        tmpreg = c16_regs_parse(regs,addr);
        if (ishalf = c16_regs_issubreg((c16_halfword) addr)){
            addr = (c16_word) *((c16_halfword*) tmpreg);
        }else{
            addr = *((c16_word*) tmpreg);
        }
        tmpreg = c16_regs_parse(regs,(c16_halfword) tmpreg_op);
        if ((ishalf = c16_regs_issubreg((c16_halfword) tmpreg_op))){
            value = (c16_word) *((c16_halfword*) tmpreg);
        }else{
            value = *((c16_word*) tmpreg);
        }
        break;
    case OP_MSET_MEMADDR:  // Write the value at a lit into a reg.
        isread = c16_true;
        c16_mem_fill_lit_reg(mem,regs,&addr,&value);
        break;
    case OP_MSET_MEMREG:  // Write the value at a dereferenced reg into a reg.
        isread = c16_true;
        c16_mem_fill_reg_reg(mem,regs,&tmpreg_op,&value);
        tmpreg = c16_regs_parse(regs,(c16_halfword) tmpreg_op);
        if (c16_regs_issubreg((c16_halfword) tmpreg_op)){
            addr = *((c16_halfword*) tmpreg);
        }else{
            addr = *((c16_word*) tmpreg);
        }
    case OP_MSET_LIT_MEMADDR:  // Write a lit into a lit memaddr.
        isread = c16_false;
        ishalf = c16_false;
        c16_mem_fill_lit_lit(mem,regs,&value,&addr);
        break;
    case OP_MSET_REG_MEMADDR:  // Write the value in a reg into a lit memaddr.
        isread = c16_false;
        c16_mem_fill_reg_lit(mem,regs,&tmpreg_op,&addr);
        tmpreg = c16_regs_parse(regs,(c16_halfword) tmpreg_op);
        if ((ishalf = c16_regs_issubreg((c16_halfword) tmpreg_op))){
            value = (c16_word) *((c16_halfword*) tmpreg);
        }else{
            value = *((c16_word*) tmpreg);
        }
        break;
    }

    if (isread){
        tmpreg = c16_regs_parse(regs,(c16_halfword) value);
        if (c16_regs_issubreg((c16_halfword) value)){
            *((c16_halfword*) tmpreg) = c16_mem_gethalfword(mem,addr);
        }else{
            *((c16_word*) tmpreg) = c16_mem_getword(mem,addr);
        }
    }else if (ishalf){
        c16_mem_loadhalfword(mem,(c16_halfword) value,addr);
    }else{
        c16_mem_loadword(mem,value,addr);
    }
}


// The swap instruction.
void c16_opswap_apply(c16_regs *regs,const c16_mem *mem){
    c16_word regop1;
    c16_word regop2;
    void    *reg1;
    void    *reg2;

    c16_mem_fill_reg_reg(mem,regs,&regop1,&regop2);

    if (regop1 == regop2){
        return;  // Fast path if they are the same register.
    }

    reg1 = c16_regs_parse(regs,regop1);
    reg2 = c16_regs_parse(regs,regop2);

    if (c16_regs_issubreg((c16_halfword) regop1)){
        if (c16_regs_issubreg((c16_halfword) regop2)){
            *((c16_halfword*) reg1) ^= *((c16_halfword*) reg2);
            *((c16_halfword*) reg2) ^= *((c16_halfword*) reg1);
            *((c16_halfword*) reg1) ^= *((c16_halfword*) reg2);
            return;
        }else{
            *((c16_halfword*) reg1) ^= *((c16_word*) reg2);
            *((c16_word*) reg2)     ^= *((c16_halfword*) reg1);
            *((c16_halfword*) reg1) ^= *((c16_word*) reg2);
            return;
        }
    }else if (c16_regs_issubreg((c16_halfword) regop2)){
        *((c16_word*) reg1)     ^= *((c16_halfword*) reg2);
        *((c16_halfword*) reg2) ^= *((c16_word*) reg1);
        *((c16_word*) reg1)     ^= *((c16_halfword*) reg2);
    }else{
        *((c16_word*) reg1) ^= *((c16_word*) reg2);
        *((c16_word*) reg2) ^= *((c16_word*) reg1);
        *((c16_word*) reg1) ^= *((c16_word*) reg2);
    }
}


// The pop instruction.
void c16_oppop_apply(c16_regs *regs,const c16_mem *mem){
    c16_halfword regop = c16_mem_fillhalfword(mem,regs);
    void        *reg   = c16_regs_parse(regs,regop);

    if (c16_regs_issubreg(regop)){
        *((c16_halfword*) reg) = c16_mem_gethalfword(mem,regs->spt);
        regs->spt += sizeof(c16_halfword);
    }else{
        *((c16_word*) reg) = c16_mem_getword(mem,regs->spt);
        regs->spt += sizeof(c16_word);
    }
}


// The peek instruction.
void c16_oppeek_apply(c16_regs *regs,const c16_mem *mem){
    c16_halfword regop = c16_mem_fillhalfword(mem,regs);
    void        *reg   = c16_regs_parse(regs,regop);

    if (c16_regs_issubreg(regop)){
        *((c16_halfword*) reg) = c16_mem_gethalfword(mem,regs->spt);
    }else{
        *((c16_word*) reg) = c16_mem_getword(mem,regs->spt);
    }
}


// The flush instruction.
void c16_opflush_apply(c16_regs *regs,const c16_mem *mem){
    regs->spt = mem->m_sptstart;
}


void c16_opread_apply(c16_regs *regs,const c16_mem *mem){
    c16_halfword *inputc = c16_mem_inputc(mem);
    c16_halfword  regop  = c16_mem_fillhalfword(mem,regs);
    void         *reg    = c16_regs_parse(regs,regop);


    if (c16_regs_issubreg(regop)){
        if (*inputc){
            *((c16_halfword*) reg) = c16_mem_gethalfword(mem,regs->inp_r++);
            *inputc -= sizeof(c16_halfword);
        }else{
            *((c16_halfword*) reg) = 255;
        }
    }else{
        if (*inputc){
            *((c16_word*) reg) = c16_mem_getword(mem,regs->inp_r++);
            *inputc == sizeof(c16_word);
        }else{
            *((c16_word*) reg) = 255;
        }
    }
    regs->tst = *c16_mem_inputb(mem) || *inputc;
}
