#ifndef FONTENGINE_H
#define FONTENGINE_H

#include <string>
#include <unordered_map>

#include "includes.h"
#include "FontAtlas.h"
#include "Program.h"

using namespace std;

class FontEngine {
    public:
        FontEngine(int width, int height, Program *_prog);
        ~FontEngine();

        bool init(GLuint _ShadeProg);

        // load a font from fontFile and map it to given handle
        // returns whether or not font was successfully added
        bool addFont(std::string handle, std::string fontFile);

        // set font handle to be active font with given size
        // returns whether or not active font successfully set
        bool useFont(std::string handle, int size);

        // returns handle of the active font
        std::string getCurrentHandle();

        // returns current size of the active font
        int getCurrentSize();

        // set active color to render with
        void setColor(float r, float g, float b, float a);

        // set active z pos to render at
        void setZ(float z);

        // render text with currently active font at x,y [-1,1]
        void renderText(std::string text, float x, float y);
        void renderTextWrapped(std::string text, float x, float y, float width);

        // get line height of active font
        float getMaxCharHeight();
        float getLineHeight();
        void addLineHeight(float val);

        // get width of given text
        float getTextWidth(std::string text);
        float getTextHeight(std::string text);

        void updateWindowSize(int width, int height);
        
        void display(glm::vec4 col, const char* text, float x, float y);
    private:
        // was the engine intialized successfully?
        bool initialized;

        // modify scaling by aspect ratio and to
        // better match with expected scaling for font sizes
        void adjustScaling(float &sx, float &sy);

        FT_Library library;
        Program* prog;
    
        typedef struct Font {
            FT_Face face;
            std::string fontFile;

            // mapping of sizes to atlas's
            std::unordered_map<int, FontAtlas*> sizes;
        } Font;
       
        // mapping of handles to fonts
        std::unordered_map<std::string, Font*> fonts;

        // current font atlas to use for rendering
        FontAtlas *curAtlas;

        // handle of the current font
        std::string currentHandle;

        // current size of the active font
        int currentSize;

        // current color to use for rendering
        GLfloat color[4];

        // current zPos to use for rendering
        float zPos;

        struct Point {
            GLfloat x, y;
            GLfloat s, t;
        };

        // shader interface
        GLuint ShadeProg;
        GLint h_aCoord;
        GLint h_uTex;
        GLint h_uCol;
        GLuint vbo;

        int windowWidth, windowHeight;
        std::string ostrichHandle;
        std::string goodDogHandle;
        std::string seasideHandle;
};

extern FontEngine* fontEngine;

#endif
