#ifndef _EDITOR_H
#define _EDITOR_H

#include "ui.h"
#include "project.h"

typedef struct Editor {
    
    bool process(UI *ui, int input) {
    
    }

    bool dirty() const noexcept { return _dirty; }

    Editor():
        _dirty(false) {}

private:
    bool _dirty;

} Editor;

#endif  //_EDITOR_H
