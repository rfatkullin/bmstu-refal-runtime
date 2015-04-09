#ifndef _BMSTU_REFC_ERRORS_STR_H_
#define _BMSTU_REFC_ERRORS_STR_H_


#define GC_MEMORY_OVERFLOW_MSG          "[GC]: Memory overflow!\n"
#define GC_CANT_COPY_TERM               "[GC]: Can't copy term!\n"
#define GC_VTERM_PROCESS_BAD_CHAIN_TAG  "[GC]: Bad lterm chain tag at GC vterm process. Chains can't contains simple chain without chain keeper!\n"
#define GC_VTERM_PROCESS_BAD_TAG        "[GC]: Bad lterm tag at GC vterm process!\n"
#define GC_BAD_CHAIN_SIMPLE_CHAIN_COPY  "[GC]: Bad chain term passed to copy simple chain func!\n"
#define GC_NULL_CHAIN_SIMPLE_CHAIN_COPY "[GC]: Null chain passed to copy simple chain func!\n"
#define GC_PARENT_CALL_NOT_MOVED        "[GC]: Parent call must be moved at subcall copy!\n"


#define BAD_EVAL_EXPR               "[VM]: Can't execute expr in evaluate brackets!\n"
#define FUNC_CALL_FAILED            "[VM]: Func call failed!\n"
#define ASSEMBLY_NOT_CHAIN          "[VM]: Trying assembly not chain!\n"
#define SIMPLE_CHAIN_AT_ASSEMBLY    "[VM]: Chain tag when assembly. Chains contains only chains keepers, not chains!\n"
#define BAD_TAG_AT_ASSEMBLY         "[VM]: Bad tag when assembly!\n"
#define FUNC_CALL_AT_ASSEMBLY       "[VM]: Func call term when assembly!\n"


#define FOV_CONTAINS_FUNC_CALL      "[Data allocator]: FieldOfView passed in funcation contains func call term!\n"
#define FOV_CONTAINS_SIMPLE_CHAIN   "[Data allocator]: FieldOfView can't contains simple chain without chain keeper!\n"

#endif
