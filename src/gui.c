
#include "gui.h"
#include "communication.h"
#include "poker_protocol.h"
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
    "}";

const char *WAITING_ROOM_CSS = "waitroom-bg";
const char *POKER_TABLE_CSS = "poker-bg";
const char *BUTTON_CSS = "button-bg";
const char *SLIDER_CSS = "slider-bg";

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
        guiPlayer->isActive = statePlayer->isActive;
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

void createWaitingRoom(GtkApplication *app, gpointer user_data)
{
    printf("Creating Waitroom\n");
    int socket = user_data;

    // creating wait room window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_HEIGHT, WINDOW_WIDTH);
    loadCss(window, CSS);
    setStyle(window, WAITING_ROOM_CSS);

    // creating box
    GtkWidget *mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), mainBox);

    // creating start button
    GtkWidget *startButton = gtk_button_new_with_label("START");
    gtk_widget_set_size_request(startButton, BUTTON_WIDTH, BUTTON_HEIGHT);
    gtk_widget_set_halign(startButton, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(mainBox), startButton, FALSE, FALSE, 0);
    setStyle(startButton, BUTTON_CSS);

    // icons for when a person joins

    gtk_widget_show_all(window);
    // setWindow(pokerGui, createWindow(app));
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
                   gameState->players[i]->isActive);
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
