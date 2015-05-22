#include "Text.h"

Text::Text(Program* prog, int width, int height) {
    fontEngine = new FontEngine(width, height);
    
    fontEngine->init(prog->getPID());
    
    ostrichHandle = "Fonts/ostrich-regular.ttf";
}

Text::~Text() {
   delete fontEngine;
}

int Text::init() {
    if (!fontEngine->addFont(ostrichHandle, ostrichHandle)) {
        return 0;
    }

    return 1;
}

void Text::display(Program* prog, glm::vec4 col, int size, const char* text) {
    prog->bind();

    fontEngine->setColor(col.r, col.g, col.b, col.a);
    if (fontEngine->useFont(ostrichHandle, size)) {
        float textWidth, yPos = 0.1;
        yPos -= fontEngine->getLineHeight();
        std::string aligned = text;
        textWidth = fontEngine->getTextWidth(aligned);
        fontEngine->renderText(aligned, 0 - textWidth / 2.0, yPos); //center of the screen
    }
    
    prog->unbind();
}
