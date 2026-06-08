#include "gui_server.h"
#include <stdio.h>
#include <string.h>

#define SERVER_MAX_CARDS 57

static const char *SERVER_CARDS_RESOURCES[SERVER_MAX_CARDS] = {
    "resources/anteater_card.png",
    "resources/two_of_hearts.png",
    "resources/three_of_hearts.png",
    "resources/four_of_hearts.png",
    "resources/five_of_hearts.png",
    "resources/six_of_hearts.png",
    "resources/seven_of_hearts.png",
    "resources/eight_of_hearts.png",
    "resources/nine_of_hearts.png",
    "resources/ten_of_hearts.png",
    "resources/jack_of_hearts.png",
    "resources/queen_of_hearts.png",
    "resources/king_of_hearts.png",
    "resources/ace_of_hearts.png",
    "resources/anteater_card.png",
    "resources/two_of_diamonds.png",
    "resources/three_of_diamonds.png",
    "resources/four_of_diamonds.png",
    "resources/five_of_diamonds.png",
    "resources/six_of_diamonds.png",
    "resources/seven_of_diamonds.png",
    "resources/eight_of_diamonds.png",
    "resources/nine_of_diamonds.png",
    "resources/ten_of_diamonds.png",
    "resources/jack_of_diamonds.png",
    "resources/queen_of_diamonds.png",
    "resources/king_of_diamonds.png",
    "resources/ace_of_diamonds.png",
    "resources/anteater_card.png",
    "resources/two_of_clubs.png",
    "resources/three_of_clubs.png",
    "resources/four_of_clubs.png",
    "resources/five_of_clubs.png",
    "resources/six_of_clubs.png",
    "resources/seven_of_clubs.png",
    "resources/eight_of_clubs.png",
    "resources/nine_of_clubs.png",
    "resources/ten_of_clubs.png",
    "resources/jack_of_clubs.png",
    "resources/queen_of_clubs.png",
    "resources/king_of_clubs.png",
    "resources/ace_of_clubs.png",
    "resources/anteater_card.png",
    "resources/two_of_spades.png",
    "resources/three_of_spades.png",
    "resources/four_of_spades.png",
    "resources/five_of_spades.png",
    "resources/six_of_spades.png",
    "resources/seven_of_spades.png",
    "resources/eight_of_spades.png",
    "resources/nine_of_spades.png",
    "resources/ten_of_spades.png",
    "resources/jack_of_spades.png",
    "resources/queen_of_spades.png",
    "resources/king_of_spades.png",
    "resources/ace_of_spades.png",
    "resources/back_of_card.png"
};

typedef cairo_surface_t Icon;

static Icon *serverImageToSurface(const char *filename)
{
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, &error);
 
    if (pixbuf == NULL) {
        fprintf(stderr, "Failed to load image %s: %s\n", filename, error->message);
        g_error_free(error);
        return NULL;
    }
 
    Icon *surface = gdk_cairo_surface_create_from_pixbuf(pixbuf, 0, NULL);
    g_object_unref(pixbuf);
    return surface;
}

static void serverCreateCardImages(Icon **imgs, int numOfImgs)
{
    for (int i = 0; i < numOfImgs; i++) {
        imgs[i] = serverImageToSurface(SERVER_CARDS_RESOURCES[i]);
        if (imgs[i] == NULL)
            printf("ERROR: failed to load card resource - %s\n", SERVER_CARDS_RESOURCES[i]);
    }
}

static void serverDrawImg(cairo_t *cr, Icon *img, float xPos, float yPos,
                          float targetW, float targetH)
{
    if (!img) {
        printf("ERROR: serverDrawImg - img is NULL\n");
        return;
    }
 
    int imgWidth  = cairo_image_surface_get_width(img);
    int imgHeight = cairo_image_surface_get_height(img);
 
    cairo_save(cr);
    cairo_translate(cr, xPos, yPos);
    cairo_scale(cr, targetW / (float)imgWidth, targetH / (float)imgHeight);
    cairo_set_source_surface(cr, img, 0, 0);
    cairo_paint(cr);
    cairo_restore(cr);
}

static Icon *serverGetCardImage(Icon **images, Card card)
{
    int suit  = card.suit;
    int rank  = card.rank;
    int index = suit * NUM_OF_RANKS + rank - 1;
 
    if (suit == ANTEATER_SUIT || rank == ANTEATER) {
        return images[0];
    }
    return images[index];
}

