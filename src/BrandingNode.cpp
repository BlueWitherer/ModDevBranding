#include "BrandingNode.hpp"

#include <Branding.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace branding;

namespace fs = std::filesystem;

class BrandingNode::Impl final {
public:
    std::string m_developer = "";
    Branding m_brand = Branding("", "");

    bool m_retried = false;

    Ref<MDTextArea> m_container = nullptr;

    float m_timeout = static_cast<float>(Mod::get()->getSettingValue<double>("timeout"));
    int64_t m_opacity = Mod::get()->getSettingValue<int64_t>("opacity");
    bool m_useWebP = Loader::get()->isModLoaded("prevter.imageplus");
};

BrandingNode::BrandingNode() {
    m_impl = std::make_unique<Impl>();
};

BrandingNode::~BrandingNode() {};

bool BrandingNode::init(MDTextArea* container, std::string dev, ZStringView modId) {
    auto b = brand(modId);
    if (b.isErr()) log::error("couldn't find branding for mod {}: {}", modId, b.unwrapErr());

    m_impl->m_developer = std::move(dev);
    m_impl->m_brand = b.unwrapOrDefault();
    m_impl->m_container = container;

    if (!CCNode::init()) return false;

    setID("branding"_spr);
    setAnchorPoint({ 1, 0 });
    setContentSize(container->getScaledContentSize());
    setPosition({ container->getScaledContentWidth(), 0.f });

    loadBrand();

    return true;
};

void BrandingNode::loadBrand() {
    setContentSize(m_impl->m_container->getScaledContentSize());
    removeAllChildrenWithCleanup(true);

    log::debug("loading brand for mod {}", m_impl->m_brand.mod);

    LazySprite* lazySprite = nullptr;

    auto localBrand = useLocalBrand();
    if (localBrand) {
        log::debug("using local brand for mod {}", m_impl->m_brand.mod);

        CCSprite* sprite = nullptr;

        log::debug("scanning brand image node type for {}...", m_impl->m_brand.mod);
        log::debug("{} uses {} as source image for branding", m_impl->m_brand.mod, m_impl->m_brand.image);

        switch (m_impl->m_brand.type) {
        case BrandImageType::URL: {
            log::debug("{} requested a url", m_impl->m_brand.mod);
            lazySprite = LazySprite::create(m_impl->m_container->getScaledContentSize(), false);
            break;
        };

        case BrandImageType::Sprite: {
            log::debug("{} requested a sprite", m_impl->m_brand.mod);
            sprite = CCSprite::create(m_impl->m_brand.image.c_str());
            break;
        };

        case BrandImageType::SpriteFrame: {
            log::debug("{} requested a sprite frame", m_impl->m_brand.mod);
            sprite = CCSprite::createWithSpriteFrameName(m_impl->m_brand.image.c_str());
            break;
        };

        default: {
            log::error("{} requested unknown image node type", m_impl->m_brand.mod);
            return;
        };
        };

        log::debug("processing local branding image {}", m_impl->m_brand.image);

        if (sprite) {
            log::debug("branding sprite found");

            sprite->setID("brand"_spr);
            sprite->setOpacity(m_impl->m_opacity);
            sprite->setAnchorPoint({ 1, 0 });
            sprite->setPosition({ getScaledContentWidth(), 0.f });
            sprite->setScale(getImageScale(sprite));

            addChild(sprite);

            log::info("loaded local branding sprite");
        } else {
            log::error("no branding sprite created");
        };
    } else {
        log::debug("using remote or test brand for mod {}", m_impl->m_brand.mod);
        lazySprite = LazySprite::create(m_impl->m_container->getScaledContentSize(), false);
        log::debug("processing remote branding image {}", m_impl->m_brand.image);
    };

    if (lazySprite) {
        log::debug("branding lazysprite found");

        lazySprite->setID("brand"_spr);
        lazySprite->setAnchorPoint({ 1, 0 });
        lazySprite->setPosition({ getScaledContentWidth(), 0.f });

        lazySprite->setLoadCallback([this, lazySprite](Result<> res) {
            if (res.isOk()) {
                log::info("loaded remote or test branding sprite");

                lazySprite->setOpacity(m_impl->m_opacity);
                lazySprite->setAnchorPoint({ 1, 0 });
                lazySprite->setPosition({ getScaledContentWidth(), 0.f });
                lazySprite->setScale(getImageScale(lazySprite));
            } else if (res.isErr()) {
                log::error("failed to load remote or test branding sprite: {}", res.unwrapErr());
                if (!m_impl->m_retried) retryRemoteLoad(lazySprite);
            } else {
                log::error("unknown error when loading or test remote branding sprite");
                lazySprite->removeMeAndCleanup();
            };
                                    });

        if (m_impl->m_brand.mod == GEODE_MOD_ID) {
            log::debug("attempting to load local test brand image");

            // @geode-ignore(unknown-setting)
            auto const path = Mod::get()->getSettingValue<fs::path>("preview-image");
            if (fs::exists(path)) {
                lazySprite->loadFromFile(path, CCImage::kFmtUnKnown, true);
                return;
            } else {
                log::error("couldn't load local test brand image");
            };
        };

        addChild(lazySprite);

        std::string reqUrl;
        if (localBrand) {
            reqUrl = m_impl->m_brand.image;
        } else {
            auto const url = fmt::format("https://moddev.cheeseworks.gay/api/v1/image?dev={}", m_impl->m_developer);
            reqUrl = fmt::format("{}{}", url, m_impl->m_useWebP ? "&fmt=webp" : "");
        };

        log::debug("requesting brand image from {} for mod {}", reqUrl, m_impl->m_brand.mod);
        if (reqUrl.size() > 0) lazySprite->loadFromUrl(reqUrl.c_str());

        // cancel load after timeout
        log::debug("scheduling image load cancel for {} after {} seconds", reqUrl, m_impl->m_timeout);
        lazySprite->runAction(CCSequence::createWithTwoActions(
            CCDelayTime::create(m_impl->m_timeout),
            CCCallFuncN::create(this, callfuncN_selector(BrandingNode::cancelRemoteLoad))
        ));
    } else {
        if (localBrand) log::error("no branding lazysprite created");
    };
};

