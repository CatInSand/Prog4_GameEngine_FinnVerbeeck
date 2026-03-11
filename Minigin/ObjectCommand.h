#ifndef OBJECT_COMMAND_H
#define OBJECT_COMMAND_H

#include "BaseCommand.h"
#include "GameObject.h"

namespace dae
{
	class ObjectCommand : public BaseCommand
	{
	public:
		ObjectCommand(GameObject* targetObject)
			: m_TargetObject{ targetObject }
		{

		}
		virtual ~ObjectCommand() = default;

	protected:
		GameObject* m_TargetObject;
	};
}

#endif