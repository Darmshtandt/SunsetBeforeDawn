#pragma once

#include <ClassManager.h>
#include <memory>

using ControllerID = ClassID;

class IController {
public:
	struct OnActivateController final {
		ControllerID ID;
	};
	struct OnDeativateController final {
		ControllerID ID;
	};
	struct OnToggleController final {
		ControllerID ID;
	};

public:
	IController() noexcept = default;
	IController(const IController&) = delete;
	IController(IController&&) noexcept = default;
	virtual ~IController() noexcept = default;

	IController& operator = (const IController&) = delete;
	IController& operator = (IController&&) noexcept = default;

	virtual void Update(const Float& deltaTime) = 0;

	void Enable() noexcept {
		m_Enabled = true;
	}
	void Disable() noexcept {
		m_Enabled = false;
	}
	void Toggle() noexcept {
		m_Enabled = !m_Enabled;
	}

	[[nodiscard]] Bool IsEnabled() const noexcept {
		return m_Enabled;
	}

private:
	Bool m_Enabled = true;
};

using ControllerPtr = std::unique_ptr<IController>;