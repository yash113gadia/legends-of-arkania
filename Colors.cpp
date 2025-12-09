#include "Colors.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

namespace Colors {
    
    // Helper function for delays
    void delay(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
    
    std::string colorize(const std::string& text, const char* color) {
        return std::string(color) + text + RESET;
    }
    
    std::string healthBar(int current, int max, int width) {
        if (max <= 0) max = 1;
        int filled = (current * width) / max;
        filled = (filled < 0) ? 0 : (filled > width) ? width : filled;
        
        std::stringstream ss;
        ss << Colors::RED << "[";
        
        // Filled portion in red
        for (int i = 0; i < filled; i++) {
            ss << "█";
        }
        
        // Empty portion
        for (int i = filled; i < width; i++) {
            ss << "░";
        }
        
        ss << "]" << Colors::RESET;
        return ss.str();
    }
    
    std::string manaBar(int current, int max, int width) {
        if (max <= 0) max = 1;
        int filled = (current * width) / max;
        filled = (filled < 0) ? 0 : (filled > width) ? width : filled;
        
        std::stringstream ss;
        ss << Colors::BLUE << "[";
        
        // Filled portion in blue
        for (int i = 0; i < filled; i++) {
            ss << "█";
        }
        
        // Empty portion
        for (int i = filled; i < width; i++) {
            ss << "░";
        }
        
        ss << "]" << Colors::RESET;
        return ss.str();
    }
    
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }
    
    void printTitle(const std::string& title) {
        std::cout << "\n" << Colors::BRIGHT_CYAN;
        std::cout << "╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║ " << std::string(title.length() < 58 ? (58 - title.length()) / 2 : 0, ' ');
        std::cout << Colors::BRIGHT_YELLOW << title << Colors::BRIGHT_CYAN;
        std::cout << std::string(title.length() < 58 ? (58 - title.length()) - (58 - title.length()) / 2 : 0, ' ') << " ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝" << Colors::RESET << "\n\n";
    }
    
    void printMenu(const std::string& title, const std::vector<std::string>& options) {
        std::cout << "\n" << Colors::BRIGHT_GREEN;
        std::cout << "┌─ " << title << " " << std::string(title.length() > 40 ? 0 : 40 - title.length(), '-') << "┐\n";
        std::cout << Colors::RESET;
        
        for (const auto& option : options) {
            std::cout << Colors::CYAN << "│ " << Colors::WHITE << option << Colors::CYAN << "\n";
        }
        
        std::cout << Colors::BRIGHT_GREEN << "└" << std::string(62, '-') << "┘" << Colors::RESET << "\n\n";
    }
    
    // Typewriter effect - prints text character by character
    void typewriter(const std::string& text, int delayMs) {
        for (char c : text) {
            std::cout << c << std::flush;
            delay(delayMs);
        }
    }
    
    // Animated loading with spinning character
    void animateLoading(const std::string& message, int durationMs) {
        const char* spinner[] = {"|", "/", "-", "\\"};
        int frames = durationMs / 100;
        
        for (int i = 0; i < frames; i++) {
            std::cout << "\r" << Colors::BRIGHT_CYAN << spinner[i % 4] << " " << message << "   " << std::flush;
            delay(100);
        }
        std::cout << "\r" << Colors::BRIGHT_GREEN << "✓ " << message << " Done!   \n" << Colors::RESET;
    }
    
    // Battle attack animation
    void animateAttack(const std::string& attacker, const std::string& target, int damage) {
        std::cout << "\n";
        
        // Wind-up
        std::cout << Colors::BRIGHT_YELLOW << attacker << " prepares to strike";
        for (int i = 0; i < 3; i++) {
            std::cout << "." << std::flush;
            delay(200);
        }
        std::cout << "\n";
        
        // Impact animation
        const std::string impacts[] = {"💥", "⚡", "✨", "💢"};
        std::cout << Colors::BRIGHT_RED;
        for (int i = 0; i < 3; i++) {
            std::cout << "\r  " << impacts[i % 4] << " *SLASH* " << impacts[(i+1) % 4] << "  " << std::flush;
            delay(150);
        }
        
        // Damage reveal
        std::cout << "\n" << Colors::BRIGHT_WHITE << "  ➤ " << target << " takes " 
                  << Colors::BRIGHT_RED << damage << Colors::BRIGHT_WHITE << " damage! 💔\n" << Colors::RESET;
        delay(300);
    }
    
    // Healing animation
    void animateHeal(const std::string& name, int amount) {
        std::cout << "\n" << Colors::BRIGHT_GREEN;
        
        // Sparkle effect
        const std::string sparkles[] = {"✨", "💚", "✨", "💖"};
        for (int i = 0; i < 4; i++) {
            std::cout << "\r  " << sparkles[i] << " Healing energy surrounds " << name << " " << sparkles[i] << "  " << std::flush;
            delay(200);
        }
        
        std::cout << "\n  ➤ " << name << " recovers " << Colors::BRIGHT_GREEN << "+" << amount 
                  << " HP" << Colors::RESET << "! 💚\n";
        delay(300);
    }
    
