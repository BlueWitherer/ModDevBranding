#pragma once

#include "Branding.hpp"

#include <Geode/Geode.hpp>

#include <Geode/loader/Dispatch.hpp>

#ifdef MY_MOD_ID
#undef MY_MOD_ID
#endif
#define MY_MOD_ID "cheeseworks.moddevbranding"

using namespace geode::prelude;

namespace branding {
    // Optional variant of `BrandingManager`
    class BrandingManagerV2 {
    public:
        static Result<> registerBrand(std::string const& modId, std::string const& image, BrandImageType type = BrandImageType::Sprite)
            GEODE_EVENT_EXPORT(&BrandingManagerV2::registerBrand, (modId, image, type));
    };
};