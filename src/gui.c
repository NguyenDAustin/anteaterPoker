
#include "gui.h"
#include "communication.h"
#include "poker_protocol.h"

const char* TITLE = "ANTEATER POKER"; 

const char *CSS =
    ".poker-bg {"
    "   background-image: url('resources/poker_table.png');"
    "   background-size: cover;"
    "   background-repeat: no-repeat;"
    "   background-position: center;"
    "}"

    ".button-bg { "
    "   background-color: rgb(205, 193, 176); "
    "   color: rgb(22, 47, 31); "
    "   font-weight: bold; "
    "   font-size: 18px; "
    "}"; 


const char* POKER_TABLE_CSS = "poker-bg"; 
const char* BUTTON_CSS = "button-bg"; 

const char* AVATAR_IMG_RESOURCE = "resources/avatars/avatar1_img.png"; 
const char* CHIP_ICON_RESOURCE = "resources/red_chip.png"; 
const char* PIXEL_FONT_RESOURCE = "resources/fonts/pixel_font.ttf"; 
const char* PIXEL_FONT_RESOURCE2 = "resources/fonts/pixel_font2.ttf"; 

const int BUTTON_HEIGHT = 25; 
const int BUTTON_WIDTH = 50; 
const int SLIDER_HEIGHT = 300; 
const int SLIDER_WIDTH = 25; 
const int CONTROL_AREA_HEIGHT = 75;
const double DEFAULT_SCREEN_PERCENTAGE = 0.90;


//GETTERS + SETTERS 

GtkWidget* getWindow(const Poker_Gui* pokerGui){  
    if(!pokerGui || !(pokerGui->Window)){ 
        printf("ERROR: pokerGui or window is NULL\n"); 
        return NULL; 
    }
    return pokerGui->Window; 
}

GtkWidget* getPokerTable(const Poker_Gui* pokerGui){ 
     if(!pokerGui || !(pokerGui->pokerTable)){ 
        printf("ERROR: cannot get poker table b/c pokerGui or poker table is NULL\n"); 
        return NULL; 
    }
    return pokerGui->pokerTable; 
}

GtkWidget* getRaiseSlider(const Poker_Gui*  pokerGui){
    if(!pokerGui || !(pokerGui->raiseSlider)){ 
        printf("ERROR: cannot get raise slider b/c pokergui or raise slider is NULL\n"); 
        return NULL;
    }
    return pokerGui->raiseSlider; 
}

Icon** getImages(const Poker_Gui* pokerGui){ 
     if(!pokerGui || !(pokerGui->images)){ 
        printf("ERROR: cannot get images b/c pokerGui or images is NULL\n"); 
        return NULL; 
    }
    return pokerGui->images; 
}

Icon** getAvatarImages(const Poker_Gui* pokerGui){ 
    if(!pokerGui || !(pokerGui->avatarImages)){ 
        printf("ERROR: cannot get avatar images b/c pokerGui or avatar images is NULL\n"); 
        return NULL; 
    }
    return pokerGui->avatarImages; 
}

Icon* getChipIcon(const Poker_Gui* pokerGui){ 
     if(!pokerGui || !(pokerGui->chipIcon)){ 
        printf("ERROR: cannot get chip icon b/c pokerGui or chipIcon is NULL\n"); 
        return NULL; 
    }
    return pokerGui->chipIcon; 
}

Player_Info** getAllPlayersInfo(const Poker_Gui* pokerGui){
    if(!pokerGui){ 
        printf("ERROR: cannot get all player info b/c pokerGui is NULL\n"); 
        return NULL; 
    }
    return pokerGui->playerInfo; 
}

Player_Info* getPlayerInfo(const Poker_Gui* pokerGui, int playerNum){ 
     if(!pokerGui || !(pokerGui->playerInfo)){ 
        printf("ERROR: cannot get player info b/c pokerGui or player info is NULL\n"); 
        return NULL; 
    }
    return pokerGui->playerInfo[playerNum]; 
} 

