#include "BrandingNode.hpp"

#include <Geode/Geode.hpp>

#include <alphalaneous.alphas_geode_utils/include/NodeModding.h>

using namespace geode::prelude;

namespace str = utils::string;

$on_mod(Loaded) {
    auto saveData = Mod::get()->getSaveContainer();
    log::info("save data size is {}", saveData.size());
};

class $nodeModify(BrandingModPopup, ModPopup) {
    struct Fields {
        Ref<MDTextArea> m_textArea = nullptr;
        Ref<BrandingNode> m_branding = nullptr;

        float m_height = 0.f;
    };

    void modify() {
        auto username = getGitUsername();
        auto mod = getModID();

        log::debug("creating brand node for {} by {}", mod, username);

        if (auto md = static_cast<MDTextArea*>(getChildByIDRecursive("textarea"))) {
            log::info("found mod desc container");

            m_fields->m_textArea = md;
            m_fields->m_height = md->getScaledContentHeight();

            m_fields->m_branding = BrandingNode::create(md, username, mod);
            m_fields->m_branding->setPositionX(m_fields->m_branding->getPositionX() + 7.794f);
            m_fields->m_branding->setZOrder(-9);

            md->addChild(m_fields->m_branding);

            log::info("added brand node for {} by {}", mod, username);

            schedule(schedule_selector(BrandingModPopup::updateBrandSize));
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
            };
        };

        return "";
    };

    std::string getModID() {
        if (auto modPageBtn = getChildByIDRecursive("mod-online-page-button")) {
            if (auto url = static_cast<CCString*>(modPageBtn->getUserObject("url"))) {
                std::string urlStr = url->getCString();
                std::string urlGeode = "https://geode-sdk.org/mods/";

                if (str::startsWith(urlStr, urlGeode)) return urlStr.erase(0, urlGeode.size());
            };
        };

        return "";
    };

    // idk if theres events for mdtextarea size changes ;-;
    void updateBrandSize(float) {
        if (m_fields->m_branding && m_fields->m_textArea) {
            if (m_fields->m_height != m_fields->m_textArea->getScaledContentHeight()) {
                m_fields->m_branding->loadBrand();
                m_fields->m_height = m_fields->m_textArea->getScaledContentHeight();
            };
        } else {
            unschedule(schedule_selector(BrandingModPopup::updateBrandSize));
        };
    };
};