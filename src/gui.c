
#include "gui.h"

const char* TITLE = "ANTEATER POKER"; 
const char* CSS = ".poker-bg { background-color: rgb(22, 47, 31);}"; 
const char* POKER_TABLE_CSS = "poker-bg"; 

const int BUTTON_HEIGHT = 25; 
const int BUTTON_WIDTH = 50; 

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

GtkWidget* createPokerTable(Icon** images){ 
    GtkWidget* pokerTable = gtk_drawing_area_new(); 
    
    gtk_widget_set_size_request(pokerTable, WINDOW_WIDTH, WINDOW_HEIGHT); 
    g_signal_connect(pokerTable, "draw", G_CALLBACK(drawPokerTable), images);

    gtk_widget_set_hexpand(pokerTable, FALSE);
    gtk_widget_set_halign(pokerTable, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(pokerTable, GTK_ALIGN_CENTER);
    return pokerTable; 
}

void create_poker_gui(GtkApplication *app, gpointer user_data){

    printf("Creating poker gui\n"); 
    Poker_Gui* pokerGui = g_malloc(sizeof(Poker_Gui));
    pokerGui->Window =  createWindow(app); 
    


    loadCss(pokerGui->Window, CSS);
    
    //creating images
    pokerGui->images = g_malloc(sizeof(Icon*) * MAX_CARDS); 
    createImages(pokerGui->images, 52);  //52 for now because we don't have anteater card png yet

    //creating main container
    GtkWidget *mainBox = createMainContainer(); 
    gtk_container_add(GTK_CONTAINER(pokerGui->Window), mainBox);

    //creating poker table
    GtkWidget* pokerTable = createPokerTable(pokerGui->images); 
    gtk_box_pack_start(GTK_BOX(mainBox), pokerTable, TRUE, TRUE, 0); 


    //creating button container 
    GtkWidget* buttonBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); 
    gtk_box_pack_start(GTK_BOX(mainBox), buttonBox, FALSE, FALSE, 0); 
    gtk_widget_set_halign(buttonBox, GTK_ALIGN_CENTER); 
    gtk_widget_set_margin_bottom(buttonBox, WINDOW_HEIGHT * 0.2); 
    
    //creating fold button 
    GtkWidget* foldButton = gtk_button_new_with_label("FOLD");
    gtk_box_pack_start(GTK_BOX(buttonBox), foldButton, FALSE, FALSE, 5);  
    gtk_widget_set_size_request(foldButton, BUTTON_WIDTH, BUTTON_HEIGHT); 

    //creating raise button
    GtkWidget* raiseButton = gtk_button_new_with_label("RAISE"); 
    gtk_box_pack_start(GTK_BOX(buttonBox), raiseButton, FALSE, FALSE, 5); 
    gtk_widget_set_size_request(raiseButton, BUTTON_WIDTH, BUTTON_HEIGHT); 

    //creating call button
    GtkWidget* callButton = gtk_button_new_with_label("CALL"); 
    gtk_box_pack_start(GTK_BOX(buttonBox), callButton, FALSE, FALSE, 5); 
    gtk_widget_set_size_request(callButton, BUTTON_WIDTH, BUTTON_HEIGHT); 

    //display pot? 

    gtk_widget_show_all(pokerGui->Window);  
}