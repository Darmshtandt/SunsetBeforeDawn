#include <Engine/RenderSystem.h>
#include <World/Components/RenderComponents.h>
#include <World/Landscape.h>
#include <UI/Canvas.h>
#include <Core/Helpers/Profiler.h>
#include <Core/StdH.h>

RenderSystem::RenderSystem(NotNull<Nt::Renderer*> pRenderer) :
	m_pRenderer(pRenderer)
{
	m_MSAA.Initialize(pRenderer);
	m_pRenderer->MatrixWorldPush();

	Profiler::Instance().Start();
	m_TerrainShader.Initialize();
	m_TerrainShader.Create();
	m_TerrainShader.CompileFromFile(Nt::Shader::VERTEX, PathManager::Shaders() + "TerrainVert.glsl");
	m_TerrainShader.CompileFromFile(Nt::Shader::FRAGMENT, PathManager::Shaders() + "TerrainFrag.glsl");
	m_TerrainShader.Link();
	m_TerrainShader.DisableStrict();

	m_pUIShader.Initialize();
	m_pUIShader.Create();
	m_pUIShader.CompileFromFile(Nt::Shader::VERTEX, PathManager::Shaders() + "UIVert.glsl");
	m_pUIShader.CompileFromFile(Nt::Shader::FRAGMENT, PathManager::Shaders() + "UIFrag.glsl");
	m_pUIShader.Link();
	m_pUIShader.DisableStrict();
	Profiler::Instance().Stop("Shader creation time");
}

void RenderSystem::RegisterObject(const GameObject& object) {
	if (object.GetType() == ObjectType::Canvas) {
		const Canvas* pCanvas = static_cast<const Canvas*>(&object);
		for (const ObjectPtr& pObject : pCanvas->GetObjects())
			RegisterObject(*pObject);

		return;
	}
	if (object.GetType() == ObjectType::Player) {
		m_pCamera = object.GetComponent<Camera2D>();
		if (m_pCamera == nullptr)
			m_pCamera = object.GetComponent<Camera3D>();
	}

	Renderable renderable = { };
	renderable.pTransform2D = object.GetComponent<Transform2D>();
	renderable.pTransform3D = object.GetComponent<Transform3D>();
	renderable.pMesh = object.GetComponent<MeshRenderer>();
	renderable.pTexture = object.GetComponent<TextureRenderer>();
	renderable.pText = object.GetComponent<Text>();
	renderable.pObject = &object;

	const Bool is2D = renderable.pTransform2D != nullptr;
	const Bool is3D = renderable.pTransform3D != nullptr;
	if (is2D && is3D)
		Raise("The object is both 2D and 3D");

	if ((!is2D && !is3D) || renderable.pMesh == nullptr)
		return;

	if (Class<Landscape>::Is(object)) {
		m_TerrainShader.SetUniform("TerrainSize",
			static_cast<const Landscape*>(&object)->GetSize());
	}

	if (object.GetType() == ObjectType::UI)
		m_UIObject.emplace_back(renderable);
	else
		m_SceneObject.emplace_back(renderable);
}

void RenderSystem::UnregisterObject(const GameObject& object) {
	if (object.GetType() == ObjectType::UI) {
		std::erase_if(m_UIObject, [&object] (const Renderable& renderable) {
			return &object == renderable.pObject;
			});
	}
	else {
		std::erase_if(m_SceneObject, [&object] (const Renderable& renderable) {
			return &object == renderable.pObject;
			});
	}
}

void RenderSystem::ClearObjects() noexcept {
	m_SceneObject.clear();
	m_UIObject.clear();
}

void RenderSystem::Resize(const Nt::Float2D& size) {
	const Float aspect = size.x / size.y;

	const Nt::FloatRect orthoRect = {
		0.f, size.y, size.x, 0.f
	};

	constexpr Float fov = 90.f;
	constexpr Float near = 0.01f;
	constexpr Float far = 10000.f;
	
	m_Projections[PERSPECTIVE] = Nt::ComputePerspective(fov, aspect, near, far);
	m_Projections[ORTHO2D] = Nt::ComputeOrtho2D(orthoRect);
	m_Projections[ORTHO] = Nt::ComputeOrtho(orthoRect, near, far);

	if (m_ActiveProjection != NONE)
		m_pRenderer->SetProjection(m_Projections[m_ActiveProjection]);

	m_MSAA.Resize(m_pRenderer->GetViewportRect());
}

void RenderSystem::ClearScreen() {
	m_MSAA.BeginForRender();
	m_pRenderer->Clear();
}

