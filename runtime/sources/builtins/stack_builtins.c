#include <string.h>

#include <helpers.h>
#include <builtins/stack_builtins.h>

void initHash()
{
    memset(_hashTable, 0, HASH_TABLE_SIZE * sizeof(struct stack_node_t));
}

uint64_t getKeyForVTermsFragment(uint64_t offset, uint64_t length)
{
    uint64_t key = 0;
    uint64_t i = 0;

    for (i = offset; i < offset + length; ++i)
    {
        if (_memMngr.vterms[i].tag != V_CHAR_TAG)
            PRINT_AND_EXIT(HASH_TABLE_BAD_ARG);

        key += _memMngr.vterms[i].ch;
    }

    return key % HASH_TABLE_SIZE;
}

struct lterm_t* popObj(uint64_t key)
{

}

struct lterm_t* getObj(uint64_t key)
{
    if (key > HASH_TABLE_SIZE)
        return 0;

    struct stack_node_t* currNode = _hashTable + key;

    if (!currNode)
        return 0;


    while (!currNode)
    {

    }


}
