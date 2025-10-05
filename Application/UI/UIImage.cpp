#include <UI/UIImage.h>
#include <Objects/Components/RenderComponents.h>
#include <StdH.h>

UIImage::UIImage() noexcept :
	GameObject(Class<UIImage>::ID(), ObjectType::UI)
{
	pTransform = AddComponent<Transform2D>();
	pLayout = AddComponent<Layout2D>();
	pScale = AddComponent<Scale2D>();
	pTexture = AddComponent<TextureComponent>();

	AddComponent<MeshComponent>()->Handler.Set(MESH_QUAD);
}

void UIImage::Update(const Float& deltaTime) {
	(void)deltaTime;
}