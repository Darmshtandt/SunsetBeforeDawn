#pragma once

#include <Core/FactoryBase.h>

template <template <class> class _Ptr, class _Ty, class _Factory>
class RegistrarBase {
public:
	using BasePtr = typename _Factory::BasePtr;
	using TypePtr = _Ptr<_Ty>;

public:
	RegistrarBase(std::string name) {
		ClassManager::Instance().BindName<_Ty>(std::move(name));

		_Factory::Instance().template Register<_Ty>([]() -> BasePtr {
			return FactoryTraits<TypePtr>::Create();
			});
	}

	RegistrarBase() = delete;
	RegistrarBase(const RegistrarBase&) = delete;
	RegistrarBase(RegistrarBase&&) = delete;
	~RegistrarBase() noexcept = default;

	RegistrarBase& operator = (const RegistrarBase&) = delete;
	RegistrarBase& operator = (RegistrarBase&&) = delete;
};