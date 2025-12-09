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
        class Impl; // PImpl class
        std::unique_ptr<Impl> m_impl; // PImpl pointer

        BrandingManager(); // Constructor
        virtual ~BrandingManager(); // Destructor

        /**
         * Returns the array of all registered developer branding
         *
         * @returns An array of all registered developer branding
         */
        std::vector<Branding> getBrands() const;

    public:
        // Get branding manager singleton
        static BrandingManager* get();

        /**
         * Register your very own branding image to appear on your mods! Internally checks for duplicate developer entries,
         *
         * @param developer GitHub username of the main developer or organization behind the mod
         * @param image Sprite name, sheet frame name, or URL of the image to use as branding
         * @param type Whether you're using a sprite, spritesheet frame, or external URL as the source of your branding
         */
        void registerBrand(const std::string& developer, const std::string& image, BrandImageType type = BrandImageType::Sprite);

        /**
         * Get the branding for a developer
         *
         * @param developer GitHub username of the main developer or organization behind the mod
         *
         * @returns The object for the branding for this developer
         */
        Branding getBrand(std::string_view developer) const;

        /**
         * Check if a developer already has their branding registered
         * @note The `registerBrand` function already uses this function internally
         *
         * @param developer GitHub username of the main developer or organization behind the mod
         *
         * @returns Whether this developer is already registered
         */
        bool doesBrandExist(std::string_view developer) const;
    };
};