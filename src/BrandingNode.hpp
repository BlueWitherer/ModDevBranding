#include <Branding.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace branding;

class BrandingNode : public CCNode {
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;

protected:
    BrandingNode();
    ~BrandingNode();

    void retryRemoteLoad(LazySprite* sender);
    void cancelRemoteLoad(CCNode* sender);

    float getImageScale(CCSprite* sprite) const;

    Result<Branding> brand(ZStringView modId) const noexcept;
    bool useLocalBrand() const noexcept;

    bool init(MDTextArea* container, std::string dev, ZStringView modId);

public:
    static BrandingNode* create(MDTextArea* container, std::string dev, ZStringView modId = GEODE_MOD_ID);

    void loadBrand();

    std::string_view getDeveloper() const noexcept;
};