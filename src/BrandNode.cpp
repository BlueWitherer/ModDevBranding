#include "BrandNode.h"

#include <API.h>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace cw::brand;

namespace fs = std::filesystem;

static constexpr CCPoint g_anchor = {1, 0};

struct BrandNode::Impl final {
    Branding brand;
    std::string developer = "";

    bool retried = false;

    Ref<NineSlice> container = nullptr;
    CCClippingNode* clippingNode = nullptr;

    float timeout = Mod::get()->getSettingValue<float>("timeout");
    int64_t opacity = Mod::get()->getSettingValue<int64_t>("opacity");
    bool useWebP = Loader::get()->isModLoaded("prevter.imageplus");

    bool useLocalBrand() const noexcept {
        if (auto bm = BrandingManager::get()) return bm->doesBrandExist(brand.mod, true);
        return false;
    };

    Result<Branding> getBrand(ZStringView modId) const noexcept {
        if (auto bm = BrandingManager::get()) return bm->getBrand(modId);
        return Err("BrandingManager not found");
    };

    float getImageScale(CCSprite* sprite) const noexcept {
        if (clippingNode && sprite) {
            auto spriteW = sprite->getScaledContentWidth();
            auto spriteH = sprite->getScaledContentHeight();

            if (spriteW <= 0.f || spriteH <= 0.f) {
                log::error("Sprite has zero or invalid content size");
                return 1.f;
            };

            auto x = clippingNode->getScaledContentWidth() / spriteW;
            auto y = clippingNode->getScaledContentHeight() / spriteH;

            auto scale = std::min(x, y);
            if (scale >= 1.f) scale = 1.f;

            return scale;
        };

        log::error("Branding container or sprite not found");
        return 1.f;
    };
};

BrandNode::BrandNode() : m_impl(std::make_unique<Impl>()) {};
BrandNode::~BrandNode() {};

bool BrandNode::init(NineSlice* container, std::string dev, ZStringView modId) {
    auto b = m_impl->getBrand(modId);

    auto ok = b.isOk();
    if (!ok) log::error("Couldn't find branding for mod {}: {}", modId, std::move(b).unwrapErr());

    m_impl->container = container;
    m_impl->developer = std::move(dev);

    if (ok) m_impl->brand = std::move(b).unwrap();

    m_impl->brand.mod = modId;

    if (!CCNode::init()) return false;

    setID("branding"_spr);
    setAnchorPoint(g_anchor);
    setContentSize(container->getScaledContentSize());

    m_impl->clippingNode = CCClippingNode::create(container);
    m_impl->clippingNode->setAnchorPoint({0.5, 0.5});
    m_impl->clippingNode->setContentSize(getScaledContentSize());
    m_impl->clippingNode->setAlphaThreshold(0.f);

    addChildAtPosition(m_impl->clippingNode, Anchor::Center, {7.5f, 0.f});

    loadBrand();

    return true;
};

