// Created by LuoYu on 2026/6/11.
#pragma once

#include "manager.h"

#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <unordered_map>

enum class ResID {
    Tex_Tileset,                // 瓦片图集纹理

    Tex_Player,                 // 玩家序列帧

    Tex_Archer,                 // 弓箭手纹理
    Tex_Axeman,                 // 野蛮人纹理
    Tex_Gunner,                 // 枪炮手纹理

    Tex_Slim,                   // 史莱姆纹理
    Tex_KingSlim,               // 皇家史莱姆纹理
    Tex_Skeleton,               // 骷髅纹理
    Tex_Goblin,                 // 哥布林纹理
    Tex_GoblinPriest,           // 哥布林萨满纹理
    Tex_SlimSketch,             // 史莱姆剪影纹理
    Tex_KingSlimSketch,         // 皇家史莱姆剪影纹理
    Tex_SkeletonSketch,         // 骷髅剪影纹理
    Tex_GoblinSketch,           // 哥布林剪影纹理
    Tex_GoblinPriestSketch,     // 哥布林萨满纹理

    Tex_BulletArrow,            // 弓箭子弹纹理
    Tex_BulletAxe,              // 斧头子弹纹理
    Tex_BulletShell,            // 炮弹子弹纹理

    Tex_Coin,                   // 金币纹理
    Tex_Home,                   // 房子纹理

    Tex_EffectFlash_Up,         // 玩家攻击特效(向上)纹理
    Tex_EffectFlash_Down,       // 玩家攻击特效(向下)纹理
    Tex_EffectFlash_Left,       // 玩家攻击特效(向左)纹理
    Tex_EffectFlash_Right,      // 玩家攻击特效(向右)纹理

    Tex_EffectImpact_Up,        // 玩家技能特效(向上)纹理
    Tex_EffectImpact_Down,      // 玩家技能特效(向下)纹理
    Tex_EffectImpact_Left,      // 玩家技能特效(向左)纹理
    Tex_EffectImpact_Right,     // 玩家技能特效(向右)纹理

    Tex_EffectExplode,          // 炮弹爆炸特效纹理

    Tex_UI_SelectCursor,        // 轮盘选择UI纹理
    Tex_UI_PlaceIdle,           // 未选择轮盘UI纹理
    Tex_UI_PlaceHoveredTop,     // 轮盘上方高亮UI纹理
    Tex_UI_PlaceHoveredLeft,    // 轮盘左下高亮UI纹理
    Tex_UI_PlaceHoveredRight,   // 轮盘右下高亮UI纹理
    Tex_UI_UpgradeIdle,         // 未选择升级轮盘UI纹理
    Tex_UI_UpgradeHoveredTop,   // 轮盘上方升级高亮UI纹理
    Tex_UI_UpgradeHoveredLeft,  // 轮盘左下升级高亮UI纹理
    Tex_UI_UpgradeHoveredRight, // 轮盘右下升级高亮UI纹理
    Tex_UI_HomeAvatar,          // 房屋头像UI纹理
    Tex_UI_PlayerAvatar,        // 玩家头像UI纹理
    Tex_UI_Heart,               // 血量UI纹理
    Tex_UI_Coin,                // 金币UI纹理
    Tex_UI_GameOverBar,         // 游戏结束遮罩条UI纹理
    Tex_UI_WinText,             // 游戏胜利文本UI纹理
    Tex_UI_LossText,            // 游戏失败文本UI纹理

    Sound_ArrowFire_1,          // 弓箭发射音效1
    Sound_ArrowFire_2,          // 弓箭发射音效2
    Sound_AxeFire,              // 斧头攻击音效
    Sound_ShellFire,            // 炮弹发射音效
    Sound_ArrowHit_1,           // 弓箭攻击音效1
    Sound_ArrowHit_2,           // 弓箭攻击音效2
    Sound_ArrowHit_3,           // 弓箭攻击音效3
    Sound_AxeHit_1,             // 斧头攻击音效1
    Sound_AxeHit_2,             // 斧头攻击音效2
    Sound_AxeHit_3,             // 斧头攻击音效3
    Sound_ShellHit,             // 炮弹攻击音效

    Sound_Flash,                // 玩家攻击音效
    Sound_Impact,               // 玩家技能音效

    Sound_Coin,                 // 金币音效
    Sound_HomeHurt,             // 房屋受伤音效
    Sound_PlaceTower,           // 防御塔放置音效
    Sound_TowerLevelUp,         // 防御塔升级音效

    Sound_Win,                  // 获胜音效
    Sound_Loss,                 // 失败音效

    Music_BGM,                  // 游戏背景音乐

    Font_Main                   // 字体
};

// 资源管理器
class ResourcesManager : public Manager<ResourcesManager> {
    friend class Manager;

public:
    using FontPool = std::unordered_map<ResID, TTF_Font*>;
    using SoundPool = std::unordered_map<ResID, Mix_Chunk*>;
    using MusicPool = std::unordered_map<ResID, Mix_Music*>;
    using TexturePool = std::unordered_map<ResID, SDL_Texture*>;

public:
    bool load_from_file(SDL_Renderer* renderer);

    const FontPool& get_font_pool();

    const SoundPool& get_sound_pool();

    const MusicPool& get_music_pool();

    const TexturePool& get_texture_pool();

protected:
    ResourcesManager() = default;
    ~ResourcesManager() = default;

private:
    FontPool font_pool;         // 字体池
    SoundPool sound_pool;       // 音效池
    MusicPool music_pool;       // 音乐池
    TexturePool texture_pool;   // 纹理池
};