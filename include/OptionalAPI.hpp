#pragma once

#include "Branding.hpp"

#include <Geode/Result.hpp>

#include <Geode/loader/Dispatch.hpp>

#ifdef MY_MOD_ID
#undef MY_MOD_ID
#endif
#define MY_MOD_ID "cheeseworks.moddevbranding"

namespace branding {
    // Optional variant of `BrandingManager`
    class BrandingManagerV2 final {
    public:
        static geode::Result<> registerBrand(std::string modId, std::string image, BrandImageType type = BrandImageType::Sprite)
            GEODE_EVENT_EXPORT(&BrandingManagerV2::registerBrand, (modId, image, type));
    };
};