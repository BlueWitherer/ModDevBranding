#include <Geode/Geode.hpp>

#include <alphalaneous.alphas_geode_utils/include/NodeModding.h>

using namespace geode::prelude;

namespace str = utils::string;

class $nodeModify(BrandingModPopup, ModPopup) {
    struct Fields {
        bool m_useWebP = Loader::get()->isModLoaded("prevter.imageplus");
    };

    void modify() {
        auto username = getGitUsername();
        log::info("git user {}", username);

        if (auto md = getChildByIDRecursive("textarea")) {
            log::info("found mod desc container");

            auto sprite = LazySprite::create(md->getScaledContentSize(), false);
            sprite->setID("branding"_spr);

            sprite->setLoadCallback([=](Result<> res) {
                if (res.isOk()) {
                    log::info("loaded branding sprite");

                    sprite->setScale(0.875f);
                    sprite->setOpacity(125);
                    sprite->setAnchorPoint({ 1, 0 });
                    sprite->setPosition({ md->getScaledContentWidth(), 0.f });
                    sprite->setZOrder(-9);
                } else {
                    log::error("failed to load branding sprite");
                    sprite->removeMeAndCleanup();
                };
                                    });

            // change this to use moddev.arcticwoof.xyz/api... when ready
            auto url = "https://i.imgur.com/LOpGTtV.png";
            auto query = "?fmt=webp";

            auto reqUrl = fmt::format("{}{}", url, m_fields->m_useWebP ? query : "");

            if (username.size() > 0) sprite->loadFromUrl(reqUrl.c_str());
            if (sprite) md->addChild(sprite);
        } else {
            log::error("couldn't find mod desc container");
        };
    };

    std::string getGitUsername() {
        if (auto ghBtn = getChildByIDRecursive("github")) {
            if (CCString* url = static_cast<CCString*>(ghBtn->getUserObject("url"))) {
                std::string ghURL = url->getCString();
                if (ghURL.empty()) return "";

                auto split = str::split(ghURL, "/");
                for (size_t i = 0; i < split.size(); i++) { // gh username
                    if (split[i] == "github.com" && i + 1 < split.size()) return split[i + 1];
                };

                return "";
            } else {
                return "";
            };
        } else {
            return "";
        };
    };
};