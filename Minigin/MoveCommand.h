#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include <glm/fwd.hpp>
#include "ObjectCommand.h"

namespace dae
{
	class MoveCommand final : public ObjectCommand
	{
	public:
		MoveCommand(GameObject* targetObject, const glm::vec2& direction);
		virtual void Execute() override;
		
	private:
		glm::vec2 m_Direction;
		const float m_Speed{ 10.f };
	};
}

#endif