#include <Branding.hpp>

using namespace branding;

class BrandingManager::Impl final {
public:
    std::vector<Branding> m_brands = {};
};

BrandingManager::BrandingManager() {
    m_impl = std::make_unique<Impl>();
};

BrandingManager::~BrandingManager() {};

void BrandingManager::registerBrand(std::string_view developer, std::string_view image, BrandImageType type) {};

void BrandingManager::registerBrandURL(std::string_view developer, std::string_view url) {};
void BrandingManager::registerBrandSprite(std::string_view developer, std::string_view spriteName) {};
void BrandingManager::registerBrandSpriteFrame(std::string_view developer, std::string_view spriteFrameName) {};

BrandingManager::Branding BrandingManager::getBrand(std::string_view developer) const {
    return Branding("", "");
};

BrandingManager* BrandingManager::get() {
    static auto inst = new BrandingManager();
    return inst;
};