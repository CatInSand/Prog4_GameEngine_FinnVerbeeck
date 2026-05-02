#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "Component.h"

class PlayerComponent final : public dae::Component
{
public:
	PlayerComponent(dae::GameObject* owner);
	virtual ~PlayerComponent() = default;
	PlayerComponent(const PlayerComponent& other) = delete;
	PlayerComponent(PlayerComponent&& other) = delete;
	PlayerComponent& operator=(const PlayerComponent& other) = delete;
	PlayerComponent& operator=(PlayerComponent&& other) = delete;

	virtual void Update() override;

private:
	enum class PlayerState {
		none = -1,
		idle,
		walking,
		digging,
		count
	};

	bool OnGrid();

	PlayerState m_PlayerState;
};

#endif // !PLAYER_COMPONENT_H