    // Level up animation
    void animateLevelUp(int newLevel) {
        std::cout << "\n";
        
        // Flash effect
        for (int i = 0; i < 3; i++) {
            std::cout << "\r" << Colors::BRIGHT_YELLOW << "  🎆 ★ LEVEL UP! ★ 🎆  " << std::flush;
            delay(200);
            std::cout << "\r" << Colors::BRIGHT_CYAN << "  🎆 ★ LEVEL UP! ★ 🎆  " << std::flush;
            delay(200);
        }
        
        std::cout << "\n\n" << Colors::BRIGHT_YELLOW;
        std::cout << "  ╔═══════════════════════════════╗\n";
        std::cout << "  ║  🎉 CONGRATULATIONS! 🎉       ║\n";
        std::cout << "  ║  You are now Level " << std::setw(2) << newLevel << "!        ║\n";
        std::cout << "  ╚═══════════════════════════════╝\n" << Colors::RESET;
        delay(500);
    }
    
    // Victory animation
    void animateVictory() {
        std::cout << "\n";
        
        // Fireworks effect
        const std::string fireworks[] = {"🎆", "🎇", "✨", "🌟", "⭐"};
        for (int round = 0; round < 2; round++) {
            for (int i = 0; i < 5; i++) {
                std::cout << "\r  ";
                for (int j = 0; j < 5; j++) {
                    std::cout << fireworks[(i + j) % 5] << " ";
                }
                std::cout << std::flush;
                delay(150);
            }
        }
        
        std::cout << "\n\n" << Colors::BRIGHT_GREEN;
        std::cout << "  ╔═══════════════════════════════════════╗\n";
        std::cout << "  ║       🏆 VICTORY! 🏆                  ║\n";
        std::cout << "  ║   The enemy has been defeated!       ║\n";
        std::cout << "  ╚═══════════════════════════════════════╝\n" << Colors::RESET;
    }
    
    // Defeat animation
    void animateDefeat() {
        std::cout << "\n" << Colors::BRIGHT_RED;
        
        // Fade effect
        for (int i = 0; i < 3; i++) {
            std::cout << "\r  💀 You have fallen... 💀  " << std::flush;
            delay(300);
            std::cout << "\r                              " << std::flush;
            delay(200);
        }
        
        std::cout << "\n\n";
        std::cout << "  ╔═══════════════════════════════════════╗\n";
        std::cout << "  ║         💀 DEFEAT 💀                  ║\n";
        std::cout << "  ║   Your journey ends here...          ║\n";
        std::cout << "  ╚═══════════════════════════════════════╝\n" << Colors::RESET;
    }
    
    // Flash text with color
    void flashText(const std::string& text, const char* color, int times) {
        for (int i = 0; i < times; i++) {
            std::cout << "\r" << color << text << Colors::RESET << std::flush;
            delay(200);
            std::cout << "\r" << std::string(text.length() + 10, ' ') << std::flush;
            delay(100);
        }
        std::cout << "\r" << color << text << Colors::RESET << "\n";
    }
    
    // Animated progress bar
    void progressBar(const std::string& label, int current, int max, int width) {
        if (max <= 0) max = 1;
        
        for (int progress = 0; progress <= current; progress += std::max(1, current / 20)) {
            int filled = (progress * width) / max;
            filled = (filled < 0) ? 0 : (filled > width) ? width : filled;
            
            std::cout << "\r" << Colors::BRIGHT_CYAN << label << " [";
            std::cout << Colors::BRIGHT_GREEN;
            for (int i = 0; i < filled; i++) std::cout << "█";
            std::cout << Colors::GRAY;
            for (int i = filled; i < width; i++) std::cout << "░";
            std::cout << Colors::BRIGHT_CYAN << "] " << (progress * 100 / max) << "%" << std::flush;
            
            delay(30);
        }
        
        // Final state
        int filled = (current * width) / max;
        std::cout << "\r" << Colors::BRIGHT_CYAN << label << " [";
        std::cout << Colors::BRIGHT_GREEN;
        for (int i = 0; i < filled; i++) std::cout << "█";
        std::cout << Colors::GRAY;
        for (int i = filled; i < width; i++) std::cout << "░";
        std::cout << Colors::BRIGHT_CYAN << "] " << (current * 100 / max) << "%" << Colors::RESET << "\n";
    }
    
