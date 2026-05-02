#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include <algorithm>
#include "DeltaTime.h"
#include "TextureComponent.h"

dae::GameObject::GameObject(dae::GameObject* pParent, const std::string& name)
	: m_Name{ name }
	, m_pParent{ pParent }
{
	if (m_pParent != nullptr)
	{
		m_pParent->AddChild(this);
	}
}

dae::GameObject::~GameObject()
{
	if (m_pParent != nullptr)
	{
		m_pParent->RemoveChild(this);
	}
	for (dae::GameObject* pChild : m_pChildren)
	{
		pChild->m_pParent = nullptr;
	}
	//Only gets deleted after Delete() is called or goes out of scope
	//	Either way, children will also get deleted
}

std::string dae::GameObject::Name() const
{
	return m_Name;
}
void dae::GameObject::Rename(const std::string& name)
{
	m_Name = name;
}

void dae::GameObject::SetParent(dae::GameObject* pParent, bool keepWorldTransform)
{
	//nullptr is parent of root

	if (pParent == m_pParent || pParent == this || IsChild(pParent) || pParent == nullptr || m_pParent == nullptr)
	{
		return;
	}

	glm::vec3 pos{ GetWorldTransform().GetPosition() };

	if (pParent == GetRoot())
	{
		SetLocalPosition(pos.x, pos.y);
	}
	else
	{
		if (keepWorldTransform)
		{
			glm::vec3 parentPos{ pParent->GetWorldTransform().GetPosition() };
			SetLocalPosition(pos.x - parentPos.x, pos.y - parentPos.y);
		}
		SetTransformDirty();
	}

	m_pParent->RemoveChild(this);
	m_pParent = pParent;
	m_pParent->AddChild(this);
}
const dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}
dae::GameObject* dae::GameObject::GetRoot()
{
	if (m_pParent == nullptr)
	{
		return this;
	}
	else
	{
		return m_pParent->GetRoot();
	}
}
const dae::GameObject* dae::GameObject::GetRoot() const
{
	if (m_pParent == nullptr)
	{
		return this;
	}
	else
	{
		return m_pParent->GetRoot();
	}
}
size_t dae::GameObject::GetChildCount() const
{
	return m_pChildren.size();
}
const dae::GameObject* dae::GameObject::GetChildAtIndex(unsigned int index) const
{
	return m_pChildren[index];
}

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
void dae::GameObject::Render()
{
	for (const std::unique_ptr<dae::Component>& pComponent : m_pComponents)
	{
		if (pComponent->m_Enabled)
		{
			dae::TextureComponent* pRenderComponent{ dynamic_cast<dae::TextureComponent*>(pComponent.get()) };
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
	for (dae::GameObject* pChild : m_pChildren)
	{
		pChild->Delete();
	}
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
	SetTransformDirty();
}
dae::Transform dae::GameObject::GetLocalTransform() const
{
	return m_LocalTransform;
}
dae::Transform dae::GameObject::GetWorldTransform()
{
	if (m_TransformRequiresUpdate)
	{
		CalculateWorldTransform();
	}
	return m_WorldTransform;
}

//private functions
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

	if (it != m_pChildren.end())
	{
		return true;
	}

	return false;
}

void dae::GameObject::SetTransformDirty()
{
	m_TransformRequiresUpdate = true;
	for (GameObject* child : m_pChildren)
	{
		child->SetTransformDirty();
	}
}
void dae::GameObject::CalculateWorldTransform()
{
	if (m_pParent == nullptr)
	{
		m_WorldTransform = m_LocalTransform;
	}
	else
	{
		glm::vec3 newWorldPos{ m_LocalTransform.GetPosition() + m_pParent->GetWorldTransform().GetPosition() };
		m_WorldTransform.SetPosition(newWorldPos.x, newWorldPos.y);
	}
	m_TransformRequiresUpdate = false;
}