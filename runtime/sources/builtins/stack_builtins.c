#include <string.h>

#include <helpers.h>
#include <vmachine.h>
#include <builtins/builtins.h>
#include <allocators/data_alloc.h>
#include <builtins/stack_builtins.h>
#include <defines/data_struct_sizes.h>

static struct fragment_t* popObject(char* name);
static void pushObject(char* name, struct fragment_t* obj);
static struct stacks_holder_t* searchStackHolder(char* name);
static struct fragment_t*  getFromStack(struct stacks_holder_t* stackHolder);
static struct fragment_t*  popFromStack(struct stacks_holder_t* stackHolder);
static struct stack_t*  pushToStack(struct stack_t* stack, struct fragment_t* obj);

struct func_result_t Br(int entryStatus)
{
    gcInitBuiltinEnv();

    uint64_t border = 0;
    for (border = 0; border < BUILTIN_FRAG->length; ++border)
    {
        if (_memMngr.vterms[BUILTIN_FRAG->offset + border].tag == V_CHAR_TAG &&
            _memMngr.vterms[BUILTIN_FRAG->offset + border].ch == '=')
            break;
    }

    if (border >= BUILTIN_FRAG->length)
        FMT_PRINT_AND_EXIT(BAD_ARG, "Br");

    uint64_t length = BUILTIN_FRAG->length;

    if (length > border)
    {
        BUILTIN_FRAG->length = border;

        char* name = vtermsToChars(BUILTIN_FRAG);

        BUILTIN_FRAG->length = length - border - 1;
        BUILTIN_FRAG->offset +=  border + 1;

        pushObject(name, BUILTIN_FRAG);
    }

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = 0, .callChain = 0};
}

struct func_result_t Dg(int entryStatus)
{
    gcInitBuiltinEnv();

    checkAndCleanHeaps(0, BUILTINS_RESULT_SIZE);

    char* name = vtermsToChars(BUILTIN_FRAG);

    struct lterm_t* res = 0;
    struct fragment_t* frag = popObject(name);

    if (frag)
        res = allocateBuiltinsResult(frag->offset, frag->length);

    free(name);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

struct func_result_t Cp(int entryStatus)
{
    gcInitBuiltinEnv();

    checkAndCleanHeaps(0, BUILTINS_RESULT_SIZE);

    char* name = vtermsToChars(BUILTIN_FRAG);

    struct stacks_holder_t* stackHolder = searchStackHolder(name);
    struct lterm_t* res = 0;

    if (stackHolder)
    {
        struct fragment_t* frag = getFromStack(stackHolder);

        if (frag)
            res = allocateBuiltinsResult(frag->offset, frag->length);
    }

    free(name);

    return (struct func_result_t){.status = OK_RESULT, .fieldChain = res, .callChain = 0};
}

static struct stacks_holder_t* searchStackHolder(char* name)
{
    struct stacks_holder_t* currNode = _stacksHolders;

    while (currNode)
    {
        if (!strcmp(currNode->name, name))
            break;

        currNode = currNode->next;
    }

    return currNode;
}

static void pushObject(char* name, struct fragment_t* obj)
{
    struct stacks_holder_t* node = searchStackHolder(name);

    if (!node)
    {
        node = (struct stacks_holder_t*)malloc(sizeof(struct stacks_holder_t));
        node->name = name;
        node->stack = 0;

        node->next = _stacksHolders;
        node->prev = 0;

        if (_stacksHolders)
            _stacksHolders->prev = node;

        _stacksHolders = node;
        node = node;
    }

    node->stack = pushToStack(node->stack, obj);
}

static struct fragment_t* popObject(char* name)
{
    struct stacks_holder_t* node = searchStackHolder(name);

    if (!node)
        return 0;

    return popFromStack(node);
}

static struct stack_t*  pushToStack(struct stack_t* stack, struct fragment_t* obj)
{
    struct stack_t* stackNode = (struct stack_t*)malloc(sizeof(struct stack_t));

    stackNode->obj = obj;
    stackNode->next = stack;
    stack = stackNode;

    return stack;
}

static struct fragment_t*  popFromStack(struct stacks_holder_t* stackHolder)
{
    struct fragment_t* obj = 0;

    if (stackHolder->stack)
    {
        obj = stackHolder->stack->obj;

        struct stack_t* currStackNode = stackHolder->stack;
        stackHolder->stack = stackHolder->stack->next;

        free(currStackNode);
    }

    if (!stackHolder->stack)
    {
        int last = 1;

        if (stackHolder->prev)
        {
            stackHolder->prev->next = stackHolder->next;
            last = 0;
        }

        if (stackHolder->next)
        {
            stackHolder->next->prev = stackHolder->prev;
            last = 0;
        }

        free(stackHolder->name);
        free(stackHolder);

        if (last)
            _stacksHolders = 0;
    }

    return obj;
}

static struct fragment_t*  getFromStack(struct stacks_holder_t* stackHolder)
{
    if (stackHolder->stack)
        return stackHolder->stack->obj;

    return 0;
}
