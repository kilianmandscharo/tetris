#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int const WINDOW_HEIGHT = 40;
int const WINDOW_WIDTH = 20; 

typedef enum {
    I_SHAPE,
    O_SHAPE,
    L_SHAPE,
    J_SHAPE,
    S_SHAPE,
    Z_SHAPE,
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

Shape * create_shape(ShapeType shape_type) {
    Shape *shape = malloc(sizeof(Shape) + 4 * sizeof(Block));
    shape->type = shape_type; 
    Block *blocks = malloc(4 * sizeof(Block));
    blocks[0].x = 9;
    blocks[0].y = 0;
    blocks[1].x = 9;
    blocks[1].y = 1;
    blocks[2].x = 10;
    blocks[2].y = 0;
    blocks[3].x = 10;
    blocks[3].y = 1;
    shape->blocks = blocks;
    return shape;
}

Shape ** add_shape(Shape **board, Shape *new_shape, int *shape_counter) {
    Shape **new_board = realloc(board, (*shape_counter + 1) * sizeof(Shape *)); 
    if (new_board == NULL) {
        printf("Memory could not be allocated./n");
    } else {
        new_board[*shape_counter] = new_shape;
        (*shape_counter)++;
        return new_board;
    }
}

Shape * tick(Shape *shape) {
    for (int i = 0; i < 4; i++) {
        shape->blocks[i].y++;
    }
    return shape;
}

int main () {
    initscr();
    cbreak();

    Shape **board = create_board();
    Shape *active_shape = create_shape(O_SHAPE);
    int shape_counter = 0;
    board = add_shape(board, active_shape, &shape_counter);

    WINDOW *win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    wrefresh(win);
    curs_set(0);
    start_color();

    draw_board(board, shape_counter);
    draw_borders(40, 20);

    for (int i = 0; i < 10; i++) {
        clear();
        active_shape = tick(active_shape);
        draw_board(board, shape_counter);
        draw_borders(40, 20);
        refresh();
        sleep(1);
    }

    endwin();
    free(board);
    
    return 0;
}
