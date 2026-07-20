#include <gba_video.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include <gba_input.h>

#define RGB15(r,g,b) ((r)|((g)<<5)|((b)<<10))

u16* videoBuffer = (u16*)0x6000000;

void plotPixel(int x, int y, u16 color)
{
    videoBuffer[y * 240 + x] = color;
}

void drawRect(int x, int y, int w, int h, u16 color)
{
    for(int py = y; py < y + h; py++)
    {
        for(int px = x; px < x + w; px++)
        {
            plotPixel(px, py, color);
        }
    }
}

int main(void)
{
    irqInit();
    irqEnable(IRQ_VBLANK);

    REG_DISPCNT = MODE_3 | BG2_ENABLE;

    // Fill screen black once
    drawRect(0, 0, 240, 160, RGB15(0,0,0));

    // Draw blue border once
    drawRect(0, 0, 240, 4, RGB15(0,0,31));
    drawRect(0, 156, 240, 4, RGB15(0,0,31));
    drawRect(0, 0, 4, 160, RGB15(0,0,31));
    drawRect(236, 0, 4, 160, RGB15(0,0,31));

    int playerX = 120;
    int playerY = 80;

    int oldX = playerX;
    int oldY = playerY;

    // Draw player initially
    drawRect(playerX, playerY, 8, 8, RGB15(31,31,31));

    while(1)
    {
        VBlankIntrWait();

        scanKeys();
        u16 keys = keysHeld();

        oldX = playerX;
        oldY = playerY;

        if((keys & KEY_LEFT) && playerX > 5)
            playerX--;

        if((keys & KEY_RIGHT) && playerX < 227)
            playerX++;

        if((keys & KEY_UP) && playerY > 5)
            playerY--;

        if((keys & KEY_DOWN) && playerY < 147)
            playerY++;

        // Only redraw if moved
        if(oldX != playerX || oldY != playerY)
        {
            // Erase old player
            drawRect(oldX, oldY, 8, 8, RGB15(0,0,0));

            // Redraw border parts if player crossed them
            drawRect(0, 0, 240, 4, RGB15(0,0,31));
            drawRect(0, 156, 240, 4, RGB15(0,0,31));
            drawRect(0, 0, 4, 160, RGB15(0,0,31));
            drawRect(236, 0, 4, 160, RGB15(0,0,31));

            // Draw new player
            drawRect(playerX, playerY, 8, 8, RGB15(31,31,31));
        }
    }

    return 0;
}