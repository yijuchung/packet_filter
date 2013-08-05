#include <sys/unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include "p.h"

char dip[MAX_IP];
char sip[MAX_IP];

int sprt;
int dprt;

int main(int argc, char ** argv)
{
	if (argc < 7)
	{
        printf("Usage:\n");
        printf("\t./a.out dest.ip dest.port src.ip src.port cmd val [num]\n");
        printf("Cmd:\n");
		printf("\t1 : set window to val in num packets(if num = -1 stop, num = 0 infinite)\n");
		printf("\t2 : discard val packets(if val = -1 stop, val = 0 infinite)\n");
    }
    else
	{
		strcpy( dip, argv[1] );
		dprt = atoi(argv[2]);
		strcpy( sip, argv[3] );
		sprt = atoi(argv[4]);
		
		int cmd = atoi(argv[5]);
		int val = atoi(argv[6]);
		int num;
		if( cmd == 1 )
			num = atoi(argv[7]);
		
		printf("%s : %d to %s : %d",dip,dprt,sip,sprt);
		switch( cmd )
		{
			case 1:
			{
				printf("set window to %d",val);
				if( num == 0 )
					printf("(infinite)\n");
				else if( num == -1 )
					printf("(stop)\n");
				else
					printf(" in %d packet\n",num);
			}break;
			case 2:
			{
				printf("discard ");
				if( val == 0 )
					printf("(infinite)\n");
				else if( val == -1 )
					printf("(stop)\n");
				else
					printf("%d packets\n",val);
			}break;
		}
		
		FILE *fp = fopen("/proc/sys/project/dip","w");
		fprintf(fp,"%s",dip);
		fclose(fp);
		FILE *fp2 = fopen("/proc/sys/project/sip","w");
		fprintf(fp2,"%s",sip);
		fclose(fp2);
		FILE *fp3 = fopen("/proc/sys/project/dprt","w");
		fprintf(fp3,"%d",dprt);
		fclose(fp3);
		FILE *fp4 = fopen("/proc/sys/project/sprt","w");
		fprintf(fp4,"%d",sprt);
		fclose(fp4);
		FILE *fp5 = fopen("/proc/sys/project/val","w");
		fprintf(fp5,"%d",val);
		fclose(fp5);
		FILE *fp6 = fopen("/proc/sys/project/cmd","w");
		fprintf(fp6,"%d",cmd);
		fclose(fp6);
		
		if( cmd == 1 )
		{
			FILE *fp7 = fopen("/proc/sys/project/num","w");
			fprintf(fp7,"%d",num);
			fclose(fp7);		
		}
	}
	//exit(0);
}
