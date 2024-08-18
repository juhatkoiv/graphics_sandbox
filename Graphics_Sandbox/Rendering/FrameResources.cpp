#include "Precompiled/Precompiled.h"
#include "FrameResources.h"

#include "PassResources.h"

BEGIN_NAMESPACE1( rendering )

void FrameResources::setPassResources( PassIndex passIndex, const PassResources& resources )
{
	passData[passIndex] = resources;
}

const PassResources& FrameResources::passResources( PassIndex passIndex ) const
{
	assert( passData.contains( passIndex ) );
	return passData.at( passIndex );
}

END_NAMESPACE1