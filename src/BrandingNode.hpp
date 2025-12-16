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

    void retryRemoteLoad(LazySprite* sender);
    void cancelRemoteLoad(CCNode* sender);

    float getImageScale(CCSprite* sprite) const;

    Branding brand(std::string_view modId) const;
    bool useLocalBrand() const;

    bool init(MDTextArea* container, std::string const& dev, std::string const& modId);

public:
    static BrandingNode* create(MDTextArea* container, std::string const& dev, std::string const& modId = GEODE_MOD_ID);

    void loadBrand();
};