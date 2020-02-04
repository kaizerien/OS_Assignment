#include "linked.h"

int action = 0;
int linked_allocation(char *token)
{
    if (strcmp(token, "add"))
    {
        action = 1;
    }
    else if (strcmp(token, "delete"))
    {
        action = 2;
    }
    else if (strcmp(token, "read"))
    {
        action = 3;
    }
    else
    {
        printf("invalid command");
        return 0;
    }
    int _token = atoi(token);
    if (_token > 0)
    {
    }
    else
    {
        return 1;
    }

    size_t len = strlen(token);
    int counter = 0;

    printf("Block\t\tIndex\tFile Data\n");
    while (counter < len)
    {
        int randomBlock = rand() % MAX_BLOCK;
        switch (action)
        {
        case 1:
          
            node[randomBlock].data[counter++] = token[counter];
        case 2:
            break;
        case 3:
            break;
        default:
            break;
        }

        printf("%d\t\t%d\t%g\n", node[randomBlock].blockNo, node[randomBlock].index, node[randomBlock].data);
    }

    return 1;
}