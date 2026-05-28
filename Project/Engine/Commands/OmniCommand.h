#ifndef OMNI_COMMAND_H
#define OMNI_COMMAND_H

#include <tuple>
#include <concepts>

#include "BaseCommand.h"

namespace dae
{
	template<typename F, typename... fArgs>
		requires std::invocable<F, fArgs...>
	class OmniCommand final : public BaseCommand
	{
	public:
		OmniCommand(F function, fArgs&&... args)
			: m_Function{ function }
			, m_Arguments{ std::make_tuple(std::forward<fArgs>(args)...) }
		{
		}
		virtual ~OmniCommand() = default;
		virtual void Execute() override
		{
			std::apply(std::move(m_Function), std::move(m_Arguments));
		}

	private:
		F m_Function;
		std::tuple<fArgs...> m_Arguments;
	};
}

#endif