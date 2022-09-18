#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

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
    bool active;
} Shape;

void draw_borders() {
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        mvprintw(i, 0, "|");
        mvprintw(i, WINDOW_WIDTH, "|");
    }
}

void draw_shape(Shape *shape) {
    for (int i = 0; i < 4; i++) {
        mvprintw(shape->blocks[i].y, shape->blocks[i].x, "X");
    }
}

void draw_board(Shape **board, int shape_counter) {
    for (int i = 0; i < shape_counter; i++) {
        draw_shape(board[i]);
    }
}

Shape ** create_board() {
    Shape **board = malloc(sizeof(Shape *));
    if (board == NULL) {
        printf("Memory could not be allocated./n");
    } else {
        return board;
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
    shape->active = true;
    return shape;
}

void add_shape(Shape **board, Shape *new_shape, int *shape_counter) {
    Shape **new_board = realloc(board, (*shape_counter + 1) * sizeof(Shape *)); 
    if (new_board == NULL) {
        printf("Memory could not be allocated./n");
    } else {
        new_board[*shape_counter] = new_shape;
        (*shape_counter)++;
    }
}

void tick(Shape *shape) {
    for (int i = 0; i < 4; i++) {
        shape->blocks[i].y++;
    }
}

bool shape_collision(Shape *shape_1, Shape *shape_2) {
    if (shape_2->active) {
        return false;
    }
    Block *blocks_1 = shape_1->blocks;
    Block *blocks_2 = shape_2->blocks;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blocks_1[i].x == blocks_2[j].x && blocks_1[i].y == blocks_2[j].y) {
                return true;
            }
        }
    }
    return false;
}

bool downwards_collision(Shape *shape, Shape **board, int shape_counter) {
    Block *blocks = shape->blocks;

    // Collision with the bottom screen border
    for (int i = 0; i < 4; i++) {
        if (blocks[i].y + 1 >= WINDOW_HEIGHT) {
            return true;
        }
    } 

    if (shape_counter <= 1) {
        return false;
    }

    // Collision with other shapes
    for (int i = 0; i < shape_counter; i++) {
        if (shape_collision(shape, board[i])) {
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

    Shape **board = create_board();
    Shape *active_shape = create_shape(random_shape_type());
    int shape_counter = 0;
    add_shape(board, active_shape, &shape_counter);

    WINDOW *win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    wrefresh(win);
    curs_set(0);
    start_color();

    draw_board(board, shape_counter);
    draw_borders();

    refresh();
    sleep(1);

    while(true) {
        clear();
        bool collision = downwards_collision(active_shape, board, shape_counter); 
        if (collision) {
            active_shape->active = false;
            active_shape = create_shape(random_shape_type());
            add_shape(board, active_shape, &shape_counter);
        }
        tick(active_shape);
        draw_board(board, shape_counter);
        draw_borders();
        refresh();
        sleep(1);
    }

    endwin();
    free(board);
    
    return 0;
}
