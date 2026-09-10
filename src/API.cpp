#define GEODE_DEFINE_EVENT_EXPORTS
#include <API.h>
#include <OptionalAPI.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace cw::brand;

Branding::Branding(std::string i, std::string m, Type t) : image(std::move(i)), mod(std::move(m)), type(t) {};

Result<cw::brand::Branding> matjson::Serialize<cw::brand::Branding>::fromJson(matjson::Value const& value) {
    if (!value.isObject()) return Err("JSON value is not an object");

    GEODE_UNWRAP_INTO(std::string image, value["image"].asString());
    GEODE_UNWRAP_INTO(std::string mod, value["mod"].asString());
    GEODE_UNWRAP_INTO(unsigned int type, value["type"].asUInt());

    return Ok(Branding(
        std::move(image),
        std::move(mod),
        static_cast<cw::brand::Type>(type)));
};

matjson::Value matjson::Serialize<cw::brand::Branding>::toJson(Branding const& value) {
    return matjson::makeObject({
        {"image", value.image},
        {"mod", value.mod},
        {"type", static_cast<int>(value.type)},
    });
};

matjson::Value Branding::toJSON() const {
    return matjson::Serialize<Branding>::toJson(*this);
};

Result<Branding> Branding::fromJSON(matjson::Value const& v) {
    return matjson::Serialize<Branding>::fromJson(v);
};

bool BrandingManager::doesBrandExist(std::string_view modId, bool checkLocal) const noexcept {
    if (auto it = m_brands.find(modId); it != m_brands.end()) return true;
    if (auto m = Mod::get()) return checkLocal && m->hasSavedValue(modId);

    return false;
};

void BrandingManager::registerBrand(std::string modId, std::string image, Type type) {
    auto b = Branding(
        std::move(image),
        modId,
        type);

    Mod::get()->setSavedValue<matjson::Value>(modId, b);

    if (doesBrandExist(modId)) {
        log::error("Could not register branding for {} because one already exists!", modId);
    } else {
        log::debug("Registered branding {} of type {} for {}", b.image, static_cast<int>(type), b.mod);
        m_brands[std::move(modId)] = std::move(b);
    };
};

Result<Branding> BrandingManager::getBrand(std::string_view modId) const {
    if (auto it = m_brands.find(modId); it != m_brands.end()) return Ok(it->second);
    if (Loader::get()->isModLoaded(std::string(modId)) && Mod::get()->hasSavedValue(modId)) return Ok(Mod::get()->getSavedValue<Branding>(modId, Branding("", std::string(modId))));

    return Err("Branding not found");
};

BrandingManager* BrandingManager::get() {
    static BrandingManager inst;
    return &inst;
};

Result<> branding::registerBrand(std::string modId, std::string image, Type type) {
    if (auto bm = BrandingManager::get()) {
        bm->registerBrand(std::move(modId), std::move(image), type);
        return Ok();
    };

    return Err("BrandingManager not found");
};