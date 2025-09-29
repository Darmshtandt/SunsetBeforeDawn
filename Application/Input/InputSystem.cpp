#include <Input/InputSystem.h>
#include <Objects/GameObject.h>
#include <Nt/Core/Input.h>

InputSystem::InputSystem() {
	using enum Nt::Key;

	m_Mapper.Bind(_W, Command::Forward);
	m_Mapper.Bind(_A, Command::Left);
	m_Mapper.Bind(_S, Command::Back);
	m_Mapper.Bind(_D, Command::Right);
	m_Mapper.Bind(_SPACE, Command::Up);
	m_Mapper.Bind(_SHIFT, Command::Down);
}

void InputSystem::Update(Scene& scene, RawInputQueue& queue) {
	while (!queue.empty()) {
		const RawInputEvent& event = queue.front();
		if (event.Pressed)
			m_ActiveKey.insert(event.Key);
		else
			m_ActiveKey.erase(event.Key);

		queue.pop();
	}

	if (m_ActiveKey.empty())
		return;

	const std::unordered_set<Command> commands = m_Mapper.ToActions(m_ActiveKey);
	for (const PlayerPtr& pPlayer : scene.GetAllPlayers()) {
		InputComponent* pInputComponent = pPlayer->GetComponent<InputComponent>();
		Assert(pInputComponent != nullptr, "Player not has InputComponent");
		pInputComponent->Commands = commands;
	}
}
