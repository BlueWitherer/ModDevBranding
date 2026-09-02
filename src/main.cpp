#include "BrandNode.h"

#include <Geode/Geode.hpp>

#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp>

using namespace geode::prelude;
using namespace cw::brand;

namespace str = utils::string;

static const ZStringView urlGeode = "https://geode-sdk.org/mods/";

class $nodeModify(BrandingModPopup, ModPopup) {
    struct Fields final {
        BrandNode* branding = nullptr;
        Ref<MDTextArea> textArea = nullptr;

        float height = 0.f;
    };

    // nesting hell
    void modify() {
        if (auto self = reinterpret_cast<FLAlertLayer*>(this)) {
            log::trace("Modifying mod popup for branding");

            // node reordering lol
            if (auto displayNode = self->m_mainLayer->getChildByType<CCNode*>(2)) {
                log::trace("Found popup display node");

                if (auto md = typeinfo_cast<MDTextArea*>(displayNode->querySelector("right-column > description-container > textarea"))) {
                    log::trace("Found mod description markdown container");

                    if (auto container = md->getChildByType<NineSlice*>()) {
                        auto username = getGitUsername();
                        auto const mod = getModID();

                        log::debug("Creating brand node for {} by {}", mod, username);

                        if (auto brand = BrandNode::create(container, std::move(username), mod)) {
                            auto f = m_fields.self();

                            f->textArea = md;
                            f->height = md->getScaledContentHeight();

                            brand->setZOrder(-9);

                            f->branding = brand;

                            md->addChildAtPosition(f->branding, Anchor::BottomRight, {(container->getScaledContentWidth() - md->getScaledContentWidth()) * 0.5f, 0.f});

                            log::info("Added brand node for {} by {}", mod, brand->getDeveloper());
                        } else {
                            log::error("Couldn't create brand node");
                        };

                        schedule(schedule_selector(BrandingModPopup::updateBrandSize));
                    } else {
                        log::error("Couldn't find mod description container background");
                    };
                } else {
                    log::error("Couldn't find mod description markdown container");
                };
            } else {
                log::error("Couldn't find mod popup display node");
            };
        } else {
            log::error("Couldn't cast to FLAlertLayer");
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