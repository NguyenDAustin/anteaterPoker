
#include "render.h"
#include "gui.h"

const char* CARDS_RESOURCES[MAX_CARDS] = {
    // Hearts
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

    // Diamonds
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

    // Clubs
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

    // Spades
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

    //Back of Card 
    "resources/back_of_card.png"
};

const char* AVATAR_RESOURCES[MAX_PLAYERS] = {
    "resources/avatars/avatar1_img.png",
    "resources/avatars/avatar2_img.png",
    "resources/avatars/avatar3_img.png",
    "resources/avatars/avatar4_img.png",
    "resources/avatars/avatar5_img.png",
    "resources/avatars/avatar6_img.png"
};

//#define MAX_PLAYER_CARDS 2
//#define MAX_DEALER_CARDS 5
//#define BACK_CARD_INDEX 56

#define PLAYER_1 0 
#define PLAYER_2 1 
#define PLAYER_3 2 
#define PLAYER_4 3 
#define PLAYER_5 4
#define PLAYER_6 5

const char* PIXEL_FONT = "VT323";
const char* PIXEL_FONT2 = "Silkscreen"; 

const double TABLE_WIDTH_PERCENTAGE = 0.70; 
const double TABLE_HEIGHT_PERCENTAGE = 0.55; 

const Color TURN_HIGHLIGHT_COLOR = {.r = 227/255.0, .g = 161/255.0, .b = 59/255.0}; 
const Color CHIP_COUNT_TEXT_COLOR = {.r = 233/255.0, .g = 89/255.0, .b = 75/255.0};             //bright red color
const Color PLAYER_TEXT_COLOR = {.r = 241/255.0, .g = 230/255.0, .b = 207/255.0};               //light yellow color
const Color POT_COLOR = {.r = 108/255.0, .g = 161/255.0, .b = 133/255.0};                       //light green color
const Color TABLE_INNER_OUTLINE_COLOR = {.r = 108/255.0, .g = 161/255.0, .b = 133/255.0};       //light green color
const Color TABLE_COLOR = {.r = 86/255.0, .g = 134/255.0, .b = 111/255.0};                      //saturated green color 
const Color TABLE_RAIL_COLOR = {.r = 149/255.0, .g = 121/255.0, .b = 79/255.0};                 //wooden color 
const Color TABLE_INNER_SHADOW_COLOR = {.r = 58/255.0, .g = 115/255.0, .b = 88/255.0};          //darker green color
const Color PLAYER_BOX_COLOR = {.r = 19/255.0, .g = 21/255.0, .b = 28/255.0}; 
const Color PLAYER_BOX_BORDER_COLOR = {.r = 71/255.0, .g = 64/255.0, .b = 81/255.0}; 
const Color AVATAR_BOX_BORDER_COLOR = {.r = 109/255, .g = 100/255.0, .b = 123/255.0};



typedef cairo_surface_t Icon; 


double getRed(const Color color){  
    return color.r; 
}

double getGreen(const Color color){ 
    return color.g; 
}

double getBlue(const Color color){ 
    return color.b; 
}

Seat_Info seatInfoCtor(double xPos, double yPos){  
    Seat_Info seatInfo; 
    seatInfo.xPos = xPos; 
    seatInfo.yPos = yPos; 
    return seatInfo; 
}


Icon* imageToSurface(const char *filename){
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, &error);

    if (pixbuf == NULL) {
        fprintf(stderr, "Failed to load image %s: %s\n", filename, error->message);
        g_error_free(error);
        return NULL;
    }

    Icon* surface = gdk_cairo_surface_create_from_pixbuf(pixbuf,0, NULL);

    g_object_unref(pixbuf);
    return surface;
}



void createImages(Icon** imgs, const char** resources, int numOfImgs){
    for(int i = 0; i < numOfImgs; i++){ 
        imgs[i] = imageToSurface(resources[i]); 

        if(imgs[i] == NULL) 
            printf("ERROR: failed to load resource - %s\n", resources[i]); 
    }  
}

void createCardImages(Icon** imgs, int numOfImgs){
    createImages(imgs, CARDS_RESOURCES, numOfImgs);
}

void createAvatarImages(Icon** imgs){ 
    createImages(imgs, AVATAR_RESOURCES, MAX_PLAYERS); 
}


