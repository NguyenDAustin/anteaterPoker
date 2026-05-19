
#include "gui.h"

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

const int BUTTON_HEIGHT = 25; 
const int BUTTON_WIDTH = 50; 


//GETTERS + SETTERS 

GtkWidget* getWindow(const Poker_Gui* pokerGui){  
    if(!pokerGui || !(pokerGui->Window)){ 
        printf("ERROR: pokerGui or window is NULL\n"); 
        return; 
    }
    return pokerGui->Window; 
}

GtkWidget* getPokerTable(const Poker_Gui* pokerGui){ 
     if(!pokerGui || !(pokerGui->pokerTable)){ 
        printf("ERROR: pokerGui or poker table is NULL\n"); 
        return; 
    }
    return pokerGui->pokerTable; 
}

Icon** getImages(const Poker_Gui* pokerGui){ 
     if(!pokerGui || !(pokerGui->images)){ 
        printf("ERROR: pokerGui or images is NULL\n"); 
        return; 
    }
    return pokerGui->images; 
}

Icon** getAvatarImages(const Poker_Gui* pokerGui){ 
    if(!pokerGui || !(pokerGui->avatarImages)){ 
        printf("ERROR: pokerGui or avatar images is NULL\n"); 
        return; 
    }
    return pokerGui->avatarImages; 
}

Icon* getChipIcon(const Poker_Gui* pokerGui){ 
     if(!pokerGui || !(pokerGui->chipIcon)){ 
        printf("ERROR: pokerGui or chipIcon is NULL\n"); 
        return; 
    }
    return pokerGui->chipIcon; 
}

Player_Info* getPlayerInfo(const Poker_Gui* pokerGui){ 
     if(!pokerGui || !(pokerGui->playerInfo)){ 
        printf("ERROR: pokerGui or player info is NULL\n"); 
        return; 
    }
    return pokerGui->playerInfo; 
} 

char* getPlayerName(const Player_Info* playerInfo){ 
    if(!playerInfo || !(playerInfo->name)){
        printf("ERROR: player info or player name is NULL\n"); 
        return; 
    }
    return playerInfo->name; 
}

int getChipCount(const Player_Info* playerInfo){ 
    if(!playerInfo){
        printf("ERROR: player info is NULL\n"); 
        return; 
    } 
    return playerInfo->chips; 
}

Icon* getAvatar(const Player_Info* playerInfo){ 
    if(!playerInfo || !(playerInfo->avatarImg)){
        printf("ERROR: player info or player avatar is NULL\n"); 
        return; 
    } 
    return playerInfo->avatarImg; 
}

Card* getPlayerCards(const Player_Info* playerInfo){ 
    if(!playerInfo || !(playerInfo->playerCards)){
        printf("ERROR: player info or player cards is NULL\n"); 
        return; 
    } 
    return playerInfo->playerCards; 
}


void setWindow(Poker_Gui* pokerGui, GtkWidget* window){  
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL\n"); 
        return; 
    }
    pokerGui->Window = window; 
}

void setPokerTable(Poker_Gui* pokerGui, GtkWidget* pokerTable){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL\n"); 
        return; 
    }
    pokerGui->pokerTable = pokerTable; 
}

void setImages(Poker_Gui* pokerGui, Icon** cardImages){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL\n"); 
        return; 
    }
    pokerGui->images = cardImages; 
}

void setAvatarImages(Poker_Gui* pokerGui, Icon** avatarImages){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL\n"); 
        return; 
    }
    pokerGui->avatarImages = avatarImages; 
}

void setChipIcon(Poker_Gui* pokerGui, Icon* chipIcon){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL\n"); 
        return; 
    }
    pokerGui->chipIcon = chipIcon; 
}

void setPlayerInfo(Poker_Gui* pokerGui, Player_Info* playerInfo){ 
    if(!pokerGui){ 
        printf("ERROR: poker gui is NULL\n"); 
        return; 
    } 
    pokerGui->playerInfo = playerInfo; 
}

void setPlayerName(Player_Info* playerInfo, const char* playerName){ 
    if(!playerInfo){ 
        printf("ERROR: player info is NULL\n"); 
        return; 
    } 
    playerInfo->name = playerName; 
}

void setChipCount(Player_Info* playerInfo, int chipCount){ 
    if(!playerInfo){ 
        printf("ERROR: player info is NULL\n"); 
        return; 
    } 
    playerInfo->chips = chipCount; 
}

void setAvatar(Player_Info* playerInfo, Icon* avatarImg){ 
    if(!playerInfo){ 
        printf("ERROR: player info is NULL\n"); 
        return; 
    } 
    playerInfo->avatarImg = avatarImg; 
}

