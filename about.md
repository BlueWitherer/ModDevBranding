# ![💻](frame:GJ_viewLevelsBtn_001.png?scale=0.5) Mod Developer Branding
**Developer branding on mod info popups.**

*by [Cheeseworks](user:6408873)!*

> ![✳️](frame:collaborationIcon_001.png) <cg>*This mod has settings you can utilize to customize your experience.*</c>

---

## About
This mod adds a beautiful branding image from the main developer of a mod or the mod itself onto its information popup.

> ![ℹ️](frame:GJ_infoIcon_001.png?scale=0.5) <cj>*This is an **online** mod. **Check [status.cheeseworks.gay](https://status.cheeseworks.gay/)** before reporting any connectivity issues!*</c>

---

> ![❗](frame:geode.loader/info-alert.png?scale=0.375) <cr>*The next section is meant for **developers**. If you have **no Geode index mod development history**, your submission may either be **postponed in review** or **rejected**.*</c>

### Submit Branding
**If you're an <cg>*active mod developer*</c>**, you may go to **[moddev.cheeseworks.gay](https://moddev.cheeseworks.gay/)** to access <cg>the dashboard to submit your branding</c>! It will <cy>automatically appear on *all* of your mods</c>. <cr>You will be required to authorize with your [GitHub account](https://www.github.com/login/) to use the website.</c>

### Previewing
You can use the <cg>**`Test Brand Image`** setting</c> to <cy>preview your branding in-game</c> to see how it looks like before submitting it through the dashboard or applying it through this mod's API!

> ![✳️](frame:collaborationIcon_001.png) <cg>*Recommended image resolution and ratio is **`903`x`777`** in `px` to best fit in the branding UI container. Looks best when expanding from the bottom-right corner outwards!*</c>

---

### Developers
Want to add branding for your mod specifically? You can register your very own branding with your mod by using this mod's optional API! You can see its [documentation here](https://www.github.com/BlueWitherer/ModDevBranding/tree/master/include/). Add some more style to your mod pages!

```cpp
$on_game(ModsLoaded){
    (void)cw::brand::registerBrand(
        GEODE_MOD_ID,
        "my-beautiful-sprite.png"_spr,
        cw::brand::Type::SpriteFrame
    );
};
```

---

### Credits
- **[ArcticWoof](user:7689052)**: Designed branding dashboard website frontend

---

![🗨️](frame:gj_discordIcon_001.png?scale=0.375) **If you need help, join my [support Discord server](https://www.dsc.gg/cheeseworks) and ask! :)**