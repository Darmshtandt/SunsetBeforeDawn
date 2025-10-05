#include <Input/InputSystem.h>
#include <World/Scene.h>
#include <Nt/Core/Input.h>

void InputSystem::LoadKeyBinds() {
	m_Mapper.Bind(Nt::KEY_W, Command::Forward);
	m_Mapper.Bind(Nt::KEY_A, Command::Left);
	m_Mapper.Bind(Nt::KEY_S, Command::Back);
	m_Mapper.Bind(Nt::KEY_D, Command::Right);
	m_Mapper.Bind(Nt::KEY_SPACE, Command::Up);
	m_Mapper.Bind(Nt::KEY_SHIFT, Command::Down);
}

void InputSystem::Update(const Scene& scene, RawInputQueue queue) {
	while (!queue.empty()) {
		const RawInputEvent& event = queue.front();
		if (event.Pressed)
			m_ActiveKey.insert(event.Key);
		else
			m_ActiveKey.erase(event.Key);

		queue.pop();
	}

	const std::unordered_set<Command> commands = m_Mapper.ToActions(m_ActiveKey);
	for (const ObjectPtr& pObject : scene.GetDynamicObjects()) {
		if (pObject->GetType() != ObjectType::Player)
			continue;

		InputComponent* pInputComponent = pObject->GetComponent<InputComponent>();
		Assert(pInputComponent != nullptr, "Player not has InputComponent");
		pInputComponent->Commands = commands;
	}
}
