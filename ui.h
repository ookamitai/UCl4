#ifndef _UI_H_
#define _UI_H_
#include <conio.h>
#include <algorithm>
#include <cmath>
#include <fstream>

#include "project.h"
#include "screen.h"

/**
 * @brief 彩色文字支持
 */
typedef struct ColorText {
    std::string prefix;
    std::string content;

    std::vector<Character> output() const {
        std::vector<Character> ret = std::vector<Character>(content.length());
        for (size_t i = 0; i < content.length(); i++)
            ret[i] = Character(content[i], prefix);
        return ret;
    }

    ColorText() = default;

    ColorText(const std::string &content, const std::string &prefix)
        : prefix(prefix), content(content) {}
} ColorText;

/**
 * @brief 获得窗口大小
 *
 * @return Coord x和y，代表窗口大小
 */
Coord getsize() {
    Coord ret = Coord(0, 0);
    std::cout << "\x1b[s\x1b[9999;9999H\x1b[6n\x1b[u";
    _getch();
    _getch();
    for (char ch; (ch = _getch()) != ';'; ret.y = ret.y * 10 + (ch - '0'))
        ;
    for (char ch; (ch = _getch()) != 'R'; ret.x = ret.x * 10 + (ch - '0'))
        ;
    return ret;
}

typedef struct UI {
    /**
     * @brief 显示 log。
     *
     * @param text 文本。
     */
    void render_log(const std::vector<Character> &text) {
        size_t x = 0;
        for (; x < screen->size().x; x++) {
            screen->set(Coord(x, screen->size().y - 1), Character(0));
        }
        x = 0;
        for (auto &&s : text) {
            screen->set(Coord(x, screen->size().y - 1), s);
            x++;
        }
    }
    /**
     * @brief 返回一个新的 UI 界面作为子 UI。
     */
    template<typename T>
    T sub_ui() {
      return T(screen);
    }
    /**
     * @brief 更新画面。
     */
    void update() { screen->show(); }

    /**
     * @brief 清屏。
     */
    void clear() { screen->clear(); }

    /**
     * @brief 获得窗口的大小。
     *
     * @return const Coord& 对窗口大小的只读引用。
     */
    const Coord &size() const noexcept { return screen->size(); }

    explicit UI(Screen *screen) : screen(screen) {}

   private:
    void render_text(size_t *x, size_t y, const ColorText &text) {
        for (auto &&s : text.output()) {
            screen->set(Coord(*x, y), s);
            (*x)++;
        }
    }

    static std::string truncate_zero(std::string x) {
      while (*x.crbegin() == '0')
        x.erase(x.length() - 1);
      return x;
    }

    Screen *screen;
} UI;

typedef struct SelectUI {
    /**
     * @brief 更新画面。
     */
    void update() { screen->show(); }

    /**
     * @brief 清屏。
     */
    void clear() { screen->clear(); }

    /**
     * @brief 获得窗口的大小。
     *
     * @return const Coord& 对窗口大小的只读引用。
     */
    const Coord &size() const noexcept { return screen->size(); }

    explicit SelectUI(Screen *screen) : screen(screen) {}
private:
    
    void render_text(size_t *x, size_t y, const ColorText &text) {
        for (auto &&s : text.output()) {
            screen->set(Coord(*x, y), s);
            (*x)++;
        }
    }
  Screen* screen;
} SelectUI;
#endif
