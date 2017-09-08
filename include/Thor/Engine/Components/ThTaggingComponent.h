//string hash tag
//int32 layer
//int32 order in layer
#include <Thor/Engine/ThInterface.h>

namespace Thor
{
    struct ThTagComponent
    {
        ThStringHash64 m_Tag;
        ThU32 m_Layer;
        ThU32 m_Order;
        ThEntity m_Owner;
    };
    
    class ThTaggingComponentManager : public ThiComponentManager
    {
    public:
    private:
        ThVirtualPoolAllocator* m_Allocator;
        ThVector<ThTagComponent> m_Data;
    };
}
