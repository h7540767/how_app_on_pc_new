#include "wrap.h"

int	 Getopt(int argc, char * const argv[], const char *optstring)
{
	int opt;

	if ( ( opt = getopt(argc, argv, optstring)) == '?')
	{
		exit(1);		/* getopt() has already written to stderr */
	}
		
	return(opt);
}

int Open(const char *pathname, int oflag, ...)
{
	int		fd;
	va_list	ap;
	mode_t	mode;

	if (oflag & O_CREAT) 
	{
		va_start(ap, oflag);		/* init ap to final named argument */
		mode = va_arg(ap, mode_t);
		if ( (fd = open(pathname, oflag, mode)) == -1)
		{
			printf("open error for %s: %s\n", pathname, strerror(errno));
			exit(-1);
		}
			
		va_end(ap);
	} 
	else 
	{
		if ( (fd = open(pathname, oflag)) == -1)
		{
			printf("open error for %s: %s\n", pathname, strerror(errno));
			exit(-1);
		}	
	}
	
	return(fd);
}

void Write(int fd, const void *buf, size_t count)
{
	if (write(fd, buf, count) != count)
	{
		printf("write error: %s", strerror(errno));
		exit(-1);
	}
	
	return ;
}

void Close(int fd)
{
	if (close(fd) == -1)
	{
		printf("close error: %s", strerror(errno));
		exit(-1);
	}
	
	return;
}

char * itoa(int num, char * str, int radix)
{	/*索引表*/
	char index[] = "0123456789ABCDEF";
	unsigned unum;/*中间变量*/
	int i = 0, j, k;
	/*确定unum的值*/
	if (radix == 10 && num < 0)/*十进制负数*/
	{
		unum = (unsigned) - num;
		str[i++] = '-';
	}
	else 
	{
		unum = (unsigned)num;/*其他情况*/
	}
	/*转换*/
	do
	{
		str[i++] = index[unum % (unsigned)radix];
		unum /= radix;
	}while (unum);
	
	str[i] = '\0';
	/*逆序*/
	if(str[0] == '-')
	{
		k = 1;/*十进制负数*/
	}
	else
	{
		k = 0;
	}
	char temp;
	for(j = k; j <= (i - 1) / 2; j++)
	{
		temp = str[j];
		str[j] = str[i - 1 + k - j];
		str[i - 1 + k - j] = temp;
	}
	return str;
}

FILE * Fopen(const char *pathname, const char *mode)
{
	FILE *fp;

	if ((fp = fopen(pathname, mode)) == NULL)
	{
		printf("open %s error: %s", pathname, strerror(errno));
		exit(-1);
	}
	
	return(fp);
}

void Fclose(FILE *fp)
{
	if (fclose(fp) != 0)
	{
		printf("fclose error: %s\n", strerror(errno));
		exit(-1);
	}
	
	return ;
}

ssize_t Getline(char **lineptr, size_t *n, FILE *stream)
{
	ssize_t ret;
	if ((ret = getline(lineptr, n, stream)) == -1)
	{
		printf("getline error: %s\n", strerror(errno));
		exit(-1);
	}
	
	return ret;
}

void * Malloc(size_t size)
{
	void * ptr;

	if ( (ptr = malloc(size)) == NULL)
	{
		printf("malloc error: %s", strerror(errno));
		exit(-1);
	}
		
	return(ptr);
}

int myrand(int start, int end)
{
	if (start > end)
	{
		cout << "myrand usage: start <= end\n";
		exit(-1);
	}
	return rand() % (end - start + 1) + start;
}