    // Animated story intro
    void playIntroStory(const std::string& playerName, const std::string& playerClass) {
        clearScreen();
        delay(500);
        
        // Scene 1: The peaceful kingdom
        std::cout << Colors::BRIGHT_CYAN;
        std::cout << "\n\n";
        std::cout << "    ╔═══════════════════════════════════════════════════════════╗\n";
        std::cout << "    ║                                                           ║\n";
        std::cout << "    ║               " << Colors::BRIGHT_YELLOW << "☀️  THE KINGDOM OF ARKANIA  ☀️" << Colors::BRIGHT_CYAN << "              ║\n";
        std::cout << "    ║                                                           ║\n";
        std::cout << "    ╚═══════════════════════════════════════════════════════════╝\n\n";
        std::cout << Colors::RESET;
        delay(1500);
        
        std::cout << Colors::WHITE;
        typewriter("    Long ago, the Kingdom of Arkania flourished under the light\n", 35);
        typewriter("    of the ", 35);
        std::cout << Colors::BRIGHT_YELLOW << "Crystal of Dawn" << Colors::WHITE;
        typewriter(" — a magical artifact that brought\n", 35);
        typewriter("    peace and prosperity to all who lived within its glow...\n\n", 35);
        delay(1000);
        
        // Scene 2: The darkness rises
        std::cout << Colors::BRIGHT_RED;
        std::cout << "\n    ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n";
        std::cout << Colors::RESET;
        delay(800);
        
        std::cout << Colors::RED;
        typewriter("\n    But darkness came...\n\n", 50);
        delay(500);
        
        std::cout << Colors::WHITE;
        typewriter("    The ", 35);
        std::cout << Colors::BRIGHT_RED << "Dark Lord Malachar" << Colors::WHITE;
        typewriter(" rose from the shadows of the\n", 35);
        typewriter("    ", 35);
        std::cout << Colors::MAGENTA << "Dark Citadel" << Colors::WHITE;
        typewriter(". With his army of monsters, he shattered\n", 35);
        typewriter("    the Crystal and plunged the land into chaos.\n\n", 35);
        delay(1000);
        
        // Scene 3: The hero's call
        std::cout << Colors::BRIGHT_MAGENTA;
        std::cout << "\n    ✦ ══════════════════════════════════════════════════════ ✦\n\n";
        std::cout << Colors::RESET;
        delay(600);
        
        std::cout << Colors::BRIGHT_CYAN;
        typewriter("    Now, a hero emerges...\n\n", 40);
        delay(500);
        
        std::cout << Colors::BRIGHT_GREEN << "    ⚔️  ";
        std::cout << Colors::BRIGHT_WHITE << playerName << Colors::GREEN;
        typewriter(", a brave ", 30);
        std::cout << Colors::BRIGHT_YELLOW << playerClass << Colors::GREEN;
        typewriter(",\n", 30);
        typewriter("        has answered the call to adventure!\n\n", 35);
        delay(800);
        
        // Scene 4: The quest
        std::cout << Colors::BRIGHT_YELLOW;
        std::cout << "\n    ╭───────────────────────────────────────────────────────────╮\n";
        std::cout << "    │                    " << Colors::BRIGHT_WHITE << "⚡ YOUR QUEST ⚡" << Colors::BRIGHT_YELLOW << "                      │\n";
        std::cout << "    ╰───────────────────────────────────────────────────────────╯\n\n";
        std::cout << Colors::RESET;
        delay(500);
        
        std::cout << Colors::WHITE;
        typewriter("    🗺️  Explore the four regions of Arkania:\n", 30);
        delay(300);
        std::cout << Colors::GREEN;
        typewriter("        🌲 Verdant Woods - Where your journey begins\n", 25);
        std::cout << Colors::YELLOW;
        typewriter("        🏜️  Scorched Dunes - The unforgiving desert\n", 25);
        std::cout << Colors::CYAN;
        typewriter("        ❄️  Frost Peaks - The frozen mountains\n", 25);
        std::cout << Colors::RED;
        typewriter("        🏰 Dark Citadel - Malachar's fortress\n\n", 25);
        delay(600);
        
        std::cout << Colors::BRIGHT_WHITE;
        typewriter("    💎 Collect the ", 30);
        std::cout << Colors::BRIGHT_CYAN << "Crystal Shards" << Colors::BRIGHT_WHITE;
        typewriter(" scattered across the land.\n", 30);
        typewriter("    ⚔️  Defeat the Dark Lord and restore peace to Arkania!\n\n", 30);
        delay(1000);
        
        // Final transition
        std::cout << Colors::BRIGHT_MAGENTA;
        std::cout << "\n    ════════════════════════════════════════════════════════════\n";
        std::cout << Colors::RESET;
        delay(500);
        
        std::cout << Colors::BRIGHT_GREEN;
        typewriter("\n    Your legend begins now...\n", 50);
        delay(800);
        
        std::cout << Colors::BRIGHT_YELLOW;
        typewriter("\n    Press ENTER to begin your adventure... ", 30);
        std::cout << Colors::RESET;
        
        // Wait for enter
        std::string dummy;
        std::getline(std::cin, dummy);
        
        clearScreen();
    }
}
