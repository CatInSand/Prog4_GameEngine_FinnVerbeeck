#ifndef BASE_COMMAND_H
#define BASE_COMMAND_H

namespace dae
{
	class BaseCommand
	{
	public:
		virtual ~BaseCommand() = default;
		virtual void Execute() = 0;

	};
}

#endif