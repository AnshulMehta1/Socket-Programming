/*
Name : Anshul Mehta
Enrolment Nummber: AU1940275
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage(void);

int main(int argc, char **argv)
{

    /* Pointer to the source file */
    FILE *src;
    /* Pointer to the destination file */
    FILE *dest;
    /* File is read one character at a time*/
    /* Character for Parsing the File */
    char ch;
    /* Character for parsing the args */
    char c;
    int inputfile_given=0;
    int outfile_given=0;

    while ((c = getopt(argc, argv, "i:o:")) != -1){
        switch (c){

            /* Case i where the Input Filename is given*/
            case 'i':
                inputfile_given=1;
                /* Open the FIle in read Mode */
                src = fopen(optarg, "r");
                if (src == NULL){
                    printf("File not found. Exiting.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            /* Case o where the outfile name ia given */
            case 'o':
                outfile_given=1;
                /* Open the destination file in write Mode*/
                dest = fopen(optarg, "w");
                if (dest==NULL){
                    printf("File not found. Exiting.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            default:
                exit(1);
        }
    }
    /* It means that the Input file has not been specified*/
    /* Similarly this can also be checked wihtout flags by checking is src==NULL */
    if(inputfile_given==0){
            src= fopen("sample.txt","r");
    }
    /* If the INput file has been specified but the Output file has not been specified*/
      /* Similarly this can also be checked wihtout flags by checking is dest==NULL */
        if(inputfile_given==1 & outfile_given==0){
            dest=fopen("sample.txt","w");

    }

    /* Now we must read and write the Files since we have all the possibilities covered */
    /* Read src until end-of-file char is encountered */
    while( (ch = fgetc(src)) != EOF ){
                        
        printf("%c", ch);
        fputc(ch, dest);
                        
    }

    
    fclose(src);
    fclose(dest);
    return 0;

 }
   
/* Usage Function */
void usage(void){

  printf("Usage:\n");
  printf(" -i <Input FIlename>\n");
  printf(" -o <Output Filename>\n");
  exit (8);
}

/*  */