#include <stdio.h>
#include "file_io.h"
#include "enums.h"

using namespace maxssau;


file_io files[5];

int CheckProgramArguments(int arg_count, char* arg_value[])
{
    if(arg_count==3 || arg_count==5)
    {
        /*
        3 args:

        input file
        key
        output file

        5 args:

        input file
        key
        output file

        fake_text_input
        fake_key_output
        */
       int file_open_index[]={0,1,3};
       for(int i=0;i<3;i++)
       {
            FILE *temp=fopen(arg_value[file_open_index[i]],"r");
            if(temp==0)
            {
                printf("File %s not found or read error\n",arg_value[file_open_index[i]]);
                return -1;
            }
            fclose(temp);
       }
       return 1;
    }
    return -1;
};

int main(int arg_count, char* arg_value[])
{
    if(CheckProgramArguments(arg_count,&arg_value[0]))
    {
        for(int i=0;i<arg_count;i++)
        {
            switch(i)
            {
                case 0:
                case 1:
                case 3:
                {
                    files[i].Init(arg_value[i],FILEMODE_READ);
                }break;
                case 2:
                case 4:
                {
                    files[i].Init(arg_value[i],FILEMODE_WRITE);
                }break;
            }
        }
    }
    else
    {
        return 1;    
    }
    return 0;
}