#ifndef _BMSTU_REFC_ERRORS_STR_H_
#define _BMSTU_REFC_ERRORS_STR_H_


#define GC_MEMORY_OVERFLOW_MSG          "[GC]: Memory overflow!\n"
#define GC_CANT_COPY_TERM               "[GC]: Can't copy term!\n"
#define GC_VTERM_PROCESS_BAD_TAG        "[GC]: Bad lterm tag at GC vterm process!\n"
#define GC_BAD_CHAIN_SIMPLE_CHAIN_COPY  "[GC]: Bad chain term passed to copy simple chain func!\n"
#define GC_NULL_CHAIN_SIMPLE_CHAIN_COPY "[GC]: Null chain passed to copy simple chain func!\n"
#define GC_PARENT_CALL_NOT_MOVED        "[GC]: Parent call must be moved at subcall copy!\n"
#define GC_ACCESS_TO_ZERO_ADDRESS       "[GC]: Accessing to vterms on zero offset - something go wrong!\n"


#define BAD_EVAL_EXPR               "[VM]: Can't execute expr in evaluate brackets!\n"
#define FUNC_CALL_FAILED            "[VM]: Func call failed!\n"
#define ASSEMBLY_NOT_CHAIN          "[VM]: Trying assembly not chain!\n"
#define SIMPLE_CHAIN_AT_ASSEMBLY    "[VM]: Chain tag when assembly. Chains contains only chains keepers, not chains!\n"
#define BAD_TAG_AT_ASSEMBLY         "[VM]: Bad tag when assembly!\n"
#define FUNC_CALL_AT_ASSEMBLY       "[VM]: Func call term when assembly!\n"


#define FOV_CONTAINS_FUNC_CALL      "[Data allocator]: FieldOfView passed in funcation contains func call term!\n"
#define FOV_CONTAINS_SIMPLE_CHAIN   "[Data allocator]: FieldOfView can't contains simple chain without chain keeper!\n"


#define TOO_FEW_ARGUMENTS           "[Builtins]: Too few arguments passed in function!\n"
#define BAD_DESCR                   "[Builtins]: Bad file descriptor! Descriptor must be in range [1,%d)! Descr: %d\n"
#define BAD_FILE_OPEN_MODE          "[Builtins]: Bad file open mode! Expected 'W','w', 'R' or 'r'\n"
#define FILE_OPEN_ERROR             "[Builtins]: Can't open file %s in '%s' mode! The reason: %s\n"
#define DESCR_ALREADY_IN_USE        "[Builtins]: Descriptor %u already in use!\n"
#define BAD_VTERM                   "[Builtins]: Bad vterm type!\n"
#define TRY_TO_TAKE_TERMINAL_DESCR  "[Builtins]: Trying to bind file to terminal descriptor! Descriptor must be in range [1,%d)!\n"
#define GET_WRONG_ARG_NUM           "[Builtins]: Get take 1 argument - file descriptor!\n"
#define ARG_FUNC_BAD_ARG_NUM        "[Builtins]: Arg take 1 argument - number of argument!\'n"
#define BAD_PROGRAM_ARG_NUM         "[Builtins]: Bad program argument number! Program args count: %d. Passed arg num: %d. But must in range [1, %d).\n"
#define SYMB_BAD_ARG                "[Builtins]: Symb takes 1 argument - int number!\n"
#define NUMB_BAD_ARG                "[Builtins]: Numb can't parse int from string!\n"
#define WRONG_OPERANDS_NUMBER       "[Builtins]: It's binary operation!\n"
#define OPERANDS_TYPES_MISMATCH     "[Builtins]: Operands must be same type!\n"
#define OPERAND_BAD_TYPE            "[Builtins]: Operand must be int or double!\n"
#define BAD_BINARY_OPERATION        "[Builtins]: Unknown binary operation\n"
#define MOD_TO_DOUBLE_ERROR         "[Builtins]: Can't applay mod operation to double!\n"


#define BAD_BUFF  "[Helpers]: Bad buffer passed to char array converter!\n"

#endif
