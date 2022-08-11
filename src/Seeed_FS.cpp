/*

    SD - a slightly more friendly wrapper for fatfs.


    Created by hongtai.liu 13 July 2019


*/
#include <Arduino.h>
#include <Seeed_FS.h>

namespace fs
{

    TCHAR FS::_path[] = {0};

    File::File(FIL f, const char *n)
    {
        // is a file.
        _file = new FIL(f);
        if (_file)
        {
            _dir = NULL;
            sprintf((char *)_name, "%s", n);
        }
        _fno = NULL;
    }

    File::File(DIR d, const char *n)
    {
        // is a directory
        _dir = new DIR(d);
        if (_dir)
        {
            _file = NULL;
            sprintf((char *)_name, "%s", n);
        }
        _fno = NULL;
    }

    File::File(void)
    {
        _file = NULL;
        _dir = NULL;
        _name[0] = 0;
        // Serial.print("Created empty file object");
        _fno = NULL;
    }

    File::~File()
    {
        // close();
    }

    // returns a pointer to the file name
    char *File::name(void)
    {
        return _name;
    }

    // a directory is a special type of file
    boolean File::isDirectory(void)
    {
        FRESULT ret = FR_OK;
        FILINFO v_fileinfo;

        if (_dir && !_file)
        {
            return true;
        }
        if ((ret = f_stat(_name, &v_fileinfo)) == FR_OK)
        {
            if (v_fileinfo.fattrib & AM_DIR)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        return false;
    }

    size_t File::write(uint8_t val)
    {
        return write(&val, 1);
    }

    size_t File::write(const uint8_t *buf, size_t size)
    {
        UINT t;
        FRESULT ret = FR_OK;
        if (!_file)
        {
            return false;
        }
        ret = f_write(_file, buf, size, &t);

        if (FR_OK == ret)
        {
            return t;
        }
        return false;
    }

    // return the peek value
    int File::peek()
    {
        if (!_file)
        {
            return 0;
        }

        int c = read();
        if (c != -1)
        {
            f_lseek(_file, f_tell(_file) - 1);
        }
        return c;
    }

    // read a value
    int File::read()
    {
        uint8_t val;
        if (_file)
        {
            return read(&val, 1) == 1 ? val : -1;
        }
        return -1;
    }

    char *File::gets(char *str, uint32_t nbyte)
    {
        if (!_file)
        {
            return NULL;
        }
        return f_gets(str, nbyte, _file);
    }

    size_t File::read(void *buf, uint32_t nbyte)
    {
        UINT t;
        if (!_file)
        {
            return 0;
        }
        if (f_read(_file, buf, nbyte, &t) == FR_OK)
        {

            return (size_t)t;
        }
        else
        {
            return 0;
        }
    }

    int File::available()
    {
        if (!_file)
        {
            return 0;
        }

        return !f_eof(_file);
    }

    void File::flush()
    {
        if (_file)
        {
            f_sync(_file);
        }
    }

    boolean File::seek(uint32_t pos)
    {
        if (!_file)
        {
            return false;
        }

        return f_lseek(_file, pos);
    }

    bool File::seek(uint32_t pos, SeekMode mode)
    {
        if (!_file)
        {
            return false;
        }
        switch (mode)
        {
        case SeekSet:
            return f_lseek(_file, pos);
            break;
        case SeekCur:
            f_lseek(_file, f_tell(_file) + pos);
            break;
        case SeekEnd:
            return f_lseek(_file, f_size(_file) - pos);
            break;
        default:
            return false;
            break;
        }
        return false;
    }

    uint32_t File::position()
    {
        if (!_file)
        {
            return -1;
        }
        return f_tell(_file);
    }

    uint32_t File::tell()
    {
        if (!_file)
        {
            return -1;
        }
        return f_tell(_file);
    }

    uint32_t File::size()
    {
        return f_size(_file);
    }

    void File::close()
    {
        if (_dir)
        {
            f_closedir(_dir);
            delete _dir;
            _dir = NULL;
        }
        if (_file)
        {
            f_close(_file);
            delete _file;
            _file = NULL;
        }
        if (_fno)
        {
            delete _fno;
            _file = NULL;
        }
    }

    // allows you to recurse into a directory
    File File::openNextFile(uint8_t mode)
    {
        FRESULT res;
        UINT i;
        static char path[257];

        strcpy(path, _name);
        if (!_fno)
        {
            _fno = new FILINFO;
        }

        for (; _fno;)
        {
            res = f_readdir(_dir, _fno); /* Read a directory item */
            if (res != FR_OK || _fno->fname[0] == 0)
            {
                break; /* Break on error or end of dir */
            }
            if (_fno->fattrib == 255)
            {
                continue; /*ignore if the addr was removed*/
            }

            i = strlen(path);
            if (i && path[i - 1] != '/')
            {
                path[i++] = '/';
            }
            sprintf((char *)path + i, "%s", _fno->fname);

            if (_fno->fattrib & AM_DIR)
            {
                /* It is a directory */
                DIR dir;
                if ((res = f_opendir(&dir, path)) == FR_OK)
                {
                    return File(dir, path);
                }
                else
                {
                    return File();
                }
            }
            else
            {
                FIL file;
                if ((res = f_open(&file, path, mode)) == FR_OK)
                {
                    return File(file, path);
                }
                else
                {
                    return File();
                }
            }
        }
        return File();
    }

    void File::rewindDirectory(void)
    {
        if (isDirectory())
        {
            f_rewinddir(_dir);
        }
    }

    File::operator bool() const
    {
        if (_file || _dir)
        {
            return true;
        }
        return false;
    }

    File &File::operator=(const File &f)
    {
        if (this != &f)
        {
            close();
            this->_file = f._file;
            this->_dir = f._dir;
            this->_fno = f._fno;
            strcpy(this->_name, f._name);
        }
        return *this;
    }

    File FS::open(const char *filepath, uint8_t mode)
    {
        FRESULT ret = FR_OK;
        FILINFO v_fileinfo;
        FS::_path[0] = _T('0' + _pdrv);
        FS::_path[1] = _T(':');
        FS::_path[2] = _T('/');
        FS::_path[3] = '\0';

        strcat(FS::_path, filepath);

        if ((ret = f_stat(FS::_path, &v_fileinfo)) == FR_OK)
        {
            if (v_fileinfo.fattrib & AM_DIR)
            {
                DIR dir;
                if ((ret = f_opendir(&dir, FS::_path)) == FR_OK)
                {
                    return File(dir, FS::_path);
                }
                else
                {
                    return File();
                }
            }
            else
            {
                FIL file;
                if ((ret = f_open(&file, FS::_path, mode)) == FR_OK)
                {
                    return File(file, FS::_path);
                }
                else
                {
                    return File();
                }
            }
        }
        else
        {
            FIL file;
            if ((ret = f_open(&file, FS::_path, mode)) == FR_OK)
            {
                return File(file, FS::_path);
            }
            else
            {
                return File();
            }
        }
    }

    File FS::open(const char *filepath, const char *mode)
    {
        if (strlen(mode) > 3)
        {
            return File();
        }
        if (!strcmp(mode, "r"))
        {
            return open(filepath, FA_READ);
        }
        if (!strcmp(mode, "r+"))
        {
            return open(filepath, FA_READ | FA_WRITE);
        }
        if (!strcmp(mode, "w"))
        {
            return open(filepath, FA_CREATE_ALWAYS | FA_WRITE);
        }
        if (!strcmp(mode, "w+"))
        {
            return open(filepath, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
        }
        if (!strcmp(mode, "a"))
        {
            return open(filepath, FA_OPEN_APPEND | FA_WRITE);
        }
        if (!strcmp(mode, "a+"))
        {
            return open(filepath, FA_OPEN_APPEND | FA_WRITE | FA_READ);
        }
        if (!strcmp(mode, "wx"))
        {
            return open(filepath, FA_CREATE_NEW | FA_WRITE);
        }
        if (!strcmp(mode, "w+x"))
        {
            return open(filepath, FA_CREATE_NEW | FA_WRITE | FA_READ);
        }

        return File();
    }

    boolean FS::exists(const char *filepath)
    {
        FRESULT ret = FR_OK;
        FILINFO v_fileinfo;
        FS::_path[0] = _T('0' + _pdrv);
        FS::_path[1] = _T(':');
        FS::_path[2] = _T('/');
        FS::_path[3] = '\0';

        strcat(FS::_path, filepath);
        if ((ret = f_stat(FS::_path, &v_fileinfo)) == FR_OK)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    boolean FS::mkdir(const char *filepath)
    {
        FRESULT ret = FR_OK;
        FS::_path[0] = _T('0' + _pdrv);
        FS::_path[1] = _T(':');
        FS::_path[2] = _T('/');
        FS::_path[3] = '\0';

        strcat(FS::_path, filepath);

        ret = f_mkdir(FS::_path);

        if (ret == FR_OK)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    boolean FS::rename(const char *pathFrom, const char *pathTo)
    {
        FRESULT ret = FR_OK;
        char file[_MAX_LFN + 2];

        FS::_path[0] = _T('0' + _pdrv);
        FS::_path[1] = _T(':');
        FS::_path[2] = _T('/');
        FS::_path[3] = '\0';

        strcpy(file, FS::_path);
        strcat(FS::_path, pathFrom);
        strcat(file, pathTo);

        ret = f_rename(FS::_path, file);

        if (ret == FR_OK)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    boolean FS::rmdir(const char *filepath)
    {
        char file[_MAX_LFN + 2];

        FS::_path[0] = _T('0' + _pdrv);
        FS::_path[1] = _T(':');
        FS::_path[2] = _T('/');
        FS::_path[3] = '\0';

        strcat(FS::_path, filepath);

        FRESULT status;
        DIR dj;
        FILINFO fno;
        status = f_findfirst(&dj, &fno, FS::_path, _T("*"));
        while (status == FR_OK && fno.fname[0])
        {
            sprintf((char *)file, "%s/%s", filepath, fno.fname);
            if (fno.fattrib & AM_DIR)
            {
                rmdir(file);
            }
            else
            {
                remove(file);
            }
            status = f_findnext(&dj, &fno);
        }
        f_closedir(&dj);
        if (remove(filepath))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    boolean FS::remove(const char *filepath)
    {
        FRESULT ret = FR_OK;

        FS::_path[0] = _T('0' + _pdrv);
        FS::_path[1] = _T(':');
        FS::_path[2] = _T('/');
        FS::_path[3] = '\0';

        strcat(FS::_path, filepath);

        ret = f_unlink(FS::_path);
        if (ret == FR_OK)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}; // namespace fs
