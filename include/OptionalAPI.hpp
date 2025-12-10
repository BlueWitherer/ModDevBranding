#pragma once

#include "Branding.hpp"

#include <Geode/Geode.hpp>

#include <Geode/loader/Dispatch.hpp>

#ifdef MY_MOD_ID
#undef MY_MOD_ID
#endif
#define MY_MOD_ID GEODE_MOD_ID

using namespace geode::prelude;

namespace branding {
    // Optional variant: slightly different name
    class BrandingManagerOpt {
    public:
        static Result<BrandingManager*> get()
            GEODE_EVENT_EXPORT(&BrandingManagerOpt::get, ());

        /**
         * Register your very own branding image to appear on your mod! Internally checks for duplicate mod entries
         *
         * @param modId ID of the mod to apply branding on
         * @param image Sprite name, sheet frame name, or URL of the image to use as branding on this mod
         * @param type Whether you're using a sprite, spritesheet frame, or external URL as the source of your branding
         */
        Result<> registerBrand(
            std::string const& modId,
            std::string const& image,
            BrandImageType type = BrandImageType::Sprite
        ) GEODE_EVENT_EXPORT(&BrandingManagerOpt::registerBrand, (modId, image, type));

        /**
         * Get the branding for a mod
         *
         * @param modId ID of the mod
         *
         * @returns The object for the branding for this mod
         */
        Result<Branding> getBrand(std::string_view modId) const
            GEODE_EVENT_EXPORT(&BrandingManagerOpt::getBrand, (modId));
    };
};