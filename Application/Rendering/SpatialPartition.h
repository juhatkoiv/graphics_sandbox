#pragma once

#include <glm/glm.hpp>
#include <span>

BEGIN_NAMESPACE1( rendering )

class SpatialPartition
{
public:
	SpatialPartition() = default;
	virtual ~SpatialPartition() = default;

	virtual void update( const std::span<unsigned>& ids, const std::span<glm::vec3>& positions, const std::span<glm::vec3>& scales, const std::span<glm::quat>& rotations ) = 0;
	virtual std::span<unsigned> intersecting( const std::span<unsigned>& ids ) const = 0;
};

END_NAMESPACE1