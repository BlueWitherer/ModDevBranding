#define GEODE_DEFINE_EVENT_EXPORTS
#include <Branding.hpp>
#include <OptionalAPI.hpp>

using namespace branding;

class BrandingManager::Impl final {
public:
    std::vector<Branding> m_brands = {}; // Array of registered branding images
};

BrandingManager::BrandingManager() {
    m_impl = std::make_unique<Impl>();
};

BrandingManager::~BrandingManager() {};

std::vector<Branding> BrandingManager::getBrands() const {
    return m_impl->m_brands;
};

bool BrandingManager::doesBrandExist(std::string_view modId) const {
    for (const auto& brand : getBrands()) {
        if (brand.mod == modId) return true;
    };

    return false;
};

void BrandingManager::registerBrand(const std::string& modId, const std::string& image, BrandImageType type) {
    if (doesBrandExist(modId)) {
        log::error("Could not register branding for {} because one already exists!", modId);
    } else {
        m_impl->m_brands.push_back(Branding(
            image,
            modId,
            type
        ));

        log::debug("Registered branding {} for {}", image, modId);
    };
};

Branding BrandingManager::getBrand(std::string_view modId) const {
    for (const auto& brand : getBrands()) {
        if (brand.mod == modId) return brand;
    };

    return Branding("", std::string(modId));
};

BrandingManager* BrandingManager::get() {
    static auto inst = new BrandingManager();
    return inst;
};

Result<> BrandingManagerV2::registerBrand(
    std::string const& modId,
    std::string const& image,
    BrandImageTypeV2 type
) {
    BrandingManager::get()->registerBrand(modId, image, static_cast<BrandImageType>(type));
    return Ok();
};