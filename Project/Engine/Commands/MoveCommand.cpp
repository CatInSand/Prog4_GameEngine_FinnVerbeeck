#include "MoveCommand.h"
#include "DeltaTime.h"

dae::MoveCommand::MoveCommand(dae::GameObject* targetObject, const glm::vec2& direction, float speed)
	: ObjectCommand(targetObject)
	, m_Direction{ direction }
	, m_Speed{ speed }
{
}
void dae::MoveCommand::Execute()
{
	glm::vec2 newPos{
		m_TargetObject->GetLocalTransform().position
		+ glm::normalize(m_Direction) * time::gDeltaTime * m_Speed
	};
	m_TargetObject->SetLocalPosition(newPos.x, newPos.y);
}