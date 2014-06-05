#include <map>

#include <string.h>

#include "FileManager.h"


const FileManager::DataBlockType FileManager::s_tagType = 1;
const FileManager::DataBlockType FileManager::s_dataType = 2;

class FileManager::Reader::DataInfo
{
protected:
    typedef FileManager::DataSize DataSize;
public:
    DataInfo(DataSize _begin, DataSize _end) :
        m_begin(_begin),
        m_end(_end)
    { }


    DataSize getBegin() const
    {
        return m_begin;
    }
    DataSize getEnd() const
    {
        return m_end;
    }

    DataSize getInterval() const
    {
        return m_end - m_begin;
    }
protected:
    DataSize m_begin;
    DataSize m_end;
};


class FileManager::Reader::TagInfo : public FileManager::Reader::DataInfo
{
private:
    typedef std::map<std::string, TagInfo> ChildrenTagMap;
    typedef std::map<std::string, DataInfo> ChildrenDataMap;

public:
    TagInfo(DataSize _begin = 0, DataSize _end = 0) :
        FileManager::Reader::DataInfo(_begin, _end)
    { }

    virtual ~TagInfo()
    { }


    void addTag(const std::string & _name, DataSize _begin, DataSize _end)
    {
        m_tags.insert( ChildrenTagMap::value_type( _name, TagInfo( _begin, _end ) ) );
    }

    void addData(const std::string & _name, DataSize _begin, DataSize _end)
    {
        m_data.insert( ChildrenDataMap::value_type( _name, DataInfo( _begin, _end ) ) );
    }

    FileManager::Reader::TagInfo * getTag(const std::string & _name)
    {
        ChildrenTagMap::iterator it = m_tags.find( _name );
        if ( it == m_tags.end() )
            return NULL;

        return &it->second;
    }

    FileManager::Reader::DataInfo * getData(const std::string & _name)
    {
        ChildrenDataMap::iterator it = m_data.find( _name );
        if ( it == m_data.end() )
            return NULL;

        return &it->second;
    }

    void clear()
    {
        m_tags.clear();
        m_data.clear();
    }

protected:
    ChildrenTagMap m_tags;
    ChildrenDataMap m_data;
};

FileManager::Reader::Reader(const char * _fn) :
    m_dataTag(NULL)
{
    m_stream.open( _fn, std::ifstream::binary );
    m_stream.seekg( 0, std::ios_base::end );
    DataSize fileSize = m_stream.tellg();
    m_stream.seekg( 0, std::ios_base::beg );

    TagInfo * tagInfo = new FileManager::Reader::TagInfo( 0, fileSize );
    openTag( tagInfo );
}

bool FileManager::Reader::isOpen() const
{
    return m_stream.is_open() && !m_tagInfoStack.empty();
}

std::string FileManager::Reader::readString()
{
    std::string s;
    char c = 0;
    do
    {
        c = m_stream.get( );

        if ( !m_stream.good( ) )
            c = 0;

        if ( c )
            s += c;
    }
    while ( c );

    return s;
}

bool FileManager::Reader::openTag(const std::string & _tagName)
{
    if ( m_tagInfoStack.empty() )
        return false;
    return openTag( m_tagInfoStack.back()->getTag(_tagName) );
}

bool FileManager::Reader::closeTag()
{
    if ( m_tagInfoStack.empty() )
        return false;

    m_tagInfoStack.back()->clear();
    m_tagInfoStack.pop_back();
    m_dataTag = NULL;

    return true;
}

bool FileManager::Reader::openTag(TagInfo * _tagInfo)
{
    if ( !m_stream.is_open() )
        return false;
    if (!_tagInfo)
        return false;

    DataBlockType type = 0;
    DataSize nextPos = _tagInfo->getBegin();

    while ( nextPos < _tagInfo->getEnd() )
    {
        m_stream.seekg( nextPos, std::ios_base::beg );

        m_stream.read( (char*)&type, sizeof(DataBlockType) );
        m_stream.read( (char*)&nextPos, sizeof(DataSize) );
        std::string name = readString();

        if ( type == s_tagType )
            _tagInfo->addTag( name, m_stream.tellg(), nextPos );
        else if ( type == s_dataType )
            _tagInfo->addData( name, m_stream.tellg(), nextPos );
        else
            break;
    }

    if ( !m_stream.good(  ) )
        return false;

    if ( nextPos != _tagInfo->getEnd() )
        return false;

    m_tagInfoStack.push_back(_tagInfo);

    return true;
}

bool FileManager::Reader::openData( const std::string & _name, DataSize & _size )
{
    if ( !isOpen() )
        return false;
    m_dataTag = m_tagInfoStack.back()->getData(_name);
    if (m_dataTag)
        _size = m_dataTag->getInterval();
    return !!m_dataTag;
}

FileManager::Reader::~Reader()
{
    if ( !m_tagInfoStack.empty() )
        delete m_tagInfoStack.front();
    m_stream.close();
}

bool FileManager::Reader::readData(void * _data)
{
    if (!m_dataTag)
        return false;

    m_stream.seekg( m_dataTag->getBegin(), std::ios_base::beg );
    m_stream.read( (char *)_data, m_dataTag->getInterval() );

    return true;
}


FileManager::Writer::Writer( const char * _fn )
{
    m_stream.open( _fn );
}

FileManager::Writer::~Writer()
{
    while ( !m_posStack.empty() )
        closeTag();
    m_stream.close();
}

void FileManager::Writer::openTag( const char * _tag )
{
    m_stream.write( (const char*)&s_tagType, sizeof(DataBlockType) );
    m_posStack.push_back( m_stream.tellp() );
    DataSize zero = 0;
    m_stream.write( (const char*)&zero, sizeof(DataSize) );
    m_stream.write( _tag, strlen(_tag) + 1 );
}

bool FileManager::Writer::isOpen() const
{
    return m_stream.is_open();
}

void FileManager::Writer::closeTag()
{
    if ( !m_posStack.empty() )
    {
        DataSize currentPos = m_stream.tellp();
        m_stream.seekp( m_posStack.back(), std::ios_base::beg );
        m_stream.write( (const char*)&currentPos, sizeof(DataSize) );
        m_stream.seekp( currentPos, std::ios_base::beg );
        m_posStack.pop_back();
    }
}

void FileManager::Writer::writeData( const char * _tag, const void * _data, DataSize _size )
{
    DataSize tagLength = strlen(_tag);
    DataSize nextPos = (DataSize)m_stream.tellp() + sizeof(DataSize) + sizeof(DataBlockType) + tagLength + 1 + _size;
    m_stream.write( (const char*)&s_dataType, sizeof(DataBlockType) );
    m_stream.write( (const char*)&nextPos, sizeof(DataSize) );
    m_stream.write( (const char*)_tag, tagLength + 1 );
    m_stream.write( (const char*)_data, _size );
}
