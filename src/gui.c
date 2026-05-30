
#include "gui.h"
#include "communication.h"
#include "lobby.h"
#include "poker_protocol.h"
#include "timer.h"
#include <string.h>

const char *TITLE = "ANTEATER POKER";

const char *CSS =
    ".waitroom-bg {"
    "   background-image: url('resources/wating_room.png');"
    "   background-size: cover;"
    "   background-repeat: no-repeat;"
    "   background-position: center;"
    "}"

    ".poker-bg {"
    "   background-image: url('resources/poker_table.png');"
    "   background-size: cover;"
    "   background-repeat: no-repeat;"
    "   background-position: center;"
    "}"

    ".button-bg { "
    "   background-color: rgb(205, 193, 176); "
    "   color: rgb(22, 47, 31); "
    "   font-family: 'VT323';"
    "   font-weight: bold; "
    "   font-size: 18px; "
    "}"

    ".slider-bg{"
    "   background-color: rgba(19, 21, 28, 0.6);"
    "   font-family: 'VT323';"
    "   color: rgb(205,193,176);"
    "   font-weight: bold;"
    "   font-size: 17px;"
    "}"

    ".lobby-label {"
    "   color: rgb(245, 240, 226);"
    "   background-color: rgba(22, 47, 31, 0.72);"
    "   font-family: 'VT323';"
    "   font-size: 24px;"
    "   padding: 8px;"
    "}";

const char *WAITING_ROOM_CSS = "waitroom-bg";
const char *POKER_TABLE_CSS = "poker-bg";
const char *BUTTON_CSS = "button-bg";
const char *SLIDER_CSS = "slider-bg";
const char *LOBBY_LABEL_CSS = "lobby-label";

const char *AVATAR_IMG_RESOURCE = "resources/avatars/avatar1_img.png";
const char *CHIP_ICON_RESOURCE = "resources/red_chip.png";
const char *PIXEL_FONT_RESOURCE = "resources/fonts/pixel_font.ttf";
const char *PIXEL_FONT_RESOURCE2 = "resources/fonts/pixel_font2.ttf";

const int BUTTON_HEIGHT = 25;
const int BUTTON_WIDTH = 50;
const int SLIDER_HEIGHT = 25;
const int SLIDER_WIDTH = 300;
const int CONTROL_AREA_HEIGHT = 75;
const double DEFAULT_SCREEN_PERCENTAGE = 0.90;

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *titleLabel;
    GtkWidget *statusLabel;
    GtkWidget *nameEntry;
    GtkWidget *saveNameButton;
    GtkWidget *playerLabels[MAX_PLAYERS_COUNT];
    GtkWidget *startButton;
    Communication_Bundle *bundle;
    int socket;
    int playerNum;
    bool waitingForInitialState;
} Lobby_Gui;

// GETTERS + SETTERS

GtkWidget *getWindow(const Poker_Gui *pokerGui)
{
    if (!pokerGui || !(pokerGui->Window))
    {
        printf("ERROR: pokerGui or window is NULL\n");
        return NULL;
    }
    return pokerGui->Window;
}

GtkWidget *getPokerTable(const Poker_Gui *pokerGui)
{
    if (!pokerGui || !(pokerGui->pokerTable))
    {
        printf("ERROR: cannot get poker table b/c pokerGui or poker table is NULL\n");
        return NULL;
    }
    return pokerGui->pokerTable;
}

GtkWidget *getRaiseSlider(const Poker_Gui *pokerGui)
{
    if (!pokerGui || !(pokerGui->raiseSlider))
    {
        printf("ERROR: cannot get raise slider b/c pokergui or raise slider is NULL\n");
        return NULL;
    }
    return pokerGui->raiseSlider;
}

Icon **getImages(const Poker_Gui *pokerGui)
{
    if (!pokerGui || !(pokerGui->images))
    {
        printf("ERROR: cannot get images b/c pokerGui or images is NULL\n");
        return NULL;
    }
    return pokerGui->images;
}

