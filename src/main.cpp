#include "BrandingNode.hpp"

#include <Geode/Geode.hpp>

#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp>

using namespace geode::prelude;

namespace str = utils::string;

static const ZStringView urlGeode = "https://geode-sdk.org/mods/";

class $nodeModify(BrandingModPopup, ModPopup) {
    struct Fields {
        Ref<MDTextArea> m_textArea = nullptr;
        BrandingNode* m_branding = nullptr;

        float m_height = 0.f;
    };

    void modify() {
        if (auto md = typeinfo_cast<MDTextArea*>(getChildByIDRecursive("textarea"))) {
            log::info("found mod desc container");

            auto username = getGitUsername();
            auto const mod = getModID();

            log::debug("creating brand node for {} by {}", mod, username);

            if (auto brand = BrandingNode::create(md, std::move(username), mod)) {
                auto f = m_fields.self();

                f->m_textArea = md;
                f->m_height = md->getScaledContentHeight();

                brand->setPositionX(brand->getPositionX() + 7.794f);
                brand->setZOrder(-9);

                f->m_branding = brand;

                md->addChild(f->m_branding);

                log::info("added brand node for {} by {}", mod, brand->getDeveloper());
            } else {
                log::error("couldn't create brand node");
            };

            schedule(schedule_selector(BrandingModPopup::updateBrandSize));
        } else {
            log::error("couldn't find mod description container");
        };
    };

    std::string const getGitUsername() {
        if (auto ghBtn = getChildByIDRecursive("github")) {
            if (auto url = typeinfo_cast<CCString*>(ghBtn->getUserObject("url"))) {
                std::string urlStr = url->getCString();
                if (urlStr.empty()) return "";

                while (urlStr.back() == '/') urlStr.pop_back();

                if (urlStr.starts_with("https://www.")) urlStr.erase(8, 4);
                if (urlStr.starts_with("http://www.")) urlStr.erase(7, 4);

                auto const split = str::split(urlStr, "/");
                for (size_t i = 0; i < split.size(); i++) if (split[i] == "github.com" && i + 1 < split.size()) return split[i + 1];
            };
        };

        return "";
    };

    std::string const getModID() {
        if (auto modPageBtn = getChildByIDRecursive("mod-online-page-button")) {
            if (auto url = typeinfo_cast<CCString*>(modPageBtn->getUserObject("url"))) {
                std::string urlStr = url->getCString();

                if (str::startsWith(urlStr, urlGeode)) return urlStr.erase(0, urlGeode.size());
            };
        };

        return "";
    };

    // idk if theres events for mdtextarea size changes ;-;
    void updateBrandSize(float) {
        auto f = m_fields.self();

        if (f->m_branding && f->m_textArea) {
            if (f->m_height != f->m_textArea->getScaledContentHeight()) {
                f->m_branding->loadBrand();
                f->m_height = f->m_textArea->getScaledContentHeight();
            };
        } else {
            unschedule(schedule_selector(BrandingModPopup::updateBrandSize));
        };
    };
};