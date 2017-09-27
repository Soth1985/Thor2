#pragma once

#include <Thor/Core/Containers/ThHashMap.h>

namespace Thor
{ 
    namespace Private
    {
        template <class KeyT>
        ThI32 LookupIndex(const KeyT& key, const ThHashMap<KeyT, ThI32> indexTable)
        {
            auto found = indexTable.Find(entity);
            
            if (found != indexTable.End())
                return found.Value();
            else
                return -1;
        }
    }
}