Icon **getAvatarImages(const Poker_Gui *pokerGui)
{
    if (!pokerGui || !(pokerGui->avatarImages))
    {
        printf("ERROR: cannot get avatar images b/c pokerGui or avatar images is NULL\n");
        return NULL;
    }
    return pokerGui->avatarImages;
}

Icon *getChipIcon(const Poker_Gui *pokerGui)
{
    if (!pokerGui || !(pokerGui->chipIcon))
    {
        printf("ERROR: cannot get chip icon b/c pokerGui or chipIcon is NULL\n");
        return NULL;
    }
    return pokerGui->chipIcon;
}

GameState* getGameState(const Poker_Gui* pokerGui){
    if(!pokerGui){ 
        printf("ERROR: cannot get game state b/c pokerGui is NULL\n"); 
        return NULL; 
    }
    return pokerGui->gameState; 
}

int getSocket(const Poker_Gui *pokerGui)
{
    if (!pokerGui)
    {
        printf("ERROR: pokerGui is NULL cannot get socket\n");
        return -1;
    }
    return pokerGui->socket;
}

////////////////////////////////////////////////////////////////////////////////////////////

void setWindow(Poker_Gui *pokerGui, GtkWidget *window)
{
    if (!pokerGui)
    {
        printf("ERROR: poker gui is NULL cannot set window\n");
        return;
    }
    pokerGui->Window = window;
}

void setPokerTable(Poker_Gui *pokerGui, GtkWidget *pokerTable)
{
    if (!pokerGui)
    {
        printf("ERROR: poker gui is NULL cannot set poker table\n");
        return;
    }
    pokerGui->pokerTable = pokerTable;
}

void setRaiseSlider(Poker_Gui *pokerGui, GtkWidget *raiseSlider)
{
    if (!pokerGui)
    {
        printf("ERROR: poker gui is NULL cannot set raise slider\n");
        return;
    }
    pokerGui->raiseSlider = raiseSlider;
}

void setImages(Poker_Gui *pokerGui, Icon **cardImages)
{
    if (!pokerGui)
    {
        printf("ERROR: poker gui is NULL cannot set images\n");
        return;
    }
    pokerGui->images = cardImages;
}

void setAvatarImages(Poker_Gui *pokerGui, Icon **avatarImages)
{
    if (!pokerGui)
    {
        printf("ERROR: poker gui is NULL cannot set avatar images\n");
        return;
    }
    pokerGui->avatarImages = avatarImages;
}

void setChipIcon(Poker_Gui *pokerGui, Icon *chipIcon)
{
    if (!pokerGui)
    {
        printf("ERROR: poker gui is NULL cannot set chip icon\n");
        return;
    }
    pokerGui->chipIcon = chipIcon;
}

void setGameState(Poker_Gui* pokerGui, GameState* gameState){
    if(!pokerGui){
        printf("ERROR: poker gui is NULL cannot set game  state\n");
        return;
    }
    pokerGui->gameState = gameState; 
}


void setSocket(Poker_Gui *pokerGui, int socket)
{
    if (!pokerGui)
    {
        printf("ERROR: poker gui is NULL cannot set socket\n");
        return;
    }
    pokerGui->socket = socket;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loadFont(const char *fontPath)
{
    FcConfig *config = FcConfigGetCurrent();
    if (!FcConfigAppFontAddFile(config, (const FcChar8 *)fontPath))
    {
        printf("ERROR: Failed to load font: %s\n", fontPath);
    }
    else
    {
        printf("Loaded font: %s\n", fontPath);
    }
}

void loadCss(GtkWidget *window, const char *CSS)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, CSS, -1, NULL);

    GdkScreen *screen = gtk_widget_get_screen(window);

    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);
}

void setStyle(GtkWidget *widget, const char *CSS)
{
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(context, CSS);
}

