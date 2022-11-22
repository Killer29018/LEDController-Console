#ifndef WINDOW_PANEL_HPP
#define WINDOW_PANEL_HPP

class WindowPanel
{
public:
    WindowPanel() = default;
    virtual ~WindowPanel() {}
    virtual void render() {}
};

#endif
