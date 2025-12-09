#include "BrandingNode.hpp"

#include <Branding.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace branding;

class BrandingNode::Impl final {
public:
    Branding m_brand = Branding("", "");

    Ref<MDTextArea> m_container = nullptr;

    float m_timeout = static_cast<float>(Mod::get()->getSettingValue<double>("timeout"));
    bool m_useWebP = Loader::get()->isModLoaded("prevter.imageplus");
};

BrandingNode::BrandingNode() {
    m_impl = std::make_unique<Impl>();
};

BrandingNode::~BrandingNode() {};

bool BrandingNode::init(MDTextArea* container, std::string_view developer) {
    m_impl->m_brand = brand(developer);
    m_impl->m_container = container;

    if (!CCNode::init()) return false;

    // no dev no brand
    if (developer.empty()) return false;

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

    log::debug("loading brand for developer {}", m_impl->m_brand.developer);

    LazySprite* lazySprite = nullptr;

    if (useLocalBrand()) {
        log::debug("using local brand for developer {}", m_impl->m_brand.developer);

        CCSprite* sprite = nullptr;

        switch (m_impl->m_brand.type) {
        case BrandImageType::Sprite:
            sprite = CCSprite::create(m_impl->m_brand.image.c_str());

        case BrandImageType::SpriteFrame:
            sprite = CCSprite::createWithSpriteFrameName(m_impl->m_brand.image.c_str());

        case BrandImageType::URL:
            lazySprite = LazySprite::create(m_impl->m_container->getScaledContentSize());

        default:
            sprite = nullptr;
            lazySprite = nullptr;
        };

        if (sprite) {
            log::debug("branding sprite found");

            auto isYBigger = sprite->getScaledContentHeight() > sprite->getScaledContentWidth();

            auto sprRes = isYBigger ? sprite->getScaledContentHeight() : sprite->getScaledContentWidth();
            auto nodeRes = isYBigger ? m_impl->m_container->getScaledContentHeight() : m_impl->m_container->getScaledContentWidth();

            sprite->setScale(nodeRes / sprRes);
            sprite->setOpacity(125);
            sprite->setAnchorPoint({ 1, 0 });
            sprite->setPosition({ getScaledContentWidth(), 0.f });

            log::info("loaded local branding sprite");

            addChild(sprite);
        } else {
            log::error("no branding sprite created");
        };
    } else {
        log::debug("using remote brand for developer {}", m_impl->m_brand.developer);
        lazySprite = LazySprite::create(m_impl->m_container->getScaledContentSize());
    };

    if (lazySprite) {
        log::debug("branding lazysprite found");

        lazySprite->setID("branding"_spr);
        lazySprite->setAutoResize(true);
        lazySprite->setOpacity(100);
        lazySprite->setAnchorPoint({ 1, 0 });
        lazySprite->setPosition({ getScaledContentWidth(), 0.f });

        lazySprite->setLoadCallback([=](Result<> res) {
            if (res.isOk()) {
                log::info("loaded remote branding sprite");

                lazySprite->setOpacity(125);
                lazySprite->setAnchorPoint({ 1, 0 });
                lazySprite->setPosition({ getScaledContentWidth(), 0.f });
            } else {
                log::error("failed to load remote branding sprite");
                lazySprite->removeMeAndCleanup();
            };
                                    });

        // change this to use moddev.cubicstudios.xyz/api... when ready
        auto url = "https://i.imgur.com/LOpGTtV.png";
        auto query = "&fmt=webp";

        auto reqUrl = fmt::format("{}{}", url, m_impl->m_useWebP ? query : "");

        if (m_impl->m_brand.developer.size() > 0) lazySprite->loadFromUrl(reqUrl.c_str());
        if (lazySprite) addChild(lazySprite);

        // cancel load after timeout
        lazySprite->runAction(CCSequence::createWithTwoActions(
            CCDelayTime::create(m_impl->m_timeout),
            CCCallFuncN::create(
                this,
                callfuncN_selector(BrandingNode::cancelRemoteLoad)
            )
        ));
    } else {
        log::error("no branding lazysprite created");
    };
};

void BrandingNode::cancelRemoteLoad(CCNode* sender) {
    if (auto lazySprite = static_cast<LazySprite*>(sender)) lazySprite->cancelLoad();
};

Branding BrandingNode::brand(std::string_view developer) const {
    if (auto bm = BrandingManager::get()) return bm->getBrand(developer);
    return Branding("", std::string(developer));
};

bool BrandingNode::useLocalBrand() const {
    if (auto bm = BrandingManager::get()) return bm->doesBrandExist(m_impl->m_brand.developer);
    return false;
};

BrandingNode* BrandingNode::create(MDTextArea* container, std::string_view developer) {
    auto ret = new BrandingNode();
    if (ret->init(container, developer)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};