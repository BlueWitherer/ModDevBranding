#pragma once

#include "Branding.hpp"

#include <Geode/Result.hpp>

#include <Geode/loader/Dispatch.hpp>

#ifdef MY_MOD_ID
#undef MY_MOD_ID
#endif
#define MY_MOD_ID "cheeseworks.moddevbranding"

namespace branding {
    // Alias for `branding::BrandImageType` enum class
    using Type = branding::BrandImageType;

    /**
     * Register your very own branding image to appear on your mod! Internally checks for duplicate mod entries
     *
     * @param modId ID of the mod to apply branding on
     * @param image Sprite name, sheet frame name, or URL of the image to use as branding on this mod
     * @param type Whether you're using a sprite, spritesheet frame, or external URL as the source of your branding
     */
    inline geode::Result<> registerBrand(std::string modId, std::string image, BrandImageType type = BrandImageType::Sprite)
        GEODE_EVENT_EXPORT(&branding::registerBrand, (modId, image, type));
};