void BrandingNode::retryRemoteLoad(LazySprite* sender) {
    if (sender) {
        m_impl->m_retried = true;

        auto const url = fmt::format("https://moddev.cheeseworks.gay/api/v1/image?dev={}&mod={}", m_impl->m_developer, m_impl->m_brand.mod);
        auto const reqUrl = fmt::format("{}{}", url, m_impl->m_useWebP ? "&fmt=webp" : "");

        log::debug("retrying request for brand image from {} for mod {}", reqUrl, m_impl->m_brand.mod);
        sender->loadFromUrl(reqUrl.c_str());
    } else {
        log::error("lazysprite is missing");
    };
};

void BrandingNode::cancelRemoteLoad(CCNode* sender) {
    log::warn("attempting to cancel remote or test brand image load");
    if (auto lazySprite = typeinfo_cast<LazySprite*>(sender)) lazySprite->cancelLoad();
};

float BrandingNode::getImageScale(CCSprite* sprite) const {
    if (m_impl->m_container && sprite) {
        auto scaleX = m_impl->m_container->getScaledContentWidth() / sprite->getScaledContentWidth();
        auto scaleY = m_impl->m_container->getScaledContentHeight() / sprite->getScaledContentHeight();

        auto scale = std::min<float>(scaleX, scaleY);
        if (scale >= 1.f) scale = 1.f;

        return scale;
    } else {
        log::error("branding container or sprite not found");
        return 1.f;
    };
};

Result<Branding> BrandingNode::brand(ZStringView modId) const noexcept {
    if (auto bm = BrandingManager::get()) return bm->getBrand(modId);
    return Err("BrandingManager not found");
};

bool BrandingNode::useLocalBrand() const noexcept {
    if (auto bm = BrandingManager::get()) return bm->doesBrandExist(m_impl->m_brand.mod, true);
    return false;
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