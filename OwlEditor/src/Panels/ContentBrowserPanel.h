#pragma once
#include <filesystem>

namespace Owl
{
    class ContentBrowserPanel
    {
    public:
        ContentBrowserPanel();

        void OnImGuiRender();
    private:
        std::filesystem::path m_CurrentDirectory;
    };
    
}