void getDefaultWindowSize(int *width, int *height)
{
    *width = WINDOW_WIDTH;
    *height = WINDOW_HEIGHT;

    GdkDisplay *display = gdk_display_get_default();
    if (!display)
    {
        return;
    }

    GdkMonitor *monitor = gdk_display_get_primary_monitor(display);
    if (!monitor)
    {
        return;
    }

    GdkRectangle workarea;
    gdk_monitor_get_workarea(monitor, &workarea);

    int screenWidth = (int)(workarea.width * DEFAULT_SCREEN_PERCENTAGE);
    int screenHeight = (int)(workarea.height * DEFAULT_SCREEN_PERCENTAGE);

    if (screenWidth > 0 && screenWidth < *width)
    {
        *width = screenWidth;
    }

    if (screenHeight > 0 && screenHeight < *height)
    {
        *height = screenHeight;
    }
}

GtkWidget *createWindow(GtkApplication *app)
{
    GtkWidget *window = gtk_application_window_new(app);
    int windowWidth;
    int windowHeight;

    getDefaultWindowSize(&windowWidth, &windowHeight);

    gtk_window_set_title(GTK_WINDOW(window), TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), windowWidth, windowHeight);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    setStyle(window, POKER_TABLE_CSS);
    return window;
}

GtkWidget *createMainContainer()
{
    GtkWidget *mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    return mainBox;
}

GtkWidget *createSecondaryContainer()
{ // holds pokertable + slider
    GtkWidget *secondBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    return secondBox;
}

GtkWidget *createPokerTable(Poker_Gui *pokerGui)
{
    GtkWidget *pokerTable = gtk_drawing_area_new();
    int windowWidth;
    int windowHeight;

    getDefaultWindowSize(&windowWidth, &windowHeight);

    gtk_widget_set_size_request(pokerTable, windowWidth, windowHeight - CONTROL_AREA_HEIGHT);
    g_signal_connect(pokerTable, "draw", G_CALLBACK(drawPokerTable), pokerGui);

    gtk_widget_set_hexpand(pokerTable, TRUE);
    gtk_widget_set_vexpand(pokerTable, TRUE);
    gtk_widget_set_halign(pokerTable, GTK_ALIGN_FILL);
    gtk_widget_set_valign(pokerTable, GTK_ALIGN_CENTER);
    return pokerTable;
}


void setPlayerNames(Poker_Gui *pokerGui, char **names)
{
    GameState* gameState = getGameState(pokerGui); 
    Player_Info* player; 
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        player = getPlayerInfo(gameState, i); 
        setName(player, names[i]); 
    }
}

/*
static void copyGameStateToGui(Poker_Gui *pokerGui, const GameState *game)
{
    if (!pokerGui || !game) {
        return;
    }

    pokerGui->turn = game->currentPlayerIndex;
    pokerGui->pot = game->pot;
    pokerGui->dealerCards = (Card *)game->board.cards;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player_Info *guiPlayer = pokerGui->playerInfo[i];
        const Player_Info *statePlayer = game->players[i];

        if (!guiPlayer) {
            continue;
        }

        strncpy(guiPlayer->name, statePlayer->name, sizeof(guiPlayer->name) - 1);
        guiPlayer->name[sizeof(guiPlayer->name) - 1] = '\0';
        guiPlayer->chips = statePlayer->chips;
        guiPlayer->currentBet = statePlayer->currentBet;
        guiPlayer->hasFolded = statePlayer->hasFolded;
        guiPlayer->canAct = statePlayer->canAct;
        guiPlayer->playerCards[0] = statePlayer->playerCards[0];
        guiPlayer->playerCards[1] = statePlayer->playerCards[1];
        guiPlayer->playerCards = guiPlayer->playerCards;
    }
}
    */  // not necessary 

