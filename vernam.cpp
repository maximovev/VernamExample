#include <stdio.h>
#include <iostream>
#include "file_io.h"
#include "enums.h"

using namespace maxssau;

const int MAX_FILES = 5;
FileIO files[MAX_FILES];

bool CheckProgramArguments(int argc, char* argv[])
{
    if(argc != 3 && argc != 5)
    {
        std::cerr << "Usage:\n"
                  << "For encryption/decryption:\n"
                  << "  program <input_file> <key_file> <output_file>\n"
                  << "For encryption with fake data:\n"
                  << "  program <input_file> <key_file> <output_file> <fake_text_file> <fake_key_output>\n";
        return false;
    }

    // Индексы файлов, которые должны существовать (input, key, fake_text)
    int files_to_check[] = {0, 1, 3};
    int files_to_check_count = (argc == 5) ? 3 : 2;

    for(int i = 0; i < files_to_check_count; i++)
    {
        FILE* temp = fopen(argv[files_to_check[i]], "r");
        if(!temp)
        {
            std::cerr << "Error: File '" << argv[files_to_check[i]] 
                      << "' not found or cannot be read\n";
            return false;
        }
        fclose(temp);
    }

    return true;
}

bool OpenFiles(int argc, char* argv[])
{
    try
    {
        // Открываем файлы для чтения
        for(int i = 0; i < 3; i++)
        {
            if(files[i].Open(argv[i], (i == 2) ? FileMode::Write : FileMode::Read)!=FileStatus::Success)
            {
                std::cerr << "Failed to open file: " << argv[i] << "\n";
                return false;
            }
        }

        // Если есть дополнительные файлы для фейковых данных
        if(argc == 5)
        {
            if(files[3].Open(argv[3], FileMode::Read)!=FileStatus::Success)
            {
                std::cerr << "Failed to open fake text file: " << argv[3] << "\n";
                return false;
            }

            if(files[4].Open(argv[4], FileMode::Write)!=FileStatus::Success)
            {
                std::cerr << "Failed to open fake key output: " << argv[4] << "\n";
                return false;
            }
        }

        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        return false;
    }
}

int main(int argc, char* argv[])
{
    if(!CheckProgramArguments(argc, argv))
    {
        return 1;
    }

    if(!OpenFiles(argc, argv))
    {
        // Закрываем все открытые файлы
        for(auto& file : files)
        {
            if(file.IsOpen())
            {
                file.Close();
            }
        }
        return 1;
    }

    // Здесь должна быть основная логика работы с файлами

    // Закрываем все файлы перед выходом
    for(auto& file : files)
    {
        if(file.IsOpen())
        {
            file.Close();
        }
    }

    return 0;
}