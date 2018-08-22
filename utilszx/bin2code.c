/*
 *	Quick 'n' dirty mym to tap converter
 *
 *      Usage: bin2code [binfile] [tapfile]
 *         or: bin2code [binfile] [tapfile] [start]
 *
 *	zack 8/2/2000
 *	Modified by Stefano	3/12/2000
 *      Modified by Metalbrain 27/06/2002
 *
 *      Creates a new TAP file (overwriting if necessary) with the CODE block.
 */

#include <stdio.h>

/* Stefano reckons stdlib.h is needed for binary files for Win compilers*/
#include <stdlib.h>

unsigned char parity;

void writebyte(unsigned char, FILE *);
void writeword(unsigned int, FILE *);

int main(int argc, char *argv[])
{
	char	name[11];
	FILE	*fpin, *fpout;
	int	c;
	int	i;
	int	len;
        int     datastart;

        if ( (argc<3) || (argc>4) ) {
		fprintf(stdout,"Usage: %s [code file] [tap file]\n",argv[0]);
                fprintf(stdout,"   or: %s [code file] [tap file] [data start address]\n",argv[0]);
		exit(1);
	}

        if (argc==3)
                {
                datastart=32768;
                }
        else
                {
                datastart=atoi(argv[3]);
                if ((datastart<0)||(datastart>65536))
                        {
                        printf("B Integer Out of Range");
                        }
                }

	if ( (fpin=fopen(argv[1],"rb") ) == NULL ) {
		printf("Can't open input file\n");
		exit(1);
	}


/*
 *	Now we try to determine the size of the file
 *	to be converted
 */
	if	(fseek(fpin,0,SEEK_END)) {
		printf("Couldn't determine size of file\n");
		fclose(fpin);
		exit(1);
	}

	len=ftell(fpin);

	fseek(fpin,0L,SEEK_SET);

	if ( (fpout=fopen(argv[2],"wb") ) == NULL ) {
		printf("Can't open output file\n");
		exit(1);
	}

/* Write out the code header file */
	fputc(19,fpout);	/* Header len */
	fputc(0,fpout);		/* MSB of len */
	fputc(0,fpout);		/* Header is 0 */
	parity=0;
	writebyte(3,fpout);	/* Filetype (Code) */
/* Deal with the filename */
	if (strlen(argv[1]) >= 10 ) {
		strncpy(name,argv[1],10);
	} else {
		strcpy(name,argv[1]);
		strncat(name,"          ",10-strlen(argv[1]));
	}
	for	(i=0;i<=9;i++)
		writebyte(name[i],fpout);
	writeword(len,fpout);
        writeword(datastart,fpout); /* load address */
	writeword(0,fpout);	/* offset */
	writebyte(parity,fpout);
/* Now onto the data bit */
	writeword(len+2,fpout);	/* Length of next block */
	parity=0;
	writebyte(255,fpout);	/* Data... */
	for (i=0; i<len;i++) {
		c=getc(fpin);
		writebyte(c,fpout);
	}
	writebyte(parity,fpout);
	fclose(fpin);
	fclose(fpout);
}
		

void writeword(unsigned int i, FILE *fp)
{
	writebyte(i%256,fp);
	writebyte(i/256,fp);
}



void writebyte(unsigned char c, FILE *fp)
{
	fputc(c,fp);
	parity^=c;
}