static char *askPlayerName(GtkWindow *parent)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Player Name",
                                                    parent,
                                                    GTK_DIALOG_MODAL,
                                                    "Join",
                                                    GTK_RESPONSE_ACCEPT,
                                                    NULL);
    GtkWidget *contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new();

    gtk_entry_set_max_length(GTK_ENTRY(entry), 19);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter your name");
    gtk_box_pack_start(GTK_BOX(contentArea), entry, FALSE, FALSE, 10);
    gtk_widget_show_all(dialog);

    char *playerName = NULL;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        const char *typedName = gtk_entry_get_text(GTK_ENTRY(entry));
        if (typedName && typedName[0] != '\0') {
            playerName = g_strdup(typedName);
        }
    }

    gtk_widget_destroy(dialog);

    if (!playerName) {
        playerName = g_strdup("Player");
    }

    return playerName;
}

void onFoldClicked(GtkWidget *button, gpointer user_data)
{
    Poker_Gui *pokerGui = user_data;
    int socket = getSocket(pokerGui);
    char message[POKER_MESSAGE_SIZE];

    if (!formatPokerActionMessage(message, sizeof(message), POKER_ACTION_FOLD, 0))
    {
        printf("ERROR: was not able to format fold message\n");
        return;
    }

    if (sendMessage(socket, message) < 0)
        printf("ERROR: was not able to send fold message\n");
}

void onRaiseClicked(GtkWidget *button, gpointer user_data)
{
    Poker_Gui *pokerGui = user_data;
    int socket = getSocket(pokerGui);
    GtkWidget *raiseSlider = getRaiseSlider(pokerGui);

    int raiseAmount = 0;
    char message[POKER_MESSAGE_SIZE];

    if (raiseSlider)
    {
        raiseAmount = (int)gtk_range_get_value(GTK_RANGE(raiseSlider));
    }

    if (!formatPokerActionMessage(message, sizeof(message), POKER_ACTION_RAISE, raiseAmount))
    {
        printf("ERROR: was not able to format raise message\n");
        return;
    }

    if (sendMessage(socket, message) < 0)
        printf("ERROR: was not able to send raise message\n");
}

void onCallClicked(GtkWidget *button, gpointer user_data)
{
    Poker_Gui *pokerGui = user_data;
    int socket = getSocket(pokerGui);
    char message[POKER_MESSAGE_SIZE];

    if (!formatPokerActionMessage(message, sizeof(message), POKER_ACTION_CALL, 0))
    {
        printf("ERROR: was not able to format call message\n");
        return;
    }

    if (sendMessage(socket, message) < 0)
        printf("ERROR: was not able to send call message\n");
}

static void updateLobbyGui(Lobby_Gui *lobbyGui, const LobbyState *lobbyState)
{
    // GUI_LOBBY_WIRING: refreshes labels/buttons whenever LOBBY_STATE arrives.
    char text[128];

    if (!lobbyGui || !lobbyState) {
        return;
    }

    snprintf(text, sizeof(text), "Waiting room: %d/%d players",
             lobbyState->joinedPlayers, lobbyState->maxPlayers);
    gtk_label_set_text(GTK_LABEL(lobbyGui->titleLabel), text);

    for (int i = 0; i < MAX_PLAYERS_COUNT; i++) {
        if (i < lobbyState->joinedPlayers) {
            const char *name = lobbyState->playerNames[i][0] ? lobbyState->playerNames[i] : "Player";
            snprintf(text, sizeof(text), "Player %d: %s", i + 1, name);
        } else {
            snprintf(text, sizeof(text), "Player %d: waiting...", i + 1);
        }

        gtk_label_set_text(GTK_LABEL(lobbyGui->playerLabels[i]), text);
    }

    if (lobbyGui->playerNum == 1) {
        if (canStartLobbyGame(lobbyState->joinedPlayers)) {
            gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), "Ready. Click START to begin.");
            gtk_widget_set_sensitive(lobbyGui->startButton, TRUE);
        } else {
            snprintf(text, sizeof(text), "Waiting for at least %d players.", LOBBY_MIN_PLAYERS);
            gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), text);
            gtk_widget_set_sensitive(lobbyGui->startButton, FALSE);
        }
    } else {
        gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), "Waiting for Player 1 to start.");
        gtk_widget_set_sensitive(lobbyGui->startButton, FALSE);
    }
}

