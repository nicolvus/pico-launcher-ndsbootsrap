#pragma once

class GraphicsContext;
class VramContext;
class IVramManager;
class ITheme;

class IThemeBackground
{
public:
    virtual ~IThemeBackground() = 0;

    /// @brief Updates the background.
    virtual void Update() { }

    /// @brief Draws the background.
    /// @param graphicsContext The graphics context to use.
    virtual void Draw(GraphicsContext& graphicsContext) { }

    /// @brief Performs vblank processes for the background.
    virtual void VBlank() { }

    /// @brief Loads the resources for this background.
    /// @param theme The theme the background belongs to.
    /// @param vramContext The vram context.
    virtual void LoadResources(const ITheme& theme, const VramContext& vramContext) { }
};

inline IThemeBackground::~IThemeBackground() { }
