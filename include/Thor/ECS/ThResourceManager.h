#pragma once

#include <Thor/Core/Util/ThUidGenerator.h>

namespace Thor
{

class ThUidGenerator;

class ThResourceManager
{
public:
    ThResourceManager()
    {
        m_UidGenerator = new ThUidGenerator();
        m_UidGenerator->SetSnowflakeConstant(255);
    }
    
    ThU64 GenerateUid()
    {
        return m_UidGenerator->GenerateSnowflake();
    }
private:
    ThUidGenerator* m_UidGenerator {nullptr};
};

}