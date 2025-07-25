#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <string>

namespace maxssau
{
    enum class FileMode
    {
        Read = 0,
        Write = 1,
        Append = 2
    };

    enum class FileStatus
    {
        Success = 0,
        Fail = 1,
        EndOfFile = 2
    };

    class FileIO
    {
    public:
        FileIO() { StartupInit(); }

        FileIO(const std::string& filename, FileMode mode)
        {
            StartupInit();
            Open(filename, mode);
        }

        FileIO(const FileIO&) = delete;
        FileIO& operator=(const FileIO&) = delete;

        ~FileIO()
        {
            Close();
        }

        FileStatus Open(const std::string& filename, FileMode mode)
        {
            if (m_status != FileStatus::Success)
            {
                m_fileMode = mode;
                
                const char* modeStr = "";
                switch (mode)
                {
                    case FileMode::Read:   modeStr = "rb"; break;
                    case FileMode::Write:  modeStr = "wb"; break;
                    case FileMode::Append: modeStr = "ab"; break;
                }

                m_handle = fopen(filename.c_str(), modeStr);
                
                if (m_handle)
                {
                    m_status = FileStatus::Success;
                    return FileStatus::Success;
                }
            }
            
            m_status = FileStatus::Fail;
            return FileStatus::Fail;
        }

        FileStatus Close()
        {
            if (m_handle)
            {
                fflush(m_handle);
                fclose(m_handle);
                m_handle = nullptr;
                m_status = FileStatus::Fail;
                return FileStatus::Success;
            }
            return FileStatus::Fail;
        }

        FileStatus SetPosition(long position)
        {
            if (m_status != FileStatus::Success)
                return FileStatus::Fail;

            if (position < 0 || position > GetFileSize())
                return FileStatus::Fail;

            if (fseek(m_handle, position, SEEK_SET) != 0)
                return FileStatus::Fail;

            return FileStatus::Success;
        }

        long GetFileSize()
        {
            if (m_status != FileStatus::Success)
                return -1;

            long currentPos = ftell(m_handle);
            fseek(m_handle, 0, SEEK_END);
            long size = ftell(m_handle);
            fseek(m_handle, currentPos, SEEK_SET);
            
            return size;
        }

        FileStatus WriteByte(unsigned char byte)
        {
            if (m_status != FileStatus::Success || 
               (m_fileMode != FileMode::Write && m_fileMode != FileMode::Append))
                return FileStatus::Fail;

            if (fputc(byte, m_handle) == EOF)
                return FileStatus::Fail;

            return FileStatus::Success;
        }

        FileStatus WriteBlock(const unsigned char* bytes, size_t count)
        {
            if (m_status != FileStatus::Success || 
               (m_fileMode != FileMode::Write && m_fileMode != FileMode::Append))
                return FileStatus::Fail;

            for (size_t i = 0; i < count; ++i)
            {
                if (fputc(bytes[i], m_handle) == EOF)
                    return FileStatus::Fail;
            }

            return FileStatus::Success;
        }

        FileStatus GetBlock(unsigned char* bytes, size_t count)
        {
            if (m_status != FileStatus::Success || m_fileMode != FileMode::Read)
                return FileStatus::Fail;

            for (size_t i = 0; i < count; ++i)
            {
                int ch = fgetc(m_handle);
                if (ch == EOF)
                    return feof(m_handle) ? FileStatus::EndOfFile : FileStatus::Fail;
                
                bytes[i] = static_cast<unsigned char>(ch);
            }

            return FileStatus::Success;
        }

        FileStatus GetByte(unsigned char& result)
        {
            if (m_status != FileStatus::Success || m_fileMode != FileMode::Read)
                return FileStatus::Fail;

            int ch = fgetc(m_handle);
            if (ch == EOF)
                return feof(m_handle) ? FileStatus::EndOfFile : FileStatus::Fail;
            
            result = static_cast<unsigned char>(ch);
            return FileStatus::Success;
        }

        bool IsOpen() const { return m_handle != nullptr; }
        FileStatus GetStatus() const { return m_status; }
        FileMode GetMode() const { return m_fileMode; }

    private:
        FILE* m_handle = nullptr;
        FileMode m_fileMode = FileMode::Read;
        FileStatus m_status = FileStatus::Fail;

        void StartupInit()
        {
            m_handle = nullptr;
            m_fileMode = FileMode::Read;
            m_status = FileStatus::Fail;
        }
    };
}

#endif // FILE_IO_H