void scale(cairo_t *cr, Icon *img, float targetXSize, float targetYSize)
{ // scales image to the desired size
  int imgWidth = cairo_image_surface_get_width(img);
  int imgHeight = cairo_image_surface_get_height(img);
  double yScaleFactor = targetYSize / (float)imgHeight;
  double xScaleFactor = targetXSize / (float)imgWidth;
  cairo_scale(cr, xScaleFactor, yScaleFactor);
}


void drawImg(cairo_t* cr, Icon *img, float xPos, float yPos, float targetW, float targetH)
{
    if(!img){
        printf("ERROR: drawImg -  img is NULL\n"); 
        return; 
    } 

    cairo_save(cr);
    cairo_translate(cr, xPos,  yPos);  //center
    scale(cr, img, targetW, targetH); 
    cairo_set_source_surface(cr, img, 0, 0);
    cairo_paint(cr);
    cairo_restore(cr);
}


int getNextPos(int xPos, int imageWidth){ 
    xPos += imageWidth; 
    return xPos; 
}

double getTableWidth(GtkWidget* pokerTable){ 
    double areaW = gtk_widget_get_allocated_width(pokerTable); //total area width
    return areaW * TABLE_WIDTH_PERCENTAGE; 
}

double getTableHeight(GtkWidget* pokerTable){ 
    double areaH = gtk_widget_get_allocated_height(pokerTable); 
    return areaH * TABLE_HEIGHT_PERCENTAGE; 
}

double getTableLeftEdge(GtkWidget* pokerTable){
    double areaW = gtk_widget_get_allocated_width(pokerTable); //total area width
    double tableW = getTableWidth(pokerTable); 
    return (areaW - tableW) / 2.0; 
}

double getTableRightEdge(GtkWidget* pokerTable){ 
    double leftEdge = getTableLeftEdge(pokerTable); //
    double tableW = getTableWidth(pokerTable); 
    return leftEdge + tableW; 
}

double getTableTopEdge(GtkWidget* pokerTable){ 
    double areaH = gtk_widget_get_allocated_height(pokerTable); 
    double tableH = getTableHeight(pokerTable); 
    return ((areaH - tableH) /  2.0); 
}

double getTableBotEdge(GtkWidget* pokerTable){ 
    double topEdge = getTableTopEdge(pokerTable); 
    double tableH = getTableHeight(pokerTable); 
    return topEdge + tableH; 
}

double getCardWidth(GtkWidget *pokerTable)
{
   return getTableWidth(pokerTable) * 0.03; 
}

double getCardHeight(GtkWidget *pokerTable)
{
   return getCardWidth(pokerTable) * 1.4;
}

double getDealerCardWidth(GtkWidget* pokerTable){ 
    return getTableWidth(pokerTable) * 0.085; 
}

double getDealerCardHeight(GtkWidget* pokerTable){ 
    return getDealerCardWidth(pokerTable) * 1.4; 
}

double getCardGap(GtkWidget *pokerTable)
{
    return getCardWidth(pokerTable) * 0.15;
}

double getCardGroupWidth(GtkWidget *pokerTable, int numCards)
{
    double cardW = getCardWidth(pokerTable);
    double gap = getCardGap(pokerTable);

    return numCards * cardW + (numCards - 1) * gap;
}


void drawCards(cairo_t* cr, Icon** images, int numOfCards, Card* playerCards, double width, double height, double xPos, double yPos){ 
    Icon* image; 
    double gap = 1.5; 

    if(!images){ 
        printf("ERROR: images is NULL\n"); 
        return; 
    }
    
    if(!playerCards){ 
        drawHiddenCards(cr, images, xPos, yPos, width, height); 
        return; 
    }


    for(int i = 0; i < numOfCards; i++){ 
        image = getCardImage(images, playerCards[i]); 
        drawImg(cr, image, xPos, yPos, width, height);  
        xPos = gap + getNextPos(xPos, width); 
    }
}


void drawHiddenCards(cairo_t* cr, Icon** images, float xPos, float yPos, float targetW, float targetH){ 
    Icon* cardBack = images[BACK_CARD_INDEX]; 
    double gap = 1.5; 
    for(int i = 0; i < MAX_PLAYER_CARDS; i++){ 
        drawImg(cr, cardBack, xPos, yPos, targetW, targetH); 
        xPos = getNextPos(xPos, targetW); 
    }
}


