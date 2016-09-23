#include <Thor/Engine/ThBlackboard.h>

using namespace Thor;

ThBlackboard::ThBlackboard()
{
    
}

ThiBlackboardItem* ThBlackboard::GetItem(ThBlackboardType type)
{
    auto found = m_Items.Find(type);
    
    if (found != m_Items.End())
        return found->Value();
    
    return nullptr;
}

ThiBlackboardItem* ThBlackboard::GetItem(ThI32 index)
{
    return m_Items.GetItem(index).Value();
}

ThI32 ThBlackboard::GetNumItems()
{
    return m_Items.Size();
}

bool ThBlackboard::AddItem(ThiBlackboardItem* item)
{
    return m_Items.Insert(item->GetType(), item);
}

bool ThBlackboard::RemoveItem(ThiBlackboardItem* item)
{
    return m_Items.Erase(item->GetType());
}