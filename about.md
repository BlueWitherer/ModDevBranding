# Mod Developer Branding
**Developer branding on mod info popups.**

*by [Cheeseworks](user:6408873)!*

> ![✳️](frame:collaborationIcon_001.png) <cg>*This mod has settings you can utilize to customize your experience.*</c>

---

## About
This mod adds a snazzy branding image from the main developer of a mod or the mod itself onto its information popup.

---

### Submit Branding
If you're a <cg>mod developer</c>, head on over to **[moddev.cheeseworks.gay](https://moddev.cheeseworks.gay/)** to access <cg>the dashboard to submit your branding</c>! It will <cy>automatically appear on *all* of your mods</c>.

<cr>You will be required to authorize with your [GitHub account](https://www.github.com/login/) to use the website.</c>

> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cj>*If you're not a **verified developer** on Geode, your branding will be put under a brief review process by our staff.*</c>

### Previewing
You can use the **`Test Brand Image`** setting to preview your branding in-game to see how it looks like before submitting it through the dashboard or applying it through this mod's API!

> ![✳️](frame:collaborationIcon_001.png) <cg>*Recommended image resolution and ratio is `903px`x`777px` to properly fit branding UI container. Looks best when expanding from the bottom-right corner outwards!*</c>

---

### Developers
Want to add branding for your mod specifically? You can register your very own branding with your mod by using this mod's optional API! You can see its [documentation here](https://www.github.com/BlueWitherer/ModDevBranding/tree/master/include/). Add some more style to your mods!

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
- **[ArcticWoof](user:7689052)**: Designed branding dashboard website frontend
- **[Alphalaneous](user:1139015)**: Original [Mod Developer Branding](https://www.github.com/Alphalaneous/ModDevBranding/) project