void drawPlayerCards(cairo_t *cr, GtkWidget *pokerTable, Icon **images, Card *playerCards, Seat_Info *seat)
{
    double cardW = getCardWidth(pokerTable);
    double cardH = getCardHeight(pokerTable);
    double gap = getCardGap(pokerTable);

    double totalW = getCardGroupWidth(pokerTable, MAX_PLAYER_CARDS);

    double x = seat->xPos - totalW / 2.0;
    double y = seat->yPos;

    
    if(!playerCards){ 
        drawHiddenCards(cr, images, x, y, cardW, cardH); 
        return; 
    }

    drawCards(cr, images, MAX_PLAYER_CARDS, playerCards, cardW, cardH, x, y); 
}


//given a card array draw those cards
void drawDealerCards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* dealerCards, int cardsToDeal){ 
    Icon* image; 

    float currWidth = gtk_widget_get_allocated_width(pokerTable); //total area width
    float currHeight = gtk_widget_get_allocated_height(pokerTable); 
    float targetW = getDealerCardWidth(pokerTable);  
    float targetH = getDealerCardHeight(pokerTable); 

    float xPos = (float)currWidth / 2.0 - (cardsToDeal * targetW/2.0); 
    float yPos = (float)currHeight / 2.0 - targetW/2.0; 

    drawCards(cr, images, cardsToDeal, dealerCards, targetW, targetH, xPos, yPos); 
}

double getTableVerticalPadding(GtkWidget* pokerTable){ 
    double tableH = getTableHeight(pokerTable); 
    return (tableH * 0.2); 
}

double getTableHorizontalPadding(GtkWidget* pokerTable){ 
    double tableW = getTableWidth(pokerTable);
    return (tableW * 0.3); 
}



Icon* getCardImage(Icon** images, Card card){  //will deal with anteater cards later... cause idk what the anteater is
    int suit = card.suit; //
    int rank = card.rank; 
    int index = suit * NUM_OF_RANKS + rank - 1; 

    if(suit == ANTEATER_SUIT || rank == ANTEATER){ 
        return images[0]; //the first card should be an anteater card
    }
    return images[index]; 
}


void roundedTablePath(cairo_t *cr, double x, double y, double w, double h)
{
    double radius = h / 2.0;

    cairo_new_path(cr);

    cairo_move_to(cr, x + radius, y);
    cairo_line_to(cr, x + w - radius, y);

    cairo_arc(cr,x + w - radius, y + radius, radius, -G_PI / 2.0, G_PI / 2.0);

    cairo_line_to(cr, x + radius, y + h);

    cairo_arc(cr, x + radius, y + radius, radius, G_PI / 2.0, 3.0 * G_PI / 2.0);

    cairo_close_path(cr);
}

void drawTableOutline(cairo_t *cr, int areaW, int areaH)
{
    double tableW = areaW * TABLE_WIDTH_PERCENTAGE;
    double tableH = areaH * TABLE_HEIGHT_PERCENTAGE;

    double x = (areaW - tableW) / 2.0;
    double y = (areaH - tableH) / 2.0;

    cairo_save(cr);


    // Outer thick rail path
    roundedTablePath(cr, x, y, tableW, tableH); 

    //fill table
    cairo_set_source_rgba(cr, getRed(TABLE_COLOR), getGreen(TABLE_COLOR), getBlue(TABLE_COLOR), 1); 
    cairo_fill_preserve(cr);

    //create outer thick rail
    cairo_set_source_rgba(cr, getRed(TABLE_RAIL_COLOR), getGreen(TABLE_RAIL_COLOR), getBlue(TABLE_RAIL_COLOR), 1);
    cairo_set_line_width(cr, 30.0);
    cairo_stroke(cr);

    //inner shadow 
    double inset = 20; 
    roundedTablePath(cr, x + inset, y + inset, tableW - 2 * inset, tableH - 2 * inset); 
    cairo_set_source_rgba(cr, getRed(TABLE_INNER_SHADOW_COLOR), getGreen(TABLE_INNER_SHADOW_COLOR), getBlue(TABLE_INNER_SHADOW_COLOR), 1); 
    cairo_set_line_width(cr, 8.0); 
    cairo_stroke(cr);

    // Inner thin green outline, slightly inset
    inset = 40.0;
    roundedTablePath(cr,x + inset, y + inset, tableW - 2 * inset, tableH - 2 * inset);
    cairo_set_source_rgba(cr, getRed(TABLE_INNER_OUTLINE_COLOR), getGreen(TABLE_INNER_OUTLINE_COLOR), getBlue(TABLE_INNER_OUTLINE_COLOR), 1);
    cairo_set_line_width(cr, 3.0);
    cairo_stroke(cr);

    cairo_restore(cr);
}