static void startPokerGuiFromLobby(Lobby_Gui *lobbyGui, const char *initialState)
{
    // GUI_LOBBY_WIRING: closes the lobby window and opens the poker table.
    if (!lobbyGui || !initialState || initialState[0] == '\0') {
        return;
    }

    lobbyGui->bundle->playerNum = lobbyGui->playerNum;
    strncpy(lobbyGui->bundle->stateMsg, initialState, sizeof(lobbyGui->bundle->stateMsg) - 1);
    lobbyGui->bundle->stateMsg[sizeof(lobbyGui->bundle->stateMsg) - 1] = '\0';

    gtk_widget_destroy(lobbyGui->window);
    create_poker_gui(lobbyGui->app, lobbyGui->bundle);
    g_free(lobbyGui);
}

static void onLobbyStartClicked(GtkWidget *button, gpointer user_data)
{
    // GUI_LOBBY_WIRING: sends Player 1's lobby start request to the server.
    Lobby_Gui *lobbyGui = user_data;
    char message[LOBBY_MESSAGE_SIZE];

    (void)button;

    if (!lobbyGui) {
        return;
    }

    if (formatLobbyStartRequest(message, sizeof(message))) {
        if (sendMessage(lobbyGui->socket, message) < 0) {
            gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), "Could not send START request.");
        }
    }
}

static void onLobbyNameSubmitted(GtkWidget *widget, gpointer user_data)
{
    // CUSTOM_PLAYER_NAME: sends the typed lobby name to the server.
    Lobby_Gui *lobbyGui = user_data;
    const char *typedName;
    char message[POKER_MESSAGE_SIZE];

    (void)widget;

    if (!lobbyGui || !lobbyGui->nameEntry) {
        return;
    }

    typedName = gtk_entry_get_text(GTK_ENTRY(lobbyGui->nameEntry));

    if (!formatPlayerNameMessage(message, sizeof(message), typedName)) {
        gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), "Enter a name first.");
        return;
    }

    if (sendMessage(lobbyGui->socket, message) < 0) {
        gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), "Could not send name.");
        return;
    }

    gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), "Name saved.");
}

static gboolean onLobbyServerMessage(GIOChannel *channel, GIOCondition condition, gpointer user_data)
{
    // GUI_LOBBY_WIRING: receives lobby protocol messages without blocking GTK.
    Lobby_Gui *lobbyGui = user_data;
    char buffer[GAME_STATE_MESSAGE_SIZE];
    char *stateStart;
    LobbyState lobbyState;

    (void)channel;

    if (!lobbyGui) {
        return FALSE;
    }

    if (condition & (G_IO_HUP | G_IO_ERR | G_IO_NVAL)) {
        gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), "Disconnected from server.");
        return FALSE;
    }

    if (!(condition & G_IO_IN)) {
        return TRUE;
    }

    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = receiveMessage(lobbyGui->socket, buffer, sizeof(buffer) - 1);

    if (bytesRead <= 0) {
        gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), "Server disconnected.");
        return FALSE;
    }

    stateStart = strstr(buffer, "STATE\n");

    if (lobbyGui->waitingForInitialState && stateStart) {
        startPokerGuiFromLobby(lobbyGui, stateStart);
        return FALSE;
    }

    if (sscanf(buffer, "YOU_ARE %d", &lobbyGui->playerNum) == 1) {
        lobbyGui->bundle->playerNum = lobbyGui->playerNum;

        char playerText[64];
        snprintf(playerText, sizeof(playerText), "You are Player %d.", lobbyGui->playerNum);
        gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), playerText);
    }

    char *lobbyStart = strstr(buffer, "LOBBY_STATE");
    if (lobbyStart && parseLobbyStateMessage(lobbyStart, &lobbyState)) {
        updateLobbyGui(lobbyGui, &lobbyState);
    }

    if (isLobbyStartGameMessage(buffer)) {
        if (stateStart) {
            startPokerGuiFromLobby(lobbyGui, stateStart);
            return FALSE;
        }

        lobbyGui->waitingForInitialState = true;
        gtk_label_set_text(GTK_LABEL(lobbyGui->statusLabel), "Game starting...");
    }

    return TRUE;
}

