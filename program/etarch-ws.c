#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 1000

int main(int argc, char *argv[])
{
	(void) argc;
    char iface[MAXCHAR], et[MAXCHAR], crc[MAXCHAR];
    FILE *fp;
    char filename[] = "/etc/etarch/etarch.conf";
 
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }

while ((fscanf(fp, "%s %s", iface, et)) == 2)
    {
	char exec1[] = "/usr/local/bin/etarch-cv ";
	strcat(exec1, et);
	//char plus[1] = "+";
	strcat(exec1, "+");
	strcat(exec1, argv[2]);
	
// INIT convert et
	FILE *fp1;
	if ((fp1 = popen(exec1, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
	}

	while (fgets(crc, MAXCHAR, fp1) != NULL) {
        // Do whatever you want here...
//        printf("%.2s", crc);
    }

    if(pclose(fp1))  {
        printf("Command not found or exited with error status\n");
        return -1;
    }
//END convert et
	char t0[MAXCHAR] = "/bin/ip link ";
	char T0[MAXCHAR] = "/bin/ip link set ";

if (strcmp(argv[1],"start") == 0) {	
	char t1[MAXCHAR] = "add link ";
	char t2[] =" address ";
	char t3[] =" ";
	char t4[] =":";
	strcat(t0,t1);
	strcat(t0,iface);
	strcat(t0,t3);
	strcat(t0,argv[2]);
	strcat(t0,t2);

char m0[3], m1[3], m2[3], m3[3], m4[3], m5[3];
sprintf(m0,"%.2s", crc);
sprintf(m1,"%.2s", &crc[2]);
sprintf(m2,"%.2s", &crc[4]);
sprintf(m3,"%.2s", &crc[6]);
sprintf(m4,"%.2s", &crc[8]);
sprintf(m5,"%.2s", &crc[10]);

	strcat(t0,m0);
	strcat(t0,t4);
	strcat(t0,m1);
	strcat(t0,t4);
	strcat(t0,m2);
	strcat(t0,t4);
	strcat(t0,m3);
	strcat(t0,t4);
	strcat(t0,m4);
	strcat(t0,t4);
	strcat(t0,m5);

	char t5[] = " type macvlan mode bridge";
	strcat(t0,t5);

	strcat(T0, argv[2]);
	strcat(T0, " up");
system(t0);
system(T0);
}
else if (strcmp(argv[1],"stop") == 0) {
	char t1[MAXCHAR] = "delete ";
	strcat(t0,t1);
	strcat(t0,argv[2]);
system(t0);
}
    }
}
