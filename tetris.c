#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

int const BOARD_WIDTH = 18;
int const BOARD_HEIGHT = 25;
int const WINDOW_HEIGHT = 25;
int const WINDOW_WIDTH = 20; 

typedef enum {
    I_SHAPE,
    O_SHAPE,
    L_SHAPE,
    J_SHAPE,
    S_SHAPE,
    Z_SHAPE,
    T_SHAPE,
} ShapeType;

typedef struct {
    int x;
    int y;
} Block;

typedef struct {
    Block *blocks;
    ShapeType type;
} Shape;

void draw_borders() {
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        mvprintw(i, 0, "|");
        mvprintw(i, WINDOW_WIDTH, "|");
    }
}

void draw_shape(Shape *shape) {
    init_pair(I_SHAPE, COLOR_BLACK, COLOR_BLUE);
    init_pair(O_SHAPE, COLOR_BLACK, COLOR_CYAN);
    init_pair(L_SHAPE, COLOR_BLACK, COLOR_YELLOW);
    init_pair(J_SHAPE, COLOR_BLACK, COLOR_GREEN);
    init_pair(S_SHAPE, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(Z_SHAPE, COLOR_BLACK, COLOR_WHITE);
    init_pair(T_SHAPE, COLOR_BLACK, COLOR_RED);

    attron(COLOR_PAIR(shape->type));
    for (int i = 0; i < 4; i++) {
        mvprintw(shape->blocks[i].y, shape->blocks[i].x, " ");
    }
    attroff(COLOR_PAIR(shape->type));
}

void draw_board(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
    init_pair(I_SHAPE, COLOR_BLACK, COLOR_BLUE);
    init_pair(O_SHAPE, COLOR_BLACK, COLOR_CYAN);
    init_pair(L_SHAPE, COLOR_BLACK, COLOR_YELLOW);
    init_pair(J_SHAPE, COLOR_BLACK, COLOR_GREEN);
    init_pair(S_SHAPE, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(Z_SHAPE, COLOR_BLACK, COLOR_WHITE);
    init_pair(T_SHAPE, COLOR_BLACK, COLOR_RED);

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j] != -1) {
                attron(COLOR_PAIR(board[i][j]));
                mvprintw(i, j, " ");
                attroff(COLOR_PAIR(board[i][j]));
            }
        }
    }
}

Block * create_o_shape() {
    Block *blocks = malloc(4 * sizeof(Block));
    blocks[0].x = 9;
    blocks[0].y = 0;
    blocks[1].x = 9;
    blocks[1].y = 1;
    blocks[2].x = 10;
    blocks[2].y = 0;
    blocks[3].x = 10;
    blocks[3].y = 1;
    return blocks;
}

Block * create_i_shape() {
    Block *blocks = malloc(4 * sizeof(Block));
    blocks[0].x = 8;
    blocks[0].y = 0;
    blocks[1].x = 9;
    blocks[1].y = 0;
    blocks[2].x = 10;
    blocks[2].y = 0;
    blocks[3].x = 11;
    blocks[3].y = 0;
    return blocks;
}

Block * create_z_shape() {
    Block *blocks = malloc(4 * sizeof(Block));
    blocks[0].x = 9;
    blocks[0].y = 0;
    blocks[1].x = 10;
    blocks[1].y = 0;
    blocks[2].x = 10;
    blocks[2].y = 1;
    blocks[3].x = 11;
    blocks[3].y = 1;
    return blocks;
}

Block * create_s_shape() {
    Block *blocks = malloc(4 * sizeof(Block));
    blocks[0].x = 11;
    blocks[0].y = 0;
    blocks[1].x = 10;
    blocks[1].y = 0;
    blocks[2].x = 10;
    blocks[2].y = 1;
    blocks[3].x = 9;
    blocks[3].y = 1;
    return blocks;
}

Block * create_t_shape() {
    Block *blocks = malloc(4 * sizeof(Block));
    blocks[0].x = 10;
    blocks[0].y = 0;
    blocks[1].x = 9;
    blocks[1].y = 1;
    blocks[2].x = 10;
    blocks[2].y = 1;
    blocks[3].x = 11;
    blocks[3].y = 1;
    return blocks;
}

Block * create_j_shape() {
    Block *blocks = malloc(4 * sizeof(Block));
    blocks[0].x = 9;
    blocks[0].y = 0;
    blocks[1].x = 9;
    blocks[1].y = 1;
    blocks[2].x = 10;
    blocks[2].y = 1;
    blocks[3].x = 11;
    blocks[3].y = 1;
    return blocks;
}

Block * create_l_shape() {
    Block *blocks = malloc(4 * sizeof(Block));
    blocks[0].x = 11;
    blocks[0].y = 0;
    blocks[1].x = 9;
    blocks[1].y = 1;
    blocks[2].x = 10;
    blocks[2].y = 1;
    blocks[3].x = 11;
    blocks[3].y = 1;
    return blocks;
}

Shape * create_shape(ShapeType shape_type) {
    Shape *shape = malloc(sizeof(Shape) + 4 * sizeof(Block));
    Block *blocks; 
    switch (shape_type) {
        case O_SHAPE:
            blocks = create_o_shape();
            break;
        case I_SHAPE:
            blocks = create_i_shape();
            break;
        case Z_SHAPE:
            blocks = create_z_shape();
            break;
        case S_SHAPE:
            blocks = create_s_shape();
            break;
        case T_SHAPE:
            blocks = create_t_shape();
            break;
        case J_SHAPE:
            blocks = create_j_shape();
            break;
        case L_SHAPE:
            blocks = create_l_shape();
            break;
    }
    shape->type = shape_type; 
    shape->blocks = blocks;
    return shape;
}

void add_shape(int board[BOARD_HEIGHT][BOARD_WIDTH], Shape *shape) {
    for (int i = 0; i < 4; i++) {
        board[shape->blocks[i].y][shape->blocks[i].x] = shape->type;
    }
}

void tick(Shape *shape) {
    for (int i = 0; i < 4; i++) {
        shape->blocks[i].y++;
    }
}

void erase_shape(Shape *shape) {
    for (int i = 0; i < 4; i++) {
        mvprintw(shape->blocks[i].y, shape->blocks[i].x, " ");
    }

}

bool downwards_collision(Shape *shape, int board[BOARD_HEIGHT][BOARD_WIDTH]) {
    Block *blocks = shape->blocks;

    // Collision with the bottom screen border
    for (int i = 0; i < 4; i++) {
        if (blocks[i].y + 1 >= WINDOW_HEIGHT) {
            return true;
        }
    } 

    // Collision with other shapes
    for (int i = 0; i < 4; i++) {
        if (board[blocks[i].y][blocks[i].x] != -1) {
            return true;
        }
    }

    return false;
}

int random_shape_type() {
    return rand() % 7;
}

int main () {
    srand(time(NULL));

    initscr();
    cbreak();

    int board[BOARD_HEIGHT][BOARD_WIDTH];
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = -1;
        }
    } 

    Shape *active_shape = create_shape(random_shape_type());

    WINDOW *win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    wrefresh(win);
    curs_set(0);
    start_color();

    draw_borders();
    draw_shape(active_shape);

    refresh();
    sleep(1);

    while(true) {
        erase_shape(active_shape);
        bool collision = downwards_collision(active_shape, board); 
        if (collision) {
            add_shape(board, active_shape);
            active_shape = create_shape(random_shape_type());
            draw_board(board);
        }
        tick(active_shape);
        draw_shape(active_shape);
        refresh();
        sleep(1);
    }

    endwin();
    free(board);
    
    return 0;
}
