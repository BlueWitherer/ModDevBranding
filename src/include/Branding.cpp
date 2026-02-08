#define GEODE_DEFINE_EVENT_EXPORTS
#include <Branding.hpp>
#include <OptionalAPI.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace branding;

Branding::Branding(std::string i, std::string m, BrandImageType t)
    : image(std::move(i)), mod(std::move(m)), type(t) {};

matjson::Value Branding::toJSON() const {
    return matjson::makeObject({
        { "image", image },
        { "mod", mod },
        { "type", static_cast<int>(type) },
                               });
};

Result<Branding> Branding::fromJSON(matjson::Value const& v) {
    if (v.isNull()) return Err("JSON value is null");

    if (v["image"].isNull()) return Err("Image value is missing");
    if (v["mod"].isNull()) return Err("Mod value is missing");
    if (v["type"].isNull()) return Err("Type value is missing");

    return Ok(Branding(
        v["image"].asString().unwrapOr(""),
        v["mod"].asString().unwrapOr(""),
        static_cast<BrandImageType>(v["type"].asInt().unwrapOr(0))
    ));
};

std::span<const Branding> BrandingManager::getBrands() const noexcept {
    return m_brands;
};

bool BrandingManager::doesBrandExist(std::string_view modId, bool checkLocal) const noexcept {
    for (auto const& brand : getBrands()) if (brand.mod == modId) return true;
    if (auto m = Mod::get()) return checkLocal && m->hasSavedValue(modId);

    return false;
};

void BrandingManager::registerBrand(std::string modId, std::string image, BrandImageType type) {
    auto b = Branding(
        std::move(image),
        std::move(modId),
        type
    );

    Mod::get()->setSavedValue<matjson::Value>(modId, b.toJSON());

    if (doesBrandExist(b.mod)) {
        log::error("Could not register branding for {} because one already exists!", b.mod);
    } else {
        log::debug("Registered branding {} of type {} for {}", b.image, static_cast<int>(b.type), b.mod);
        m_brands.push_back(std::move(b));
    };
};

Result<Branding> BrandingManager::getBrand(std::string_view modId) const {
    for (auto const& b : getBrands()) if (b.mod == modId) return Ok(b);
    if (Loader::get()->isModInstalled(std::string(modId)) && Mod::get()->hasSavedValue(modId)) return Branding::fromJSON(Mod::get()->getSavedValue<matjson::Value>(modId, Branding("", std::string(modId)).toJSON()));

    return Err("Branding not found");
};

BrandingManager* BrandingManager::get() {
    static auto inst = new (std::nothrow) BrandingManager();
    return inst;
};

Result<> branding::registerBrand(std::string modId, std::string image, BrandImageType type) {
    if (auto bm = BrandingManager::get()) {
        bm->registerBrand(std::move(modId), std::move(image), type);
        return Ok();
    };

    return Err("BrandingManager not found");
};