/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include "menu_controller.h"

typedef struct {
    Controller super;
    MenuView *view;
    MenuItemView *cursor;
} MenuController;

static void handle_key_scroll(MenuController *controller, EventKeyType type)
{
    if (!controller->cursor) {
        controller->cursor = MenuView_get_first_item(controller->view);
    } else {
        MenuItemView_on_release(controller->cursor);
        
        if (type == EVENT_KEY_UP) {
            controller->cursor = controller->cursor->up;
        } else if (type == EVENT_KEY_DOWN) {
            controller->cursor = controller->cursor->down;
        } else if (type == EVENT_KEY_LEFT) {
            controller->cursor = controller->cursor->left;
        } else if (type == EVENT_KEY_RIGHT) {
            controller->cursor = controller->cursor->right;;
        }
    }
    MenuItemView_on_focus(controller->cursor);
}

static void handle_key_enter(MenuController *controller)
{
    MenuItemView_on_click(controller->cursor);
}

static void handle_mouse_click(MenuController *controller, size_t x, size_t y)
{
    MenuItemView *item_view = MenuView_get_positioned_item(controller->view, x, y);
    MenuItemView_on_click(item_view);
}

static void handle_mouse_move(MenuController *controller, size_t x, size_t y)
{
    MenuItemView *item_view = MenuView_get_positioned_item(controller->view, x, y);
    if (item_view) {
        if (item_view == controller->cursor) {
            return;
        }
        if (controller->cursor) {
            MenuItemView_on_release(controller->cursor);
        }
        MenuItemView_on_focus(item_view);
        controller->cursor = item_view;
    } else {
        MenuItemView_on_release(controller->cursor);
        controller->cursor = NULL;
    }
}

static void handle_key_event(MenuController *controller, EventKey key)
{
    if (key.type == EVENT_KEY_UP || key.type == EVENT_KEY_DOWN ||
        key.type == EVENT_KEY_LEFT || key.type == EVENT_KEY_RIGHT) {
        handle_key_scroll(controller, key.type);
    } else if (key.type == EVENT_KEY_ENTER) {
        handle_key_enter(controller);
    }
}

static void handle_mouse_event(MenuController *controller, EventMouse mouse)
{
    if (mouse.type == EVENT_MOUSE_LEFT_CLICK) {
        handle_mouse_click(controller, mouse.x, mouse.y);
    } else if (mouse.type == EVENT_MOUSE_MOVE) {
        handle_mouse_move(controller, mouse.x, mouse.y);
    }
}

static void handle_screen_event(MenuController *controller, EventScreen screen)
{
    if (screen.type == EVENT_SCREEN_RESIZE) {
        MenuView_on_screen_resize(controller->view, screen.width, screen.height);
    }
}

static void handle_event(Controller *super, const Event *event)
{
    MenuController *controller = (MenuController *)super;
    
    if (event->type == EVENT_KEY) {
        handle_key_event(controller, event->key);
    } else if (event->type == EVENT_MOUSE) {
        handle_mouse_event(controller, event->mouse);
    } else if (event->type == EVENT_SCREEN) {
        handle_screen_event(controller, event->screen);
    }
}

Controller *MenuController_new(MenuView *view)
{
    MenuController *controller = malloc(sizeof(MenuController));
    if (!controller) {
        return NULL;
    }
    *controller = (MenuController) {
        .super = {
            .handle_event = handle_event,
        },
        .view = view,
        .cursor = NULL,
    };
    
    MenuView_on_draw(view);
    
    return (Controller *)controller;
}

void MenuController_delete(Controller *super)
{
    MenuController *controller = (MenuController *)super;
    
    MenuView_on_erase(controller->view);
    
    free(controller);
}