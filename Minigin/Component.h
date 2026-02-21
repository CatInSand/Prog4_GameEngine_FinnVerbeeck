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
		Component(GameObject* owner);
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update() = 0;

		void SetEnabled(bool enabled);
		void ToggleEnabled();

		void Delete();
		bool IsMarkedForDeletion() const;

		GameObject* GetOwner();
		const GameObject* GetOwner() const;

	protected:
		bool m_Enabled{ true };

	private:
		bool m_MarkedForDeletion{ false };
		GameObject* m_Owner;
	};
}

#endif