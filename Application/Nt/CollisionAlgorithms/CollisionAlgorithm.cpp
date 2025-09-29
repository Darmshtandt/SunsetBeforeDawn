#include <Nt/CollisionAlgorithms/CollisionAlgorithm.h>
#include <Nt/Collider.h>

namespace Nt {
	CollisionAlgorithm::CollisionAlgorithm(NotNull<Collider*> pCollider) noexcept :
		m_pCollider(pCollider)
	{
	}
}