char* getPlayerName(const Player_Info** playerInfo, int playerNum){ 
    if(!playerInfo || !(playerInfo[playerNum]) || !(playerInfo[playerNum]->name)){
        printf("ERROR: cannot get player name b/c player info or player # %d or player name is NULL\n", playerNum); 
        return NULL; 
    } 

    return playerInfo[playerNum]->name; 
}

int getPlayerChipCount(const Player_Info** playerInfo, int playerNum){ 
    if(!playerInfo){
        printf("ERROR: player info is NULL\n"); 
        return -1; 
    } 
    return playerInfo[playerNum]->chips; 
}

Icon* getPlayerAvatar(const Player_Info** playerInfo, int playerNum){ 
    if(!playerInfo || !(playerInfo[playerNum]) || !(playerInfo[playerNum]->avatarImg)){
        printf("ERROR: player info or player # %d or player avatar is NULL\n", playerNum); 
        return NULL; 
    } 
    return playerInfo[playerNum]->avatarImg; 
}

Card* getPlayerCards(const Player_Info** playerInfo, int playerNum){ 
    if(!playerInfo || !(playerInfo[playerNum]) || !(playerInfo[playerNum]->playerCards)){
        printf("ERROR: player info or player # %d or player cards is NULL\n", playerNum); 
        return NULL; 
    } 
    return playerInfo[playerNum]->playerCards; 
}

///////////////////////////////////////////////////////////////////////////

char* getName(const Player_Info* playerInfo){ 
    if(!playerInfo){
        printf("ERROR: player info is NULL\n"); 
        return NULL;
    }
    return playerInfo->name; 
}

int getChipCount(const Player_Info* playerInfo){ 
    if(!playerInfo){
        printf("ERROR: player info is NULL\n"); 
        return -1;
    }
    return playerInfo->chips; 
}

Icon* getAvatar(const Player_Info* playerInfo){ 
    if(!playerInfo){
        printf("ERROR: player info is NULL\n"); 
        return NULL;
    }
    return playerInfo->avatarImg; 
}

Card* getCards(const Player_Info* playerInfo){ 
     if(!playerInfo){
        printf("ERROR: player info is NULL\n"); 
        return NULL;
    }
    return playerInfo->playerCards; 
}
////////////////////////////////////////////////////////////////////////////////////////////

int getPot(const Poker_Gui* pokerGui){
    if(!pokerGui){
        printf("ERROR: pokerGui is NULL cannot get pot\n");
        return -1; 
    } 
    return pokerGui->pot; 
}

int getSocket(const Poker_Gui* pokerGui){ 
    if(!pokerGui){ 
        printf("ERROR: pokerGui is NULL cannot get socket\n");
        return -1; 
    }
    return pokerGui->socket; 
}

////////////////////////////////////////////////////////////////////////////////////////////

void setWindow(Poker_Gui* pokerGui, GtkWidget* window){  
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL cannot set window\n"); 
        return; 
    }
    pokerGui->Window = window; 
}

void setPokerTable(Poker_Gui* pokerGui, GtkWidget* pokerTable){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL cannot set poker table\n"); 
        return; 
    }
    pokerGui->pokerTable = pokerTable; 
}

void setRaiseSlider(Poker_Gui* pokerGui, GtkWidget* raiseSlider){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL cannot set raise slider\n"); 
        return; 
    }
    pokerGui->raiseSlider = raiseSlider; 
}

void setImages(Poker_Gui* pokerGui, Icon** cardImages){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL cannot set images\n"); 
        return; 
    }
    pokerGui->images = cardImages; 
}

void setAvatarImages(Poker_Gui* pokerGui, Icon** avatarImages){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL cannot set avatar images\n"); 
        return; 
    }
    pokerGui->avatarImages = avatarImages; 
}

void setChipIcon(Poker_Gui* pokerGui, Icon* chipIcon){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL cannot set chip icon\n"); 
        return; 
    }
    pokerGui->chipIcon = chipIcon; 
}

