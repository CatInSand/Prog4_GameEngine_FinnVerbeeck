#ifndef OMNI_COMMAND_H
#define OMNI_COMMAND_H

#include <tuple>
#include <concepts>

#include "BaseCommand.h"

namespace dae
{
	template<typename Fn, typename... fArgs>
		requires std::invocable<Fn, fArgs...>
	class OmniCommand final : public BaseCommand
	{
	public:
		OmniCommand(Fn function, fArgs&&... args)
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
		Fn m_Function;
		std::tuple<fArgs...> m_Arguments;
	};
}

#endif