//entityid owner
//key mesh_instance

enum class eIndexType : ThU32
{
    Int16,
    Int32
};

struct ThResource
{
    ThU32 m_KindID;
    ThU32 m_TypeID;
    ThU64 m_Uid;
    ThU64 m_Size;
};

struct IndexBufferResource : public ThResource
{
    eIndexType m_Type;
};

struct IndexBufferData
{
    IndexBufferDesc m_Desc;
    ThU64 m_Offset;
};

struct MetalIndexBuffer
{
    IndexBufferDesc m_Desc;
    id<MTLBuffer> m_Buffer;
};

enum class eVertexFormat : ThU32
{
    Invalid = 0,
    
    UChar2 = 1,
    UChar3 = 2,
    UChar4 = 3,
    
    Char2 = 4,
    Char3 = 5,
    Char4 = 6,
    
    UChar2Normalized = 7,
    UChar3Normalized = 8,
    UChar4Normalized = 9,
    
    Char2Normalized = 10,
    Char3Normalized = 11,
    Char4Normalized = 12,
    
    UShort2 = 13,
    UShort3 = 14,
    UShort4 = 15,
    
    Short2 = 16,
    Short3 = 17,
    Short4 = 18,
    
    UShort2Normalized = 19,
    UShort3Normalized = 20,
    UShort4Normalized = 21,
    
    Short2Normalized = 22,
    Short3Normalized = 23,
    Short4Normalized = 24,
    
    Half2 = 25,
    Half3 = 26,
    Half4 = 27,
    
    Float = 28,
    Float2 = 29,
    Float3 = 30,
    Float4 = 31,
    
    Int = 32,
    Int2 = 33,
    Int3 = 34,
    Int4 = 35,
    
    UInt = 36,
    UInt2 = 37,
    UInt3 = 38,
    UInt4 = 39,
    
    Int1010102Normalized = 40,
    UInt1010102Normalized = 41,
    
    UChar4Normalized_BGRA = 42,
    
    UChar = 45,
    Char = 46,
    UCharNormalized = 47,
    CharNormalized = 48,
    
    UShort = 49,
    Short = 50,
    UShortNormalized = 51,
    ShortNormalized = 52,
    
    Half = 53,    
};

enum class eVertexStepFunction : ThU32
{
    Constant = 0,
    PerVertex = 1,
    PerInstance = 2,
    PerPatch = 3,
    PerPatchControlPoint = 4,
};

struct VertexBufferResource : public ThResource
{
    ThU32 m_Stride;
    eVertexStepFunction m_StepFunction;
    ThU32 m_StepRate;
};

struct ThVertexBufferData
{
    VertexBufferDesc m_Desc;
    ThU64 m_Offset;
};

struct ThVertexAttributeDesc
{
    eVertexFormat m_Format;
    ThU32 m_Offset;
    ThU32 m_BufferIndex;
};

struct ThSubmeshDesc
{
    ThU64 m_IndexBufferUid;
    ThU64 m_Offset;
};

struct ThMeshResource : public ThResource
{
    static const ThU32 MAX_VERTEX_ATTRIBUTES = 31;
    static const ThU32 MAX_SUBMESHES = 16;

    ThU32 m_NumVertexBuffers;
    VertexBufferDesc m_VertexBuffers[MAX_VERTEX_ATTRIBUTES];
    ThU32 m_NumAttributes;
    VertexAttributeDesc m_VertexAttributes[MAX_VERTEX_ATTRIBUTES];
    ThU32 m_NumSubmeshes;
    SubmeshDesc m_Submeshes[MAX_SUBMESHES];
};

struct ThMeshInstance
{
    ThU64 m_MeshResource;
    ThU64 m_Materials[ThMeshResource::MAX_SUBMESHES];
}

enum class eTextureUsage : ThU32
{
    Unknown,
    ShaderRead,
    ShaderWrite,
    RenderTarget,
    PixelFormatView
};

struct ThTextureResource : public ThResource
{
    eTextureUsage m_Usage;
};

struct ThResourceFileDesc : public ThResourceDesc
{
    ThString m_Path;
}

struct ThResourcePak
{
    ThString m_Archive;
    ThVector<ThResourceFileDesc> m_Resources;
};