void setPlayerInfo(Poker_Gui* pokerGui, Player_Info* playerInfo, int playerNum){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL cannot set player info\n"); 
        return; 
    } 
    pokerGui->playerInfo[playerNum] = playerInfo; 
}

void setPlayerName(Player_Info** playerInfo, const char* playerName, int playerNum){ 
    if(!playerInfo || !playerInfo[playerNum]){ 
        printf("ERROR: player info or player #%d is NULL cannot set player name\n", playerNum); 
        return; 
    } 
    playerInfo[playerNum]->name = playerName; 
}

void setChipCount(Player_Info** playerInfo, int chipCount, int playerNum){ 
    if(!playerInfo || !playerInfo[playerNum]){ 
        printf("ERROR: player info or player #%d is NULL cannot set chip count\n", playerNum); 
        return; 
    } 
    playerInfo[playerNum]->chips = chipCount; 
}

void setAvatar(Player_Info** playerInfo, Icon* avatarImg, int playerNum){ 
    if(!playerInfo || !playerInfo[playerNum]){ 
        printf("ERROR: player info or player #%d is NULL cannot set avatar\n", playerNum); 
        return; 
    } 
    playerInfo[playerNum]->avatarImg = avatarImg; 
}

void setPlayerCards(Player_Info** playerInfo, Card* playerCards, int playerNum){ 
    if(!playerInfo || !playerInfo[playerNum]){ 
        printf("ERROR: player info or player #%d is NULL annot set player cards\n", playerNum); 
        return; 
    } 
    playerInfo[playerNum]->playerCards = playerCards; 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setPot(Poker_Gui* pokerGui,  int potAmt){
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL cannot set pot\n"); 
        return; 
    }
    pokerGui->pot = potAmt; 
}

void setSocket(Poker_Gui* pokerGui, int socket){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL cannot set socket\n"); 
        return;
    }
    pokerGui->socket = socket; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loadFont(const char *fontPath){
    FcConfig *config = FcConfigGetCurrent();
    if (!FcConfigAppFontAddFile(config, (const FcChar8 *)fontPath)) {
        printf("ERROR: Failed to load font: %s\n", fontPath);
    } else {
        printf("Loaded font: %s\n", fontPath);
    }
}

void loadCss(GtkWidget* window, const char* CSS){ 
    GtkCssProvider* provider = gtk_css_provider_new(); 
    gtk_css_provider_load_from_data(provider, CSS, -1, NULL); 

    GdkScreen* screen = gtk_widget_get_screen(window);

    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    g_object_unref(provider);
}

void setStyle(GtkWidget* widget, const char* CSS){ 
    GtkStyleContext* context = gtk_widget_get_style_context(widget); 
    gtk_style_context_add_class(context, CSS); 
}

void getDefaultWindowSize(int* width, int* height){
    *width = WINDOW_WIDTH;
    *height = WINDOW_HEIGHT;

    GdkDisplay* display = gdk_display_get_default();
    if(!display){
        return;
    }

    GdkMonitor* monitor = gdk_display_get_primary_monitor(display);
    if(!monitor){
        return;
    }

    GdkRectangle workarea;
    gdk_monitor_get_workarea(monitor, &workarea);

    int screenWidth = (int)(workarea.width * DEFAULT_SCREEN_PERCENTAGE);
    int screenHeight = (int)(workarea.height * DEFAULT_SCREEN_PERCENTAGE);

    if(screenWidth > 0 && screenWidth < *width){
        *width = screenWidth;
    }

    if(screenHeight > 0 && screenHeight < *height){
        *height = screenHeight;
    }
}

GtkWidget* createWindow(GtkApplication* app){
    GtkWidget* window = gtk_application_window_new(app); 
    int windowWidth;
    int windowHeight;

    getDefaultWindowSize(&windowWidth, &windowHeight);

    gtk_window_set_title(GTK_WINDOW(window), TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), windowWidth, windowHeight);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    setStyle(window, POKER_TABLE_CSS); 
    return window; 
}

