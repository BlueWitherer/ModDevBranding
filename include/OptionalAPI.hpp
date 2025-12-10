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
        static Result<> registerBrand(
            std::string const& modId,
            std::string const& image,
            BrandImageType type = BrandImageType::Sprite
        ) GEODE_EVENT_EXPORT(&BrandingManagerOpt::registerBrand, (modId, image, type));
    };
};