double getTextWidth(cairo_t* cr, const char* fontFace, const char* text, double fontSize){ 
    double width; 
    cairo_text_extents_t extents;
    cairo_save(cr); 

    cairo_select_font_face(cr, fontFace, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, fontSize);
    cairo_text_extents(cr, text, &extents);
    width = extents.width; 

    cairo_restore(cr); 
    return width; 
}

void drawText(cairo_t* cr, const Color textColor, const char* text, double fontSize, double xPos, double yPos){ //add a color field
    cairo_save(cr); 

    cairo_set_source_rgb(cr, getRed(textColor), getGreen(textColor), getBlue(textColor));
    cairo_select_font_face(cr, PIXEL_FONT, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, fontSize);
    cairo_move_to(cr, xPos, yPos + fontSize);
    cairo_show_text(cr, text);

    cairo_restore(cr); 
}

double getCenter(double top, double bot){ 
    return (top + bot) * 0.5; 
}

void drawBorder(cairo_t* cr, Color borderColor, double borderWidth, double width, double height, double xPos, double yPos){
    cairo_save(cr);
    drawRoundedBoxPath(cr, xPos, yPos, width, height);  
    cairo_set_source_rgba(cr, getRed(borderColor), getGreen(borderColor), getBlue(borderColor), 1); 
    cairo_set_line_width(cr, borderWidth); 
    cairo_stroke(cr);  
    cairo_restore(cr); 
}

void drawTurnHighlight(cairo_t* cr, GtkWidget* drawArea, Poker_Gui* pokerGui, double xPos, double yPos){
    double width = 2.0; 
    double spacing = 3.0; 
    drawBorder(cr, PLAYER_TEXT_COLOR, width, getPlayerBoxWidth(getPokerTable(pokerGui)) + spacing, getPlayerBoxHeight(getPokerTable(pokerGui)) + spacing, xPos, yPos); 
}


void drawPot(cairo_t* cr, Poker_Gui* pokerGui){ 
    GtkWidget* pokerTable = getPokerTable(pokerGui);
    GameState* gameState = getGameState(pokerGui);
    double tableH = getTableHeight(pokerTable); 
    double targetH = tableH * 0.08; 
    double targetW =  getTableWidth(pokerTable) * 0.15; 

    double textWidth = getTextWidth(cr, PIXEL_FONT, "POT", targetH); 

    double yPos = getTableTopEdge(pokerTable) + (tableH * 0.1);
    double xPos = getCenter(getTableLeftEdge(pokerTable), getTableRightEdge(pokerTable)) - (textWidth / 2.0); 


    //drawing pot label
    drawText(cr, POT_COLOR, "POT", targetH, xPos, yPos);  

    //drawing pot amount
    char pot[MAX_NUMBER_LENGTH];
    moneyBuilder(pot, getPot(gameState));
    double potFontSize = targetH * 0.85; 
    textWidth = getTextWidth(cr, PIXEL_FONT, pot, potFontSize); 
    double potXPos = getCenter(getTableLeftEdge(pokerTable), getTableRightEdge(pokerTable)) - (textWidth/2.0); 
    double potYPos = yPos + tableH * 0.1; 
    drawText(cr, POT_COLOR, pot, potFontSize, potXPos, potYPos); 
}


double getPBoxHorizPadding(GtkWidget* pokerTable){ 
    return getTableWidth(pokerTable) * 0.2; 
}

double getPBoxVertPadding(GtkWidget* pokerTable){ 
    return getTableHeight(pokerTable) * 0.25; 
}


