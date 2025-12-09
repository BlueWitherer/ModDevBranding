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

    Branding brand(std::string_view developer) const;
    bool useLocalBrand() const;

    void loadBrand();

    void cancelRemoteLoad(CCNode* sender);

    bool init(CCNode* container, std::string_view developer);

public:
    static BrandingNode* create(CCNode* container, std::string_view developer);
};