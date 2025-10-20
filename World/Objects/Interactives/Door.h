#pragma once

#include <World/Objects/Interactives/Interactive.h>

class Door : public Interactive {
public:
	Door() noexcept;
	Door(const Door&) noexcept = default;
	Door(Door&&) noexcept = default;
	~Door() noexcept override = default;

	Door& operator = (const Door&) noexcept = default;
	Door& operator = (Door&&) noexcept = default;

private:
	//inline static InteractiveRegistrar<Door> m_Registrar { "Door" };
};