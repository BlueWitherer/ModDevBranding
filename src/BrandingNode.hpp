#include <Branding.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace branding;

class BrandingNode : public CCNode {
protected:
    class Impl;
    std::unique_ptr<Impl> m_impl;

    BrandingNode();
    virtual ~BrandingNode();

    Branding brand(std::string_view modId) const;
    bool useLocalBrand() const;

    void retryRemoteLoad(LazySprite* sender);
    void cancelRemoteLoad(CCNode* sender);

    bool init(MDTextArea* container, const std::string& dev, const std::string& modId);

public:
    static BrandingNode* create(MDTextArea* container, const std::string& dev, const std::string& modId = GEODE_MOD_ID);

    void loadBrand();
};