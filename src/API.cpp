#define GEODE_DEFINE_EVENT_EXPORTS
#include <API.h>
#include <OptionalAPI.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace cw::brand;

Branding::Branding(std::string i, std::string m, Type t) : image(std::move(i)), mod(std::move(m)), type(t) {};

matjson::Value Branding::toJSON() const {
    return matjson::makeObject({
        {"image", image},
        {"mod", mod},
        {"type", static_cast<int>(type)},
    });
};

Result<Branding> Branding::fromJSON(matjson::Value const& v) {
    if (!v.isObject()) return Err("JSON value is not an object");

    GEODE_UNWRAP_INTO(std::string image, v["image"].asString());
    GEODE_UNWRAP_INTO(std::string mod, v["mod"].asString());
    GEODE_UNWRAP_INTO(unsigned int type, v["type"].asUInt());

    return Ok(Branding(
        std::move(image),
        std::move(mod),
        static_cast<Type>(type)));
};

bool BrandingManager::doesBrandExist(std::string_view modId, bool checkLocal) const noexcept {
    if (auto it = m_brands.find(modId); it != m_brands.end()) return true;
    if (auto m = Mod::get()) return checkLocal && m->hasSavedValue(modId);

    return false;
};

void BrandingManager::registerBrand(std::string modId, std::string image, BrandImageType type) {
    auto b = Branding(
        std::move(image),
        modId,
        type);

    Mod::get()->setSavedValue<matjson::Value>(modId, b.toJSON());

    if (doesBrandExist(b.mod)) {
        log::error("Could not register branding for {} because one already exists!", b.mod);
    } else {
        log::debug("Registered branding {} of type {} for {}", b.image, static_cast<int>(b.type), b.mod);
        m_brands[std::move(modId)] = std::move(b);
    };
};

Result<Branding> BrandingManager::getBrand(std::string_view modId) const {
    if (auto it = m_brands.find(modId); it != m_brands.end()) return Ok(it->second);
    if (Loader::get()->isModLoaded(std::string(modId)) && Mod::get()->hasSavedValue(modId)) return Branding::fromJSON(Mod::get()->getSavedValue<matjson::Value>(modId, Branding("", std::string(modId)).toJSON()));

    return Err("Branding not found");
};

BrandingManager* BrandingManager::get() {
    static auto inst = new (std::nothrow) BrandingManager();
    return inst;
};

Result<> branding::registerBrand(std::string modId, std::string image, Type type) {
    if (auto bm = BrandingManager::get()) {
        bm->registerBrand(std::move(modId), std::move(image), type);
        return Ok();
    };

    return Err("BrandingManager not found");
};