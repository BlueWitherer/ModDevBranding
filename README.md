# [<img src="logo.png" width="25" alt="The mod's logo." />](https://www.geode-sdk.org/mods/cheeseworks.moddevbranding) Mod Developer Branding
Developer branding on mod info popups.

> [<img alt="YouTube" src="https://img.shields.io/youtube/channel/subscribers/UCi2M6N_ff1UC6MyfWzKQvgg?style=for-the-badge&logo=youtube&logoColor=ffffff&label=YouTube">](https://www.youtube.com/@cheese_works/) [<img alt="Bluesky" src="https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Fpublic.api.bsky.app%2Fxrpc%2Fapp.bsky.actor.getProfile%2F%3Factor%3Dcheeseworks.gay&query=%24.followersCount&style=for-the-badge&logo=bluesky&logoColor=ffffff&label=Bluesky">](https://bsky.app/profile/cheeseworks.gay) [<img alt="Discord" src="https://img.shields.io/discord/460081436637134859?style=for-the-badge&logo=discord&logoColor=ffffff&label=Discord">](https://dsc.gg/cubic)

> [<img alt="Latest Version" src="https://img.shields.io/github/v/release/BlueWitherer/ModDevBranding?include_prereleases&sort=semver&display_name=release&style=for-the-badge&logo=github&logoColor=ffffff&label=Version">](../../releases/) [<img alt="Code License" src="https://img.shields.io/github/license/BlueWitherer/ModDevBranding?style=for-the-badge&logo=gnu&logoColor=ffffff&label=License">](LICENSE.md)
>  
> [<img alt="Downloads" src="https://img.shields.io/github/downloads/BlueWitherer/ModDevBranding/total?style=for-the-badge&logo=geode&logoColor=ffffff&label=Downloads">](https://www.geode-sdk.org/mods/cheeseworks.moddevbranding)

> [!TIP]
> *This mod has settings you can utilize to customize your experience.*

---

## About
This mod adds a branding image from the main developer of a mod onto its mod information popup.

---

### Submit Branding
If you're a mod developer, visit **[moddev.cheeseworks.gay](https://moddev.cheeseworks.gay/)** to access the dashboard to submit your branding! It will automatically appear on all of your mods. You will be required to authorize with your [GitHub account](https://www.github.com/login/) to use the website.

> [!NOTE]
> *If you're not a **verified developer** on Geode, your branding will be put under a brief review process by our staff!*

---

### Developers
Want to add branding for your mod specifically? You can register your very own branding by using this mod's optional API! You can see its [documentation here](./include).

```cpp
using namespace branding;

$execute {
    (void)[&]()->Result<> {
        GEODE_UNWRAP(BrandingManagerV2::registerBrand(
            "me.mymod",
            "my-sprite.png"_spr,
            BrandImageType::SpriteFrame
        ));
        return Ok();
    }();
};
```

---

### Credits
- **[ArcticWoof](https://www.github.com/DumbCaveSpider/)**: Designed branding dashboard website frontend
- **[Alphalaneous](https://www.github.com/Alphalaneous/)**: Original [Mod Developer Branding](https://www.github.com/Alphalaneous/ModDevBranding/) project

---

### Changelog
###### What's new?!
**[📜 View the latest updates and patches](./changelog.md)**

### Issues
###### What's wrong?!
**[⚠️ Report a problem with the mod](../../issues/)**