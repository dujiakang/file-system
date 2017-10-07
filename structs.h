#ifndef STRUCTS_H
#define STRUCTS_H

#include <time.h>
/*
 *
 * Define page/sector structures here as well as utility structures
 * such as directory entries.
 *
 * Sectors/Pages are 512 bytes
 * The filesystem is 4 megabytes in size.
 * You will have 8K pages total.
 *
 */

#define BLOCKSIZE 512  // size of block
#define SIZE 4194304  // size of the file system
#define END 65535  // FAT:sign of end
#define FREE 0  // FAT free
#define ROOTBLOCKNUM 2  // number of root block
#define MAXOPENFILE 10  
#define MAXTEXT 10000


/* file control block */
typedef struct boot_sector
{
    char filename[8];  
    char exname[3];  // extension
    unsigned char attribute;  
    unsigned short time;  // create time
    unsigned short date;  // create data
    unsigned short first;  // start block number
    unsigned long length;  
    char free;  // state of free 
}fcb ;


typedef struct FAT
{
    unsigned short id;  //state: free last one or next one
}fat;

/* boot sector */
typedef struct USEROPEN
{
    char filename[8];  // file name
    char exname[3];  // extension
    unsigned char attribute;  
    unsigned short time;  
    unsigned short date;  
    unsigned short first;  
    unsigned long length;  
    char free;  

	unsigned short dirno;  // the number of the opened file in its parent directory
    int diroff;  // 
    char dir[80];  // 
    int father;  
	int count;  // number of character in the file
    char fcbstate;  // update state
	char topenfile;  
}useropen;


typedef struct BLOCK0
{
    char magic[10];  // magic number 
	char information[200];  // information 
    unsigned short root;  //start position of the root file
	unsigned char *startblock;  //data start position in the disk
}block0;

void my_format();  // formatting the disk
void my_cd(char *dirname);  
void my_mkdir(char *dirname);  
void my_rmdir(char *dirname); 
void my_ls();  
int my_open(char *filename);  // open 
int my_close(int fd);  // close the file
int do_write(int fd, char *text, int len, char wstyle);  
int do_read (int fd, int len,char *text);  
unsigned short findblock();  // find free block
int findopenfile();  

#endif
