// COMPILE TIME CRT CONFIGURATION

#pragma output CRT_ORG_CODE           = 24500      // org of compile todo this should be higher to allocate sprites
#pragma output REGISTER_SP            = 0xd000     // typical stack location when using sp1

#pragma output CRT_ENABLE_CLOSE       = 0          // don't bother closing files on exit
#pragma output CRT_ENABLE_EIDI        = 0x1        // disable interrupts at start
#pragma output CRT_ON_EXIT            = 0          // jump to 0 on exit

#pragma output CLIB_STDIO_HEAP_SIZE   = 0          // no stdio heap (no files)

#pragma output CLIB_EXIT_STACK_SIZE  = 0         // no atexit() functions

#pragma output CLIB_FOPEN_MAX         = -1         // no FILE* list
#pragma output CLIB_OPEN_MAX          = -1         // no fd table

// CREATE A BLOCK MEMORY ALLOCATOR WITH ONE QUEUE

#pragma output CLIB_BALLOC_TABLE_SIZE = 1

// thanks, alvin, for this feedback
#pragma output CRT_STACK_SIZE = 128
