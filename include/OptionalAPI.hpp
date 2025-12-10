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
    // Optional variant of `BrandImageType`
    enum class BrandImageTypeV2 : int {
        URL = 0,
        Sprite = 1,
        SpriteFrame = 2
    };

    // Optional variant of `BrandingManager`
    class BrandingManagerV2 {
    public:
        static Result<> registerBrand(std::string const& modId, std::string const& image, BrandImageTypeV2 type = BrandImageTypeV2::Sprite)
            GEODE_EVENT_EXPORT(&BrandingManagerV2::registerBrand, (modId, image, type));
    };
};