#include "linked.h"

int action = 0, dir_counter = 0, block_index = 0, current_dir_num = 0, block_num;

int randomIndex();
int check_new_block();
int allocate_block(char *token);

int linked_allocation(char *token, int num)
{
    int _token = atoi(token);
    if (num == 0)
    {
        if (strcmp(token, "add") == 0)
        {
            action = 1;
        }
        else if (strcmp(token, "read") == 0)
        {
            action = 2;
        }
        else if (strcmp(token, "delete") == 0)
        {
            action = 3;
        }
        else
        {
            printf("Invalid command");
        }
        return 0;
    }
    else if (num == 1 && action == 1)
    {
        vcb[0].start = check_new_block();
        current_file_number = atoi(token);
    }
    else if (action != 0)
    {
        if (token != "\n")
        {
            allocate_block(token);
        }
        else
        {
            /* code */
        }
        
    }

}

int randomBlock()
{
    int _randomBlock = rand() % noOfBlocks;
    //return (node[_randomBlock].blockNo == 0) ? randomBlock() : _randomBlock;
    return _randomBlock;
}

int check_new_block()
{
    block_num = randomBlock();

    int current_space = 0, valid = 0;
    block_index = 0;
    do
    {
        valid = (node[block_index].blockNo == block_num) ? 1 : valid;
        block_index = (valid == 1) ? block_index : block_index + blockSize;
    } while (valid == 0);

    int current_index = block_index;
    for (int j = 0; j < blockSize; j++)
    {
        current_space = current_space + node[current_index].data;
        current_index++;
    }

    return ((current_space + blockSize) != 0) ? check_new_block() : block_num;
}

int allocate_block(char *token)
{
    //printf("test %s", token);
    //int token_value = atoi(token);
    // int first_digit;
    // while(token_value >= 10)
    // {
    //     first_digit = token_value / 10;
    // }

    //Choose the action
    switch (action)
    {
    case 1:
        node[block_index].filename = current_file_number;
        
        if (node[block_index + 1].blockNo == block_num )
        {
            node[block_index].data = atoi(token);
            printf("%d\t\t%d\t%d\t%d\n", node[block_index].blockNo, node[block_index].index, node[block_index].data, node[block_index].filename);
            block_index++;
        }
        else
        {
            int temp_index = block_index;
            node[temp_index].data = check_new_block();
            vcb[0].end = node[temp_index].data;
            printf("%d\t\t%d\t%d\t%d\n", node[temp_index].blockNo, node[temp_index].index, node[temp_index].data, node[temp_index].filename);
        }

        break;
    case 2:
        printf("\nRead %s", token);
        for (int i = 0; i < MAX_BLOCK; i++)
        {
            if (node[i].data == atoi(token))
            {
                printf("%d\t\t%d\t%d\t%d\n", node[i].blockNo, node[i].index, node[i].data, node[i].filename);
            }
        }
        break;
    case 3:
        printf("\nDelete %s\n", token);
        for (int i = 0; i < MAX_BLOCK; i++)
        {
            if (node[i].filename == atoi(token))
            {
                node[i].data = -1;
                node[i].filename = 0;
                printf("%d\t\t%d\t%d\t%d\n", node[i].blockNo, node[i].index, node[i].data, node[i].filename);
            }
        }
        break;
    default:
        break;
        int test = 306;  
    }
    
    return (node[block_index -1].data != atoi(token) && action == 1) ? allocate_block(token) : 1;
}