void createWaitingRoom(GtkApplication *app, gpointer user_data)
{
    // GUI_LOBBY_WIRING: creates the interactive GTK lobby before the poker table.
    Communication_Bundle *bundle = user_data;
    Lobby_Gui *lobbyGui;
    GtkWidget *mainBox;

    printf("Creating Waitroom\n");

    if (!bundle) {
        return;
    }

    lobbyGui = g_malloc0(sizeof(Lobby_Gui));
    lobbyGui->app = app;
    lobbyGui->bundle = bundle;
    lobbyGui->socket = bundle->socket;
    lobbyGui->playerNum = bundle->playerNum;

    lobbyGui->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(lobbyGui->window), TITLE);
    gtk_window_set_default_size(GTK_WINDOW(lobbyGui->window), WINDOW_HEIGHT, WINDOW_WIDTH);
    loadCss(lobbyGui->window, CSS);
    loadFont(PIXEL_FONT_RESOURCE);
    loadFont(PIXEL_FONT_RESOURCE2);
    setStyle(lobbyGui->window, WAITING_ROOM_CSS);

    mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_halign(mainBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(mainBox, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(lobbyGui->window), mainBox);

    lobbyGui->titleLabel = gtk_label_new("Waiting room: connecting...");
    setStyle(lobbyGui->titleLabel, LOBBY_LABEL_CSS);
    gtk_box_pack_start(GTK_BOX(mainBox), lobbyGui->titleLabel, FALSE, FALSE, 0);

    lobbyGui->statusLabel = gtk_label_new("Connecting to server...");
    setStyle(lobbyGui->statusLabel, LOBBY_LABEL_CSS);
    gtk_box_pack_start(GTK_BOX(mainBox), lobbyGui->statusLabel, FALSE, FALSE, 0);

    lobbyGui->nameEntry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(lobbyGui->nameEntry), 19);
    gtk_entry_set_placeholder_text(GTK_ENTRY(lobbyGui->nameEntry), "Enter your name");
    gtk_box_pack_start(GTK_BOX(mainBox), lobbyGui->nameEntry, FALSE, FALSE, 0);
    g_signal_connect(lobbyGui->nameEntry, "activate", G_CALLBACK(onLobbyNameSubmitted), lobbyGui);

    lobbyGui->saveNameButton = gtk_button_new_with_label("SAVE NAME");
    gtk_widget_set_halign(lobbyGui->saveNameButton, GTK_ALIGN_CENTER);
    setStyle(lobbyGui->saveNameButton, BUTTON_CSS);
    gtk_box_pack_start(GTK_BOX(mainBox), lobbyGui->saveNameButton, FALSE, FALSE, 0);
    g_signal_connect(lobbyGui->saveNameButton, "clicked", G_CALLBACK(onLobbyNameSubmitted), lobbyGui);

    for (int i = 0; i < MAX_PLAYERS_COUNT; i++) {
        char labelText[64];
        snprintf(labelText, sizeof(labelText), "Player %d: waiting...", i + 1);
        lobbyGui->playerLabels[i] = gtk_label_new(labelText);
        setStyle(lobbyGui->playerLabels[i], LOBBY_LABEL_CSS);
        gtk_box_pack_start(GTK_BOX(mainBox), lobbyGui->playerLabels[i], FALSE, FALSE, 0);
    }

    lobbyGui->startButton = gtk_button_new_with_label("START");
    gtk_widget_set_size_request(lobbyGui->startButton, BUTTON_WIDTH * 2, BUTTON_HEIGHT * 2);
    gtk_widget_set_halign(lobbyGui->startButton, GTK_ALIGN_CENTER);
    gtk_widget_set_sensitive(lobbyGui->startButton, FALSE);
    setStyle(lobbyGui->startButton, BUTTON_CSS);
    gtk_box_pack_start(GTK_BOX(mainBox), lobbyGui->startButton, FALSE, FALSE, 0);
    g_signal_connect(lobbyGui->startButton, "clicked", G_CALLBACK(onLobbyStartClicked), lobbyGui);

    GIOChannel *serverChannel = g_io_channel_unix_new(lobbyGui->socket);
    g_io_add_watch(serverChannel, G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL, onLobbyServerMessage, lobbyGui);
    g_io_channel_unref(serverChannel);

    gtk_widget_show_all(lobbyGui->window);
}

