#pragma once

#include <World/Objects/GameObject.h>

class Interactive : public GameObject {
protected:
	explicit Interactive(const ClassID& id) noexcept;

public:

	Interactive() = delete;
	Interactive(const Interactive&) noexcept = default;
	Interactive(Interactive&&) noexcept = default;
	~Interactive() noexcept override = default;

	Interactive& operator = (const Interactive&) noexcept = default;
	Interactive& operator = (Interactive&&) noexcept = default;
};

using InteractivePtr = std::shared_ptr<Interactive>;
using InteractiveFactory = FactoryBase<std::shared_ptr, Interactive, InteractivePtr(*)()>;

template <class _Ty>
using InteractiveRegistrar = RegistrarBase<std::shared_ptr, Interactive, InteractiveFactory>;