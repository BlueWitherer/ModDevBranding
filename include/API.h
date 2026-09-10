#pragma once

#ifdef GEODE_IS_WINDOWS
#ifdef ModDevBranding_EXPORTS
#define CW_MODDEVBRANDING_API_DLL __declspec(dllexport)
#else
#define CW_MODDEVBRANDING_API_DLL __declspec(dllimport)
#endif
#else
#define CW_MODDEVBRANDING_API_DLL __attribute__((visibility("default")))
#endif

#include <cocos2d.h>

#include <Geode/Result.hpp>

namespace branding {
    // Source of the provided branding image
    enum class BrandImageType : unsigned int {
        URL = 0,          // Remote image URL
        Sprite = 1,       // Mod sprite image
        SpriteFrame = 2,  // Mod spritesheet frame
    };

    // Alias for `cw::brand::BrandImageType` enum class
    using Type = branding::BrandImageType;

    struct Branding final {
        std::string image;
        std::string mod;
        Type type = Type::Sprite;

        Branding() = default;
        Branding(std::string i, std::string m, Type t = Type::Sprite);

        /// Returns a matjson object constructed from a constructed Branding struct
        /// @returns Constructed matjson object
        /// @deprecated Will be removed in v3
        [[deprecated("Use matjson::Serialize instead")]] matjson::Value toJSON() const;

        /// Returns a Branding struct constructed from a matjson object
        /// @param v matjson object
        /// @returns Constructed Branding object
        /// @deprecated Will be removed in v3
        [[deprecated("Use matjson::Serialize instead")]] static geode::Result<Branding> fromJSON(matjson::Value const& v);
    };

    class CW_MODDEVBRANDING_API_DLL BrandingManager final {
    private:
        geode::utils::StringMap<Branding> m_brands;  // Array of registered branding images

    protected:
        BrandingManager() = default;
        ~BrandingManager() = default;

        BrandingManager(const BrandingManager&) = delete;
        BrandingManager& operator=(const BrandingManager&) = delete;

        BrandingManager(BrandingManager&&) = delete;
        BrandingManager& operator=(BrandingManager&&) = delete;

    public:
        // Get branding manager singleton
        static BrandingManager* get();

        /// Register your very own branding image to appear on your mod! Internally checks for duplicate mod entries
        /// @param modId ID of the mod to apply branding on
        /// @param image Sprite name, sheet frame name, or URL of the image to use as branding on this mod
        /// @param type Whether you're using a sprite, spritesheet frame, or external URL as the source of your branding
        void registerBrand(std::string modId, std::string image, Type type = Type::Sprite);

        /// Get the branding for a mod
        /// @param modId ID of the mod
        /// @returns The object for the branding for this mod, if any
        [[nodiscard]] geode::Result<Branding> getBrand(std::string_view modId) const;

        /// Check if a mod already has its branding registered
        /// @attention The `registerBrand` function already uses this function internally
        /// @param modId ID of the mod
        /// @param checkLocal Whether to also check local save data
        /// @returns Whether this mod is already registered
        bool doesBrandExist(std::string_view modId, bool checkLocal = false) const noexcept;
    };
};

namespace cw::brand {  // backwards compat or something
    using namespace branding;
};

template <>
struct matjson::Serialize<cw::brand::Branding> final {
    static geode::Result<cw::brand::Branding> fromJson(matjson::Value const& value);
    static matjson::Value toJson(cw::brand::Branding const& value);
};