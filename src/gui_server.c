#include "gui.h"
#include "gui_server.h"
#include <stdio.h>
#include <string.h>

struct ServerWindow {
    GtkWidget *window;
    GtkWidget *stateLabel;
    GtkWidget *drawArea;
    GameState *gameState;
    Poker_Gui *pokerGui;
};

static void formatServerStateLabel(char *buffer, size_t bufferSize, const GameState *gameState)
{
    if (!buffer || bufferSize == 0 || !gameState) {
        return;
    }

    buffer[0] = '\0';
    size_t offset = 0;
    int written = snprintf(buffer + offset, bufferSize - offset, "Password: anteater \n");
    if (written > 0 && (size_t)written < bufferSize - offset) {
    offset += (size_t)written;
    }
    for (int i = 0; i < gameState->numPlayers; i++) {
        Player_Info *player = getPlayerInfo(gameState, i);
        if (!player) {
            continue;
        }

        written = snprintf(buffer + offset, bufferSize - offset,
                               "player %d: name=%s chips=%d currentBet=%d folded=%d active=%d\n",
                               i,
                               player->name[0] ? player->name : "Unknown",
                               player->chips,
                               player->currentBet,
                               player->hasFolded ? 1 : 0,
                               player->canAct ? 1 : 0);
        if (written < 0 || (size_t)written >= bufferSize - offset) {
            break;
        }

        offset += (size_t)written;
    }
}

static gboolean on_state_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    ServerWindow *stateWindow = (ServerWindow *)user_data;
    if (!stateWindow || !stateWindow->gameState || !stateWindow->pokerGui) {
        return FALSE;
    }

    int areaW = gtk_widget_get_allocated_width(widget);
    int areaH = gtk_widget_get_allocated_height(widget);
    double margin = 14.0;
    double x = margin;
    double y = margin;
    GameState *gameState = stateWindow->gameState;
    Icon **images = getImages(stateWindow->pokerGui);

    cairo_set_source_rgb(cr, 0.12, 0.10, 0.09);
    cairo_paint(cr);

    drawText(cr, PLAYER_TEXT_COLOR, "Board:", 18.0, x, y);
    y += 26.0;

    const double cardW = 35.0;
    const double cardH = 49.0;
    const double cardGap = 6.0;

    if (gameState->board.count > 0 && images) {
        double totalW = gameState->board.count * cardW + (gameState->board.count - 1) * cardGap;
        double xCards = (areaW - totalW) / 2.0;
        if (xCards < margin) {
            xCards = margin;
        }

        drawCards(cr, images, gameState->board.count, gameState->board.cards, cardW, cardH, xCards, y);
        y += cardH + 18.0;
    } else {
        drawText(cr, PLAYER_TEXT_COLOR, "No board cards dealt yet.", 16.0, x, y);
        y += 26.0;
    }

    drawText(cr, PLAYER_TEXT_COLOR, "Dealt hole cards:", 18.0, x, y);
    y += 26.0;

    double cardX = areaW - margin - ((cardW * MAX_PLAYER_CARDS) + cardGap);

    for (int i = 0; i < gameState->numPlayers; i++) {
        Player_Info *player = getPlayerInfo(gameState, i);
        if (!player) {
            continue;
        }

        char playerText[128];
        snprintf(playerText, sizeof(playerText), "player %d: %s%s",
                 i,
                 player->name[0] ? player->name : "Unknown",
                 player->hasFolded ? " (folded)" : "");
        drawText(cr, PLAYER_TEXT_COLOR, playerText, 14.0, x, y);

        if (images) {
            drawCards(cr, images, MAX_PLAYER_CARDS, player->playerCards, cardW, cardH, cardX, y - 4.0);
        }

        y += cardH + 6.0;
        if (y + cardH > areaH - margin) {
            break;
        }
    }

    return FALSE;
}

ServerWindow *createServerWindow(GtkApplication *app, Poker_Gui *pokerGui)
{
    if (!app || !pokerGui)
    {
        return NULL;
    }

    ServerWindow *stateWindow = g_malloc0(sizeof(ServerWindow));
    stateWindow->gameState = getGameState(pokerGui);
    stateWindow->pokerGui = pokerGui;

    stateWindow->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(stateWindow->window), "Game State");
    gtk_window_set_default_size(GTK_WINDOW(stateWindow->window), 520, 380);
    gtk_window_set_resizable(GTK_WINDOW(stateWindow->window), TRUE);

    GtkWidget *mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(stateWindow->window), mainBox);
    gtk_widget_set_margin_top(mainBox, 12);
    gtk_widget_set_margin_bottom(mainBox, 12);
    gtk_widget_set_margin_start(mainBox, 12);
    gtk_widget_set_margin_end(mainBox, 12);

    stateWindow->stateLabel = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(stateWindow->stateLabel), 0.0);
    gtk_label_set_line_wrap(GTK_LABEL(stateWindow->stateLabel), TRUE);
    gtk_label_set_line_wrap_mode(GTK_LABEL(stateWindow->stateLabel), PANGO_WRAP_WORD_CHAR);
    gtk_label_set_justify(GTK_LABEL(stateWindow->stateLabel), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(mainBox), stateWindow->stateLabel, FALSE, FALSE, 0);

    stateWindow->drawArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(stateWindow->drawArea, 500, 260);
    gtk_widget_set_hexpand(stateWindow->drawArea, TRUE);
    gtk_widget_set_vexpand(stateWindow->drawArea, TRUE);
    g_signal_connect(stateWindow->drawArea, "draw", G_CALLBACK(on_state_draw), stateWindow);
    gtk_box_pack_start(GTK_BOX(mainBox), stateWindow->drawArea, TRUE, TRUE, 0);

    gtk_widget_show_all(stateWindow->window);
    return stateWindow;
}


void updateServerWindow(ServerWindow *window, const char *stateMsg, GameState *gameState)
{
    if (!window)
    {
        return;
    }

    if (gameState)
    {
        window->gameState = gameState;
    }

    if (window->gameState) {
        char formattedState[1024];
        formatServerStateLabel(formattedState, sizeof(formattedState), window->gameState);
        gtk_label_set_text(GTK_LABEL(window->stateLabel), formattedState);
    } else if (stateMsg) {
        gtk_label_set_text(GTK_LABEL(window->stateLabel), stateMsg);
    }

    if (window->drawArea)
    {
        gtk_widget_queue_draw(window->drawArea);
    }
}

void destroyServerWindow(ServerWindow *window)
{
    if (!window)
    {
        return;
    }

    if (window->window)
    {
        gtk_widget_destroy(window->window);
    }

    g_free(window);
}