GtkWidget* createMainContainer(){
    GtkWidget* mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); 
    return mainBox; 
}


GtkWidget* createSecondaryContainer(){//holds pokertable + slider
    GtkWidget* secondBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); 
    return secondBox; 
}

GtkWidget* createPokerTable(Poker_Gui* pokerGui){ 
    GtkWidget* pokerTable = gtk_drawing_area_new(); 
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

void setPlayerNames(Poker_Gui* pokerGui, char** names){ 
    for(int i = 0; i < MAX_PLAYERS; i++){ 
        setPlayerName(pokerGui->playerInfo, names[i], i); 
    }
}

void allocatePlayerInfos(Player_Info** playerInfo){ 
    for(int i = 0; i < MAX_PLAYERS; i++){ 
        playerInfo[i] = g_malloc(sizeof(Player_Info)); 
    }
}

void onFoldClicked(GtkWidget* button, gpointer user_data){ 
    Poker_Gui* pokerGui = user_data; 
    int socket = getSocket(pokerGui);
    char message[POKER_MESSAGE_SIZE];

    if (!formatPokerActionMessage(message, sizeof(message), POKER_ACTION_FOLD, 0)) {
        printf("ERROR: was not able to format fold message\n");
        return;
    }

    if(sendMessage(socket, message) < 0)
        printf("ERROR: was not able to send fold message\n"); 
}


void onRaiseClicked(GtkWidget* button, gpointer user_data){ 
    Poker_Gui* pokerGui = user_data; 
    int socket = getSocket(pokerGui); 
    GtkWidget* raiseSlider = getRaiseSlider(pokerGui);
    int raiseAmount = 0;
    char message[POKER_MESSAGE_SIZE];

    if (raiseSlider) {
        raiseAmount = (int)gtk_range_get_value(GTK_RANGE(raiseSlider));
    }

    if (!formatPokerActionMessage(message, sizeof(message), POKER_ACTION_RAISE, raiseAmount)) {
        printf("ERROR: was not able to format raise message\n");
        return;
    }

    if(sendMessage(socket, message) < 0)
        printf("ERROR: was not able to send raise message\n"); 
}


void onCallClicked(GtkWidget* button, gpointer user_data){ 
    Poker_Gui* pokerGui = user_data; 
    int socket = getSocket(pokerGui); 
    char message[POKER_MESSAGE_SIZE];

    if (!formatPokerActionMessage(message, sizeof(message), POKER_ACTION_CALL, 0)) {
        printf("ERROR: was not able to format call message\n");
        return;
    }

    if(sendMessage(socket, message) < 0)
        printf("ERROR: was not able to send call message\n"); 
}

void create_poker_gui(GtkApplication *app, gpointer user_data){
    printf("Creating poker gui\n"); 
    Poker_Gui* pokerGui = g_malloc(sizeof(Poker_Gui));
    setWindow(pokerGui, createWindow(app));

    int socket = GPOINTER_TO_INT(user_data);
    setSocket(pokerGui, socket); 
    
    
    loadCss(pokerGui->Window, CSS); 
    loadFont(PIXEL_FONT_RESOURCE); 
    loadFont(PIXEL_FONT_RESOURCE2); 


    //allocating player info 
    pokerGui->playerInfo = g_malloc(sizeof(Player_Info*) * MAX_PLAYERS);
    allocatePlayerInfos(pokerGui->playerInfo); 

    //creating card images
    pokerGui->images = g_malloc(sizeof(Icon*) * (MAX_CARDS + 1)); //because we want to include back of card
    createCardImages(pokerGui->images, 52); //52 for now because we don't have anteater card png yet 
    
    //creating avatar images 
    pokerGui->avatarImages = g_malloc(sizeof(Icon*) * MAX_PLAYERS); //allocate for max players which is 6 
    createAvatarImages(pokerGui->avatarImages); 

    //initializing avatar images 
    Icon** avatarImages = getAvatarImages(pokerGui); 
    for(int i = 0; i < MAX_PLAYERS; i++){ 
        setAvatar(pokerGui->playerInfo, avatarImages[i], i); 
    }

    //initializing player names 
    char* names[MAX_PLAYERS] = {"YOSHI #1", "YOSHI #2", "YOSHI #3", "YOSHI #4", "YOSHI #5", "YOSHI #6"}; 
    setPlayerNames(pokerGui, names); 

    //creating chip icon 
    pokerGui->chipIcon = imageToSurface(CHIP_ICON_RESOURCE); 

    //creating main container
    GtkWidget *mainBox = createMainContainer(); 
    gtk_container_add(GTK_CONTAINER(pokerGui->Window), mainBox);

    //creating secondary container for pokertable and slider 
    GtkWidget* secondBox = createSecondaryContainer(); 
    gtk_box_pack_start(GTK_BOX(mainBox), secondBox, TRUE, TRUE, 0); 

    //creating poker table
    pokerGui->pokerTable = createPokerTable(pokerGui); 
    //gtk_box_pack_start(GTK_BOX(mainBox), pokerGui->pokerTable, TRUE, TRUE, 0); //change
    gtk_box_pack_start(GTK_BOX(secondBox), getPokerTable(pokerGui), TRUE, TRUE, 0); 

    //creating slider 
    GtkWidget* raiseSlider = gtk_scale_new_with_range(GTK_ORIENTATION_VERTICAL, 0, 1000, 10); //min val, max val, step size
    gtk_box_pack_start(GTK_BOX(secondBox), raiseSlider, FALSE, FALSE, 5);
    gtk_widget_set_size_request(raiseSlider, SLIDER_WIDTH, SLIDER_HEIGHT); 
    setRaiseSlider(pokerGui, raiseSlider);
    
    //creating button container 
    GtkWidget* buttonBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); 
    gtk_box_pack_start(GTK_BOX(mainBox), buttonBox, FALSE, FALSE, 0); 
    gtk_widget_set_halign(buttonBox, GTK_ALIGN_CENTER); 
    gtk_widget_set_margin_bottom(buttonBox, 10); 

    
    //creating fold button 
    GtkWidget* foldButton = gtk_button_new_with_label("FOLD");
    gtk_box_pack_start(GTK_BOX(buttonBox), foldButton, FALSE, FALSE, 5);  
    gtk_widget_set_size_request(foldButton, BUTTON_WIDTH, BUTTON_HEIGHT); 
    setStyle(foldButton, BUTTON_CSS); 
    g_signal_connect(foldButton, "clicked", G_CALLBACK(onFoldClicked), pokerGui); 
    
    //creating raise button
    GtkWidget* raiseButton = gtk_button_new_with_label("RAISE"); 
    gtk_box_pack_start(GTK_BOX(buttonBox), raiseButton, FALSE, FALSE, 5); 
    gtk_widget_set_size_request(raiseButton, BUTTON_WIDTH, BUTTON_HEIGHT); 
    setStyle(raiseButton, BUTTON_CSS);  //when this is clicked show raise slider ui
    g_signal_connect(raiseButton, "clicked", G_CALLBACK(onRaiseClicked), pokerGui); 

    //creating call button
    GtkWidget* callButton = gtk_button_new_with_label("CALL"); 
    gtk_box_pack_start(GTK_BOX(buttonBox), callButton, FALSE, FALSE, 5); 
    gtk_widget_set_size_request(callButton, BUTTON_WIDTH, BUTTON_HEIGHT); 
    setStyle(callButton, BUTTON_CSS);
    g_signal_connect(callButton, "clicked", G_CALLBACK(onCallClicked), pokerGui);
    
    printf("finished creating poker gui\n"); 
    gtk_widget_show_all(pokerGui->Window);  
}
