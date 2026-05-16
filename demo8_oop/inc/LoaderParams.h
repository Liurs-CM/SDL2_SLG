#ifndef __LoaderParams__
#define __LoaderParams__
#include <cstdint>
#include <string>

class LoaderParams
{
    public:
        LoaderParams(int x, int y, int width, int height, std::string textureID, uint8_t numFrames, int callbackID = 0, uint8_t animSpeed = 4) : 
            m_x(x), 
            m_y(y), 
            m_width(width), 
            m_height(height), 
            m_textureID(textureID),
            m_numFrames(numFrames),
            m_callbackID(callbackID),
            m_animSpeed(animSpeed) 
    { }
        int getX() const { return m_x; }
        int getY() const { return m_y; }
        uint16_t getWidth() const { return m_width; }
        uint16_t getHeight() const { return m_height; }
        uint8_t getAnimSpeed() const { return m_animSpeed; }
        int getCallbackID() const { return m_callbackID; }
        uint8_t getNumFrames() const { return m_numFrames; }
        std::string getTextureID() const { return m_textureID; }
    private:
        int m_x;
        int m_y;
        uint16_t m_width;
        uint16_t m_height;
        std::string m_textureID;
        uint8_t m_numFrames;
        int m_callbackID;
        uint8_t m_animSpeed;
};

#endif /* defined(__LoaderParams__) */
