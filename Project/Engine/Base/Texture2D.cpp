#include "Texture2D.h"

#include <stdexcept>

#include <SDL3/SDL.h>

#include "Renderer.h"

dae::Texture2D::Texture2D(const std::string& fullPath)
    : m_Texture{ nullptr }
{
    SDL_Surface* surface{ SDL_LoadPNG(fullPath.c_str()) };
    if (surface == nullptr)
        throw std::runtime_error(std::string("Failed to load PNG: ") + SDL_GetError());

    m_Texture = SDL_CreateTextureFromSurface(
        Renderer::Instance().GetSDLRenderer(),
        surface
    );

    SDL_DestroySurface(surface);

    if (m_Texture == nullptr)
        throw std::runtime_error(std::string("Failed to create texture from surface: ") + SDL_GetError());
}

dae::Texture2D::Texture2D(SDL_Texture* texture)
    : m_Texture{ texture }
{
    assert(m_Texture != nullptr);
}

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

glm::vec2 dae::Texture2D::GetSize() const
{
    float w{}, h{};
    SDL_GetTextureSize(m_Texture, &w, &h);
    return { w, h };
}
