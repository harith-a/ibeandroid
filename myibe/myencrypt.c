#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "format.h"
#include "ibe_progs.h"
#include <errno.h>

CONF_CTX *cnfctx;

params_t params;


int main(int argc, char const **argv)
{
    FILE *fpin, *fpou;
    int result = 0;
    char defaultcnffile[] = "ibe.cnf";
    char buff[1000];

    char **idarray;
    int i;
    int count;
    argv[1] = "har";
    
    //printf("%d\n",argc);
    
    //char defaultcnffile[100];
    char *cnffile = defaultcnffile;
    char *paramsfile;
    int status;


    cnfctx = LoadConfig(cnffile);

    if (!cnfctx) {
	fprintf(stderr, "error opening %s\n", cnffile);
	exit(1);
    }
    //paramsfile = GetPathParam(cnfctx, "params", 0, "AAoACQAEAEAAFACUAJQABAAEABQAlGliZS0wLjcuMnRlc3Q6s8YXkD/VVgOSWRjKqYqNdIM5wkj/cjtbiWWCjG8advOf5is+GJFv9T0R1Ovd1qXvFqIAsuwjyYKzefX+mXlfgAAAAAAAP/////////////////8AAgBHAEcAAgBAAAEFkZYTS3IosiQAXf6RA8V8M2Cq64HgViiyXfrkZAhXCVyM/FIzXR1QCbEqNIyPIax28od9+FNs/SC98FjtPV7IAAACAEAAAQRFvXpCnjuu1+FTiBFa6UAEH/U5K5m4n61MgWNTkwJ7P+e9QZ0Q0PXGgAV/ojJ4tL/qPSBYx/wmG35kRL3XIWkAAAIARwBHAAIAQAABIvnuT24FG5MUF5CrbblWPP2bAfUre+YSLaiUPgWCe6TAwcr3/CQfsFnqoSHjx6KFCrYDxQS/DdtUYU/Tb4DbvgAAAgBAAAE1aLco1TttGSKx6rRjNztCPxY8Qf9Bw6gQMlCWsuDZHstorDeL+Qxu4moKrBqgDGA4Fv9oO2ehg5y/7xGSY1dfAAAAAAEAAAABecihVD+ih5zYoLqyJCNNI36agCUAAgBHAEcAAgBAAAErZix/YE4iItNA0blSSN2io3z5/e36DiwkVWbk2g4Zhrt6sGT8tprQukcgfk5DYmL0SO7QZwFDMzPcW89KYjDqAAACAEAAAQrkycEV1z2cb7OmgRCGmwPdMJjsn0P3VXOBJYIFTn/RyTN1OuRtBelXMmLg07lXEvwYdm1YRd0jtWypCkWQKIMA");
    
    paramsfile = GetPathParam(cnfctx, "params", 0, "params.txt");


    IBE_init();
    status = FMT_load_params(params, paramsfile);
    if (status != 1) {
	fprintf(stderr, "error loading params file %s\n", paramsfile);
	exit(1);
    }

    //create id array
    count = 1;
    idarray = (char **) alloca(sizeof(char *) * count);

    for (i=0; i<count; i++) {
    idarray[i] = FMT_make_id(argv[i + 1], NULL, params);
    }

    if ((fpin = fopen ("plain.txt", "r")) == NULL) {
        printf ("Error %d opening 'plain.txt'\n", errno);
        return 1;
    }
	
    if ((fpou = fopen ("cipher.txt", "w")) == NULL) {
        printf ("Error %d opening 'cipher.txt'\n", errno);
        return 1;
    }
        
    dup2(fileno(fpou), fileno(stdout));

    FMT_encrypt_stream_array(idarray, 1 , fpin, fpou, params);

//    dup2(fileno(fpou), STDOUT_FILENO);

    fclose(fpou);
    fclose(fpin);
    //fclose(fpou);
    for (i=0; i<count; i++) {
    //printf("%s\n",idarray[0] );
    free(idarray[i]);
    }

    params_clear(params);

    IBE_clear();
    

    return result;
}



