#ifndef __file_io__
#define __file_io__

#include <stdio.h>

namespace maxssau
{

    enum FILE_MODE
    {
        FILEMODE_READ=0,
        FILEMODE_WRITE=1,
        FILEMODE_APPEND=2
    };

    enum FILE_STATUS
    {
        FILEMODE_SUCCESS=0,
        FILEMODE_FAIL=1
    };

    class file_io
    {
        public:

            

            file_io()
            {

            };

            file_io(char filename[], unsigned char mode)
            {
                Init(filename,mode);
            };

            int Init(char filename[], unsigned char mode)
            {
                if(status!=FILEMODE_SUCCESS)
                {
                    file_mode=mode;
                    switch(mode)
                    {
                        case FILEMODE_READ:
                        {
                            handle=fopen(filename,"r");
                        }break;
                        case FILEMODE_WRITE:
                        {
                            handle=fopen(filename,"w");
                        }break;
                        case FILEMODE_APPEND:
                        {
                            handle=fopen(filename,"a");
                        }break;
                    }

                    if(handle!=0)
                    {
                        status=FILEMODE_SUCCESS;
                        return FILEMODE_SUCCESS;
                    }
                    else
                    {
                        status=FILEMODE_FAIL;
                        return FILEMODE_FAIL;
                    }
                }
                status=FILEMODE_FAIL;
                return FILEMODE_FAIL;
            };

            int SetPosition(long position)
            {
                if(status==FILEMODE_SUCCESS)
                {
                    if(position<GetFileSize() && position>=0)
                    {
                        status_operation=FILEMODE_FAIL;
                        return FILEMODE_FAIL;    
                    }
                    else
                    {
                        fseek(handle,0,position);
                        status_operation=FILEMODE_SUCCESS;
                        return FILEMODE_SUCCESS;
                    }
                }
                else
                {
                    status_operation=FILEMODE_FAIL;
                    return FILEMODE_FAIL;
                }

            }

            long GetFileSize()
            {
                long filesize=0;
                if(status==FILEMODE_SUCCESS)
                {
                    long current_position=ftell(handle);
                    fseek(handle,0,SEEK_END);
                    filesize=ftell(handle);
                    fseek(handle,0,current_position);
                    return filesize;
                }
                else
                {
                    status_operation=FILEMODE_FAIL;
                    return -1;
                }
            }

            int WriteByte(unsigned char byte)
            {
                if(file_mode==FILEMODE_WRITE || file_mode==FILEMODE_APPEND)
                {
                    if(status==FILEMODE_SUCCESS)
                    {
                        fputc(byte, handle);
                        if(ferror(handle)!=0)
                        {
                            status_operation=FILEMODE_FAIL;
                            return FILEMODE_FAIL;
                        }
                        else
                        {
                            status_operation=FILEMODE_SUCCESS;
                            return FILEMODE_SUCCESS;
                        }
                    }
                }
            }

            int WriteBlock(unsigned char bytes[], unsigned int count)
            {
                if(file_mode==FILEMODE_WRITE || file_mode==FILEMODE_APPEND)
                {
                    if(status==FILEMODE_SUCCESS)
                    {
                        for(int i=0;i<count;i++)
                        {
                            fputc(bytes[i],handle);
                            if(ferror(handle)!=0)
                            {
                                status_operation=FILEMODE_FAIL;
                                return FILEMODE_FAIL;
                            }
                        }
                        status_operation=FILEMODE_SUCCESS;
                        return FILEMODE_SUCCESS;
                    }
                }
            }

            int GetBlock(unsigned char bytes[], unsigned int count)
            {
                if(status==FILEMODE_SUCCESS)
                {
                    if(file_mode==FILEMODE_READ)
                    {
                        for(int i=0;i<count;i++)
                        {
                            bytes[i]=(unsigned char)fgetc(handle);
                            if(feof(handle))
                            {
                                status_operation=FILEMODE_FAIL;
                                return FILEMODE_FAIL;
                            }
                        }
                    }
                }
                status_operation=FILEMODE_SUCCESS;
                return FILEMODE_SUCCESS;
            }

            int GetByte(unsigned char *result)
            {
                if(status==FILEMODE_SUCCESS)
                {
                    if(file_mode==FILEMODE_READ)
                    {
                        result[0]=(unsigned char)fgetc(handle);
                        if(feof(handle))
                        {
                            status_operation=FILEMODE_FAIL;
                            return FILEMODE_FAIL;
                        }
                        status_operation=FILEMODE_SUCCESS;
                        return FILEMODE_SUCCESS;
                    }
                    else
                    {
                        return FILEMODE_FAIL;
                    }
                }
                else
                {
                    status_operation=FILEMODE_FAIL;
                    return -1;
                }
            }

            ~file_io()
            {
                if(handle!=0)
                {
                    fflush(handle);
                    fclose(handle);
                }
            }

            int GetStatus()
            {
                return status;
            }

            int GetStatusOperation()
            {
                return status_operation;
            }

        private:
            FILE *handle;
            unsigned char file_mode;
            int status;
            int status_operation;
    };
}

#endif