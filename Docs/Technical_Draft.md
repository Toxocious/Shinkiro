# Proposal: “Shinkiro” — A C++20 / OpenGL 3.3 Game Engine & Editor for a Pokémon‑like Experience

## 1) Vision & Tenets

**Vision.** Build a lean, modern, extensible 3D engine + editor that excels at stylized, creature‑collecting, tile‑centric RPGs. Target Windows first, with a clean modular architecture and in‑house DLLs that enable hot‑reload, plugin workflows, and long‑term maintainability.

**Design Tenets**

* **Editor‑first**: All runtime features are authorable in‑editor with immediate feedback.
* **Data‑driven**: Creatures, moves, encounters, items, dialogues, and tiles are declarative assets.
* **Small & sharp**: Prioritize features that matter for a Pokémon‑like (grid overworld, encounters, turn‑based battle runtime, stylized look) over AAA generalization.
* **Stable DLL seams**: Clear in‑house DLL boundaries with ABI‑aware interfaces, versioning, and hot‑reload.
* **Deterministic core**: Simulation remains deterministic for reproducible saves and combat.

## 2) Scope & Out‑of‑Scope

**In Scope**

* Windows (x64), C++20, OpenGL 3.3 Core, GLFW, GLAD, ImGui.
* Stylized forward renderer (toon/outline), shadow maps, skeletal animation, particles.
* [ASD](https://cloudsofeternity.com/engine/#/2025/asd-framework) scene runtime; grid navigation; navmesh for free‑movement zones.
* Asset pipeline (glTF2, PNG/TGA → GPU compressed), asset DB, bundles, zstd.
* Editor with docking, gizmos, asset browser, material graph, tile map editor, dialogue graph, battle designer, sequencer.
* Deterministic turn‑based battle runtime and encounter system.
* Save/load, replay, photo mode, localization, input rebinding.

**Out of Scope (initially)**

* Mobile/console, Vulkan/DirectX backends, compute shaders.
* Advanced PBR/IBL, clustered lighting, volumetrics, ray tracing.
* Networking / multiplayer.

## 3) Why OpenGL 3.3 + C++20

* **GL 3.3** is widely supported on PC and sufficient for a stylized look (toon shading, outlines, shadow maps, particles, instancing, post FX).
* **C++20** offers concepts, ranges, constexpr improvements, coroutines (optionally), strong typing, and modules potential (later) without sacrificing performance.

## 4) DLL Strategy (In‑House)

We intentionally separate responsibilities into DLLs to enable fast iteration, parallel development, and hot‑reloading.

**Planned DLLs**

* `Shinkiro.Core.dll` — Foundation types, logging, memory, job system, RTTI/Reflection, config.
* `Shinkiro.Platform.dll` — Windowing (GLFW), input, file system, timing, OS services.
* `Shinkiro.Math.dll` — Linear algebra, geometry, curves, noise.
* `Shinkiro.Renderer.dll` — OpenGL device, resource managers, materials, frame graph, post FX.
* `Shinkiro.Assets.dll` — Importers (glTF/Assimp), texture tools, compression (zstd), asset DB + packer.
* `Shinkiro.Audio.dll` — Audio device (OpenAL Soft or miniaudio), mixer, spatialization.
* `Shinkiro.Physics.dll` — Lightweight collisions (AABB/OBB), queries; optional integration with Jolt/Box2D.
* `Shinkiro.ASD.dll` — Entity component system, systems, serialization.
* `Shinkiro.UI.dll` — ImGui layer, debugging widgets, editor UI components.
* `Shinkiro.Editor.dll` — Host app logic, panels, tools, command system, prefab/pipeline tooling.
* `Shinkiro.Gameplay.dll` — Data schemas, state machines, cutscenes, quests, inventory.
* `Shinkiro.Battle.dll` — Deterministic turn‑based battle runtime (creatures, moves, status, AI).
* `Shinkiro.Scripting.dll` (optional) — Lua via sol2, or solely C++ gameplay for v1.

**ABI Boundaries**

* Pure abstract C++ interfaces + PIMPL, or extern "C" factories that return interface pointers.
* Don’t pass STL across DLL boundaries; use POD structs, spans, handles, and stable ABI types.
* Version every interface (semantic version, GUID). Central `PluginRegistry` mediates discovery.

## 5) Risks & Mitigations

* **C++ ABI drift** → Constrain toolchain (VS 2022 v143). Interfaces are versioned; no STL in APIs.
* **Editor/Runtime divergence** → Shared runtime DLLs; Play‑In‑Editor embeds same DLL set.
* **GL state hazards** → Strict RAII GL wrappers + debug groups; frame graph manages passes.
* **Performance regressions** → Job system, frame capture benchmarks, golden image tests.
* **Data bloat / slow loads** → Asset bundles with zstd, dependency pruning, texture atlases.

## 6) Success Criteria (MVP → Alpha → Beta)

* **MVP (12–16 weeks)**: Editor shell, scene viewport, [ASD](https://cloudsofeternity.com/engine/#/2025/asd-framework), GL renderer (toon + outline), shadowed directional light, skeletal/skin, particles, tile map authoring, dialogue tool, battle MVP (damage, status, turn order), asset pipeline + packer, hot‑reload materials/shaders, saving.
* **Alpha**: Content authoring loop is smooth; all tools stable; full game loop (overworld ↔ battle) playable; localization; photo mode.
* **Beta**: Performance pass, memory budgets locked; zero‑crash stability under burn‑in.

---

# System Design

## A) High‑Level Architecture

```
[Editor Host EXE]
   ├─ Shinkiro.Editor.dll (tools, panels, command bus)
   ├─ Shinkiro.UI.dll     (ImGui toolkit, docking, gizmos)
   ├─ Shinkiro.Core.dll   (memory, logging, jobs, reflection)
   ├─ Shinkiro.Platform.dll (window, input, FS)
   ├─ Shinkiro.Renderer.dll (GL device, frame graph, materials)
   ├─ Shinkiro.ASD.dll    (entities, components, systems)
   ├─ Shinkiro.Assets.dll (importers, DB, packer, VFS)
   ├─ Shinkiro.Audio.dll  (audio device/mixer)
   ├─ Shinkiro.Gameplay.dll (overworld runtime, cutscenes)
   ├─ Shinkiro.Battle.dll (battle-based runtime using Showdown Simulator)
   └─ Plugins/*.dll      (optional: custom importers, tools)
```

**Play‑In‑Editor** embeds the same DLLs used by the standalone **Game Player EXE**, guaranteeing parity.

## B) Core Foundations (Shinkiro.Core.dll)

* **Memory**: Linear arenas for frame allocations; pool allocators for components; small‑vector.
* **Jobs**: Work‑stealing thread pool; fiber‑friendly tasks (optional) for streaming.
* **Time**: Fixed‑step simulation (e.g., 60 Hz) with accumulator; variable‑rate rendering.
* **Reflection/RTTI**: Macro‑assisted registration to expose components/properties to editor and serializer.
* **Math**: Column‑major matrices, RH coordinate system, units wrappers (Degrees/Radians), SIMD.
* **Handles**: 64‑bit opaque IDs (index|generation) for cross‑DLL object references.

## C) Rendering (Shinkiro.Renderer.dll)

**Goals**: Stylized forward renderer tuned for GL 3.3, fast on mid‑range laptops; consistent toon aesthetic.

* **Device Layer**: RAII wrappers for VAO/VBO/IBO/UBO/SSBO (where available), FBO, textures, shaders; automatic state tracking; debug markers via `KHR_debug`.
* **Material System**: Lightweight graph compiled to GLSL 330; parameters uniform‑buffer packed; hot‑reload on file change.
* **Frame Graph**: Declarative passes (shadow → main → post FX). Resource aliasing; automatic FBO setup.
* **Lighting**: Directional + N point/spot (forward). For style, limit to few dynamic lights.
* **Toon Pipeline**:

  * Cel‑shaded BRDF (ramped/stepped N·L), artistic shadows.
  * Sobel/normal‑based outline pass (screen‑space) and/or vertex‑normal expansion.
  * Dithering for gradients to mimic hand‑drawn vibe.
* **Shadows**: Cascaded shadow maps (2–3 cascades) for directional sun; PCF filtering.
* **Animation**: GPU skinning (UBO bones up to N = 120). Optional CPU skinning for tools.
* **Particles**: Billboarded sprites; CPU simulate + GPU instancing; sub‑UV/flipbook; simple collisions.
* **Post**: Bloom, vignette, color‑grade (LUT), FXAA; configurable via material stack.
* **UI**: ImGui render path isolated from game passes; SRGB‑correct text.

## D) Scene & [ASD](https://cloudsofeternity.com/engine/#/2025/asd-framework) (Shinkiro.[ASD](https://cloudsofeternity.com/engine/#/2025/asd-framework).dll)

* **Layout**: SoA component storage; per‑component sparse set (index|generation handles).
* **Systems**: Update phases (PrePhysics → Logic → Animation → Audio → RenderPrep). Deterministic ordering.
* **Serialization**: Binary snapshot with versioned headers; JSON debug export; delta‑patch for diffs.
* **Prefabs**: Immutable base + override layers; resolve at spawn with copy‑on‑write.
* **Events**: Lock‑free MPSC buses for gameplay and editor notifications.

### Core Components (initial set)

* `Transform`, `Camera`, `Light`, `MeshRenderer`, `SkinnedMeshRenderer`, `Animator`
* `SpriteEmitter`, `AudioSource`, `RigidBody` (optional), `Collider` (AABB/OBB/Capsule)
* `GridAgent` (grid movement), `Interactable`, `TriggerVolume`, `Script` (if Lua enabled)

## E) Asset System (Shinkiro.Assets.dll)

* **Importers**: glTF 2.0 (preferred), FBX via Assimp (fallback), PNG/TGA/JPEG → mipmapped GPU formats.
* **Processing**: Mesh OPT (index reorder, vertex cache optimization), tangents (MikkTSpace), LOD gen.
* **Textures**: Atlas pipeline (tiles/UI), normal map packing; optional BC1‑5/ETC2 offline.
* **Asset DB**: Content‑addressable store (hash = key); metadata in SQLite with GUIDs and dependency edges.
* **Bundles**: `.jpak` files contain header, TOC, chunk table, and zstd‑compressed payloads. Streaming by chunk.
* **Hot‑Reload**: File watcher invalidates DB entries; renderer swaps resources next frame when safe.

**`.jpak` Header (v1)**

```
struct JpakHeader {
  char     magic[4];    // 'JPAK'
  uint16_t version;     // 1
  uint16_t flags;       // endian, compression mode
  uint32_t tocOffset;   // Table of contents
  uint32_t chunkOffset; // Data chunks start
  uint64_t buildId;     // monotonic build number
};
```

## F) Audio (Shinkiro.Audio.dll)

* **Backend**: OpenAL Soft or miniaudio for device abstraction.
* **Mixer**: Buses (SFX/Music/Voice/UI), DSP chain (EQ, reverb), snapshots per scene.
* **Assets**: OGG/FLAC import; streaming for music; random‑pitch SFX variations.

## G) Physics & Navigation (Shinkiro.Physics.dll)

* **Collisions**: Broadphase grid or SAP; narrowphase for primitives; triggers.
* **Grid Navigation**: 2D/3D grid for overworld tiles with height steps; A\* pathfinding; movement rules (ice, ledges, surf zones) authored in tile metadata.
* **NavMesh (free areas)**: Recast/Detour optional for interiors.

## H) Scripting (Shinkiro.Scripting.dll) — Optional for v1

* Lua runtime with sandboxes for tools (dialogue/battle scripting). Hot‑reload.
* Data remains declarative; scripts handle branching logic and one‑off interactions.

## I) Gameplay Framework (Shinkiro.Gameplay.dll)

* **State Machines**: Overworld, Menu, Battle, Cutscene. Explicit transitions; coroutine helpers for sequences.
* **Data Schemas**: Creatures, types, moves, status effects, items, trainers, encounters.
* **Triggers**: On‑enter tile, interact, collision, time‑of‑day, chance.
* **Camera**: Follow, rail, cinematic; collision‑aware framing.
* **Localization**: ICU‑based formatting; string tables with gender/plural rules.

## J) Battle Runtime (Shinkiro.Battle.dll)

* **Battle Logic**: Makes use of the [Pokemon Showdown](https://github.com/smogon/pokemon-showdown) battle simulator to handle most battle logic. Extensible if needed.
* **Deterministic**: Fixed‑step, fixed‑point RNG with seed persisted in save games.
* **Core Concepts**: Party, Creature, Stats, Types, Moves, Effects, Conditions, Weather, Terrain.
* **Turn System**: Speed‑based order; priority tiers; on‑enter/on‑leave hooks; stackable statuses.
* **Damage**: Final damage = Base × STAB × TypeEffect × Critical × Random × Buffs/Debuffs × Weather.
* **AI**: Utility scoring: move value − risk + goal progress; consider PP, state, opponent typing.
* **FX**: Camera shakes, particles, sprite/mesh overlays; authored in Battle Designer.

## K) Editor Architecture (Shinkiro.Editor.dll + Shinkiro.UI.dll)

* **Host**: Docking layout (Viewport, Hierarchy, Inspector, Console, Assets, Profiler, Sequencer, Battle Designer, Dialogue Graph, Tile Map, Material Graph).
* **Command System**: Undo/redo with command objects; scoped transactions with diffing for complex edits.
* **Gizmos**: Translate/Rotate/Scale; grid‑snapping; tile paint/erase/fill; collision painter.
* **Inspectors**: Auto‑generated from reflection; custom drawers for colors, curves, enums.
* **Sequencer**: Timeline for cutscenes (tracks: camera, animation, dialogue, script calls).
* **Dialogue Graph**: Nodes (Line, Choice, Condition, SetVar, Branch). Preview & localization export.
* **Battle Designer**: Creature/move editor; type charts; effect stacks; simulation sandbox.
* **Material Graph**: Node editor → GLSL codegen; preview sphere/mesh.
* **Profiler**: CPU (jobs, systems), GPU (timers via `ARB_timer_query`), memory, frame capture.
* **Play Modes**: In‑Editor (single‑process); Standalone (spawn Player EXE with same content build).

## L) Data & File Formats

* **Models/Anim**: glTF 2.0 preferred; animations split or embedded. Retargeting lists.
* **Textures/UI**: PNG/TGA source; atlas JSON describes regions and 9‑slice insets.
* **Gameplay**: JSON5 or YAML during authoring → compiled to FlatBuffers‑like binary for runtime.
* **Dialogue**: Graph JSON + string table; export to CSV/XLIFF for localization. Ideally uses i18n for multi-language support.
* **Saves**: Stored within a server database with integration via protobufs or something similar.

## M) Hot‑Reload & Live Editing

* **Shaders/Materials**: File watcher → recompile → swap pipeline.
* **Assets**: Import on save; patch instances in scene when safe.
* **Gameplay DLL**: `Shinkiro.Gameplay.dll` and `Shinkiro.Battle.dll` hot‑swapped via proxy interfaces; serialize ephemeral state on reload.

## N) Build, Toolchain, and CI

* **Build**: Premake5 config for all Shinkiro deps; MSVC v143; warning level `/W4` + `/WX` for core; sanitizers via clang‑cl job.
* **Deps**: Configured with Premake5 for GLFW, GLAD, ImGui, OpenAL/miniaudio, zstd, freetype, Assimp.
* **CI**: GitHub Actions/Azure Pipelines → build matrix (Debug/Release), artifact upload (Editor, Player, content bundles), unit + headless render tests.
* **Signing**: Code signing for shipped EXEs/DLLs.

## O) Testing Strategy

* **Unit Tests**: Math, serialization, [ASD](https://cloudsofeternity.com/engine/#/2025/asd-framework), battle calculations.
* **Golden Images**: Deterministic scenes rendered offscreen compared against baselines.
* **Soak Tests**: 12‑hour play loops, memory leak detection, file handle audits.
* **Content Validator**: Lint assets (tri count, skeleton limits, texture sizes, missing refs).

## P) Performance & Memory

* **Budgets (target mid‑range laptop)**: 1080p60, <10 ms render, <5 ms sim, <2 ms audio/IO.
* **Streaming**: Background IO threads; double‑buffered staging; texture residency budget.
* **Allocations**: Frame arenas, freelists for components, string interning in asset DB.

## Q) Security & IP Hygiene

* Only original art/data in source control; import proprietary assets through packer only.
* Editor watermarks for pre‑release; hashed runtime bundles; optional integrity check.

## R) Example API & Build Patterns

### Export Macros & Factory
Inclusion guarded with #ifndef #define #endif.

```cpp
// shinkiro_api.h
#if defined(_WIN32)
  #if defined(SHINKIRO_BUILD)
    #define SHINKIRO_API __declspec(dllexport)
  #else
    #define SHINKIRO_API __declspec(dllimport)
  #endif
#else
  #define SHINKIRO_API
#endif

#ifndef SHINKIRO_API
  #define SHINKIRO_API

struct Renderer; // ABI‑safe forward decl
struct RendererDesc { int width, height; bool vsync; };

extern "C" SHINKIRO_API Renderer* Shinkiro_CreateRenderer(const RendererDesc*);
extern "C" SHINKIRO_API void      Shinkiro_DestroyRenderer(Renderer*);

#endif
```

### ABI‑Safe Handles Across DLLs

```cpp
using Handle64 = uint64_t; // index|generation
struct MeshHandle { Handle64 value; };
```

### Editor Panel Registration (Plugin)

```cpp
struct IEditorPanel { virtual const char* Name() const = 0; virtual void Draw() = 0; protected: virtual ~IEditorPanel() = default; };
extern "C" SHINKIRO_API void RegisterEditorPanels(PluginRegistry& r);
```

## S) Milestones / Roadmap (Detail)

**Phase 0 — Bootstrap (2 weeks)**

* Premakee5 workspace; CI; `Shinkiro.Core/Platform/Renderer` stubs; window + GL context; ImGui dockspace; logging; job system skeleton.

**Phase 1 — Scenes & Rendering (4–5 weeks)**

* [ASD](https://cloudsofeternity.com/engine/#/2025/asd-framework), transforms, cameras, GL wrappers, material/shader hot‑reload, toon shading & outline pass, directional light + shadows, static mesh rendering, skybox.

**Phase 2 — Assets & Animation (3–4 weeks)**

* glTF import, skeletal animation GPU skinning, asset DB, packer (`.jpak`), zstd, live reimport.

**Phase 3 — Tools (4 weeks)**

* Hierarchy/Inspector/Console/Profiler; gizmos; tile map editor; material graph; dialogue graph; command/undo; save/load.

**Phase 4 — Gameplay (4 weeks)**

* Overworld state, grid agent, triggers, cutscene sequencer, localization, photo mode.

**Phase 5 — Battle Runtime (4 weeks)**

* Core battle loop, type chart, move/effect system, statuses, AI baseline, Battle Designer with sandbox.

**Phase 6 — Polish & Beta (ongoing)**

* Performance, memory budgets, UX polish, stability, authoring guides.

---

# Appendix

## A1) Coding Standards

* `east_const`, `gsl::span`‑like views internally; explicit lifetimes; `not_null` for pointers.
* No exceptions crossing DLL boundaries; error codes + `expected<T,E>` internally.
* `constexpr` where viable; `noexcept` on hot paths; insists on SRGB correctness.
* Class members follow `m_classMember` naming scheme.
* All headers must include `#ifndef #define SHINKIRO_<HEADER>_H #endif` guards.

## A2) Logging/Telemetry

* Channels (Core, Render, IO, Gameplay). Sinks: console, file, ring buffer.
* Editor overlay for warnings (missing textures, shader compile errors) with click‑through to asset.

## A3) Profiling

* CPU: Tracy or Remotery integration; GPU timers via `ARB_timer_query`; frame capture export JSON.

## A4) Save Game Schema (v1)

* Header (magic/version/build), RNG seed, world state hash, party data, location, time.
* Compressed with zstd; checksum; optional cloud sync later.

## A5) Determinism Notes

* Use fixed‑point or double‑checked float paths for battle math; isolate RNG; deterministic component update order; serialize and restore transient state for hot‑reload.

## A6) Editor UX Principles

* Single‑key repeatable actions (paint/erase/fill), sticky gizmos, snap defaults (grid on), gentle confirmations, always‑visible context help in status bar.

## A7) Extensibility Hooks

* Plugin SDK headers (interfaces + GUIDs). Plugins can add: importers, panels, components, systems, menu items, render passes. Safe sandbox via capability flags.

---

## J) Battle Runtime — Deep Dive (Deterministic, Data‑Driven) [Shinkiro.Battle.dll]

**Data Model (authoring → compiled)**

* `CreatureDef`: `{ guid, name, baseStats{hp,atk,def,spa,spd,spe}, types[2], learnset[], growthCurve, captureRate, expYield, abilities[], body{height,weight}, assetRefs }`
* `MoveDef`: `{ guid, name, type, category: Physical|Special|Status, power, accuracy, priority, pp, targets: Self|Single|AllOpponents|Allies, flags{contact,protectable,sheerForce,mirror}, effects[] }`
* `Effect`: `{ when: OnCast|OnHit|OnDamage|TurnStart|TurnEnd|OnSwitchIn|OnSwitchOut, apply: Op[], duration, stacking, dispellable, tags[] }`
* `StatusDef`: `{ guid, name, counters?, immunities[], tick: Op[], end: Op[] }`
* `TypeChart`: 18×18 table (float multipliers) + overrides.
* `BattleScene`: `{ weather, terrain, fieldStates[], ruleset }`

**Fixed‑Step Loop**

```
while (battle.active) {
  Phase::StartTurn();
  enqueue(Player/AI decisions);
  order = SortBy(Priority, Speed, tieBreakSeed);
  for (actor in order) { ExecuteAction(actor); if (battle.over) break; }
  Phase::EndTurn();
}
```

* **RNG**: Xoshiro128\*\* seeded from save; every RNG call is labeled (channel id) and recorded for replays. No `std::random` across DLLs.
* **Speed Calc**: `speed' = base * stageMult * itemMult * weatherMult * statusMult` (clamped). Tie‑break: deterministic coin using RNG channel `SpeedTie`.

**Effect VM (bytecode)**

* Tiny stack VM executes Ops (`AddStat`, `MulPower`, `ApplyStatus`, `If`, `RandomPct`, `Heal`, `SpawnVFX`, `CameraShake`, `PlaySFX`).
* Verified opcode set for determinism; no floating script callbacks in deterministic path.
* Authoring front‑end: node graph → bytecode with constant folding.

**Damage Pipeline**

```
base = Move.power * Attacker.atkStat / Defender.defStat
stab = (Move.type ∈ Attacker.types) ? 1.5 : 1.0
mod  = stab * TypeChart[Move.type][Defender.typeA] * ... * Weather * Crit * Random(0.85..1.00)
final = floor(base * mod * buffs)
```

* Crit rules configurable; random channel `DamageRoll` uses 16 fixed slots per hit for replay matching.

**Statuses & Stacking**

* Two layers: *Ailments* (sleep, burn, freeze) exclusive; *Volatile* stacks (e.g., Leech Seed, Confuse) keyed by tag.
* Each status owns a counter (turns) and can register hooks (e.g., `OnBeforeMove`).

**AI (Utility‑based)**

```
score(move) = E[damage] * w1 + coverage(move, opponent) * w2
            + disrupt(value) * w3 - risk(selfKO|disable) * w4
```

* Look‑ahead depth 1 (default), depth 2 for boss fights using beam search with pruning.
* Heuristics are data‑driven per ruleset; PP conservation modeled as future value.

**Testing**

* Golden vectors: CSV with (attacker, defender, move, seed → expected damage range).
* Replay lock‑step: serialize every decision and RNG draw, ensure checksum matches across builds.


* **Deterministic Loop**: Fixed‑step simulation, seeded RNG for reproducible results.
* **Core Data**: Creatures, stats, moves, types, effects, statuses, items.
* **Turn Order**: Sorted by speed and priority, tie‑breaks resolved via RNG channel.
* **Damage Formula**: Classic Pokémon‑like formula (STAB, type effectiveness, crits, randomness, buffs/debuffs).
* **Statuses**: Ailments (sleep, burn, freeze, paralysis) and volatile states (confusion, leech seed).
* **Effects**: Executed via bytecode VM with small deterministic op set (ApplyDamage, ApplyStatus, Heal, ModifyStat, TriggerAnim).
* **AI**: Utility‑based evaluation of moves/targets; depth‑1 look‑ahead, with optional depth‑2 for bosses.
* **Replay & Testing**: Serialize every action + RNG call; golden vector tests verify damage ranges.

---

## K) Editor Deep Dive (Shinkiro.Editor.dll + Shinkiro.UI.dll)

* **Tile Map Tool**: Multi‑layer editing (ground, decoration, collision, triggers). Supports paint, rectangle, flood fill, pattern brushes. Autotiling via bitmask rules.
* **Dialogue Graph**: Node‑based editor with `Line`, `Choice`, `Branch`, `SetVar`. Supports localization (ICU placeholders) and CSV export.
* **Battle Designer**: Creature/move editor, type chart visualizer, status/effect stack authoring, test sandbox.
* **Material Graph**: Node‑based graph compiled to GLSL 330. Hot‑reload, preview mesh, uniform packing.
* **Sequencer**: Timeline for cutscenes, tracks for camera, dialogue, animations.
* **Command System**: Undo/redo with transactions and autosave snapshots.
* **Profiler Panel**: CPU/GPU timeline integration, memory usage view.

---

## L) Asset & Bundle System (Shinkiro.Assets.dll)

* **Import Pipeline**: Decode → Optimize (meshopt, MikkTSpace) → Generate (LODs, mips) → Bake → Serialize.
* **Texture Policy**: Authoring in PNG/TGA → runtime BC1/BC3 (diffuse), BC5 (normals), BC4 (masks).
* **Bundle Format (.jpak)**:

  * Header with version, flags, offsets.
  * TOC with GUID, type, offsets, hash.
  * Data chunks (64KB aligned, zstd compressed).
* **Integrity**: xxHash64 per asset, overall bundle hash.
* **Hot‑Reload**: File watcher triggers reimport → updated asset swaps into runtime safely at frame boundary.

---

## M) Hot‑Reload Lifecycle

1. File change detected.
2. Background import/validation.
3. AssetDB update.
4. Renderer issues ReloadTicket.
5. Staging resource built.
6. Atomic swap at frame boundary.
7. Old resources deferred‑delete.

**Gameplay DLL reload**: Proxy saves component state → unload DLL → load new DLL → restore state.

---

## N) Build & CI

* **Premake5 Presets** for Debug/RelWithDebInfo builds.
* **Build-Utils.lua** dependencies: glfw3, glad, imgui, glm, assimp, meshoptimizer, zstd, freetype, openal-soft, stb, spdlog.
* **CI**: GitHub Actions → build matrix (Debug/Release), run unit + render tests, package Editor + Player EXEs + .jpak bundles, code‑sign artifacts.

---

## O) Performance Targets

* **CPU budgets**: Render ≤10 ms, Update ≤5 ms, IO ≤2 ms (target 1080p60).
* **GPU**: Instancing for props/particles, batched tile rendering, capped outline pass cost via downsample.
* **Tools**: Frame capture export (JSON), overdraw heatmap, material complexity debug view.

---

## P) Memory & Streaming

* **Texture Residency**: Cap (e.g., 1.5 GB). Mip streaming + LRU eviction.
* **Mesh Chunks**: ≤64k indices for cache friendliness.
* **Saves**: zstd‑compressed binary snapshots (<256 KB typical).

---

## Q) Stability & Security

* DLL signature verification + allow‑list for plugins.
* Crash dump generation and autosave recovery.
* Content validation pipeline for missing refs, bad assets, oversized textures.

---

## R) Example APIs

**Renderer Factory (DLL boundary)**

```cpp
extern "C" SHINKIRO_API Renderer* Shinkiro_CreateRenderer(const RendererDesc*);
extern "C" SHINKIRO_API void      Shinkiro_DestroyRenderer(Renderer*);
```

**[ASD](https://cloudsofeternity.com/engine/#/2025/asd-framework) Query Syntax**

```cpp
for (auto [e, transform, mesh] : world.view<Transform, MeshRenderer>())
    DrawMesh(transform, mesh);
```

**Frame Graph Pass**

```cpp
Graph g;
auto shadow = g.Add(PassDesc{"Shadow", ...});
auto main   = g.Add(PassDesc{"Main", ...});
g.SetOrder({shadow, main});
```

---

## S) Milestones & Deliverables

* **MVP**: Load sample town, grid movement, random encounters, battle execution, save/load, asset bundles, shader/material hot‑reload.
* **Alpha**: All editor tools stable (tile, dialogue, battle, material, sequencer). Playable loop (overworld ↔ battle ↔ save).
* **Beta**: Performance tuned to 1080p60, no crash defects, 24‑hour soak stability, asset authoring workflow complete.

---

## T) Detailed Data Schemas (Authoring → Binary)

**Authoring (JSON5)** kept human‑readable; build converts to packed binary tables with string‑ID interning.

* `creature.json`, `move.json`, `status.json`, `item.json`, `typechart.json`, `encounter_tables.json`.
* Binary format uses little‑endian, versioned headers, and offset tables. All cross‑refs by 64‑bit GUID.

---

## U) Shader Snippets (GLSL 330 Core)

**Toon Ramp**

```glsl
float nl = max(dot(normalize(n), normalize(l)), 0.0);
float stepCount = 3.0;
float ramp = floor(nl * stepCount) / (stepCount - 1.0);
vec3 color = mix(shadowColor, lightColor, ramp);
```

**Sobel Outline (post)**

```glsl
float e = length( texelFetch(normalTex, P+ivec2(1,0),0).xyz - texelFetch(normalTex, P+ivec2(-1,0),0).xyz )
        + length( texelFetch(normalTex, P+ivec2(0,1),0).xyz - texelFetch(normalTex, P+ivec2(0,-1),0).xyz );
float edge = smoothstep(th0, th1, e);
fragColor = mix(sceneColor, outlineColor, edge);
```

---

## V) DLL ABI & Versioning Guide

* **No STL across boundaries**. Use POD, spans, handles, `string_view`‑like with size+ptr.
* **Versioned interfaces**: `IID_Renderer_v1` GUID + semantic version. Factories accept `requestedIID`.
* **Ownership rules**: Creator destroys; custom allocators passed in descriptor for cross‑DLL free.
* **Error transport**: No exceptions across DLLs; return `Result<T, ErrorCode>`; `GetLastError()` style for details.

---

## W) Test Matrices

* Platforms: Win10/11, GPUs (Intel/AMD/NVIDIA), drivers (latest, −1).
* Content scales: town (small/med/large), battle FX (light/heavy), tile density (sparse/dense).
* Golden images for 6 key scenes × 3 quality tiers.

---

## X) Disaster Recovery & Backups

* Per‑project autosave ring (10 files); crash recovery loads latest.
* Asset DB snapshots daily; content packer writes atomic JPaks with temp → swap.

---

## Y) Team & RACI (example)

* **Rendering** (Owner: A) — R, **Editor** (Owner: B) — R, **Gameplay** (Owner: C) — R, **Battle** (Owner: D) — R.
* **Core/Platform** — Shared; **QA** — V\&V; **Build/CI** — DevOps.

---

## Z) Future Enhancements

* GPU particles (TF/SSBO) where GL permits; cascaded contact hardening shadows; scripting DSL for effects; console backends; asset diff/merge tool; photo mode exports with DOF/bokeh.
