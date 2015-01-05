set(16C_FILES
    main.c
    proc.c
    mem.c
    regs.c
    op.c
    op/bin.c
    op/cmp.c
    op/un.c
    op/misc.c)

set(EXECUTABLE_OUTPUT_PATH ${16CANDLES_BINARY_DIR})
add_executable(16c ${16C_FILES})
