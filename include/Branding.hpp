#pragma once

#ifdef GEODE_IS_WINDOWS
#ifdef CW_MODDEVBRANDING_API_EXPORTING
#define MODDEVBRANDING_API_DLL __declspec(dllexport)
#else
#define MODDEVBRANDING_API_DLL __declspec(dllimport)
#endif
#else
#define MODDEVBRANDING_API_DLL __attribute__((visibility("default")))
#endif

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace branding {
    enum class MODDEVBRANDING_API_DLL BrandImageType {
        URL = 0,
        Sprite = 1,
        SpriteFrame = 2
    };

    struct Branding {
        std::string image;
        std::string developer;
        BrandImageType type = BrandImageType::Sprite;

        Branding(
            std::string i,
            std::string d,
            BrandImageType t = BrandImageType::Sprite
        ) : image(i), developer(d), type(t) {};
    };

    class MODDEVBRANDING_API_DLL BrandingManager : public CCObject {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        BrandingManager();
        virtual ~BrandingManager();

        std::vector<Branding> getBrands() const;

    public:
        static BrandingManager* get();

        void registerBrand(const std::string& developer, const std::string& image, BrandImageType type = BrandImageType::Sprite);

        Branding getBrand(std::string_view developer) const;

        bool doesBrandExist(std::string_view developer) const;
    };
};