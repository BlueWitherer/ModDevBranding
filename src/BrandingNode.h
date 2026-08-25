#pragma once

#include <API.h>

#include <Geode/Geode.hpp>

class BrandingNode final : public cocos2d::CCNode {
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;

    float getImageScale(cocos2d::CCSprite* sprite) const;

    bool useLocalBrand() const noexcept;

    geode::Result<cw::brand::Branding> brand(geode::ZStringView modId) const noexcept;

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