static void serverDrawCards(cairo_t *cr, Icon **images, int numOfCards,
                            Card *cards, double width, double height,
                            double xPos, double yPos)
{
    if (!images) {
        printf("ERROR: serverDrawCards - images is NULL\n");
        return;
    }

    double gap = 1.5; /* same gap as render.c */
    for (int i = 0; i < numOfCards; i++) {
        Icon *image;
        if (cards) {
            image = serverGetCardImage(images, cards[i]);
        } else {
            /* No cards dealt yet; draw card backs (index 56 is back of card) */
            image = images[56];
        }
        serverDrawImg(cr, image, xPos, yPos, width, height);
        xPos += width + gap;
    }
}

static void serverDrawText(cairo_t *cr, double r, double g, double b,
                           const char *text, double fontSize,
                           double xPos, double yPos)
{
    cairo_save(cr);
    cairo_set_source_rgb(cr, r, g, b);
    /* SAME font as render.c: PIXEL_FONT = "VT323" */
    cairo_select_font_face(cr, "VT323", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, fontSize);
    /* SAME yPos offset as render.c: move_to(xPos, yPos + fontSize) */
    cairo_move_to(cr, xPos, yPos + fontSize);
    cairo_show_text(cr, text);
    cairo_restore(cr);
}


struct ServerWindow {
    GtkWidget *window;
    GtkWidget *stateLabel;
    GtkWidget *drawArea;
    GameState *gameState;
    Icon **cardImages;
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
    if (!stateWindow || !stateWindow->gameState) {
        return FALSE;
    }

    int areaW = gtk_widget_get_allocated_width(widget);
    int areaH = gtk_widget_get_allocated_height(widget);
    double margin = 14.0;
    double x = margin;
    double y = margin;
    GameState *gameState = stateWindow->gameState;
    Icon **images = stateWindow->cardImages;

    cairo_set_source_rgb(cr, 0.12, 0.10, 0.09);
    cairo_paint(cr);

    const double cardW   = 35.0;
    const double cardH   = cardW * 1.4; 
    const double cardGap = 1.5; 

    serverDrawText(cr, 241/255.0, 230/255.0, 207/255.0, "Board:", 18.0, x, y);
    y += 26.0;


    if (gameState->board.count > 0 && images) {
        double totalW = gameState->board.count * cardW + (gameState->board.count - 1) * cardGap;
        double xCards = (areaW - totalW) / 2.0;
        if (xCards < margin) {
            xCards = margin;
        }

        serverDrawCards(cr, images, gameState->board.count, gameState->board.cards, cardW, cardH, xCards, y);
        y += cardH + 18.0;
    } else {
        serverDrawText(cr, 241/255.0, 230/255.0, 207/255.0, "No board cards dealt yet.", 16.0, x, y);
        y += 26.0;
    }

    serverDrawText(cr, 241/255.0, 230/255.0, 207/255.0, "Dealt hole cards:", 18.0, x, y);
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
        serverDrawText(cr, 241/255.0, 230/255.0, 207/255.0, playerText, 14.0, x, y);

        if (images) {
            serverDrawCards(cr, images, MAX_PLAYER_CARDS, player->playerCards, cardW, cardH, cardX, y - 4.0);
        }

        y += cardH + 6.0;
        if (y + cardH > areaH - margin) {
            break;
        }
    }

    return FALSE;
}

ServerWindow *createServerWindow(GtkApplication *app, GameState *gameState)
{
    if (!app || !gameState)
    {
        return NULL;
    }

    ServerWindow *stateWindow = g_malloc0(sizeof(ServerWindow));
    stateWindow->gameState = gameState;

    stateWindow->cardImages = g_new0(Icon *, SERVER_MAX_CARDS);
    serverCreateCardImages(stateWindow->cardImages, SERVER_MAX_CARDS);

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

    if (window->cardImages) {
        for (int i = 0; i < SERVER_MAX_CARDS; i++) {
            if (window->cardImages[i]) {
                cairo_surface_destroy(window->cardImages[i]);
            }
        }
        g_free(window->cardImages);
    }

    if (window->window)
    {
        gtk_widget_destroy(window->window);
    }

    g_free(window);
}
