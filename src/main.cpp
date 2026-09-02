#include "BrandingNode.h"

#include <Geode/Geode.hpp>

#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp>

using namespace geode::prelude;
using namespace cw::brand;

namespace str = utils::string;

static const ZStringView urlGeode = "https://geode-sdk.org/mods/";

class $nodeModify(BrandingModPopup, ModPopup) {
    struct Fields final {
        Ref<MDTextArea> textArea = nullptr;
        BrandingNode* branding = nullptr;

        float height = 0.f;
    };

    void modify() {
        if (auto md = typeinfo_cast<MDTextArea*>(getChildByIDRecursive("textarea"))) {
            auto username = getGitUsername();
            auto const mod = getModID();

            log::debug("Creating brand node for {} by {}", mod, username);

            if (auto brand = BrandingNode::create(md, std::move(username), mod)) {
                auto f = m_fields.self();

                f->textArea = md;
                f->height = md->getScaledContentHeight();

                brand->setPositionX(brand->getPositionX() + 7.794f);
                brand->setZOrder(-9);

                f->branding = brand;

                md->addChild(f->branding);

                log::info("Added brand node for {} by {}", mod, brand->getDeveloper());
            } else {
                log::error("Couldn't create brand node");
            };

            schedule(schedule_selector(BrandingModPopup::updateBrandSize));
        } else {
            log::error("Couldn't find mod description container");
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
                for (size_t i = 0; i < split.size(); i++) {
                    if (split[i] == "github.com" && i + 1 < split.size()) return split[i + 1];
                };
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

        if (f->branding && f->textArea) {
            if (f->height != f->textArea->getScaledContentHeight()) {
                f->branding->loadBrand();
                f->height = f->textArea->getScaledContentHeight();
            };
        } else {
            unschedule(schedule_selector(BrandingModPopup::updateBrandSize));
        };
    };
};