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
		Component(std::unique_ptr<GameObject>& owner);
		virtual ~Component() = default;

		virtual void Update() = 0;
		virtual void Render() const = 0;

		void SetEnabled(bool enabled);
		void ToggleEnabled();

		void Delete();
		bool IsMarkedForDeletion() const;

	protected:
		bool m_Enabled{ true };
		std::unique_ptr<GameObject>& m_Owner;

	private:
		bool m_MarkedForDeletion{ false };
	};
}

#endif