static gboolean onServerMessage(GIOChannel *channel, GIOCondition condition, gpointer user_data)
{
    Poker_Gui *pokerGui = user_data;

    printf("onServerMessage condition = %d\n", condition);

    if (condition & G_IO_IN) {
        int socket = getSocket(pokerGui);

        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));

        ssize_t n = receiveMessage(socket, buffer, sizeof(buffer) - 1);

        if (n <= 0) {
            printf("server disconnected while reading\n");
            return FALSE;
        }

        buffer[n] = '\0';

        printf("server says:\n%s\n", buffer);

        GameState *gameState = getGameState(pokerGui);

        bool parsed = parseGameStateMessage(buffer, gameState);

        printf("parsed = %d, pot = %d, numPlayers = %d\n",
               parsed,
               gameState->pot,
               gameState->numPlayers);

        for (int i = 0; i < gameState->numPlayers; i++) {
            printf("player %d: name=%s chips=%d currentBet=%d folded=%d active=%d\n",
                   i,
                   gameState->players[i]->name,
                   gameState->players[i]->chips,
                   gameState->players[i]->currentBet,
                   gameState->players[i]->hasFolded,
                   gameState->players[i]->canAct);
        }

        if (parsed) {
            pokerGui->stateMsg = buffer; 
            gtk_widget_queue_draw(getPokerTable(pokerGui));
        }
    }

    if (condition & (G_IO_HUP | G_IO_ERR | G_IO_NVAL)) {
        printf("server disconnected or socket error, condition=%d\n", condition);
        return FALSE;
    }

    return TRUE;
}

void create_poker_gui(GtkApplication *app, gpointer user_data)
{
    printf("Creating poker gui\n");
    Communication_Bundle* bundle = user_data; 
    Poker_Gui *pokerGui = g_malloc(sizeof(Poker_Gui));
    setWindow(pokerGui, createWindow(app));

    //setting socket
    int socket = bundle->socket; 
    setSocket(pokerGui, socket); 

    //setting player num
    int playerNum = bundle->playerNum; 
    pokerGui->playerNum = playerNum; 

    //setting message 
    pokerGui->stateMsg = bundle->stateMsg; 

    //loading css stuff
    loadCss(pokerGui->Window, CSS);
    loadFont(PIXEL_FONT_RESOURCE);
    loadFont(PIXEL_FONT_RESOURCE2);
    
    //allocating game state -- updating  once with initial state 
    GameState* gameState = malloc(sizeof(GameState)); 
    initGameState(gameState); 
    setGameState(pokerGui, gameState); 
    parseFullGameState(pokerGui->stateMsg, gameState); 
    
    // creating card images
    pokerGui->images = g_malloc(sizeof(Icon *) * (MAX_CARDS + 1)); // because we want to include back of card
    createCardImages(pokerGui->images, 57); //CHANGED - QUEENCY                       // 52 for now because we don't have anteater card png yet

    // creating avatar images
    pokerGui->avatarImages = g_malloc(sizeof(Icon *) * MAX_PLAYERS); // allocate for max players which is 6
    createAvatarImages(pokerGui->avatarImages);

    // initializing avatar images
    Icon **avatarImages = getAvatarImages(pokerGui);
    Player_Info* playerInfo; 
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        playerInfo = getPlayerInfo(gameState, i); 
        setAvatar(playerInfo, avatarImages[i]);
       //setAvatar(getPlayerInfo(pokerGui->gameState))
    }

    // initializing player names
    char *names[MAX_PLAYERS] = {"YOSHI #1", "YOSHI #2", "YOSHI #3", "YOSHI #4", "YOSHI #5", "YOSHI #6"};
    setPlayerNames(pokerGui, names);

    // creating chip icon
    pokerGui->chipIcon = imageToSurface(CHIP_ICON_RESOURCE);

    // creating main container
    GtkWidget *mainBox = createMainContainer();
    gtk_container_add(GTK_CONTAINER(pokerGui->Window), mainBox);

    //create timer
    GtkWidget *timerLabel = gtk_label_new("Time: 0");
    gtk_box_pack_start(GTK_BOX(mainBox), timerLabel, FALSE, FALSE, 5);
    setStyle(timerLabel, LOBBY_LABEL_CSS);
    Timer *timer = g_malloc(sizeof(Timer));
    timer->label = timerLabel;

