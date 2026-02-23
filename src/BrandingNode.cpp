#include "BrandingNode.hpp"

#include <Branding.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace branding;

namespace fs = std::filesystem;

class BrandingNode::Impl final {
public:
    std::string developer = "";
    Branding brand = Branding("", "");

    bool retried = false;

    Ref<MDTextArea> container = nullptr;

    float timeout = static_cast<float>(Mod::get()->getSettingValue<double>("timeout"));
    int64_t opacity = Mod::get()->getSettingValue<int64_t>("opacity");
    bool useWebP = Loader::get()->isModLoaded("prevter.imageplus");
};

BrandingNode::BrandingNode() : m_impl(std::make_unique<Impl>()) {};
BrandingNode::~BrandingNode() {};

bool BrandingNode::init(MDTextArea* container, std::string dev, ZStringView modId) {
    auto b = brand(modId);
    if (b.isErr()) log::error("Couldn't find branding for mod {}: {}", modId, b.unwrapErr());

    m_impl->developer = std::move(dev);
    m_impl->brand = modId == GEODE_MOD_ID ? Branding("", GEODE_MOD_ID) : b.unwrapOrDefault();
    m_impl->container = container;

    if (!CCNode::init()) return false;

    setID("branding"_spr);
    setAnchorPoint({ 1, 0 });
    setContentSize(container->getScaledContentSize());
    setPosition({ container->getScaledContentWidth(), 0.f });

    loadBrand();

    return true;
};

void BrandingNode::loadBrand() {
    setContentSize(m_impl->container->getScaledContentSize());
    removeAllChildrenWithCleanup(true);

    log::debug("Loading brand for mod {}", m_impl->brand.mod);

    LazySprite* lazySprite = nullptr;

    auto localBrand = useLocalBrand();
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
            } return;
        };

        log::debug("Processing local branding image {}", m_impl->brand.image);

        if (sprite) {
            log::debug("Branding sprite found");

            sprite->setID("brand"_spr);
            sprite->setOpacity(m_impl->opacity);
            sprite->setAnchorPoint({ 1, 0 });
            sprite->setPosition({ getScaledContentWidth(), 0.f });
            sprite->setScale(getImageScale(sprite));

            addChild(sprite);

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
        lazySprite->setAnchorPoint({ 1, 0 });
        lazySprite->setPosition({ getScaledContentWidth(), 0.f });

        lazySprite->setLoadCallback([this, lazySprite](Result<> res) {
            if (res.isOk()) {
                log::info("Loaded remote or test branding sprite");

                lazySprite->setOpacity(m_impl->opacity);
                lazySprite->setAnchorPoint({ 1, 0 });
                lazySprite->setPosition({ getScaledContentWidth(), 0.f });
                lazySprite->setScale(getImageScale(lazySprite));
            } else if (res.isErr()) {
                log::error("Failed to load remote or test branding sprite: {}", res.unwrapErr());
                if (m_impl->retried) return lazySprite->stopAllActions();
                if (!m_impl->retried) retryRemoteLoad(lazySprite);
            } else {
                log::error("Unknown error when loading or test remote branding sprite");
                lazySprite->removeMeAndCleanup();
            };
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

        addChild(lazySprite);

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
                CCCallFuncN::create(this, callfuncN_selector(BrandingNode::cancelRemoteLoad))
            ));
        };
    } else {
        if (localBrand) log::error("no branding lazysprite created");
    };
};

void BrandingNode::retryRemoteLoad(LazySprite* sender) {
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

void BrandingNode::cancelRemoteLoad(CCNode* sender) {
    log::warn("Attempting to cancel remote or test brand image load");
    if (auto lazySprite = typeinfo_cast<LazySprite*>(sender)) lazySprite->cancelLoad();
};

float BrandingNode::getImageScale(CCSprite* sprite) const {
    if (m_impl->container && sprite) {
        auto scaleX = m_impl->container->getScaledContentWidth() / sprite->getScaledContentWidth();
        auto scaleY = m_impl->container->getScaledContentHeight() / sprite->getScaledContentHeight();

        auto scale = std::min<float>(scaleX, scaleY);
        if (scale >= 1.f) scale = 1.f;

        return scale;
    } else {
        log::error("Branding container or sprite not found");
        return 1.f;
    };
};

Result<Branding> BrandingNode::brand(ZStringView modId) const noexcept {
    if (auto bm = BrandingManager::get()) return bm->getBrand(modId);
    return Err("BrandingManager not found");
};

bool BrandingNode::useLocalBrand() const noexcept {
    if (auto bm = BrandingManager::get()) return bm->doesBrandExist(m_impl->brand.mod, true);
    return false;
};

std::string_view BrandingNode::getDeveloper() const noexcept {
    return m_impl->developer;
};

BrandingNode* BrandingNode::create(MDTextArea* container, std::string dev, ZStringView modId) {
    auto ret = new BrandingNode();
    if (ret->init(container, std::move(dev), modId)) {
        ret->autorelease();
        return ret;
    };

    delete ret;
    return nullptr;
};