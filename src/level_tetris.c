#include "level_tetris.h"
#include "frameratecontroller.h"
#include "graphicsmanager.h"
#include "inputmanager.h"
#include <stdio.h>
#include "defs.h"
#include "string.h"
#include "vector.h"

static const int GRID_HEIGHT = 18;
static const int GRID_WIDTH = 10;

static u8 block_grid[18][10];

static vec2_t single_block;
static vec2_t starting_pos;

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
    || ( block_grid[y][x] ) )
        return 1;
        
    return 0;
}

static void Load( void )
{
    printf("Load::level_tetris\n");
    Vec2Set( starting_pos, (GRID_WIDTH/2 - 1), (GRID_HEIGHT - 1) );
}

static void Init( void )
{
    // clear block grid
    memset( block_grid, 0, sizeof(block_grid) );
    
    // set single_block at the starting position
    Vec2Copy( single_block, starting_pos );
    
    // add some starting blocks for debugging
    u8 line3[10] = {0,0,0,0,0,0,0,0,1,0};
    u8 line2[10] = {0,0,0,0,0,1,0,0,1,0};
    u8 line1[10] = {0,0,1,0,1,1,1,1,1,0};
    u8 line0[10] = {0,1,1,1,1,0,1,1,1,0};
    memcpy( block_grid[3], line3, sizeof(line3) );
    memcpy( block_grid[2], line2, sizeof(line2) );
    memcpy( block_grid[1], line1, sizeof(line1) );
    memcpy( block_grid[0], line0, sizeof(line0) );
    
    printf("Init::level_tetris\n");
}

static void Update( void )
{    
    static vec2_t new_pos;
    
    printf("Update::level_tetris\n");
    printf("\tframe_time = %.04f sec\n\tlevel_time = %.04f sec\n", frame_time, level_time);
    
    // Left
    if( KeyTriggered( KEY_a ) )
    {
        Vec2Copy( new_pos, single_block );
        new_pos[0] -= 1;
        if( !collision( new_pos ) )
            Vec2Copy( single_block, new_pos );
    }
    
    // Right
    if( KeyTriggered( KEY_d ) )
    {
        Vec2Copy( new_pos, single_block );
        new_pos[0] += 1;
        if( !collision( new_pos ) )
            Vec2Copy( single_block, new_pos );
    }
    
    // Down
    if( KeyTriggered( KEY_s ) )
    {
        Vec2Copy( new_pos, single_block );
        new_pos[1] -= 1;
        if( !collision( new_pos ) )
            Vec2Copy( single_block, new_pos );
    }
    
    // Up; debugging
    if( KeyTriggered( KEY_w ) )
    {
        Vec2Copy( new_pos, single_block );
        new_pos[1] += 1;
        if( !collision( new_pos ) )
            Vec2Copy( single_block, new_pos );
    }
    
    if( KeyTriggered( KEY_r ) )
        GS_next = GS_RESTART;
    
    // Quit if escape is pressed
    if( KeyTriggered( KEY_ESCAPE ) )
        GS_next = GS_QUIT;
}

static void Draw( void )
{
    int x, y;
    vec2_t pos;
    
    printf("Draw::level_tetris\n\n");
    
    // draw single block
    Graphics_SetColor( 0xff0000ff );
    Graphics_DrawBlock( single_block );
    
    // draw set blocks
    Graphics_SetColor( 0xff00ffff );
    for( y=0; y<GRID_HEIGHT; ++y )
        for( x=0; x<GRID_WIDTH; ++x )
            if( block_grid[y][x] )
            {
                Vec2Set( pos, x, y );
                Graphics_DrawBlock( pos );
            }
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

