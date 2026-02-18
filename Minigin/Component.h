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

		virtual std::string GetID() { return m_ID; };

		void SetEnabled(bool enabled);
		void ToggleEnabled();

	protected:
		bool m_Enabled{ true };
		std::unique_ptr<GameObject>& m_Owner;

	private:
		const std::string m_ID{ "Component" };
	};
}

#endif