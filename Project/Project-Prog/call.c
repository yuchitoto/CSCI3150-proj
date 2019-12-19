#include "call.h"
#include <stdio.h>
#include "superblock.h"
#include "inode.h"
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>

#define KB 1024
#define MB 1024*KB
#ifndef DIR
#define DIR 1
#endif
#ifndef SUBFILE
#define SUBFILE 0
#endif

/*const superblock basics=
{
	.inode_offset = 4*KB;
	.data_offset = 10*MB;
	.max_inode = 100;
	.max_data_blk = 25600;
	.blk_size = 4*KB;
};*/

const char *HD = "HD";

inode* read_inode(int fd, int inode_num)
{
	inode *i = malloc(sizeof(inode));
	int cursor_pos = lseek(fd, INODE_OFFSET+inode_num*sizeof(inode),SEEK_SET);
	if(cursor_pos < 0)
	{
		dprintf(2,"lseek err\n");
		exit(-1);
	}
	if(read(fd,i,sizeof(inode)) != sizeof(inode))
	{
		dprintf(2,"read err\n");
		exit(-2);
	}
	return i;
}

superblock* read_sb(int fd)//copied from Tutorial 1 superblock.c
{
	superblock* sb = malloc(sizeof(superblock));
	int currpos=lseek(fd, SB_OFFSET, SEEK_SET);
	if(currpos<0){
		printf("Error: lseek()\n");
		return NULL;
	}

	//read superblock from disk
	int ret = read(fd, sb, sizeof(superblock));
	if(ret != sizeof (superblock) ){
		printf("Error: read()\n");
		return NULL;
	}
	return sb;
}

int next_dir(int fd, int i_number, char *dir_name)
{
	int next_inode = -1;
	inode *ip;
	ip = read_inode(fd,i_number);
	if(ip->i_type != DIR)
	{
		dprintf(2,"not a file\n");
		exit(-2);
	}

	DIR_NODE *data_blk = (DIR_NODE*)malloc(BLOCK_SIZE);

	int block_number = ip->direct_blk[0];
	int currpos = lseek(fd,DATA_OFFSET+block_number*BLOCK_SIZE,SEEK_SET);
	read(fd,data_blk,BLOCK_SIZE);
	for(int i=0;i<ip->file_num;i++)
	{
		if(strcmp(data_blk[i].dir,dir_name)==0)
		{
			next_inode = data_blk[i].inode_number;
			break;
		}
	}

	return next_inode;
}

int open_t(char *pathname)
{
	int inode_number;
	int file = open("./HD",O_RDONLY);
	inode *rt = read_inode(file,0); //root
	inode_number = 0;

	char *dir_name = strtok(pathname,"/")
	while(dir_name!=NULL)
	{
		inode_number = next_dir(fd,inode_number,dir_name);
		dir_name = strtok(NULL,"/");
		if(inode_number<0)
		{
			dprintf(2,"wrong path\n");
			exit(0);
		}
	}
	return inode_number;
}

int read_t(int inode_number, int offest, void *buf, int count)
{
	int read_bytes;
	// write your code here.
	return read_bytes;
}

// you are allowed to create any auxiliary functions that can help your implementation. But only “open_t()” and "read_t()" are allowed to call these auxiliary functions.
