#include <UI/UIImage.h>
#include <World/Components/RenderComponents.h>
#include <Core/StdH.h>

UIImage::UIImage() noexcept :
	GameObject(Class<UIImage>::ID(), ObjectType::UI)
{
	pTransform = AddComponent<Transform2D>();
	pScale = AddComponent<Scale2D>();
	pTexture = AddComponent<TextureRenderer>();

	AddComponent<MeshRenderer>()->Set(MESH_QUAD);
}