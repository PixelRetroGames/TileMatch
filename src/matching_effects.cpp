#include "matching_effects.h"

const char PATH_TO_ALL_EFFECTS[]={"res/all_effects.txt"};

void All_Matching_Effects::Load()
{
 FILE *in=fopen(PATH_TO_ALL_EFFECTS,"r");
 fscanf(in,"%d %d ",&effect_start,&effect_loop);
 int sqq=0;
 while(!feof(in))
       {
        char aux[TEXT_LENGTH_MAX];
        int sq;
        fgets(aux,sizeof aux,in);
        sq=strlen(aux)-1;
        if(aux[sq]=='\n')
           aux[sq]=NULL;
        all_effects[sq]=Load_Transparent_Texture(aux);
        effect_start.push_back(sqq);
        sqq++;

        fgets(aux,sizeof aux,in);
        sq=strlen(aux)-1;
        if(aux[sq]=='\n')
           aux[sq]=NULL;
        all_effects[sqq]=Load_Transparent_Texture(aux);
        effect_loop.push_back(sqq);
        sqq++;
       }
 fclose(in);
}
