# Mod Developer Branding
**Developer branding on mod info popups.**

*by [Cheeseworks](user:6408873)!*

> ![✳️](frame:collaborationIcon_001.png) <cg>*This mod has settings you can utilize to customize your experience.*</c>

---

## About
This mod adds a snazzy branding image from the main developer of a mod or the mod itself onto its information popup.

---

### Submit Branding
**If you're a <cg>mod developer</c>**, you may go to **[moddev.cheeseworks.gay](https://moddev.cheeseworks.gay/)** to access <cg>the dashboard to submit your branding</c>! It will <cy>automatically appear on *all* of your mods</c>. <cr>You will be required to authorize with your [GitHub account](https://www.github.com/login/) to use the website.</c>

<cr>If you have no Geode index history, your submission may either be postponed in review or rejected.</c>

### Previewing
You can use the <cg>**`Test Brand Image`** setting</c> to <cy>preview your branding in-game</c> to see how it looks like before submitting it through the dashboard or applying it through this mod's API!

> ![✳️](frame:collaborationIcon_001.png) <cg>*Recommended image resolution and ratio is **`903`x`777`** in `px` to best fit in the branding UI container. Looks best when expanding from the bottom-right corner outwards!*</c>

---

### Developers
Want to add branding for your mod specifically? You can register your very own branding with your mod by using this mod's optional API! You can see its [documentation here](https://www.github.com/BlueWitherer/ModDevBranding/tree/master/include/). Add some more style to your mods!

```cpp
using namespace branding;

$on_game(Loaded){
    auto res = BrandingManagerV2::registerBrand(
        "awesomeguy.myepicmod",
        "my-beautiful-sprite.png"_spr,
        BrandImageType::SpriteFrame
    );

    if (res.isErr()) log::error("couldn't load my mod branding: {}", res.unwrapErr());
};
```

---

### Credits
- **[ArcticWoof](user:7689052)**: Designed branding dashboard website frontend
- **[Alphalaneous](user:1139015)**: Original [Mod Developer Branding](https://www.github.com/Alphalaneous/ModDevBranding/) project