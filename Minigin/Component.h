#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

namespace dae
{
	class Component
	{
	public:
		Component() = default;

		virtual void Update() = 0;

		void SetEnabled(bool enabled);
		void ToggleEnabled();

		static const std::string m_ID;

	private:
		bool m_Enabled{ true };
	};
}

#endif