void RenderSystem::UpdateCamera() {
	if (m_pCamera == nullptr)
		return;

	if (m_pCamera->IsDirty()) {
		Nt::Float3D cameraPosition;
		Nt::Float3D cameraRotation;

		if (m_pCamera->Is2D()) {
			const Transform2D* pTransform2D =
				static_cast<const Camera2D*>(m_pCamera)->pTransform;

			cameraPosition.xy = pTransform2D->Position();
			cameraRotation.xy = pTransform2D->Rotation();
		}
		else if (m_pCamera->Is3D()) {
			const Transform3D* pTransform3D =
				static_cast<const Camera3D*>(m_pCamera)->pTransform;

			cameraPosition = pTransform3D->Position();
			cameraRotation = pTransform3D->Rotation();
		}

		m_TerrainShader.SetUniform("cameraPosition", cameraPosition);

		Nt::Double3D lightDirection(
			cosf(cameraRotation.y), 1.0, sinf(cameraRotation.y));
		m_TerrainShader.SetUniform("lightDirection", lightDirection);

		lightDirection = Nt::Double3D(1.0, -1.0, 1.0).GetNormalize();
		m_TerrainShader.SetUniform("lightDirection", lightDirection);
	}

	m_pRenderer->SetView(m_pCamera->View());
}

void RenderSystem::RenderScene() {
	SetActiveProjection(PERSPECTIVE);

	m_pRenderer->SetShader(&m_TerrainShader);
	for (const Renderable& renderable : m_SceneObject) {
		if (Class<Landscape>::Is(*renderable.pObject))
			m_TerrainShader.SetUniform("IsTerrain", true);

		RenderObject(renderable);

		if (Class<Landscape>::Is(*renderable.pObject))
			m_TerrainShader.SetUniform("IsTerrain", false);
	}
}

void RenderSystem::RenderUI() {
	SetActiveProjection(ORTHO2D);

	m_pRenderer->SetShader(&m_pUIShader);
	for (const Renderable& renderable : m_UIObject) {
		RenderObject(renderable);

		if (renderable.pText != nullptr)
			RenderText(renderable);
	}

	m_MSAA.ResolveToBuffer(nullptr);
}

void RenderSystem::RenderObject(const Renderable& renderable) {
	if (!renderable.pMesh->IsVisible)
		return;

	if (renderable.pTexture && renderable.pTexture->IsValid())
		m_pRenderer->BindTexture(renderable.pTexture->Get());

	if (renderable.pTransform2D != nullptr)
		m_pRenderer->SetWorld(renderable.pTransform2D->LocalToWorld());
	else
		m_pRenderer->SetWorld(renderable.pTransform3D->LocalToWorld());

	m_pRenderer->Render(renderable.pMesh->Get());
	m_pRenderer->MatrixWorldPop();
}

void RenderSystem::RenderText(const Renderable& renderable) {
	const Bool notHasFont = 
		(renderable.pText == nullptr || !renderable.pText->Font.IsLoaded());
	const Bool isEmptyText = renderable.pText->TextString.empty();
	const Bool isInvisibleMesh = !renderable.pMesh->IsVisible;

	if (notHasFont || isEmptyText || isInvisibleMesh)
		return;

	if (renderable.pTransform2D != nullptr)
		m_pRenderer->SetWorld(renderable.pTransform2D->LocalToWorld());
	else
		m_pRenderer->SetWorld(renderable.pTransform3D->LocalToWorld());

	constexpr Nt::Float3D spaceSize(32.f, 0.f, 0.f);
	Nt::Float3D prevGlyphSize(1.f, 1.f, 0.f);

	for (const Char& symbol : renderable.pText->TextString) {
		if (symbol == ' ') {
			m_pRenderer->Translate(spaceSize / prevGlyphSize);
			continue;
		}

		const Nt::FTFont::Character& character =
			renderable.pText->Font.GetCharacter(symbol);

		const Nt::Float3D glyphSize(character.Size, 0.f);
		const Bool hasTexture = character.pGlyph != nullptr;

		if (hasTexture)
			m_pRenderer->BindTexture(character.pGlyph.get());

		const Nt::Float3D bearing(
			0.5f + Float(character.Bearing.x) / glyphSize.x * 2.f, 
			0.5f, 0.f);
		const Nt::Float3D advance(
			Float(character.Advance) / glyphSize.x,
			0.f, 0.f);

		m_pRenderer->Scale(glyphSize / prevGlyphSize);
		m_pRenderer->Translate(bearing);
		m_pRenderer->Render(renderable.pMesh->Get());
		m_pRenderer->Translate(advance - bearing);

		if (hasTexture)
			m_pRenderer->UnbindTexture();

		prevGlyphSize = glyphSize;
	}

	m_pRenderer->MatrixWorldPop();
}

void RenderSystem::SetActiveProjection(const Projection& projection) {
	if (m_ActiveProjection == projection)
		return;

	m_ActiveProjection = projection;
	if (projection != NONE)
		m_pRenderer->SetProjection(m_Projections[projection]);
}