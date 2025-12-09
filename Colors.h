#ifndef COLORS_H
#define COLORS_H

#include <string>
#include <vector>

// ANSI Color Codes for Terminal Output
namespace Colors {
    // Reset
    constexpr const char* RESET = "\033[0m";
    
    // Text Colors
    constexpr const char* BLACK = "\033[30m";
    constexpr const char* RED = "\033[31m";
    constexpr const char* GREEN = "\033[32m";
    constexpr const char* YELLOW = "\033[33m";
    constexpr const char* BLUE = "\033[34m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* CYAN = "\033[36m";
    constexpr const char* WHITE = "\033[37m";
    
    // Bright/Bold Colors
    constexpr const char* BRIGHT_RED = "\033[1;31m";
    constexpr const char* BRIGHT_GREEN = "\033[1;32m";
    constexpr const char* BRIGHT_YELLOW = "\033[1;33m";
    constexpr const char* BRIGHT_BLUE = "\033[1;34m";
    constexpr const char* BRIGHT_MAGENTA = "\033[1;35m";
    constexpr const char* BRIGHT_CYAN = "\033[1;36m";
    constexpr const char* BRIGHT_WHITE = "\033[1;37m";
    
    // Dim/Dark Colors
    constexpr const char* DARK_GRAY = "\033[2;37m";
    constexpr const char* GRAY = "\033[90m";
    
    // Background Colors
    constexpr const char* BG_RED = "\033[41m";
    constexpr const char* BG_GREEN = "\033[42m";
    constexpr const char* BG_YELLOW = "\033[43m";
    constexpr const char* BG_BLUE = "\033[44m";
    constexpr const char* BG_MAGENTA = "\033[45m";
    constexpr const char* BG_CYAN = "\033[46m";
    
    // Text Styles
    constexpr const char* BOLD = "\033[1m";
    constexpr const char* UNDERLINE = "\033[4m";
    constexpr const char* BLINK = "\033[5m";
    
    // Emojis
    namespace Emoji {
        constexpr const char* WARRIOR = "🗡️ ";
        constexpr const char* MAGE = "🧙";
        constexpr const char* ARCHER = "🏹";
        constexpr const char* HEALTH = "❤️ ";
        constexpr const char* MANA = "💙";
        constexpr const char* SWORD = "⚔️ ";
        constexpr const char* SHIELD = "🛡️ ";
        constexpr const char* GOLD = "💰";
        constexpr const char* EXPERIENCE = "⭐";
        constexpr const char* LEVEL_UP = "🎆";
        constexpr const char* VICTORY = "🏆";
        constexpr const char* DEFEAT = "💀";
        constexpr const char* FIRE = "🔥";
        constexpr const char* FROST = "❄️ ";
        constexpr const char* POISON = "☠️ ";
        constexpr const char* GRASS = "🌿";
        constexpr const char* FOREST = "🌲";
        constexpr const char* MOUNTAIN = "⛰️ ";
        constexpr const char* WATER = "💧";
        constexpr const char* DESERT = "🏜️ ";
        constexpr const char* DUNGEON = "🏚️ ";
        constexpr const char* CASTLE = "🏰";
        constexpr const char* TOWN = "🏘️ ";
        constexpr const char* SHOP = "🏪";
        constexpr const char* PLAYER = "🧑";
        constexpr const char* ENEMY = "👹";
        constexpr const char* GOBLIN = "👺";
        constexpr const char* TROLL = "🪨";
        constexpr const char* BANDIT = "🗡️ ";
        constexpr const char* BOSS = "👹";
        constexpr const char* POTION = "🧪";
        constexpr const char* SCROLL = "📜";
        constexpr const char* CHEST = "💎";
        constexpr const char* MAP = "🗺️ ";
        constexpr const char* MOVE_UP = "⬆️ ";
        constexpr const char* MOVE_DOWN = "⬇️ ";
        constexpr const char* MOVE_LEFT = "⬅️ ";
        constexpr const char* MOVE_RIGHT = "➡️ ";
        constexpr const char* ATTACK = "💥";
        constexpr const char* DEFEND = "🛡️ ";
        constexpr const char* ITEM = "🎒";
        constexpr const char* INFO = "ℹ️ ";
        constexpr const char* MENU = "📋";
        constexpr const char* QUESTION = "❓";
        constexpr const char* CHECK = "✅";
        constexpr const char* CROSS = "❌";
        constexpr const char* ARROW_RIGHT = "➜ ";
    }
    
    // Utility functions
    std::string colorize(const std::string& text, const char* color);
    std::string healthBar(int current, int max, int width = 20);
    std::string manaBar(int current, int max, int width = 20);
    void clearScreen();
    void printTitle(const std::string& title);
    void printMenu(const std::string& title, const std::vector<std::string>& options);
    
    // Animation functions
    void typewriter(const std::string& text, int delayMs = 30);
    void animateLoading(const std::string& message, int durationMs = 1000);
    void animateAttack(const std::string& attacker, const std::string& target, int damage);
    void animateHeal(const std::string& name, int amount);
    void animateLevelUp(int newLevel);
    void animateVictory();
    void animateDefeat();
    void flashText(const std::string& text, const char* color, int times = 3);
    void progressBar(const std::string& label, int current, int max, int width = 30);
    void delay(int milliseconds);
    
    // Story intro animation
    void playIntroStory(const std::string& playerName, const std::string& playerClass);
}

#endif
