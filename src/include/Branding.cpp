#define GEODE_DEFINE_EVENT_EXPORTS
#include <Branding.hpp>
#include <OptionalAPI.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace branding;

Branding::Branding(std::string i, std::string m, BrandImageType t)
    : image(std::move(i)), mod(std::move(m)), type(t) {};

matjson::Value Branding::toJson() const {
    return matjson::makeObject({
        { "image", image },
        { "mod", mod },
        { "type", static_cast<int>(type) }
                               });
};

Branding Branding::fromJson(matjson::Value const& v) {
    return Branding(
        v["image"].asString().unwrapOr(""),
        v["mod"].asString().unwrapOr(""),
        static_cast<BrandImageType>(v["type"].asInt().unwrapOr(0))
    );
};

class BrandingManager::Impl final {
public:
    std::vector<Branding> m_brands; // Array of registered branding images
};

BrandingManager::BrandingManager() {
    m_impl = std::make_unique<Impl>();
};

BrandingManager::~BrandingManager() {};

std::span<Branding> BrandingManager::getBrands() const noexcept {
    return m_impl->m_brands;
};

bool BrandingManager::doesBrandExist(std::string_view modId, bool checkLocal) const noexcept {
    for (auto const& brand : getBrands()) if (brand.mod == modId) return true;
    if (auto m = Mod::get()) return checkLocal && m->hasSavedValue(modId);

    return false;
};

void BrandingManager::registerBrand(std::string modId, std::string image, BrandImageType type) {
    auto const b = Branding(
        std::move(image),
        std::move(modId),
        type
    );

    Mod::get()->setSavedValue<matjson::Value>(modId, b.toJson());

    if (doesBrandExist(modId)) {
        log::error("Could not register branding for {} because one already exists!", modId);
    } else {
        m_impl->m_brands.push_back(b);
        log::debug("Registered branding {} of type {} for {}", image, static_cast<int>(type), modId);
    };
};

Result<Branding> BrandingManager::getBrand(std::string_view modId) const {
    for (auto const& b : getBrands()) if (b.mod == modId) return Ok(b);
    if (Loader::get()->isModInstalled(std::string(modId)) && Mod::get()->hasSavedValue(modId)) return Ok(Branding::fromJson(Mod::get()->getSavedValue<matjson::Value>(modId, Branding("", std::string(modId)).toJson())));

    return Err("Branding not found");
};

BrandingManager* BrandingManager::get() {
    static auto inst = new (std::nothrow) BrandingManager();
    return inst;
};

Result<> BrandingManagerV2::registerBrand(std::string modId, std::string image, BrandImageType type) {
    BrandingManager::get()->registerBrand(std::move(modId), std::move(image), type);
    return Ok();
};