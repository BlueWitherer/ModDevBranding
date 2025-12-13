#pragma once

#ifdef GEODE_IS_WINDOWS
#ifdef CW_MODDEVBRANDING_API_EXPORTING
#define CW_MODDEVBRANDING_API_DLL __declspec(dllexport)
#else
#define CW_MODDEVBRANDING_API_DLL __declspec(dllimport)
#endif
#else
#define CW_MODDEVBRANDING_API_DLL __attribute__((visibility("default")))
#endif

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace branding {
    enum class BrandImageType : unsigned int {
        URL = 0,
        Sprite = 1,
        SpriteFrame = 2,
    };

    struct Branding {
        std::string image;
        std::string mod;
        BrandImageType type = BrandImageType::Sprite;

        Branding(const std::string& i, const std::string& m, BrandImageType t = BrandImageType::Sprite);

        /**
         * Returns a matjson object constructed from a constructed Branding struct
         *
         * @returns Constructed matjson object
         */
        matjson::Value toJson() const;

        /**
         * Returns a Branding struct constructed from a matjson object
         *
         * @param v matjson object
         *
         * @returns Constructed Branding object
         */
        static Branding fromJson(const matjson::Value& v);
    };

    class CW_MODDEVBRANDING_API_DLL BrandingManager : public CCObject {
    protected:
        class Impl; // PImpl class
        std::unique_ptr<Impl> m_impl; // PImpl pointer

        BrandingManager(); // Constructor
        virtual ~BrandingManager(); // Destructor

        /**
         * Returns the array of all registered mod branding
         *
         * @returns An array of all registered mod branding
         */
        std::vector<Branding> getBrands() const;

    public:
        // Get branding manager singleton
        static BrandingManager* get();

        /**
         * Register your very own branding image to appear on your mod! Internally checks for duplicate mod entries
         *
         * @param modId ID of the mod to apply branding on
         * @param image Sprite name, sheet frame name, or URL of the image to use as branding on this mod
         * @param type Whether you're using a sprite, spritesheet frame, or external URL as the source of your branding
         */
        void registerBrand(const std::string& modId, const std::string& image, BrandImageType type = BrandImageType::Sprite);

        /**
         * Get the branding for a mod
         *
         * @param modId ID of the mod
         *
         * @returns The object for the branding for this mod
         */
        Branding getBrand(std::string_view modId) const;

        /**
         * Check if a mod already has its branding registered
         * @attention The `registerBrand` function already uses this function internally
         *
         * @param modId ID of the mod
         * @param checkLocal Whether to check local save data
         *
         * @returns Whether this mod is already registered
         */
        bool doesBrandExist(std::string_view modId, bool checkLocal = false) const;
    };
};