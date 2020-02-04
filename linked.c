#include "linked.h"

int action = 0;
int randomBlock();
int linked_allocation(char *token)
{
    int _token = atoi(token);
    if (strcmp(token, "add") == 0)
    {
        action = 1;
        return 1;
    }
    else if (strcmp(token, "delete") == 0)
    {
        action = 2;
        return 1;
    }
    else if (strcmp(token, "read") == 0)
    {
        action = 3;
        return 1;
    }
    else
    {
        if (_token > 0)
        {
        }
        else
        {
            printf("invalid command");
            return 0;
        }
    }

    size_t len = strlen(token);

    int dataNo = 0;
    int start = 0;
    int end = 0;

    printf("Block\t\tIndex\tFile Data\n");
    while (dataNo < len)
    {
        int blockNumber;
        //Find a empty data block to insert the data
        do
        {
            blockNumber = randomBlock();
        } while (atoi(node[blockNumber].data) != -1);

        //Choose the action
        switch (action)
        {
        case 1:
            node[blockNumber].data[dataNo] = '1';
        case 2:

        case 3:

        default:
            break;
        }

        if (dataNo == 0)
        {
            start = blockNumber;
        }
        else
        {
            node[blockNumber].data[0] = '-';
            node[blockNumber].data[1] = '1';
            end = blockNumber;
        }
        printf("%d\t\t%d\t%s\n", node[blockNumber].blockNo, node[blockNumber].index, node[blockNumber].data);
        dataNo++;
    }
    return 1;
}

int randomBlock()
{
    int _randomBlock = rand() % MAX_BLOCK;
    return (_randomBlock == 0) ? randomBlock() : _randomBlock;
}