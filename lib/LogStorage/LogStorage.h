#ifndef _LOGSTORAGE_H_
#define _LOGSTORAGE_H_

enum _EndlCode { endl };

#define LOG_BUFFER_LEN 8192

class LogStorage {
    private:
        const unsigned int bufferSize;    
        unsigned int writePos;
        char buffer[LOG_BUFFER_LEN];
        void writeToBuffer( char const* buffer );
    public:
        LogStorage();
        LogStorage& operator<< ( char const * str );
        LogStorage& operator<< ( long int num );
        LogStorage& operator<< ( long unsigned int num );
        LogStorage& operator<< ( int num );
        LogStorage& operator<< ( unsigned int num );
        LogStorage& operator<< ( double num );
        LogStorage& operator<< (  _EndlCode );
        void sendToSerial();
};

extern LogStorage Log;

#endif