Seat_Info getPlayer1Seat(GtkWidget* pokerTable){ 
    double playerBoxWidth = getPlayerBoxWidth(pokerTable); 
    double playerBoxHeight = getPlayerBoxHeight(pokerTable); 

    double xPos = getTableLeftEdge(pokerTable) + getPBoxHorizPadding(pokerTable) - (playerBoxWidth/2.0); 
    double yPos = getTableBotEdge(pokerTable) + getPBoxVertPadding(pokerTable) - (playerBoxHeight/2.0);   

    Seat_Info seatInfo = {.xPos = xPos, .yPos = yPos}; 
    return seatInfo; 
}

Seat_Info getPlayer2Seat(GtkWidget* pokerTable){
    double playerBoxWidth = getPlayerBoxWidth(pokerTable); 
    double playerBoxHeight = getPlayerBoxHeight(pokerTable); 

    double xPos = getTableRightEdge(pokerTable) - getPBoxHorizPadding(pokerTable) - (playerBoxWidth/2.0); 
    double yPos = getTableBotEdge(pokerTable) + getPBoxVertPadding(pokerTable) - (playerBoxHeight/2.0); 

    Seat_Info seatInfo = {.xPos = xPos, .yPos = yPos}; 
    return seatInfo; 
}


Seat_Info getPlayer3Seat(GtkWidget* pokerTable){ 
    double playerBoxWidth = getPlayerBoxWidth(pokerTable); 
    double playerBoxHeight = getPlayerBoxHeight(pokerTable); 

    double xPos = getTableRightEdge(pokerTable) - playerBoxWidth/2.0; 
    double yPos = getCenter(getTableTopEdge(pokerTable), getTableBotEdge(pokerTable)) - playerBoxHeight/2.0; 

    Seat_Info seatInfo = {.xPos = xPos, .yPos = yPos}; 
    return seatInfo; 
}

Seat_Info getPlayer4Seat(GtkWidget* pokerTable){ 
    double playerBoxWidth = getPlayerBoxWidth(pokerTable);  
    double playerBoxHeight = getPlayerBoxHeight(pokerTable); 

    double xPos = getTableRightEdge(pokerTable) - getPBoxHorizPadding(pokerTable) - playerBoxWidth/2.0; 
    double yPos = getTableTopEdge(pokerTable) - getPBoxVertPadding(pokerTable)  - playerBoxHeight/2.0; 

    Seat_Info seatInfo = {.xPos = xPos, .yPos = yPos}; 
    return seatInfo;
}

Seat_Info getPlayer5Seat(GtkWidget* pokerTable){ 
    double playerBoxWidth = getPlayerBoxWidth(pokerTable); 
    double playerBoxHeight = getPlayerBoxHeight(pokerTable); 

    double xPos = getTableLeftEdge(pokerTable) + getPBoxHorizPadding(pokerTable) - playerBoxWidth/2.0;  //and then subtract the width of my playerBox?
    double yPos = getTableTopEdge(pokerTable) - getPBoxVertPadding(pokerTable) - playerBoxHeight/2.0; 
    
    Seat_Info seatInfo = {.xPos = xPos, .yPos = yPos}; 
    return seatInfo;
}

Seat_Info getPlayer6Seat(GtkWidget* pokerTable){
    double playerBoxWidth = getPlayerBoxWidth(pokerTable); 
    double playerBoxHeight = getPlayerBoxHeight(pokerTable); 

    double xPos = getTableLeftEdge(pokerTable) - playerBoxWidth/2.0; 
    double yPos = getCenter(getTableTopEdge(pokerTable), getTableBotEdge(pokerTable)) - playerBoxHeight/2.0; 

    Seat_Info seatInfo = {.xPos = xPos, .yPos = yPos}; 
    return seatInfo;
}

void intializePlayerSeats(GtkWidget* pokerTable, Seat_Info* seats){
    seats[PLAYER_1] = getPlayer1Seat(pokerTable); 
    seats[PLAYER_2] = getPlayer2Seat(pokerTable); 
    seats[PLAYER_3] = getPlayer3Seat(pokerTable); 
    seats[PLAYER_4] = getPlayer4Seat(pokerTable); 
    seats[PLAYER_5] = getPlayer5Seat(pokerTable); 
    seats[PLAYER_6] = getPlayer6Seat(pokerTable); 
}

