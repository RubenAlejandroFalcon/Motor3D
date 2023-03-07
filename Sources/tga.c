#include "tga.h"

unsigned char texture[NUM_TEXTURAS][128][128];
char palt[256][3];

void LoadTarga(char *filename)
{       
        short i;
        FILE *in;

        if ( (in=fopen(filename, "rb")) == NULL )
        {
                ShutDownGraphicMode();
                printf("\n\t\t\tOoops no se puede abrir %s :(\n\n\n", filename);
                exit(1);
        }
        
        fseek(in, 18, SEEK_SET);
        fread(palt, 768, 1, in);

        fstat(fileno(in), &file);
        fread(texture, file.st_size-18-768, 1, in); //para x texturas
                                                    //128x128
        fclose(in);

        disable();
        outportb(0x3c8,0);
        for (i=0; i<256; i++)
        {
                outportb(0x3c9, palt[i][2]>>2);
                outportb(0x3c9, palt[i][1]>>2);
                outportb(0x3c9, palt[i][0]>>2);
        }
        enable();
}       