void BrandNode::loadBrand() {
    setContentSize(m_impl->container->getScaledContentSize());

    m_impl->clippingNode->setContentSize(getScaledContentSize());
    m_impl->clippingNode->removeAllChildren();

    log::debug("Loading brand for mod {}", m_impl->brand.mod);

    LazySprite* lazySprite = nullptr;

    auto localBrand = m_impl->useLocalBrand();
    if (localBrand) {
        log::debug("Using local brand for mod {}", m_impl->brand.mod);

        CCSprite* sprite = nullptr;

        log::debug("Scanning brand image node type for {}...", m_impl->brand.mod);
        log::debug("{} uses {} as source image for branding", m_impl->brand.mod, m_impl->brand.image);

        switch (m_impl->brand.type) {
            case BrandImageType::URL: {
                log::debug("{} requested a url", m_impl->brand.mod);
                lazySprite = LazySprite::create(m_impl->container->getScaledContentSize(), false);
            } break;

            case BrandImageType::Sprite: {
                log::debug("{} requested a sprite", m_impl->brand.mod);
                sprite = CCSprite::create(m_impl->brand.image.c_str());
            } break;

            case BrandImageType::SpriteFrame: {
                log::debug("{} requested a sprite frame", m_impl->brand.mod);
                sprite = CCSprite::createWithSpriteFrameName(m_impl->brand.image.c_str());
            } break;

            default: {
                log::error("{} requested unknown image node type", m_impl->brand.mod);
            }
                return;
        };

        log::debug("Processing local branding image {}", m_impl->brand.image);

        if (sprite) {
            log::debug("Branding sprite found");

            sprite->setID("brand"_spr);
            sprite->setOpacity(m_impl->opacity);
            sprite->setAnchorPoint(g_anchor);
            sprite->setScale(m_impl->getImageScale(sprite));

            m_impl->clippingNode->addChildAtPosition(sprite, Anchor::BottomRight, {-7.5f, 0.f});

            log::info("Loaded local branding sprite");
        } else {
            log::error("No branding sprite created");
        };
    } else {
        log::debug("Using remote or test brand for mod {}", m_impl->brand.mod);
        lazySprite = LazySprite::create(m_impl->container->getScaledContentSize(), false);
        log::debug("Processing remote branding image {}", m_impl->brand.image);
    };

    if (lazySprite) {
        log::debug("Branding lazysprite found");

        lazySprite->setID("brand"_spr);
        lazySprite->setAnchorPoint(g_anchor);

        m_impl->clippingNode->addChildAtPosition(lazySprite, Anchor::BottomRight, {-7.5f, 0.f});

        lazySprite->setLoadCallback([this, lazySprite](Result<> res) {
            if (res.isErr()) {
                log::error("Failed to load remote or test branding sprite: {}", res.unwrapErr());

                if (m_impl->retried) return lazySprite->stopAllActions();
                if (!m_impl->retried) retryRemoteLoad(lazySprite);

                return;
            };

            log::info("Loaded remote or test branding sprite");

            lazySprite->setAnchorPoint(g_anchor);
            lazySprite->setOpacity(m_impl->opacity);
            lazySprite->setScale(lazySprite->getScale() * m_impl->getImageScale(lazySprite));

            updateLayout();
        });

        if (m_impl->brand.mod == GEODE_MOD_ID) {
            log::debug("Attempting to load local test brand image");

            // @geode-ignore(unknown-setting)
            auto const path = Mod::get()->getSettingValue<fs::path>("preview-image");
            if (fs::exists(path)) {
                lazySprite->loadFromFile(path, CCImage::kFmtUnKnown, true);
                return;
            } else {
                log::error("Couldn't load local test brand image");
            };
        };

        std::string reqUrl;
        if (localBrand) {
            reqUrl = m_impl->brand.image;
        } else {
            auto const url = fmt::format("https://moddev.cheeseworks.gay/api/v1/image?dev={}", m_impl->developer);
            reqUrl = fmt::format("{}{}", url, m_impl->useWebP ? "&fmt=webp" : "");
        };

        if (reqUrl.size() > 0) {
            log::debug("Requesting brand image from {} for mod {}", reqUrl, m_impl->brand.mod);
            lazySprite->loadFromUrl(reqUrl.c_str());

            // cancel load after timeout
            log::debug("Scheduling image load cancel for {} after {} seconds", reqUrl, m_impl->timeout);
            lazySprite->runAction(CCSequence::createWithTwoActions(
                CCDelayTime::create(m_impl->timeout),
                CCCallFuncN::create(this, callfuncN_selector(BrandNode::cancelRemoteLoad))));
        };
    } else {
        if (localBrand) log::error("no branding lazysprite created");
    };

    updateLayout();
};

void BrandNode::retryRemoteLoad(LazySprite* sender) {
    if (sender) {
        m_impl->retried = true;

        auto url = fmt::format("https://moddev.cheeseworks.gay/api/v1/image?dev={}&mod={}", m_impl->developer, m_impl->brand.mod);
        auto const reqUrl = fmt::format("{}{}", std::move(url), m_impl->useWebP ? "&fmt=webp" : "");

        log::debug("Retrying request for brand image from {} for mod {}", reqUrl, m_impl->brand.mod);
        sender->loadFromUrl(reqUrl.c_str());
    } else {
        log::error("LazySprite is missing");
    };
};

void BrandNode::cancelRemoteLoad(CCNode* sender) {
    log::warn("Attempting to cancel remote or test brand image load");
    if (auto lazySprite = typeinfo_cast<LazySprite*>(sender)) lazySprite->cancelLoad();
};

std::string_view BrandNode::getDeveloper() const noexcept {
    return m_impl->developer;
};

BrandNode* BrandNode::create(NineSlice* container, std::string dev, ZStringView modId) {
    auto ret = new BrandNode();
    if (ret->init(container, std::move(dev), modId)) {
        ret->autorelease();
        return ret;
    };

    delete ret;
    return nullptr;
};