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

inode* read_inode(int fd, int inode_num, superblock *sb)
{
	inode *i = (inode*)malloc(sizeof(inode));
	int cursor_pos = lseek(fd, sb->inode_offset+inode_num*sizeof(inode),SEEK_SET);
	if(cursor_pos < 0)
	{
		dprintf(2,"lseek err\n");
		return NULL;
	}
	if(read(fd,i,sizeof(inode)) != sizeof(inode))
	{
		dprintf(2,"read err\n");
		return NULL;
	}
	return i;
}

superblock* read_sb(int fd)//copied from Tutorial 1 superblock.c
{
	superblock* sb = (superblock*)malloc(sizeof(superblock));
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

int next_dir(int fd, int i_number, char *dir_name, superblock *sb)
{
	int next_inode = -1;
	inode *ip;
	ip = read_inode(fd,i_number, sb);
	if(ip==NULL)
		return -1;
	if(ip->i_type != DIR)
	{
		dprintf(2,"not a file\n");
		return -1;
	}

	DIR_NODE *data_blk = (DIR_NODE*)malloc(sb->blk_size);

	int block_number = ip->direct_blk[0];
	int currpos = lseek(fd,sb->data_offset+block_number*sb->blk_size,SEEK_SET);
	read(fd,data_blk,sb->blk_size);
	for(int i=0;i<ip->file_num;i++)
	{
		if(strcmp(data_blk[i].dir,dir_name)==0)
		{
			next_inode = data_blk[i].inode_number;
			break;
		}
	}
	free(data_blk);
	free(ip);
	return next_inode;
}

int open_t(char *pathname)
{
	int inode_number;
	int file = open("./HD",O_RDONLY);
	superblock *sb = read_sb(file);
	inode *rt = read_inode(file,0,sb); //root
	if(rt==NULL)
		return -1;
	inode_number = 0;

	char *dir_name = strtok(pathname,"/");
	while(dir_name!=NULL)
	{
		inode_number = next_dir(file,inode_number,dir_name,sb);
		dir_name = strtok(NULL,"/");
		if(inode_number<0)
		{
			dprintf(2,"wrong path\n");
			return -1;
		}
	}
	close(file);
	return inode_number;
}

int read_t(int inode_number, int offest, void *buf, int count)
{
	int read_bytes;
	int fd = open("./HD",O_RDONLY);
	superblock *sb = read_sb(fd);
	inode *some_inode = read_inode(fd, inode_number,sb);
	if(some_inode == NULL)
		return -1;
	if(some_inode->i_type!=SUBFILE)
	{
		dprintf(2,"not a file\n");
		return -1;
	}

	int end = count + offest;
	read_bytes = (end <= some_inode->i_size)?count:some_inode->i_size-offest;
	read_bytes = (read_bytes<0)?0:read_bytes;
	if(read_bytes>0)
	{
		int pt=0;
		int ed, st;
		st = offest/sb->blk_size;
		ed = (offest+read_bytes)/sb->blk_size;
		//printf("st: %d ed: %d\n\n",st,ed);

		offest -= st*sb->blk_size;
		if(st==0 && ed>=0)
		{
			//printf("going to read dir blk 0 pt: %d\n",pt);
			int sz = sb->blk_size-offest;
			sz = (sz<read_bytes)?sz:read_bytes;
			lseek(fd, sb->data_offset+some_inode->direct_blk[0]*sb->blk_size,SEEK_SET);
			read(fd,(char*)buf+pt,sz);
			pt+=sz;
			offest = 0;
			//printf("read dir blk 0 cur pt: %d\n \n",pt);
		}
		if(st<=1 && ed >=1)
		{
			//printf("going to read dir blk 1 pt: %d\n",pt);
			int t = read_bytes-pt;
			int sz = sb->blk_size-offest;
			sz = (sz<t)?sz:t;
			lseek(fd, sb->data_offset+some_inode->direct_blk[1]*sb->blk_size,SEEK_SET);
			read(fd,(char*)buf+pt,sz);
			pt+=sz;
			offest=0;
			//printf("read dir blk 1 cur pt: %d\n \n",pt);
		}
		int indr_ind;
		for(int rdblk=(st<2)?2:st;rdblk<=ed;rdblk++)
		{
			indr_ind = rdblk-2;
			//printf("going to read indir blk %d pt %d\n",indr_ind,pt);
			lseek(fd, sb->data_offset+some_inode->indirect_blk*sb->blk_size+indr_ind*sizeof(int), SEEK_SET);
			read(fd,&indr_ind,sizeof(int));
			//printf("indir blk at %d\n",indr_ind);
			int t = read_bytes-pt;
			int sz = sb->blk_size-offest;
			sz = (sz<t)?sz:t;
			lseek(fd, sb->data_offset+indr_ind*sb->blk_size,SEEK_SET);
			read(fd,(char*)buf+pt,sz);
			pt+=sz;
			offest=0;
			//printf("read indir blk %d cur pt %d\n \n", rdblk-2, pt);
		}
	}
	close(fd);
	return read_bytes;
}

// you are allowed to create any auxiliary functions that can help your implementation. But only “open_t()” and "read_t()" are allowed to call these auxiliary functions.
