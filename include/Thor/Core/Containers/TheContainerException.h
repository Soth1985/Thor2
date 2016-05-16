#pragma once

#include "Thor/Framework/ThException.h"

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					TheContainerException
//
//----------------------------------------------------------------------------------------
class TheContainerException:public ThException
{
public:
	TheContainerException(const ThString& name);
};
}