void setPlayerCards(Player_Info* playerInfo, Card* playerCards){ 
    if(!playerInfo){ 
        printf("ERROR: player info is NULL\n"); 
        return; 
    }
    playerInfo->playerCards = playerCards; 
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

GtkWidget* createWindow(GtkApplication* app){
    GtkWidget* window = gtk_application_window_new(app); 
    gtk_window_set_title(GTK_WINDOW(window), TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_HEIGHT, WINDOW_WIDTH);
    return window; 
}

GtkWidget* createMainContainer(){
    GtkWidget* mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); 
    setStyle(mainBox, POKER_TABLE_CSS); 
    return mainBox; 
}

GtkWidget* createPokerTable(Poker_Gui* pokerGui){ 
    GtkWidget* pokerTable = gtk_drawing_area_new(); 
    
    gtk_widget_set_size_request(pokerTable, WINDOW_WIDTH, WINDOW_HEIGHT); 
    g_signal_connect(pokerTable, "draw", G_CALLBACK(drawPokerTable), pokerGui);

    gtk_widget_set_hexpand(pokerTable, TRUE);
    gtk_widget_set_vexpand(pokerTable, TRUE); 
    gtk_widget_set_halign(pokerTable, GTK_ALIGN_FILL);
    gtk_widget_set_valign(pokerTable, GTK_ALIGN_CENTER);
    return pokerTable; 
}

void create_poker_gui(GtkApplication *app, gpointer user_data){

    printf("Creating poker gui\n"); 
    Poker_Gui* pokerGui = g_malloc(sizeof(Poker_Gui));
    pokerGui->Window =  createWindow(app); 
    
    loadCss(pokerGui->Window, CSS); 

    //allocating player info 
    pokerGui->playerInfo = g_malloc(sizeof(Player_Info));
    
    //creating card images
    pokerGui->images = g_malloc(sizeof(Icon*) * (MAX_CARDS + 1)); //because we want to include back of card
    createCardImages(pokerGui->images, 52); //52 for now because we don't have anteater card png yet 
    
    //creating avatar images 
    pokerGui->avatarImages = g_malloc(sizeof(Icon*) * MAX_PLAYERS); //allocate for max players which is 6 
    createAvatarImages(pokerGui->avatarImages); 

    //creating chip icon 
    pokerGui->chipIcon = imageToSurface(CHIP_ICON_RESOURCE); 

    //creating avatar img 
    pokerGui->playerInfo->avatarImg = imageToSurface(AVATAR_IMG_RESOURCE); 

    //creating main container
    GtkWidget *mainBox = createMainContainer(); 
    gtk_container_add(GTK_CONTAINER(pokerGui->Window), mainBox);

    //creating poker table
    pokerGui->pokerTable = createPokerTable(pokerGui); 
    gtk_box_pack_start(GTK_BOX(mainBox), pokerGui->pokerTable, TRUE, TRUE, 0); //change

    //creating button container 
    GtkWidget* buttonBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); 
    gtk_box_pack_start(GTK_BOX(mainBox), buttonBox, FALSE, FALSE, 0); 
    gtk_widget_set_halign(buttonBox, GTK_ALIGN_CENTER); 
    gtk_widget_set_margin_bottom(buttonBox, WINDOW_HEIGHT * 0.05); 
    
    //creating fold button 
    GtkWidget* foldButton = gtk_button_new_with_label("FOLD");
    gtk_box_pack_start(GTK_BOX(buttonBox), foldButton, FALSE, FALSE, 5);  
    gtk_widget_set_size_request(foldButton, BUTTON_WIDTH, BUTTON_HEIGHT); 
    setStyle(foldButton, BUTTON_CSS); 
    
    //creating raise button
    GtkWidget* raiseButton = gtk_button_new_with_label("RAISE"); 
    gtk_box_pack_start(GTK_BOX(buttonBox), raiseButton, FALSE, FALSE, 5); 
    gtk_widget_set_size_request(raiseButton, BUTTON_WIDTH, BUTTON_HEIGHT); 
    setStyle(raiseButton, BUTTON_CSS);  //when this is clicked show raise slider ui

    //creating call button
    GtkWidget* callButton = gtk_button_new_with_label("CALL"); 
    gtk_box_pack_start(GTK_BOX(buttonBox), callButton, FALSE, FALSE, 5); 
    gtk_widget_set_size_request(callButton, BUTTON_WIDTH, BUTTON_HEIGHT); 
    setStyle(callButton, BUTTON_CSS);

    //display pot? 

    gtk_widget_show_all(pokerGui->Window);  
}