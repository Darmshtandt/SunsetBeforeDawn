#include <UI/UIText.h>
#include <Core/StdH.h>

UIText::UIText() :
	GameObject(Class<UIText>::ID(), ObjectType::UI)
{
	pTransform = AddComponent<Transform2D>();
	pLayout = AddComponent<Layout2D>();
	pScale = AddComponent<Scale2D>();
	pText = AddComponent<Text>();

	AddComponent<MeshRenderer>()->Set(MESH_QUAD);

	pText->Font.LoadFromFile(PathManager::Fonts() + "NotoSansDisplay-Black.ttf");
}