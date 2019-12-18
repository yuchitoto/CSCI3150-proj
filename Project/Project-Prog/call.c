#include "call.h"
#include <stdio.h>
#include "superblock.h"
#include "inode.h"

#define KB 1024
#define MB 1024*KB

/*const superblock basics=
{
	.inode_offset = 4*KB;
	.data_offset = 10*MB;
	.max_inode = 100;
	.max_data_blk = 25600;
	.blk_size = 4*KB;
};*/

const char *HD = "HD";

int open_t(char *pathname)
{
	int inode_number;
	FILE file = fopen("./HD","rb");
	fseek(file,INODE_OFFSET,SEEK_SET);
	return inode_number;
}

int read_t(int inode_number, int offest, void *buf, int count)
{
	int read_bytes;
	// write your code here.
	return read_bytes;
}

// you are allowed to create any auxiliary functions that can help your implementation. But only “open_t()” and "read_t()" are allowed to call these auxiliary functions.
