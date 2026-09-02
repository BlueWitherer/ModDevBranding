#pragma once

#include <API.h>

#include <Geode/Geode.hpp>

namespace cw::brand {
    class BrandNode final : public cocos2d::CCNode {
    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        BrandNode();
        ~BrandNode();

        void retryRemoteLoad(geode::LazySprite* sender);
        void cancelRemoteLoad(CCNode* sender);

        bool init(geode::NineSlice* container, std::string dev, geode::ZStringView modId);

    public:
        static BrandNode* create(geode::NineSlice* container, std::string dev, geode::ZStringView modId = GEODE_MOD_ID);

        void loadBrand();

        std::string_view getDeveloper() const noexcept;
    };
};