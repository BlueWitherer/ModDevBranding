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

You can directly access the Mod Developer Branding optional API by including the [`OptionalAPI.hpp`](OptionalAPI.hpp) file in your code. Make sure to include the **`branding`** namespace to directly access all needed classes and methods.
```cpp
#include <cheeseworks.moddevbranding/include/OptionalAPI.hpp>

using namespace branding;
```

### Classes
Here are some important classes we highly suggest you keep in mind while working with the API.

#### class `branding::BrandingManagerV2`
The optional variant of manager class `BrandingManager`.
- `static Result<>` **`registerBrand(std::string modId, std::string image, BrandImageType type = BrandImageType::Sprite)`**: Register your very own branding image to appear on your mod! Internally checks for duplicate mod entries
  - `std::string` **`modId`**: ID of the mod to apply branding on
  - `std::string` **`image`**: Sprite name, sheet frame name, or URL of the image to use as branding on this mod
  - `BrandImageType` **`type`**: Whether you're using a sprite, spritesheet frame, or external URL as the source of your branding

#### enum class `branding::BrandImageType`
An enum class that defines the type of image source for your branding.

#### Summary
| Type         | Name                | Description                                |
| ------------ | ------------------- | ------------------------------------------ |
| `class`      | `BrandingManagerV2` | Manager for developer branding             |
| `enum class` | `BrandImageType`    | Determines the expected source of an image |

### Branding
You can register a brand for your mod through this optional API.

> [!IMPORTANT]
> Don't forget to include your brand sprite in your mod's resources and configuration!
> 
> ```jsonc
> "resources": {
>     "sprites": [
>         "resources/*.png"
>     ]
> }
> ```

#### Registering
To register your very own fabulous branding for your mod, you can call **`BrandingManagerV2::registerBrand`** inside an `$on_game(Loaded)` block.

*Required fields are, in order: `modId` and `image`. Optional field is `type`.*

```cpp
$on_game(Loaded){
    auto res = BrandingManagerV2::registerBrand(
        "me.mymod",
        "my-sprite-image.png"_spr
    ); // Register a sprite image as your branding

    if (res.isErr()) log::error("couldn't load my branding: {}", res.unwrapErr());
};
```

You can include the optional field **`type`** as well! You can set it to `BrandImageType::URL` if you want to include an external image URL as the image source for your branding. With `BrandImageType::Sprite`, you can provide a separate sprite image as your mod branding, and `BrandImageType::SpriteFrame` if you're providing a sprite that is part of a spritesheet.

```cpp
$on_game(Loaded){
    auto res = BrandingManagerV2::registerBrand(
        "me.mymod",
        "my-sprite-frame.png"_spr,
        BrandImageType::SpriteFrame
    );

    if (res.isErr()) log::error("couldn't load my branding: {}", res.unwrapErr());
};
```

> [!NOTE]
> *With this method, your branding will **only** load and save once your mod is installed and loaded. To cover all of your mods with a base branding image before users decide to download your mods, head to **[moddev.cheeseworks.gay](https://moddev.cheeseworks.gay/)** to upload your universal mod developer branding.*

Congrats! Your branding image will now load on your mod info popup once the user chooses to use your mod.

*Happy modding!*