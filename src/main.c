/* main.c --- 16c entry point.
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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "c16.h"
#include "proc.h"
#include "op.h"
#include "mem.h"
#include "regs.h"


int main(int argc,char **argv){
    FILE               *in;
    pthread_t           input_thread;
    c16_regs            regs;
    c16_mem             mem;
    c16_halfword       *buf;
    c16_proc_stdin_args stdin_args;

    if (argc == 1){
        printf("Usage: %s BINARY",argv[0]);
        return 0;
    }

    if (!c16_regs_init(&regs)){
        fprintf(stderr,"main: c16_regs_init: Unable to initialize registers");
        return EXIT_FAILURE;
    }

    if (!c16_mem_init(&mem,NULL)){  // NULL means use default memfile name.
        c16_regs_destory(&regs);
        fprintf(stderr,"main: c16_mem_init: Unable to initialize memory");
        return EXIT_FAILURE;
    }

    if (!(in = fopen(argv[1],"r"))){
        c16_regs_destory(&regs);
        c16_mem_destroy(&mem);
        perror("main: fopen");
        return EXIT_FAILURE;
    }

    if (c16_mem_loadfile(&mem,&regs,in)){
        c16_regs_destory(&regs);
        c16_mem_destroy(&mem);
        fprintf(stderr,
                "main: c16_mem_loadfile: Unable to load file %s",
                argv[1]);
        return EXIT_FAILURE;
    }

    // Setup the stdin thread.
    stdin_args.a_mem  = &mem;
    stdin_args.a_regs = &regs;
    pthread_create(&input_thread,NULL,c16_process_stdin,&stdin_args);

    c16_proc_run(&regs,&mem,-1);  // Execute the program

    pthread_cancel(input_thread);
    c16_regs_destory(&regs);
    c16_mem_destroy(&mem);
    return EXIT_SUCCESS;
}
