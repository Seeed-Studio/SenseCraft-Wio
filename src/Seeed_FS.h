#ifndef __SEEED_FS__
#define __SEEED_FS__

/*
    ________________________________________________________________________________________________________________________________________
    |    Flags         |                            Meaning                                                                                  |
    |----------------------------------------------------------------------------------------------------------------------------------------|
    | FA_READ          | Specifies read access to the object. Data can be read from the file.                                                |
    | FA_WRITE         | Specifies write access to the object. Data can be written to the file. Combine with FA_READ for read-write access.  |
    | FA_OPEN_EXISTING | Opens the file. The function fails if the file is not existing. (Default)                                           |
    | FA_CREATE_NEW    | Creates a new file. The function fails with FR_EXIST if the file is existing.                                       |
    | FA_CREATE_ALWAYS | Creates a new file. If the file is existing, it will be truncated and overwritten.                                  |
    | FA_OPEN_ALWAYS   | Opens the file if it is existing. If not, a new file will be created.                                               |
    | FA_OPEN_APPEND   | Same as FA_OPEN_ALWAYS except the read/write pointer is set end of the file.                                        |
    |__________________|_____________________________________________________________________________________________________________________|
*/

/*
    _____________________________________________________
    |    POSIX    |               FatFs                   |
    |-------------|---------------------------------------|
    |"r"          |FA_READ                                |
    |"r+"         |FA_READ | FA_WRITE                     |
    |"w"          |FA_CREATE_ALWAYS | FA_WRITE            |
    |"w+"         |FA_CREATE_ALWAYS | FA_WRITE | FA_READ  |
    |"a"          |FA_OPEN_APPEND | FA_WRITE              |
    |"a+"         |FA_OPEN_APPEND | FA_WRITE | FA_READ    |
    |"wx"         |FA_CREATE_NEW | FA_WRITE               |
    |"w+x"        |FA_CREATE_NEW | FA_WRITE | FA_READ     |
    |_____________|_______________________________________|
*/

#define FILE_READ FA_READ
#define FILE_WRITE (FA_CREATE_ALWAYS | FA_WRITE | FA_READ)
#define FILE_APPEND (FA_OPEN_APPEND | FA_WRITE)

extern "C"
{
#include "diskio.h"
#include "ffconf.h"
#include "ff.h"
    char CRC7(const char *data, int length);
    unsigned short CRC16(const char *data, int length);
}

namespace fs
{

    enum SeekMode
    {
        SeekSet = 0,
        SeekCur = 1,
        SeekEnd = 2
    };

    class File : public Stream
    {
    private:
        char _name[_MAX_LFN + 2]; // file name
        FIL *_file;               // underlying file pointer
        DIR *_dir;                // if open a dir
        FILINFO *_fno;            // for traverse directory

    public:
        File(FIL f, const char *name); // wraps an underlying SdFile
        File(DIR d, const char *name);
        File(void); // 'empty' constructor
        ~File();
        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *buf, size_t size);
        virtual int read();
        virtual int peek();
        virtual int available();
        virtual void flush();
        size_t read(void *buf, uint32_t nbyte);
        char *gets(char *str, uint32_t nbyte);
        bool seek(uint32_t pos);
        bool seek(uint32_t pos, SeekMode mode);
        uint32_t position();
        uint32_t tell();
        uint32_t size();
        void close();
        operator bool() const;
        char *name();

        boolean isDirectory(void);
        File openNextFile(uint8_t mode = FA_READ);
        void rewindDirectory(void);

        File &operator=(const File &f);

        using Print::write;
    };
    class FS
    {
    protected:
        FATFS root;
        uint8_t _pdrv;
        TCHAR _drv[2] = {_T(char('0' + _pdrv)), _T(':')};

    public:
        static TCHAR _path[_MAX_LFN + 2];
        // Open the specified file/directory with the supplied mode (e.g. read or
        // write, etc). Returns a File object for interacting with the file.
        // Note that currently only one file can be open at a time.
        File open(const char *filepath, uint8_t mode = FILE_READ);
        File open(const String &filepath, uint8_t mode = FILE_READ)
        {
            return open(filepath.c_str(), mode);
        }
        File open(const char *filepath, const char *mode);
        File open(const String &filepath, const char *mode)
        {
            return open(filepath.c_str(), mode);
        }

        // Methods to determine if the requested file path exists.
        boolean exists(const char *filepath);
        boolean exists(const String &filepath)
        {
            return exists(filepath.c_str());
        }

        // Create the requested directory heirarchy--if intermediate directories
        // do not exist they will be created.
        boolean mkdir(const char *filepath);
        boolean mkdir(const String &filepath)
        {
            return mkdir(filepath.c_str());
        }

        boolean rename(const char *pathFrom, const char *pathTo);
        boolean rename(const String &pathFrom, const String &pathTo)
        {
            return rename(pathFrom.c_str(), pathTo.c_str());
        };

        // Delete the file.
        boolean remove(const char *filepath);
        boolean remove(const String &filepath)
        {
            return remove(filepath.c_str());
        }

        boolean rmdir(const char *filepath);
        boolean rmdir(const String &filepath)
        {
            return rmdir(filepath.c_str());
        }
    };
};
using namespace fs;

#endif