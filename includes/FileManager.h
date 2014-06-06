#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include <string>
#include <fstream>

class FileManager
{
public:
    typedef unsigned int DataSize;
private:
    typedef unsigned char DataBlockType;

public:

    class Reader
    {
    private:
        class TagInfo;
        class DataInfo;

        typedef std::vector<TagInfo*> TagInfoStack;

    public:
        Reader(const char * _fn);

        bool isOpen() const;

        virtual ~Reader();

        bool openData( const std::string & _name, DataSize & _size );

        DataSize readData(void * _data);
        DataSize readData(void * _data, DataSize _size);


        bool openTag(const std::string & _tagName);

        bool closeTag();
    protected:

        bool openTag(TagInfo * _tagInfo);

        std::string readString();

    private:
        std::ifstream m_stream;

        TagInfoStack m_tagInfoStack;

        DataInfo * m_dataTag;
    };

    class Writer
    {
    private:
        typedef std::vector<DataSize> EndPosStack;
    public:
        Writer( const char * _fn );

        virtual ~Writer();

        void openTag( const char * _tag );

        bool isOpen() const;

        void closeTag();

        void writeData( const char * _tag, const void * _data, DataSize _size );

        void writeData( const char * _tag, const char * _dataString);

    private:
        EndPosStack m_posStack;
        std::ofstream m_stream;
    };

private:

    static const DataBlockType s_tagType;
    static const DataBlockType s_dataType;
};

#endif // FILEMANAGER_H
