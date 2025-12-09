#include "BrandingNode.hpp"

#include <Branding.hpp>

#include <Geode/Geode.hpp>

#include <alphalaneous.alphas_geode_utils/include/NodeModding.h>

using namespace geode::prelude;
using namespace branding;

namespace str = utils::string;

class $nodeModify(BrandingModPopup, ModPopup) {
    void modify() {
        auto username = getGitUsername();
        log::info("git user {}", username);

        if (auto md = getChildByIDRecursive("textarea")) {
            log::info("found mod desc container");

            auto branding = BrandingNode::create(md, username);
            branding->setZOrder(-9);

            md->addChild(branding);
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