#include "level_tetris.h"
#include "frameratecontroller.h"
#include "graphicsmanager.h"
#include "inputmanager.h"
#include <stdio.h>
#include "defs.h"
#include "string.h"

static const int GRID_HEIGHT = 18;
static const int GRID_WIDTH = 10;

static u8 blocks[18][10];

static int collision( vec2_t pos )
{
    static int x;
    static int y;
    x = pos[0];
    y = pos[1];
    
    if( x < 0 
    || x >= GRID_WIDTH
    || y < 0
    || y >= GRID_HEIGHT
    || ( blocks[y][x] ) )
        return 1;
        
    return 0;
}

static void Load( void )
{
    printf("Load::level_tetris\n");
}

static void Init( void )
{
    // clear block grid
    memset( blocks, 0, sizeof(blocks) );
    
    printf("Init::level_tetris\n");
}

static void Update( void )
{    
    printf("Update::level_tetris\n");
    printf("\tframe_time = %.04f sec\n\tlevel_time = %.04f sec\n", frame_time, level_time);
       
    if( KeyTriggered( KEY_r ) )
        GS_next = GS_RESTART;
    
    // Quit if escape is pressed
    if( KeyTriggered( KEY_ESCAPE ) )
        GS_next = GS_QUIT;
}

static void Draw( void )
{
    printf("Draw::level_tetris\n\n");
    
}

static void Free( void )
{
    printf("Free::level_tetris\n");
}

static void Unload( void )
{
    printf("Unload::level_tetris\n");
}

// global accessor
const struct GameState GS_level_tetris = {Load, Init, Update, Draw, Free, Unload};

