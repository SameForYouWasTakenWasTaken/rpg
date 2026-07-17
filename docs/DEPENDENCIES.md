# 📦 Dependencies

All third-party libraries are listed below. They are **not** installed by hand.
`dependencies.cmake` (in the project root) fetches them with CMake `FetchContent`
when they are not already found on your system.

| Library | Version | Role in the project | Link |
| --- | --- | --- | --- |
| **SFML** | `3.1.0` | Window, input, and low-level graphics (textures, vertices, views). | <https://github.com/SFML/SFML> |
| **GLM** | `1.0.1` | Math types (`vec2`, `vec3`) used for positions, scale, sizes. | <https://github.com/g-truc/glm> |
| **EnTT** | `v3.13.2` | Entity-Component-System: the `entt::registry` and `entt::dispatcher` (event bus). | <https://github.com/skypjack/entt> |
| **nlohmann/json** | `v3.12.0` | Parses the texture-atlas `.json` file (sprite sheet frames). | <https://github.com/nlohmann/json> |

---

## 🔧 How they are set up

Each library has a small `setup_*` function in `dependencies.cmake`:

1. `setup_sfml()` — looks for `SFML 3` (Graphics, Window, System). If missing, fetches tag `3.1.0`.
2. `setup_glm()` — looks for `glm`. If missing, fetches tag `1.0.1`.
3. `setup_entt()` — looks for `EnTT`. If missing, fetches tag `v3.13.2`.
4. `setup_nlohmann_json()` — looks for `nlohmann_json`. If missing, fetches tag `v3.12.0`.

When found locally, the code creates the matching CMake target alias
(`glm::glm`, `EnTT::EnTT`, `nlohmann_json::nlohmann_json`) so the rest of the
build works the same way.

The targets linked into the final executable (`src/CMakeLists.txt`) are:

- `SFML::Graphics`
- `SFML::Window`
- `SFML::System`
- `glm::glm`
- `EnTT::EnTT`

> 💡 `nlohmann_json` is header-only and is consumed through its include path
> inside `Atlas.cpp`; it does not need to be linked explicitly.

---

## 🌐 Where to find them

- SFML docs: <https://www.sfml-dev.org/documentation/3.0.0/>
- GLM docs: <https://glm.g-truc.net/0.9.9/index.html>
- EnTT docs: <https://skypjack.github.io/entt/>
- nlohmann/json docs: <https://json.nlohmann.me/>

---

## ⬆️ Updating a version

To bump a dependency, edit the `GIT_TAG` inside the matching `setup_*` function
in `dependencies.cmake`, then delete your `build/` folder and reconfigure.
