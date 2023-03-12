#pragma once

#include <Thor/Core/Common.h>

namespace Thor
{

template <class TManager>
struct ThResource
{
protected:
    void Acquire(ThU64 uid, TManager* manager)
    {
        m_Uid = uid;
        m_Manager = manager;

        if (m_Manager)
        {
            m_Manager->Acquire(uid);
        }
    }

    void Release()
    {
        if (m_Manager)
        {
            m_Manager->Release(m_Uid);
        }
    }
private:
    ThU64 m_Uid = 0;
    TManager* m_Manager = nullptr;
};

class ThMemoryBuffer
{
public:
    void Init(ThI8* start, ThU64 size)
    {
        m_Start = start;
        m_Size = size;
    }

    ThU64 GetPos()const
    {
        return m_Pos;
    }

    void SetPos(ThU64 pos)const
    {
        if (pos > m_Size)
        {
            pos = m_Size;
        }

        m_Pos = pos;
    }

    const ThI8* Data()const
    {
        return m_Start + m_Pos;
    }

    ThI8* Data()
    {
        return m_Start + m_Pos;
    }

    bool CanWrite(ThU64 size)const
    {
        ThU64 newPos = m_Pos + size;
        return newPos <= m_Size;
    }

    bool Write(ThI8* bytes, ThU64 size)
    {
        ThU64 newPos = m_Pos + size;

        if (newPos > m_Size)
        {
            return false;
        }

        return true;
    }

private:
    ThI8* m_Start = nullptr;
    mutable ThU64 m_Pos = 0;
    ThU64 m_Size = 0;
};

struct ThStringTableHeader
{
    static constexpr ThChar RESOURCE_TYPE[9] = "strtbl";
    static constexpr ThU64 VERSION = 1;
    static constexpr ThU64 NUM_VARIABLE_SIZE_TYPE_ARRAYS = 1;
    static constexpr ThU64 STRING_DATA_ARRAY_INDEX = 0;

    template <class T>
    struct FixedSizeEntryArray
    {
        ThU64 m_Offset = 0;
        ThU64 m_Size = 0;
    };

    struct VariableSizeEntryBuffer
    {
        ThU64 m_Offset = 0;
        ThU64 m_Size = 0;
    };

    template <class T>
    struct VariableSizeEntryArray
    {
        ThU64 m_BufferId = 0;
        ThU64 m_ElementsOffset = 0;
        ThU64 m_ElementsSize = 0;
    };

    struct VariableSizeEntryArrayElement
    {
        ThU64 m_Offset = 0;
        ThU64 m_Size = 0;
    };

    ThU64 m_FileId = Make8CC(RESOURCE_TYPE);
    ThU64 m_FileVersion = VERSION;
    FixedSizeEntryArray<ThI32> m_Hashes;
    FixedSizeEntryArray<ThU64> m_Uids;
    VariableSizeEntryArray<ThChar> m_Strings;
    VariableSizeEntryBuffer m_Buffers[NUM_VARIABLE_SIZE_TYPE_ARRAYS];
};

struct ThInstanceId
{
    ThU32 m_Index;
    ThU32 m_Chunk;
};

class ThStringResourceManager
{
public:
    ThU64 CreateString(const ThChar* data);
    ThInstanceId GetInstanceId(ThU64 uid);
    const ThChar* GetString(ThInstanceId instanceId);
    ThI32 GetStringHash(ThInstanceId instanceId);
    ThI32 GetChunksCount();
    ThI32 GetStringsCount(ThU32 chunk);
private:

};

}