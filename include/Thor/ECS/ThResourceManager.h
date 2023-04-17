#pragma once

#include <Thor/ECS/ThEntity.h>
#include <Thor/Core/Containers/ThVector.h>
#include <Thor/Core/Containers/ThHashMap.h>
#include <Thor/Core/Util/ThUidGenerator.h>

namespace Thor
{

class ThUidGenerator;

constexpr ThU8 ThMaxResourcePath {128};

struct TheCompressionMethod
{
    using Value = ThU32;

    enum 
    {
        Zlib = 0,
        LZFSE = 1,
        LZ4 = 2,
        LZMA = 3,
        LZBitmap = 4,
        None = 65535
    };
};

struct TheResourceState
{
    using Value = ThU32;
    
    enum
    {
        Created,
        DescriptorLoading,
        DescriptorLoaded,
        ResourceLoading,
        ResourceLoaded
    };
};

struct TheResourceId
{
    using Value = ThResourceId;

    enum
    {
        Texture,
        StaticMesh,
        Scene,
        Reserved = 128
    };
};

struct ThArchivedResourceInfo
{
    ThChar m_Path[ThMaxResourcePath] {'\0'};
    ThU64 m_Uid {0};
    ThU64 m_Offset {0};
    ThU64 m_Size {0};
    ThU64 m_CompressedSize {0};
    TheCompressionMethod::Value m_CompressionMethod {TheCompressionMethod::None};
};

struct ThDescriptorsInfo
{
    ThResourceId m_ResourceType {0};
    ThU64 m_DescriptorVersion {0};
    ThU64 m_DescriptorSize {0};
    ThU64 m_NumDescriptors {0};
    ThU64 m_Offset {0};
    ThU64 m_Size {0};
    ThU64 m_CompressedSize {0};
    TheCompressionMethod::Value m_CompressionMethod {TheCompressionMethod::None};
};

struct ThResourceDescriptorBase
{
public:

    /*const ThChar* GetPath()const
    {
        return m_Path;
    }

    void SetPath(const ThChar* path)
    {
        ThMemory::StringCopy(m_Path, sizeof(m_Path), path);
    }*/

private:
    ThArchivedResourceInfo m_FileInfo;
};

class ThiResourceRuntime
{
public:
    virtual ~ThiResourceRuntime()
    {

    }
private:
    TheResourceState::Value m_State {TheResourceState::Created};
    ThI32 m_ReferenceCount {0};
    ThF64 m_LastAccessTime {0};
};

struct ThResourceIndex
{
    ThResourceId m_ResourceId {ThResourceNull};
    ThSize m_IndexInPool {0};
};

class ThiResourcePool
{
public:
    virtual ~ThiResourcePool()
    {

    }
private:
};

template <class TResource>
class ThResourcePool: public ThiResourcePool
{
public:
    using TDescriptor = typename TResource::Descriptor;
    using TRuntime = typename TResource::Runtime;
private:
    ThVector<TDescriptor> m_Descriptors;
    ThVector<TRuntime> m_Runtimes;
};

struct ThArchiveHeader
{
    static constexpr ThU64 MagicNumber = Make8CC("ARCHIVE");
    static constexpr ThU32 Version = 1;

    ThU64 m_MagicNumber {MagicNumber};
    ThU32 m_Version {Version};
    ThU64 m_NumResourceTypes {0};
    ThU64 m_DescriptorsOffset {0};
};

/*
Archive:
ThArchiveHeader
(resource file data)*
(ThDescriptorsInfo + (ThArchivedResourceInfo + resource descriptor)*)*
*/
template <class TDescriptor, class TRuntime>
struct ThResourceReference
{
public:
    operator bool()const
    {
        return m_Descriptor || m_Runtime;
    }
private:
    TDescriptor* m_Descriptor {nullptr};
    TRuntime* m_Runtime {nullptr};
};

class ThResourceTexture
{
public:
    static constexpr ThResourceId ResourceId {0};
    struct Descriptor: public ThResourceDescriptorBase
    {
        static constexpr ThU32 Version {1};
    };

    class Runtime: public ThiResourceRuntime
    {

    };
};

class ThResourceScene
{
public:
    static constexpr ThResourceId ResourceId {0};
    static constexpr ThU32 MaxComponents {1024};
    struct ComponentDataDescriptor
    {
        ThComponentId m_ComponentId {0}; 
        ThSize m_ComponentDataSize {0};
        ThU16 m_PageSize {0};
        ThI8* m_Buffer {nullptr};
        ThU16 m_Size {0};
        ThU16 m_Capacity {0};
        ThSize m_ComponentDataSize {0};
    };
    struct Descriptor: public ThResourceDescriptorBase
    {
        static constexpr ThU32 Version {1};

        ComponentDataDescriptor m_Components[MaxComponents];
    };

    class Runtime: public ThiResourceRuntime
    {

    };

    static bool LoadFromArchive()
    {

    }

    static bool SaveToArchive()
    {

    }
};

class ThResourceManager
{
public:
    ThResourceManager()
    {
        m_UidGenerator = new ThUidGenerator();
    }
    
    template <class TResource>
    ThU64 GenerateResourceUid()
    {
        return m_UidGenerator->GenerateSnowflake(TResource::ResourceId);
    }
private:
    ThUidGenerator* m_UidGenerator {nullptr};
    ThHashMap<ThResourceId, ThiResourcePool*> m_ResourcePools;
    ThHashMap<ThU64, ThResourceIndex> m_ResourceIndex;
};

}
