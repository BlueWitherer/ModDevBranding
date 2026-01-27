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

    Result<Branding> brand(ZStringView modId) const noexcept;
    bool useLocalBrand() const noexcept;

    bool init(MDTextArea* container, std::string dev, ZStringView modId);

public:
    static BrandingNode* create(MDTextArea* container, std::string dev, ZStringView modId = GEODE_MOD_ID);

    void loadBrand();
};