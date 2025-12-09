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

std::vector<Branding> BrandingManager::getBrands() const {
    return m_impl->m_brands;
};

bool BrandingManager::doesBrandExist(std::string_view developer) const {
    for (const auto& brand : getBrands()) {
        if (brand.developer == developer) return true;
    };

    return false;
};

void BrandingManager::registerBrand(const std::string& developer, const std::string& image, BrandImageType type) {
    if (doesBrandExist(developer)) {
        log::error("Could not register branding for {} because one already exists!", developer);
    } else {
        m_impl->m_brands.push_back(Branding(
            image,
            developer,
            type
        ));
    };
};

Branding BrandingManager::getBrand(std::string_view developer) const {
    for (const auto& brand : getBrands()) {
        if (brand.developer == developer) return brand;
    };

    return Branding("", std::string(developer));
};

BrandingManager* BrandingManager::get() {
    static auto inst = new BrandingManager();
    return inst;
};