#include "BrandingNode.hpp"

#include <Branding.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace branding;

class BrandingNode::Impl final {
public:
    Branding m_brand = Branding("", "");
    std::string developer = "";

    Ref<MDTextArea> m_container = nullptr;

    float m_timeout = static_cast<float>(Mod::get()->getSettingValue<double>("timeout"));
    bool m_useWebP = Loader::get()->isModLoaded("prevter.imageplus");
};

BrandingNode::BrandingNode() {
    m_impl = std::make_unique<Impl>();
};

BrandingNode::~BrandingNode() {};

bool BrandingNode::init(MDTextArea* container, std::string_view dev, std::string_view modId) {
    m_impl->m_brand = brand(modId);
    m_impl->developer = dev;
    m_impl->m_container = container;

    if (!CCNode::init()) return false;

    // no dev no brand
    if (modId.empty()) return false;

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

    auto localBrand = useLocalBrand();
    LazySprite* lazySprite = nullptr;

    if (localBrand) {
        log::debug("using local brand for mod {}", m_impl->m_brand.mod);

        CCSprite* sprite = nullptr;

        log::debug("scanning brand image node type for {}...", m_impl->m_brand.mod);
        log::debug("{} uses {} as source image for branding", m_impl->m_brand.mod, m_impl->m_brand.image);
        switch (m_impl->m_brand.type) {
        case BrandImageType::URL: {
            log::debug("{} requested a url", m_impl->m_brand.mod);
            lazySprite = LazySprite::create(m_impl->m_container->getScaledContentSize());
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

        if (sprite) {
            log::debug("branding sprite found");

            sprite->setID("brand"_spr);
            sprite->setOpacity(100);
            sprite->setAnchorPoint({ 1, 0 });
            sprite->setPosition({ getScaledContentWidth(), 0.f });

            float scaleX = m_impl->m_container->getScaledContentWidth() / sprite->getScaledContentWidth();
            float scaleY = m_impl->m_container->getScaledContentHeight() / sprite->getScaledContentHeight();

            float scale = std::min(scaleX, scaleY);
            if (scale >= 1.0f) scale = 1.0f;

            sprite->setScale(scale);

            addChild(sprite);

            log::info("loaded local branding sprite");
        } else {
            log::error("no branding sprite created");
        };
    } else {
        log::debug("using remote brand for mod {}", m_impl->m_brand.mod);
        lazySprite = LazySprite::create(m_impl->m_container->getScaledContentSize());
    };

    if (lazySprite) {
        log::debug("branding lazysprite found");

        lazySprite->setID("brand"_spr);
        lazySprite->setOpacity(100);
        lazySprite->setAnchorPoint({ 1, 0 });
        lazySprite->setPosition({ getScaledContentWidth(), 0.f });

        lazySprite->setLoadCallback([=](Result<> res) {
            if (res.isOk()) {
                log::info("loaded remote branding sprite");

                lazySprite->setOpacity(100);
                lazySprite->setAnchorPoint({ 1, 0 });
                lazySprite->setPosition({ getScaledContentWidth(), 0.f });

                float scaleX = m_impl->m_container->getScaledContentWidth() / lazySprite->getScaledContentWidth();
                float scaleY = m_impl->m_container->getScaledContentHeight() / lazySprite->getScaledContentHeight();

                float scale = std::min(scaleX, scaleY);
                if (scale >= 1.0f) scale = 1.0f;

                lazySprite->setScale(scale);
            } else {
                log::error("failed to load remote branding sprite");
                lazySprite->removeMeAndCleanup();
            };
                                    });

        std::string reqUrl = "";

        if (localBrand) {
            reqUrl = m_impl->m_brand.image;
        } else {
            auto url = fmt::format("https://moddev.cheeseworks.gay/api/v1/image?dev={}", m_impl->developer);
            auto query = "&fmt=webp";

            reqUrl = fmt::format("{}{}", url, m_impl->m_useWebP ? query : "");
        };

        log::debug("requesting brand image from {} for mod {}", reqUrl, m_impl->m_brand.mod);
        if (m_impl->m_brand.mod.size() > 0) lazySprite->loadFromUrl(reqUrl.c_str());
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

Branding BrandingNode::brand(std::string_view modId) const {
    if (auto bm = BrandingManager::get()) return bm->getBrand(modId);
    return Branding("", std::string(modId));
};

bool BrandingNode::useLocalBrand() const {
    if (auto bm = BrandingManager::get()) return bm->doesBrandExist(m_impl->m_brand.mod);
    return false;
};

BrandingNode* BrandingNode::create(MDTextArea* container, std::string_view dev, std::string_view modId) {
    auto ret = new BrandingNode();
    if (ret->init(container, dev, modId)) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};