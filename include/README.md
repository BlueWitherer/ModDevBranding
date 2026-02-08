# [<img src="../logo.png" width="25" alt="The mod's logo." />](https://www.geode-sdk.org/mods/cheeseworks.moddevbranding) Mod Developer Branding
Developer branding on mod info popups.

## Development
Let's start off by adding this mod as a dependency in your `mod.json`!
```jsonc
"dependencies": {
    "cheeseworks.moddevbranding": {
        "importance": "suggested",
        "version": ">=1.0.0"
    }
}
```

You can directly access the Mod Developer Branding optional API by including the [`OptionalAPI.hpp`](OptionalAPI.hpp) file in your code.
```cpp
#include <cheeseworks.moddevbranding/include/OptionalAPI.hpp>
```

> [!WARNING]
> Avoid declaring `using namespace branding;` when working with the optional API.

### Classes
Classes and methods to keep in mind while using the API.

#### inline `Result<>` `branding::registerBrand(std::string modId, std::string image, Type type = Type::Sprite)`: Register your very own branding image to appear on your mod! Internally checks for duplicate mod entries
- `std::string` **`modId`**: ID of the mod to apply branding on
- `std::string` **`image`**: Sprite name, sheet frame name, or URL of the image to use as branding on this mod
- `Type` **`type`**: Whether you're using a sprite, spritesheet frame, or external URL as the source of your branding

#### enum class `branding::Type`
An enum class that defines the type of image source for your branding. Alias for `branding::BrandImageType`.

#### Summary
| Type                    | Name              | Description                                |
| ----------------------- | ----------------- | ------------------------------------------ |
| `inline` **`Result<>`** | `registerBrand()` | Register your developer branding           |
| `enum class`            | `Type`            | Determines the expected source of an image |

### Branding
You can register a brand for your mod through this optional API.

> [!IMPORTANT]
> Don't forget to include your brand sprite in your mod's resources and configuration!
> 
> ```jsonc
> "resources": {
>     "sprites": [
>         "resources/my-brand.png"
>     ]
> }
> ```

#### Registering
To register your very own fabulous branding for your mod, you can call **`branding::registerBrand`** inside an `$on_game(Loaded)` block.

*Required fields are, in order: `modId` and `image`. Optional field is `type`.*

```cpp
$on_game(Loaded){
    auto res = branding::registerBrand(
        "me.mymod",
        "my-brand.png"_spr
    ); // Register a sprite image as your branding

    if (res.isErr()) log::error("couldn't load my branding: {}", res.unwrapErr());
};
```

You can include the optional field **`type`** as well! You can set it to `Type::URL` if you want to include an external image URL as the image source for your branding. With `Type::Sprite`, you can provide a separate sprite image as your mod branding, and `Type::SpriteFrame` if you're providing a sprite that is part of a spritesheet.

```cpp
$on_game(Loaded){
    auto res = branding::registerBrand(
        "me.mymod",
        "my-brand.png"_spr,
        branding::Type::SpriteFrame
    ); // Register a spritesheet frame as your branding

    if (res.isErr()) log::error("couldn't load my branding: {}", res.unwrapErr());
};
```

> [!NOTE]
> *With this method, your branding will **only** load and save once your mod is installed and then loaded once. To cover all of your mods with a base branding image before users decide to download your mods, head to **[moddev.cheeseworks.gay](https://moddev.cheeseworks.gay/)** to upload your universal mod developer branding.*

Congrats! Your branding image will now load on your mod info popup once the user chooses to use your mod.

*Happy modding!*