start_timer(timer);

    // creating poker table
    pokerGui->pokerTable = createPokerTable(pokerGui);
    gtk_box_pack_start(GTK_BOX(mainBox), getPokerTable(pokerGui), TRUE, TRUE, 0);

    // creating button container
    GtkWidget *buttonBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(mainBox), buttonBox, FALSE, FALSE, 0);
    gtk_widget_set_halign(buttonBox, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(buttonBox, 25);

    // creating fold button
    GtkWidget *foldButton = gtk_button_new_with_label("FOLD");
    gtk_box_pack_start(GTK_BOX(buttonBox), foldButton, FALSE, FALSE, 5);
    gtk_widget_set_size_request(foldButton, BUTTON_WIDTH, BUTTON_HEIGHT);
    setStyle(foldButton, BUTTON_CSS);
    g_signal_connect(foldButton, "clicked", G_CALLBACK(onFoldClicked), pokerGui);

    // creating raise button
    GtkWidget *raiseButton = gtk_button_new_with_label("RAISE");
    gtk_box_pack_start(GTK_BOX(buttonBox), raiseButton, FALSE, FALSE, 5);
    gtk_widget_set_size_request(raiseButton, BUTTON_WIDTH, BUTTON_HEIGHT);
    setStyle(raiseButton, BUTTON_CSS); // when this is clicked show raise slider ui
    g_signal_connect(raiseButton, "clicked", G_CALLBACK(onRaiseClicked), pokerGui);

    // creating call button
    GtkWidget *callButton = gtk_button_new_with_label("CALL");
    gtk_box_pack_start(GTK_BOX(buttonBox), callButton, FALSE, FALSE, 5);
    gtk_widget_set_size_request(callButton, BUTTON_WIDTH, BUTTON_HEIGHT);
    setStyle(callButton, BUTTON_CSS);
    g_signal_connect(callButton, "clicked", G_CALLBACK(onCallClicked), pokerGui);

    // creating horizontal slider
    GtkWidget *raiseSlider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 1000, 50); // min val, max val, step size
    gtk_widget_set_size_request(raiseSlider, SLIDER_WIDTH, SLIDER_HEIGHT);
    gtk_box_pack_start(GTK_BOX(buttonBox), raiseSlider, FALSE, FALSE, 0);
    setStyle(raiseSlider, SLIDER_CSS);
    setRaiseSlider(pokerGui, raiseSlider);


    //update 
    GIOChannel *serverChannel = g_io_channel_unix_new(socket);
    g_io_add_watch(serverChannel, G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL, onServerMessage, pokerGui);
    g_io_channel_unref(serverChannel);

    printf("finished creating poker gui\n");
    gtk_widget_show_all(pokerGui->Window);
}
