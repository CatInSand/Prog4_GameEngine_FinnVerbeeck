#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include <algorithm>
#include "DeltaTime.h"
#include "RenderComponent.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (std::unique_ptr<dae::Component>& pComponent : m_pComponents)
	{
		if (pComponent->m_Enabled)
		{
			pComponent->Update();
		}
	}
}

void dae::GameObject::Render() const
{
	for (const std::unique_ptr<dae::Component>& pComponent : m_pComponents)
	{
		if (pComponent->m_Enabled)
		{
			dae::RenderComponent* pRenderComponent{ dynamic_cast<dae::RenderComponent*>(pComponent.get()) };
			if (pRenderComponent != nullptr)
			{
				pRenderComponent->Render();
			}
		}
	}
}

void dae::GameObject::Delete()
{
	m_MarkedForDeletion = true;
}
bool dae::GameObject::IsMarkedForDeletion() const
{
	return m_MarkedForDeletion;
}
void dae::GameObject::DeleteQueue()
{
	m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
		[](std::unique_ptr<dae::Component>& component) { return component->IsMarkedForDeletion(); }),
		m_pComponents.end());
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_LocalTransform.SetPosition(x, y, 0.0f);
	m_TransformRequiresUpdate;
}
dae::Transform dae::GameObject::GetLocalTransform() const
{
	return m_LocalTransform;
}
dae::Transform dae::GameObject::GetWorldTransform() const
{
	if (m_TransformRequiresUpdate)
	{
		CalculateWorldTransform();
	}
	return m_WorldTransform;
}

void dae::GameObject::SetParent(dae::GameObject* pParent, bool keepWorldTransform = true)
{
	//nullptr is root

	if (pParent == m_pParent || pParent == this || IsChild(pParent))
	{
		return;
	}

	if (keepWorldTransform)
	{
		
	}
	m_TransformRequiresUpdate = true;

	m_pParent->RemoveChild(this);
	m_pParent = pParent;
	m_pParent->AddChild(this);
}
const dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}
size_t dae::GameObject::GetChildCount() const
{
	return m_pChildren.size();
}
const dae::GameObject* dae::GameObject::GetChildAtIndex(unsigned int index) const
{
	return m_pChildren[index];
}

void dae::GameObject::AddChild(dae::GameObject* pChild)
{
	m_pChildren.push_back(pChild);
}
void dae::GameObject::RemoveChild(dae::GameObject* pChild)
{
	m_pChildren.erase(std::find_if(m_pChildren.begin(), m_pChildren.end(),
		[pChild](dae::GameObject* pCurrentChild) { return pCurrentChild == pChild; }),
		m_pChildren.end());
}
bool dae::GameObject::IsChild(dae::GameObject* pChild)
{
	auto it = std::find_if(m_pChildren.begin(), m_pChildren.end(),
		[pChild](dae::GameObject* pCurrentChild)
		{
			//true if pChild is child of this, or pChild is child of pCurrentchild
			return pCurrentChild == pChild || pCurrentChild->IsChild(pChild);
		});


}