#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "support.h"
#include "structs.h"
#include "filesystem.h"
#include <sys/mman.h>

useropen openfilelist[MAXOPENFILE];  
int curdir;  
char currentdir[80];  //record current dir
unsigned char* startp;  //data block start position
char myfilename[] = "myfilesys";
char *map; 
/*
 * generateData() - Converts source from hex digits to
 * binary data. Returns allocated pointer to data
 * of size amt/2.
 */
char* generateData(char *source, size_t size)
{
	char *retval = (char *)malloc((size >> 1) * sizeof(char));

	for(size_t i=0; i<(size-1); i+=2)
	{
		sscanf(&source[i], "%2hhx", &retval[i>>1]);
	}
	return retval;
}


/*
 * filesystem() - loads in the filesystem and accepts commands
 */
void filesystem(char *file)
{
	/* pointer to the memory-mapped filesystem */
	

	/*
	 * open file, handle errors, create it if necessary.
	 * should end up with map referring to the filesystem.
	 */
	FILE *fp;
    unsigned char buf[SIZE];
    fcb *root;
	int i;
	map = (char *)malloc(SIZE);
    memset(map, 0, SIZE);
    char *map = mmap(0, 4*1024*1024, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if((fp = fopen(myfilename, "r")) != NULL)
	{
	    fread(buf, SIZE, 1, fp);
	    fclose(fp);
	    if(strcmp(((block0 *)buf)->magic, "10101010"))
	    {
			printf("myfilesys does not exist,begin to creat the file...\n");
			my_format();
			
	    }
	    else
	    {
	        for(i = 0; i < SIZE; i++)
                map[i] = buf[i];
	    }
	}
	else
	{
	    printf("myfilesys is not exist,begin to creat the file...\n");
	    my_format();
	}
	root = (fcb *)(map + 5 * BLOCKSIZE * 2);
	strcpy(openfilelist[0].filename, root->filename);
	strcpy(openfilelist[0].exname, root->exname);
	openfilelist[0].attribute = root->attribute;
	openfilelist[0].time = root->time;
	openfilelist[0].date = root->date;
	openfilelist[0].first = root->first;
	openfilelist[0].length = root->length;
	openfilelist[0].free = root->free;
	openfilelist[0].dirno = 5;
	openfilelist[0].diroff = 0;
	strcpy(openfilelist[0].dir, "\\root\\");
	openfilelist[0].father = 0;
	openfilelist[0].count = 0;
	openfilelist[0].fcbstate = 0;
	openfilelist[0].topenfile = 1;
	for(i = 1; i < MAXOPENFILE; i++)
        openfilelist[i].topenfile = 0;
	curdir = 0;
	strcpy(currentdir, "\\root\\");
	startp = ((block0 *)map)->startblock;

	/* You will probably want other variables here for tracking purposes */
    
	

	/*
	 * Accept commands, calling accessory functions unless
	 * user enters "quit"
	 * Commands will be well-formatted.
	 */
	
	char *buffer = NULL;
	size_t size = 0;
	char *param;
	while(getline(&buffer, &size, stdin) != -1)
	{
		/* Basic checks and newline removal */
		printf("%s>", openfilelist[curdir].dir);
		size_t length = strlen(buffer);
		if(length == 0)
		{
			continue;
		}
		if(buffer[length-1] == '\n')
		{
			buffer[length-1] = '\0';
		}
		param = strtok(buffer," ");
		//printf("%s,%s",buffer,param);

		/* TODO: Complete this function */
		/* You do not have to use the functions as commented (and probably can not)
		 *	They are notes for you on what you ultimately need to do.
		 */

		if(!strcmp(buffer, "quit"))
		{
			break;
		}
		else if(!strncmp(buffer, "dump ", 5))
		{
			if(isdigit(buffer[5]))
			{
				//dump(stdout, atoi(buffer + 5));
			}
			else
			{
				//char *filename = buffer + 5;
				char *space = strstr(buffer+5, " ");
				*space = '\0';
				//open and validate filename
				//dumpBinary(file, atoi(space + 1));
			}
		}
		else if(!strncmp(buffer, "usage", 5))
		{
			//usage();
		}
		else if(!strncmp(buffer, "pwd", 3))
		{
			//pwd();
		}
		else if(!strncmp(buffer, "cd ", 2))
		{
			//cd(buffer+3);
			//printf("1");
			param = strtok(NULL," ");
			//printf("%s\n",param);
			my_cd(param);
		}
		else if(!strncmp(buffer, "ls", 2))
		{
			//ls();
			param = strtok(NULL," ");
			my_ls(param);
		}
		else if(!strncmp(buffer, "mkdir ", 5))
		{
			//mkdir(buffer+6);
			param = strtok(NULL," ");
			my_mkdir(param);
		}
		else if(!strncmp(buffer, "cat ", 4))
		{
			//cat(buffer + 4);
		}
		else if(!strncmp(buffer, "write ", 6))
		{
			//char *filename = buffer + 6;
			char *space = strstr(buffer+6, " ");
			*space = '\0';
			size_t amt = atoi(space + 1);
			space = strstr(space+1, " ");

			char *data = generateData(space+1, amt<<1);
			//write(filename, amt, data);
			free(data);
		}
		else if(!strncmp(buffer, "append ", 7))
		{
			//char *filename = buffer + 7;
			char *space = strstr(buffer+7, " ");
			*space = '\0';
			size_t amt = atoi(space + 1);
			space = strstr(space+1, " ");

			char *data = generateData(space+1, amt<<1);
			//append(filename, amt, data);
			free(data);
		}
		else if(!strncmp(buffer, "getpages ", 9))
		{
			//getpages(buffer + 9);
		}
		else if(!strncmp(buffer, "get ", 4))
		{
			//char *filename = buffer + 4;
			char *space = strstr(buffer+4, " ");
			*space = '\0';
			//size_t start = atoi(space + 1);
			space = strstr(space+1, " ");
			//size_t end = atoi(space + 1);
			//get(filename, start, end);
		}
		else if(!strncmp(buffer, "rmdir ", 5))
		{
			//rmdir(buffer + 6);
			param = strtok(NULL," ");
			my_rmdir(param);
		}
		else if(!strncmp(buffer, "rm -rf ", 7))
		{
			//rmForce(buffer + 7);
		}
		else if(!strncmp(buffer, "rm ", 3))
		{
			//rm(buffer + 3);
		}
		else if(!strncmp(buffer, "scandisk", 8))
		{
			//scandisk();
		}
		else if(!strncmp(buffer, "undelete ", 9))
		{
			//undelete(buffer + 9);
		}



		free(buffer);
		buffer = NULL;
	}
	free(buffer);
	buffer = NULL;

}

/*
 * help() - Print a help message.
 */
void help(char *progname)
{
	printf("Usage: %s [FILE]...\n", progname);
	printf("Loads FILE as a filesystem. Creates FILE if it does not exist\n");
	exit(0);
}

/*
 * main() - The main routine parses arguments and dispatches to the
 * task-specific code.
 */
int main(int argc, char **argv)
{
	/* for getopt */
	long opt;

	/* run a student name check */
	check_student(argv[0]);

	/* parse the command-line options. For this program, we only support */
	/* the parameterless 'h' option, for getting help on program usage. */
	while((opt = getopt(argc, argv, "h")) != -1)
	{
		switch(opt)
		{
		case 'h':
			help(argv[0]);
			break;
		}
	}

	if(argv[1] == NULL)
	{
		fprintf(stderr, "No filename provided, try -h for help.\n");
		return 1;
	}

	filesystem(argv[1]);
	
	return 0;
}

void my_format()
{
	
    FILE *fp;
	fat *fat1, *fat2;
	block0 *blk0;
	time_t now;
	struct tm *nowtime;
	fcb *root;
	int i;
	blk0 = (block0 *)map;
	fat1 = (fat *)(map + BLOCKSIZE * 2);
	fat2 = (fat *)(map + 3 * BLOCKSIZE * 2);
	root = (fcb *)(map + 5 * BLOCKSIZE * 2);
	strcpy(blk0->magic, "10101010");
	strcpy(blk0->information, "My FileSystem Ver 1.0 \n BLOCKSIZE = 512B Whole size=4MB RootBlocknum=2\n");
	blk0->root = 5;
	blk0->startblock = (unsigned char *)root;
	
	for(i = 0; i < 5; i++)
    {
        fat1->id = END;
        fat2->id = END;
        fat1++;
        fat2++;
	}
	
    fat1->id = 6;
    fat2->id = 6;
    fat1++;
    fat2++;
    fat1->id = END;
    fat2->id = END;
    fat1++;
    fat2++;
	for(i = 7; i < SIZE / BLOCKSIZE * 2; i++)
    {
        fat1->id = FREE;
        fat2->id = FREE;
        fat1++;
        fat2++;
    }
    now = time(NULL);
    nowtime = localtime(&now);
    strcpy(root->filename, ".");
    strcpy(root->exname, "");
    root->attribute = 0x28;
    root->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;
	root->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;
	root->first = 5;
	root->length = 2 * sizeof(fcb);
	root->free = 1;
	root++;
	now = time(NULL);
    nowtime = localtime(&now);
	strcpy(root->filename, "..");
    strcpy(root->exname, "");
    root->attribute = 0x28;
    root->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;
	root->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;
	root->first = 5;
	root->length = 2 * sizeof(fcb);
	root->free = 1;
    fp = fopen(myfilename, "w");
    fwrite(map, SIZE, 1, fp);
    fclose(fp);
}
void my_cd(char *dirname)
{
    char *dir;
	int fd;
	dir = strtok(dirname, "\\");
	if(strcmp(dir, ".") == 0)
        return;
    else if(strcmp(dir, "..") == 0)
    {
        if(curdir)
            curdir = my_close(curdir);
        return;
    }
    else if(strcmp(dir, "root") == 0)
    {
        while(curdir)
            curdir = my_close(curdir);
        dir = strtok(NULL, "\\");
    }
    while(dir)
    {
        fd = my_open(dir);
        if(fd != -1)
            curdir = fd;
        else
            return;
        dir = strtok(NULL, "\\");
    }
}
void my_mkdir(char *dirname)
{
    fcb *fcbptr;
    fat *fat1, *fat2;
    time_t now;
	struct tm *nowtime;
    char text[MAXTEXT];
    unsigned int blkno;
    int rbn, fd, i;
    fat1 = (fat *)(map + BLOCKSIZE * 2);
    fat2 = (fat *)(map + 3 * BLOCKSIZE * 2);
    openfilelist[curdir].count = 0;
    rbn = do_read(curdir, openfilelist[curdir].length, text);
    fcbptr = (fcb *)text;
    for(i = 0; i < rbn / sizeof(fcb); i++)//find duplicate under current directory
    {
        if(strcmp(fcbptr->filename, dirname) == 0 && strcmp(fcbptr->exname, "") == 0)
        {
            printf("Error,the dirname is already exist!\n");
            return;
        }
        fcbptr++;
    }
    fcbptr = (fcb *)text;
    for(i = 0; i < rbn / sizeof(fcb); i++)
    {
        if(fcbptr->free == 0)
            break;
        fcbptr++;
    }
    blkno = findblock();//find free block
    if(blkno == -1)
        return;
    (fat1 + blkno)->id = END;
    (fat2 + blkno)->id = END;
	now = time(NULL);
    nowtime = localtime(&now);
    strcpy(fcbptr->filename, dirname);
    strcpy(fcbptr->exname, "");
    fcbptr->attribute = 0x30;
    fcbptr->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;
	fcbptr->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;
	fcbptr->first = blkno;
	fcbptr->length = 2 * sizeof(fcb);
	fcbptr->free = 1;
	openfilelist[curdir].count = i * sizeof(fcb);
	do_write(curdir, (char *)fcbptr, sizeof(fcb), 2);

	fd = my_open(dirname);//make '.','..'directory
	if(fd == -1)
        return;
	fcbptr = (fcb *)malloc(sizeof(fcb));
    now = time(NULL);
    nowtime = localtime(&now);
	strcpy(fcbptr->filename, ".");
	strcpy(fcbptr->exname, "");
	fcbptr->attribute = 0x28;
	fcbptr->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;
	fcbptr->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;
	fcbptr->first = blkno;
	fcbptr->length = 2 * sizeof(fcb);
	fcbptr->free = 1;
	do_write(fd, (char *)fcbptr, sizeof(fcb), 2);
	now = time(NULL);
    nowtime = localtime(&now);
	strcpy(fcbptr->filename, "..");
	strcpy(fcbptr->exname, "");
	fcbptr->attribute = 0x28;
	fcbptr->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;
	fcbptr->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;
	fcbptr->first = blkno;
	fcbptr->length = 2 * sizeof(fcb);
	fcbptr->free = 1;
	do_write(fd, (char *)fcbptr, sizeof(fcb), 2);
	free(fcbptr);
	my_close(fd);

	fcbptr = (fcb *)text;
	fcbptr->length = openfilelist[curdir].length;
	openfilelist[curdir].count = 0;
	do_write(curdir, (char *)fcbptr, sizeof(fcb), 2);
    openfilelist[curdir].fcbstate = 1;
}

void my_rmdir(char *dirname)
{
    fcb *fcbptr,*fcbptr2;
    fat *fat1, *fat2, *fatptr1, *fatptr2;
	char text[MAXTEXT], text2[MAXTEXT];
	unsigned int blkno;
	int rbn, rbn2, fd, i, j;
	fat1 = (fat *)(map + BLOCKSIZE * 2);
    fat2 = (fat *)(map + 3 * BLOCKSIZE * 2);
	if(strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)
	{
	    printf("Error,can't remove this directory.\n");
	    return;
	}
	openfilelist[curdir].count = 0;
	rbn = do_read(curdir, openfilelist[curdir].length, text);
	fcbptr = (fcb *)text;
	for(i = 0; i < rbn / sizeof(fcb); i++)//find the dir to delete
	{
	    if(strcmp(fcbptr->filename, dirname) == 0 && strcmp(fcbptr->exname, "") == 0)
            break;
	    fcbptr++;
	}
	if(i == rbn / sizeof(fcb))
	{
	    printf("Error,the directory is not exist.\n");
        return;
	}
	fd = my_open(dirname);
	rbn2 = do_read(fd, openfilelist[fd].length, text2);
	fcbptr2 = (fcb *)text2;
	for(j = 0; j < rbn2 / sizeof(fcb); j++)//judge if the dir is empty
	{
	    if(strcmp(fcbptr2->filename, ".") && strcmp(fcbptr2->filename, "..") && strcmp(fcbptr2->filename, ""))
	    {
	        my_close(fd);
	        printf("Error,the directory is not empty.\n");
			return;
	    }
	    fcbptr2++;
	}
    blkno = openfilelist[fd].first;
    while(blkno != END)
    {
        fatptr1 = fat1 + blkno;
        fatptr2 = fat2 + blkno;
        blkno = fatptr1->id;
        fatptr1->id = FREE;
        fatptr2->id = FREE;
    }
	my_close(fd);
    strcpy(fcbptr->filename, "");
    fcbptr->free = 0;
    openfilelist[curdir].count = i * sizeof(fcb);
    do_write(curdir, (char *)fcbptr, sizeof(fcb), 2);
    openfilelist[curdir].fcbstate = 1;
}
void my_ls()
{
    fcb *fcbptr;
	char text[MAXTEXT];
	int rbn, i;
	openfilelist[curdir].count = 0;
	rbn = do_read(curdir, openfilelist[curdir].length, text);
	fcbptr = (fcb *)text;
	for(i = 0; i < rbn / sizeof(fcb); i++)
    {
        if(fcbptr->free)
        {
            if(fcbptr->attribute & 0x20)
				printf("%s\\\t\t<DIR>\t\t%d/%d/%d\t%02d:%02d:%02d\n", fcbptr->filename, (fcbptr->date >> 9) + 1980, (fcbptr->date >> 5) & 0x000f, fcbptr->date & 0x001f, fcbptr->time >> 11, (fcbptr->time >> 5) & 0x003f, fcbptr->time & 0x001f * 2);
			else
                printf("%s.%s\t\t%dB\t\t%d/%d/%d\t%02d:%02d:%02d\t\n", fcbptr->filename, fcbptr->exname, (int)(fcbptr->length), (fcbptr->date >> 9) + 1980, (fcbptr->date >> 5) & 0x000f, fcbptr->date & 0x1f, fcbptr->time >> 11, (fcbptr->time >> 5) & 0x3f, fcbptr->time & 0x1f * 2);
        }
        fcbptr++;
    }
}
int my_open(char *filename)
{
    fcb *fcbptr;
	char *fname, exname[3], *str, text[MAXTEXT];
	int rbn, fd, i;
	fname = strtok(filename, ".");
	str = strtok(NULL, ".");
	if(str)
        strcpy(exname, str);
    else
        strcpy(exname, "");
	for(i = 0; i < MAXOPENFILE; i++)
	{
	    if(strcmp(openfilelist[i].filename, fname) == 0 && strcmp(openfilelist[i].exname, exname) == 0 && i != curdir)
	    {
	        printf("Error,the file is already open.\n");
	        return -1;
	    }
	}
	openfilelist[curdir].count = 0;
	rbn = do_read(curdir, openfilelist[curdir].length, text);
	fcbptr = (fcb *)text;
	for(i = 0; i < rbn / sizeof(fcb); i++)
	{
	    if(strcmp(fcbptr->filename, fname) == 0 && strcmp(fcbptr->exname, exname) == 0)
            break;
        fcbptr++;
	}
	if(i == rbn / sizeof(fcb))
    {
        printf("Error,the file is not exist.\n");
        return -1;
    }
	fd = findopenfile();
    if(fd == -1)
        return -1;
    strcpy(openfilelist[fd].filename, fcbptr->filename);
	strcpy(openfilelist[fd].exname, fcbptr->exname);
	openfilelist[fd].attribute = fcbptr->attribute;
	openfilelist[fd].time = fcbptr->time;
	openfilelist[fd].date = fcbptr->date;
	openfilelist[fd].first = fcbptr->first;
	openfilelist[fd].length = fcbptr->length;
	openfilelist[fd].free = fcbptr->free;
	openfilelist[fd].dirno = openfilelist[curdir].first;
	openfilelist[fd].diroff = i;
	strcpy(openfilelist[fd].dir, openfilelist[curdir].dir);
	strcat(openfilelist[fd].dir, filename);
	if(fcbptr->attribute & 0x20)
        strcat(openfilelist[fd].dir, "\\");
	openfilelist[fd].father = curdir;
	openfilelist[fd].count = 0;
	openfilelist[fd].fcbstate = 0;
	openfilelist[fd].topenfile = 1;
	return fd;
}
int my_close(int fd)
{
    fcb *fcbptr;
	int father = 0;
	if(fd < 0 || fd >= MAXOPENFILE)
	{
	    printf("Error,the file is not exist.\n");
	    return -1;
	}
	if(openfilelist[fd].fcbstate)
	{
	    fcbptr = (fcb *)malloc(sizeof(fcb));
	    strcpy(fcbptr->filename, openfilelist[fd].filename);
	    strcpy(fcbptr->exname, openfilelist[fd].exname);
        fcbptr->attribute = openfilelist[fd].attribute;
        fcbptr->time = openfilelist[fd].time;
        fcbptr->date = openfilelist[fd].date;
        fcbptr->first = openfilelist[fd].first;
        fcbptr->length = openfilelist[fd].length;
        fcbptr->free = openfilelist[fd].free;
        father = openfilelist[fd].father;
        openfilelist[father].count = openfilelist[fd].diroff * sizeof(fcb);
        do_write(father, (char *)fcbptr, sizeof(fcb), 2);
        free(fcbptr);
        openfilelist[fd].fcbstate = 0;
	}
	strcpy(openfilelist[fd].filename, "");
	strcpy(openfilelist[fd].exname, "");
	openfilelist[fd].topenfile = 0;
	return father;
}
int do_write(int fd, char *text, int len, char wstyle)
{
    fat *fat1, *fat2, *fatptr1, *fatptr2;
    unsigned char *buf, *blkptr;
    unsigned int blkno, blkoff;
    int i, ll;
    fat1 = (fat *)(map + BLOCKSIZE * 2);
    fat2 = (fat *)(map + 3 * BLOCKSIZE * 2);
    buf = (unsigned char *)malloc(BLOCKSIZE * 2);
    if(buf == NULL)
    {
        printf("malloc failed!\n");
        return -1;
    }
	blkno = openfilelist[fd].first;
    blkoff = openfilelist[fd].count;
    fatptr1 = fat1 + blkno;
    fatptr2 = fat2 + blkno;
    while(blkoff >= BLOCKSIZE * 2)
    {
        blkno = fatptr1->id;
        if(blkno == END)
        {
            blkno = findblock();
            if(blkno == -1)
            {
                free(buf);
                return -1;
            }
            fatptr1->id = blkno;
            fatptr2->id = blkno;
            fatptr1 = fat1 + blkno;
            fatptr2 = fat2 + blkno;
            fatptr1->id = END;
            fatptr2->id = END;
        }
        else
        {
            fatptr1 = fat1 + blkno;
            fatptr2 = fat2 + blkno;
        }
        blkoff = blkoff - BLOCKSIZE * 2;
    }

    ll = 0;
	while(ll < len)
    {
        blkptr = (unsigned char *)(map + blkno * BLOCKSIZE * 2);
        for(i = 0; i < BLOCKSIZE * 2; i++)
            buf[i] = blkptr[i];
        for(;blkoff < BLOCKSIZE * 2; blkoff++)
        {
            buf[blkoff] = text[ll++];
            openfilelist[fd].count++;
            if(ll == len)
                break;
        }
        for(i = 0; i < BLOCKSIZE * 2; i++)
            blkptr[i] = buf[i];
        if(ll < len)
        {
            blkno = fatptr1->id;
            if(blkno == END)
            {
                blkno = findblock();
                if(blkno == -1)
                    break;
                fatptr1->id = blkno;
                fatptr2->id = blkno;
                fatptr1 = fat1 + blkno;
                fatptr2 = fat2 + blkno;
                fatptr1->id = END;
                fatptr2->id = END;
            }
            else
            {
                fatptr1 = fat1 + blkno;
                fatptr2 = fat2 + blkno;
            }
            blkoff = 0;
			}
    }
	if(openfilelist[fd].count > openfilelist[fd].length)
        openfilelist[fd].length = openfilelist[fd].count;
    openfilelist[fd].fcbstate = 1;
    free(buf);
    return ll;
}
int do_read(int fd, int len, char *text)
{
    fat *fat1, *fatptr;
    unsigned char *buf, *blkptr;
    unsigned short blkno, blkoff;
    int i, ll;
    fat1 = (fat *)(map + BLOCKSIZE * 2);
    buf = (unsigned char *)malloc(BLOCKSIZE * 2);
    if(buf == NULL)
    {
        printf("malloc failed!\n");
        return -1;
    }
    blkno = openfilelist[fd].first;
    blkoff = openfilelist[fd].count;
    if(blkoff >= openfilelist[fd].length)
    {
        puts("Read out of range!");
        free(buf);
        return -1;
    }
    fatptr = fat1 + blkno;
    while(blkoff >= BLOCKSIZE * 2)
    {
        blkno = fatptr->id;
        blkoff = blkoff - BLOCKSIZE * 2;
        fatptr = fat1 + blkno;
    }
	ll = 0;
    while(ll < len)
    {
        blkptr = (unsigned char *)(map + blkno * BLOCKSIZE * 2);
        for(i = 0; i < BLOCKSIZE * 2; i++)
            buf[i] = blkptr[i];
        for(; blkoff < BLOCKSIZE * 2; blkoff++)
        {
            text[ll++] = buf[blkoff];
            openfilelist[fd].count++;
            if(ll == len || openfilelist[fd].count == openfilelist[fd].length)
                break;
        }
        if(ll < len && openfilelist[fd].count != openfilelist[fd].length)
        {
            blkno = fatptr->id;
            if(blkno == END)
                break;
            blkoff = 0;
            fatptr = fat1 + blkno;
        }
    }
    text[ll] = '\0';
    free(buf);
    return ll;
}
unsigned short findblock()
{
    unsigned short i;
	fat *fat1, *fatptr;
	fat1 = (fat *)(map + BLOCKSIZE * 2);
	for(i = 7; i < SIZE / BLOCKSIZE * 2; i++)
	{
		fatptr = fat1 + i;
		if(fatptr->id == FREE)
			return i;
	}
	printf("Error,Can't find free block!\n");
	return -1;
}
int findopenfile()
{
    int i;
    for(i = 0; i < MAXTEXT; i++)
    {
        if(openfilelist[i].topenfile == 0)
            return i;
    }
    printf("Error,open too many files!\n");
    return -1;
}