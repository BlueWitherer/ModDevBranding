#pragma once

#include <API.h>

#include <Geode/Geode.hpp>

namespace cw::brand {
    class BrandingNode final : public cocos2d::CCNode {
    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        BrandingNode();
        ~BrandingNode();

        void retryRemoteLoad(geode::LazySprite* sender);
        void cancelRemoteLoad(CCNode* sender);

        bool init(geode::MDTextArea* container, std::string dev, geode::ZStringView modId);

    public:
        static BrandingNode* create(geode::MDTextArea* container, std::string dev, geode::ZStringView modId = GEODE_MOD_ID);

        void loadBrand();

        std::string_view getDeveloper() const noexcept;
    };
};