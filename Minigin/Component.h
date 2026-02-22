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
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update() = 0;

		void Delete();
		bool IsMarkedForDeletion() const;

		bool m_Enabled{ true };

	protected:
		explicit Component(GameObject* owner);
		GameObject* GetOwner();
		const GameObject* GetOwner() const;

	private:
		bool m_MarkedForDeletion{ false };
		GameObject* m_Owner;
	};
}

#endif