#define GEODE_DEFINE_EVENT_EXPORTS
#include <Branding.hpp>
#include <OptionalAPI.hpp>

using namespace branding;

matjson::Value Branding::toJson() const {
    return matjson::makeObject({
        { "image", image },
        { "mod", mod },
        { "type", static_cast<int>(type) }
                               });
};

Branding Branding::fromJson(const matjson::Value& v) {
    return Branding(
        v["image"].asString().unwrapOrDefault(),
        v["mod"].asString().unwrapOrDefault(),
        static_cast<BrandImageType>(v["type"].asInt().unwrapOrDefault())
    );
};

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
        auto b = Branding(
            image,
            modId,
            type
        );

        m_impl->m_brands.push_back(b);
        Mod::get()->setSavedValue(modId, b.toJson());

        log::debug("Registered branding {} for {}", image, modId);
    };
};

Branding BrandingManager::getBrand(std::string_view modId) const {
    for (const auto& b : getBrands()) {
        if (b.mod == modId) return b;
    };

    if (Loader::get()->isModInstalled(std::string(modId))) return Branding::fromJson(Mod::get()->getSavedValue<matjson::Value>(modId));

    return Branding("", std::string(modId));
};

BrandingManager* BrandingManager::get() {
    static auto inst = new BrandingManager();
    return inst;
};

Result<> BrandingManagerV2::registerBrand(
    std::string const& modId,
    std::string const& image,
    BrandImageType type
) {
    BrandingManager::get()->registerBrand(modId, image, type);
    return Ok();
};