#include "TextureManager.h"
#include "TextureManager.h"
#include "mGame/Game.h"


TextureManager::TextureManager(Game* game) {
	Logger::info(L"Инициализация TextureManager успешна.");
}

sf::Texture* TextureManager::getTexture(std::wstring name)
{
    if (m_textures.find(name) != m_textures.end()) {
        return m_textures[name];
    }
    else {
        return NULL;
    }
}

sf::Texture* TextureManager::getTexture(int index)
{
    return getTexture(m_order.at(index));
}

void TextureManager::loadTexturesFromRootResources(fs::path dir)
{
    fs::path dirtoremove = fs::current_path() / L"Resources\\Textures";

    struct _stat sb;
    
    fs::path path = fs::path(dir);
    for (const auto& entry : fs::directory_iterator(path.string().c_str())) {
        fs::path outfilename = entry.path();
        std::wstring outfilename_str = outfilename.wstring();
        const wchar_t* path = outfilename_str.c_str();

        if (_wstat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)) {
            outfilename_str.erase(0, dirtoremove.wstring().size() + 1);

            outfilename_str.erase(outfilename_str.end() - 4, outfilename_str.end());

            outfilename_str = Utils::string::replace(outfilename_str, '\\', '_');

            sf::Texture* texture = new sf::Texture();

            if (texture->loadFromFile(entry.path().string()))
            {
                m_textures[outfilename_str] = texture;
                m_order.push_back(outfilename_str);
            }
            else
            {
                Logger::info(std::format(L"[-] Загрузка тестуры {}", outfilename_str).c_str());
                delete texture;
                continue;
            }
        }
        else {
            loadTexturesFromRootResources(path);
        }
    }
    Logger::info(std::format(L"Загружено {} текстур", m_order.size()).c_str());
}

