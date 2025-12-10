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
    // Optional API variant (wrapper struct)
    struct BrandImageTypeV2 {
        enum {
            URL = 0b0001,
            Sprite = 0b0010,
            SpriteFrame = 0b0100,
        } m_value;

        BrandImageTypeV2() = default;
        BrandImageTypeV2(uint32_t value)
            : m_value(static_cast<decltype(m_value)>(value)) {};

        operator BrandImageType() const {
            return static_cast<BrandImageType>(m_value);
        };

        operator uint32_t() const {
            return static_cast<uint32_t>(m_value);
        };
    };

    // Optional operators (like ModifierV2)
    inline bool operator&(BrandImageTypeV2 const& a, BrandImageTypeV2 const& b) {
        return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
    };

    inline BrandImageTypeV2 operator|(BrandImageTypeV2 const& a, BrandImageTypeV2 const& b) {
        return BrandImageTypeV2(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    };

    inline BrandImageTypeV2& operator|=(BrandImageTypeV2& a, BrandImageTypeV2 const& b) {
        a = a | b;
        return a;
    };

    // Optional variant of `BrandingManager`
    class BrandingManagerV2 {
    public:
        static Result<> registerBrand(std::string const& modId, std::string const& image, BrandImageTypeV2 type)
            GEODE_EVENT_EXPORT(&BrandingManagerV2::registerBrand, (modId, image, type));
    };
};