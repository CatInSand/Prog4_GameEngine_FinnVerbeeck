#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include <algorithm>
#include "DeltaTime.h"
#include "RenderComponent.h"
#include "SceneManager.h"

dae::GameObject::GameObject(dae::GameObject* pParent, const std::string& name)
	: m_Name{ cat::make_hash(name) }
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

cat::hash_t dae::GameObject::Name() const
{
	return m_Name;
}
void dae::GameObject::Rename(const cat::hash_t& name)
{
	m_Name = name;
}
bool dae::GameObject::HasTag(const cat::hash_t& tag) const
{
	const auto it{ std::find(m_Tags.cbegin(), m_Tags.cend(), tag) };
	if (it != m_Tags.cend())
	{
		return true;
	}
	return false;
}
void dae::GameObject::AddTag(const cat::hash_t& tag)
{
	m_Tags.push_back(tag);
}
void dae::GameObject::RemoveTag(const cat::hash_t& tag)
{
	m_Tags.erase(std::remove(m_Tags.begin(), m_Tags.end(), tag), m_Tags.end());
}
std::vector<cat::hash_t> dae::GameObject::Tags() const
{
	return m_Tags;
}

void dae::GameObject::SetParent(dae::GameObject* pParent, bool keepWorldTransform)
{
	//nullptr is parent of root

	if (pParent == m_pParent || pParent == this || IsChild(pParent) || pParent == nullptr || m_pParent == nullptr)
	{
		return;
	}

	glm::vec2 pos{ GetWorldTransform().position };

	if (pParent == GetRoot())
	{
		SetLocalPosition(pos);
	}
	else
	{
		if (keepWorldTransform)
		{
			glm::vec2 parentPos{ pParent->GetWorldTransform().position };
			SetLocalPosition(pos - parentPos);
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
dae::GameObject* dae::GameObject::GetChildAtIndex(size_t index)
{
	return m_pChildren[index];
}

void dae::GameObject::Start()
{
	for (std::unique_ptr<dae::Component>& pComponent : m_pComponents)
	{
		if (pComponent->m_Enabled)
		{
			pComponent->Start();
		}
	}
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
	for (dae::GameObject* pChild : m_pChildren)
	{
		pChild->Delete();
	}
	SetParent(SceneManager::Instance().CurrentScene()->Root());
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
	SetLocalPosition({ x, y });
}
void dae::GameObject::SetLocalPosition(glm::vec2 pos)
{
	m_LocalTransform.position = pos;
	SetTransformDirty();
}
void dae::GameObject::SetLocalScale(float scale)
{
	m_LocalTransform.scale = { scale, scale };
	SetTransformDirty();
}
void dae::GameObject::SetLocalScale(glm::vec2 scale)
{
	m_LocalTransform.scale = scale;
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

dae::Transform* dae::GameObject::LocalTransformPtr()
{
	return &m_LocalTransform;
}

//trivial because of consistent API design
void dae::GameObject::RenderPrioritize(int priority)
{
	m_RenderPriority = priority;
}
int dae::GameObject::RenderPriority() const
{
	return m_RenderPriority;
}

//private functions
void dae::GameObject::AddChild(dae::GameObject* pChild)
{
	m_pChildren.push_back(pChild);
}
void dae::GameObject::RemoveChild(dae::GameObject* pChild)
{
	m_pChildren.erase(std::find_if(m_pChildren.begin(), m_pChildren.end(),
		[pChild](dae::GameObject* pCurrentChild) { return pCurrentChild == pChild; }));
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
		m_WorldTransform = m_LocalTransform * m_pParent->GetWorldTransform();
	}
	m_TransformRequiresUpdate = false;
}