void drawCurrentBetBox(cairo_t* cr, const Poker_Gui* pokerGui, double areaWidth, double areaHeight){
    char* message[100]; 
    GameState* gameState = getGameState(pokerGui);
    Player_Info* currPlayer = getPlayerInfo(gameState, pokerGui->playerNum - 1); 
    
    moneyBuilder(message, gameState->currentBet - currPlayer->currentBet); //changed this call
    double betBoxW = areaWidth * 0.1;  
    double betBoxH = betBoxW * 0.35; 
    double fontSize = betBoxH * 0.35;  //ok this is good
    double textWidth = getTextWidth(cr, PIXEL_FONT, message, fontSize); 
    double xPos = getCenter(0, areaWidth) - betBoxW/2.0; 
    double yPos = areaHeight * 0.1 - betBoxH/2.0;  

    double betFontXPos = getCenter(xPos, xPos + betBoxW) - getTextWidth(cr, PIXEL_FONT, "CURRENT BET", fontSize)/2.0; 
    double betFontYPos = yPos + (betBoxH * 0.125); 
    double fontXPos = getCenter(xPos, xPos + betBoxW) - textWidth/2.0; 
    double fontYPos = betFontYPos + fontSize + 1.5;  
    drawRoundedBoxPath(cr, xPos, yPos, betBoxW, betBoxH);
    cairo_set_source_rgba(cr, getRed(PLAYER_BOX_COLOR), getGreen(PLAYER_BOX_COLOR), getBlue(PLAYER_BOX_COLOR), 1);  
    cairo_fill_preserve(cr);
    drawText(cr, PLAYER_TEXT_COLOR, "- TO CALL -", fontSize, betFontXPos, betFontYPos); 
    drawText(cr, PLAYER_TEXT_COLOR, message, fontSize, fontXPos, fontYPos); 

}

gboolean drawPokerTable(GtkWidget *widget, cairo_t *cr, gpointer user_data){ //in future game state will be read from user data!!
    Poker_Gui* pokerGui = (Poker_Gui*)user_data; 
    Icon** images = pokerGui->images;  
    GameState* gameState = getGameState(pokerGui); 
    int currPlayerIndex = pokerGui->playerNum - 1; 
    Player_Info* currPlayer = getPlayerInfo(gameState, currPlayerIndex); 

    parseGameStateMessage(pokerGui->stateMsg, gameState);  //this should update game state properly

    Card* dealerCards = getDealerCards(gameState); 
    int areaWidth = gtk_widget_get_allocated_width(widget);
    int areaHeight = gtk_widget_get_allocated_height(widget);
    drawTableOutline(cr, areaWidth, areaHeight);

    Round round = getRound(gameState); 
    int cardsToDeal; 

    if(round == ROUND_PRE_FLOP){ 
        cardsToDeal = 0; 
    }
    else if(round == ROUND_FLOP){ 
        cardsToDeal = 3; 
    }
    else if(round == ROUND_TURN){ 
        cardsToDeal = 4; 
    }
    else if(round == ROUND_RIVER){ 
        cardsToDeal = 5; 
    }
    else if(round == ROUND_SHOWDOWN){ // draw a button to wait until next round 
        cardsToDeal = 5; 
    }
    else{
        cardsToDeal = 0; 
    }

    //build the current bet amount
    drawCurrentBetBox(cr, pokerGui, areaWidth, areaHeight); 

    drawPot(cr, pokerGui); 
    drawDealerCards(widget, cr, images, dealerCards, cardsToDeal); //cards to deal aka what turn
    int totalBet = gameState->currentBet - currPlayer->currentBet;
    int chipRaise = (getChipCount(currPlayer) - totalBet); 
    int maxChipRaise = (chipRaise > 0) ? chipRaise : 0;   
    gtk_range_set_range(GTK_RANGE(getRaiseSlider(pokerGui)), 0, maxChipRaise); //set scale to proper chip count for player 

    //drawing player boxes 
    Seat_Info seats[MAX_PLAYERS]; 
    intializePlayerSeats(widget, seats);
    drawPlayerBoxes(cr, widget, pokerGui, seats); 

    int currentPlayerIndex = getCurrentPlayerIndex(gameState); 
    printf("CURR PLAYER INDEX %d", currentPlayerIndex);
    drawTurnHighlight(cr, widget, pokerGui, seats[currentPlayerIndex].xPos, seats[currentPlayerIndex].yPos); 
    printf("finished drawing poker table\n");

    //draw pop ups for when someone calls, checks, raises, goes all in etc 
} //gui for server side, 