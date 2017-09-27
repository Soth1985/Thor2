#include <Thor/Engine/ThEngineInterface.h>
#include <Thor/Engine/ThStringHash.h>

namespace Thor
{
    class ThTaggingComponentManager;
    typedef ThInstance<ThTaggingComponentManager, eComponentKind::Utility, eComponentType::Tag> ThTagInstance;
    
    struct ThTagComponentData
    {
        ThStringHash64 m_Tag;
        ThU32 m_Layer;
        ThU32 m_Order;
        ThEntity m_Owner;
    };
    
    class ThTaggingComponentManager : public ThiComponentManager
    {
    public:
        
        ThTaggingComponentManager()
            :
        m_Data(&m_Allocator),
        m_EntityIndex(32, 32)
        {
            m_Allocator.Init(16 * ThMemory::Kilobyte());
        }
        
        ~ThTaggingComponentManager()
        {
            m_Data.Free();
        }
        
        virtual ThComponentType GetType()override
        {
            return ThComponentType(eComponentKind::Utility, eComponentType::Tag);
        }
        
        virtual ThI32 GetIndex(const ThEntity& entity)override
        {
            return Private::LookupIndex(entity, m_EntityIndex);
        }
        
        ThTagInstance Attach(const ThEntity& entity)
        {
            ThTagInstance instance = GetInstance(entity);
            
            if (instance)
                return instance;
            else
            {
                m_Data.PushBack(ThTagComponentData());
                m_Data.Back().m_Owner = entity;
                m_EntityIndex[entity] = m_Data.Size() - 1;
            }
        }
        
        void Detach(ThTagInstance instance)
        {
            ThEntity owner = GetOwner(instance);
            m_EntityIndex.Erase(owner);
            m_Data.MoveToBackAndRemove(instance);
        }
        
        ThTagInstance GetInstance(const ThEntity& entity)
        {
            ThI32 index = Private::LookupIndex(entity, m_EntityIndex);
            return ThTagInstance(index);
        }
        
        ThStringHash64 GetTag(ThTagInstance instance)
        {
            return m_Data[instance].m_Tag;
        }
        
        ThU32 GetLayer(ThTagInstance instance)
        {
            return m_Data[instance].m_Layer;
        }
        
        ThU32 GetOrder(ThTagInstance instance)
        {
            return m_Data[instance].m_Order;
        }
        
        ThEntity GetOwner(ThTagInstance instance)
        {
            return m_Data[instance].m_Owner;
        }
        
        void SetTag(ThTagInstance instance, ThStringHash64 tag)
        {
            m_Data[instance].m_Tag = tag;
        }
        
        void SetLayer(ThTagInstance instance, ThU32 layer)
        {
            m_Data[instance].m_Layer = layer;
        }
        
        void SetOrder(ThTagInstance instance, ThU32 order)
        {
            m_Data[instance].m_Order = order;
        }
    private:
        ThVirtualGrowingStackAllocator m_Allocator;
        ThVector<ThTagComponentData> m_Data;
        ThHashMap<ThEntity, ThI32> m_EntityIndex;
    };
}
