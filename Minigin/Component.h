#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <memory>

namespace dae
{
	class GameObject;
	class Component
	{
	public:
<<<<<<< Updated upstream
		Component(std::unique_ptr<GameObject>& owner);
=======
		Component(GameObject* owner);
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;
>>>>>>> Stashed changes

		virtual void Update() = 0;

		void SetEnabled(bool enabled);
		void ToggleEnabled();

		static const std::string m_ID;

		GameObject* GetOwner();
		const GameObject* GetOwner() const;

	protected:
		bool m_Enabled{ true };
<<<<<<< Updated upstream
		std::unique_ptr<GameObject>& m_Owner;
=======

	private:
		bool m_MarkedForDeletion{ false };
		GameObject* m_Owner;
>>>>>>> Stashed changes
	};
}

#endif