
#include "render.h"
#include "gui.h"

const char* CARDS_RESOURCES[MAX_CARDS] = {
    // Hearts
    "resources/ace_of_hearts.png",
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

    // Diamonds
    "resources/ace_of_diamonds.png",
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

    // Clubs
    "resources/ace_of_clubs.png",
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

    // Spades
    "resources/ace_of_spades.png",
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

const double TABLE_WIDTH_PERCENTAGE = 0.70; 
const double TABLE_HEIGHT_PERCENTAGE = 0.55; 

#define MAX_PLAYER_CARDS 2
#define MAX_DEALER_CARDS 5
#define BACK_CARD_INDEX 56


typedef cairo_surface_t Icon; 

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
    //uploading the back of card - temp manual solu 
    imgs[BACK_CARD_INDEX] = imageToSurface(CARDS_RESOURCES[52]); //currently at 52 but change when we add anteater card to 57
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
    double leftEdge = getTableLeftEdge(pokerTable); 
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
    return getTableWidth(pokerTable) * 0.065;
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
    for(int i = 0; i < numOfCards; i++){ 
        image = getCardImage(images, playerCards[i]); 
        drawImg(cr, image, xPos, yPos, width, height);  
        xPos = gap + getNextPos(xPos, width); 
    }
}


void drawHiddenCards(cairo_t* cr, Icon** images, float xPos, float yPos, float targetW, float targetH){ 
    Icon* cardBack = images[BACK_CARD_INDEX]; 
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

    drawCards(cr, images, MAX_DEALER_CARDS, dealerCards, targetW, targetH, xPos, yPos); 
}

void drawPlayer1Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards)
{ 
    double tableLeft = getTableLeftEdge(pokerTable);
    double tableBot = getTableBotEdge(pokerTable);
    double tableW = getTableWidth(pokerTable);
    double tableH = getTableHeight(pokerTable);

    double cardH = getCardHeight(pokerTable);

    double xCenter = tableLeft + tableW * 0.30;
    double y = tableBot + tableH * 0.08;

    Seat_Info player1 = {xCenter, y}; 
    drawPlayerCards(cr, pokerTable, images, playerCards, &player1); 
}

void drawPlayer2Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards)
{ 
    double tableLeft = getTableLeftEdge(pokerTable);
    double tableBot = getTableBotEdge(pokerTable);
    double tableW = getTableWidth(pokerTable);
    double tableH = getTableHeight(pokerTable);

    double xCenter = tableLeft + tableW * 0.70;
    double y = tableBot + tableH * 0.08;

    Seat_Info player2 = {xCenter, y};  
    drawPlayerCards(cr, pokerTable, images, playerCards, &player2); 
}

void drawPlayer3Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards)
{ 
    double tableRight = getTableRightEdge(pokerTable);
    double tableTop = getTableTopEdge(pokerTable);
    double tableH = getTableHeight(pokerTable);

    double xCenter = tableRight + getCardGroupWidth(pokerTable, 2) / 2.0 + 25;
    double y = tableTop + tableH * 0.50 - getCardHeight(pokerTable) / 2.0;

    Seat_Info player3 = {xCenter, y}; 
    drawPlayerCards(cr, pokerTable, images, playerCards, &player3); 
}

void drawPlayer4Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards)
{ 
    double tableLeft = getTableLeftEdge(pokerTable);
    double tableTop = getTableTopEdge(pokerTable);
    double tableW = getTableWidth(pokerTable);
    double tableH = getTableHeight(pokerTable);

    double cardH = getCardHeight(pokerTable);

    double xCenter = tableLeft + tableW * 0.70;
    double y = tableTop - cardH - tableH * 0.08;

    Seat_Info player4 = {xCenter, y};   
    drawPlayerCards(cr, pokerTable, images, playerCards, &player4); 
}

void drawPlayer5Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards)
{ 
    double tableLeft = getTableLeftEdge(pokerTable);
    double tableTop = getTableTopEdge(pokerTable);
    double tableW = getTableWidth(pokerTable);
    double tableH = getTableHeight(pokerTable);

    double cardH = getCardHeight(pokerTable);

    double xCenter = tableLeft + tableW * 0.30;
    double y = tableTop - cardH - tableH * 0.08;

    Seat_Info player5 = {xCenter, y}; 
    drawPlayerCards(cr, pokerTable, images, playerCards, &player5); 
}

void drawPlayer6Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards)
{ 
    double tableLeft = getTableLeftEdge(pokerTable);
    double tableTop = getTableTopEdge(pokerTable);
    double tableH = getTableHeight(pokerTable);

    double xCenter = tableLeft - getCardGroupWidth(pokerTable, 2) / 2.0 - 25;
    double y = tableTop + tableH * 0.50 - getCardHeight(pokerTable) / 2.0;

    Seat_Info player6 = {xCenter, y}; 
    drawPlayerCards(cr, pokerTable, images, playerCards, &player6); 
}



Icon* getCardImage(Icon** images, Card card){  //will deal with anteater cards later... cause idk what the anteater is
    int suit = card.suit; //
    int index = suit * NUM_OF_RANKS + card.rank - 1; 
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
    cairo_set_source_rgba(cr, 86/255.0, 134/255.0, 111/255.0, 1);   // dark green
    cairo_fill_preserve(cr);

    //create outer thick rail
    cairo_set_source_rgba(cr, 149/255.0, 121/255.0, 79/255.0, 1);
    cairo_set_line_width(cr, 30.0);
    cairo_stroke(cr);

    //inner shadow 
    double inset = 20; 
    roundedTablePath(cr, x + inset, y + inset, tableW - 2 * inset, tableH - 2 * inset); 
    cairo_set_source_rgba(cr, 58/255.0, 115/255.0, 88/255.0, 1); 
    cairo_set_line_width(cr, 8.0); 
    cairo_stroke(cr);

    
    // Inner thin green outline, slightly inset
    inset = 40.0;

    roundedTablePath(cr,x + inset, y + inset, tableW - 2 * inset, tableH - 2 * inset);

    cairo_set_source_rgba(cr, 108/255.0, 161/255.0, 133/255.0, 1);
    cairo_set_line_width(cr, 3.0);
    cairo_stroke(cr);

    cairo_restore(cr);
}

void drawRoundedBoxPath(cairo_t* cr, double x, double y, double boxWidth, double boxHeight)
{
    double r = 8.0; // corner radius

    cairo_new_path(cr);

    // Start at top-left, after the corner
    cairo_move_to(cr, x + r, y);

    // Top edge
    cairo_line_to(cr, x + boxWidth - r, y);

    // Top-right corner
    cairo_arc(cr, x + boxWidth - r, y + r, r, -G_PI / 2.0, 0);

    // Right edge
    cairo_line_to(cr, x + boxWidth, y + boxHeight- r);

    // Bottom-right corner
    cairo_arc(cr, x + boxWidth - r, y + boxHeight - r, r, 0, G_PI / 2.0);

    // Bottom edge
    cairo_line_to(cr, x + r, y + boxHeight);

    // Bottom-left corner
    cairo_arc(cr, x + r, y + boxHeight - r, r, G_PI / 2.0, G_PI);

    // Left edge
    cairo_line_to(cr, x, y + r);

    // Top-left corner
    cairo_arc(cr, x + r, y + r, r, G_PI, 3.0 * G_PI / 2.0);

    cairo_close_path(cr);
}


double getTextWidth(cairo_t* cr, const char* text){
    cairo_text_extents_t extents;
    cairo_text_extents(cr, text, &extents);
    return extents.width; 
}

double drawText(cairo_t* cr, const char* text, double fontSize, double xPos, double yPos){
    cairo_save(cr); 

    cairo_set_source_rgb(cr, 0.95, 0.90, 0.80);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, fontSize);
    cairo_move_to(cr, xPos, yPos + fontSize);
    cairo_show_text(cr, text);

    double textWidth = getTextWidth(cr, text);
    cairo_restore(cr); 

    return textWidth;  
}

void drawAvatar(cairo_t* cr, Icon* avatarImg, double xPos, double yPos, double avatarWidth, double avatarHeight, double inset){ 
    cairo_save(cr);  
    drawRoundedBoxPath(cr, xPos + inset, yPos + inset, avatarWidth, avatarHeight); 
    cairo_clip(cr);
    drawImg(cr, avatarImg, xPos + inset, yPos+inset, avatarWidth, avatarHeight);  
    cairo_restore(cr);; 
}

double getCenter(double top, double bot){ 
    return (top + bot) * 0.5; 
}

void drawPlayerInfoBox(cairo_t* cr, Poker_Gui* pokerGui, double xPos, double yPos){ //the x and y pos that you want to start drawing at (left corner)
    int borderWidth = 3.0;
    int avatarBorderWidth = borderWidth * 0.5;  

    //loading gui stuff
    GtkWidget* drawArea = pokerGui->pokerTable; 
    Icon** images = pokerGui->images; 

    //loading player stuff
    Player_Info* playerInfo = pokerGui->playerInfo; 
    const char* playerName = playerInfo->name;  
    Icon* avatarImg = playerInfo->avatarImg; 
    Card* playerCards = playerInfo->playerCards; 

    double areaW = gtk_widget_get_allocated_width(drawArea);
    double areaH = gtk_widget_get_allocated_height(drawArea); 
    double boxWidth = areaW * 0.2; 
    double boxHeight = boxWidth * 0.5; 
    double cardW = boxWidth * 0.2; 
    double cardH = cardW * 1.4; 
    double avatarHeight = boxHeight * 0.9;
    double avatarWidth = avatarHeight; 
    double topTextMargin = borderWidth + (boxHeight * 0.05); 
    double leftTextMargin = borderWidth + (boxWidth * 0.05); 

    double playerTextWidth; 
    double chipTextWidth; 

    cairo_save(cr); 

    drawRoundedBoxPath(cr, xPos, yPos, boxWidth, boxHeight);  //xPos and yPos is top left corner

    //fill the box
    cairo_set_source_rgba(cr, 19/255.0, 21/255.0, 28/255.0, 1);  
    cairo_fill_preserve(cr);

    //draw player box border
    cairo_set_source_rgba(cr, 71/255.0, 64/255.0, 81/255.0, 1); 
    cairo_set_line_width(cr, borderWidth); 
    cairo_stroke(cr);   

    //draw the avatar photo 
    double avatarYPos = getCenter(yPos + borderWidth, yPos + boxHeight - borderWidth) - (avatarHeight/2.0); 
    double avatarXPos = xPos + (boxHeight * 0.05); 
    drawAvatar(cr, avatarImg, avatarXPos, avatarYPos, avatarWidth, avatarHeight, 0); 

    //draw avatar border 
    drawRoundedBoxPath(cr, avatarXPos, avatarYPos, avatarWidth, avatarHeight); 
    cairo_set_line_width(cr, borderWidth); 
    cairo_stroke(cr);  

    //add the player name 
    double nameYPos = avatarYPos + topTextMargin;
    double nameXPos = avatarXPos + avatarWidth + leftTextMargin; 
    double playerFontSize = boxHeight * 0.15;  
    playerTextWidth = drawText(cr, playerName, playerFontSize, nameXPos, nameYPos);  


    double chipYPos = nameYPos + boxHeight * 0.3; 
    double chipXPos = avatarXPos + avatarWidth + leftTextMargin; 
    double chipFontSize = boxHeight * 0.08;  
    double chipH = boxHeight * 0.1; 
    double chipW = chipH;

    //draw chip icon
    cairo_save(cr); 
    drawImg(cr, pokerGui->chipIcon, chipXPos, chipYPos, chipW, chipH); 
    cairo_restore(cr);

    //add the player chip count
    chipTextWidth = drawText(cr, "CHIPS", chipFontSize, chipXPos + chipW, chipYPos); 

    //add the cards 
    double cardYPos = getCenter(yPos + borderWidth, yPos + boxHeight - borderWidth) - (cardH/2.0); //center --> make helper function
    double cardXPos = (playerTextWidth > chipTextWidth) ? (nameXPos + playerTextWidth + 5.0) : (chipXPos + chipTextWidth + 5.0); 
    drawCards(cr, images, MAX_PLAYER_CARDS, playerCards, cardW, cardH, cardXPos, cardYPos); 
    
    cairo_restore(cr);
}

gboolean drawPokerTable(GtkWidget *widget, cairo_t *cr, gpointer user_data){
    Poker_Gui* pokerGui = (Poker_Gui*)user_data; 
    Icon** images = pokerGui->images;  
    Icon* avatarImg = pokerGui->avatarImages[0]; 

    //temp solu for testing --> future dealer cards should be passed in from user_data //////////////////////////
    Card card1 = cardCtor(SPADES, ACE); 
    Card card2 = cardCtor(HEARTS, TEN); 
    Card card3 = cardCtor(SPADES, TWO); 
    Card card4 = cardCtor(DIAMONDS, QUEEN); 
    Card card5 = cardCtor(CLUBS, FIVE); 

    Card dealerCards[MAX_DEALER_CARDS] = {card1, card2, card3, card4, card5}; 
    int cardsToDeal = MAX_DEALER_CARDS;  

    ////////////////////////////////////////////////////

    //temp solu for testing --> future player cards should be passed in from user_data //////////////////////////

    card1 = cardCtor(CLUBS, THREE); 
    card2 = cardCtor(HEARTS, FOUR); 

    Card player1Cards[MAX_PLAYER_CARDS] = {card1, card2};  
    ///zrrzy of card arrays --? depending on which player initialize that card array --> rest is auto NULL and thus won't be drawn

    ////////////////////////////////////////////////////

    int areaWidth = gtk_widget_get_allocated_width(widget);
    int areaHeight = gtk_widget_get_allocated_height(widget);
    drawTableOutline(cr, areaWidth, areaHeight);

    //temp solu just for testing 
    /* char* name; 
    int chips; 
    Icon* avatarImg; 
    Card* playerCards; */ 

    *(pokerGui->playerInfo) = (Player_Info){.name = "YOSHI", .chips = 1000, .avatarImg = avatarImg, .playerCards = player1Cards}; 
    drawPlayerInfoBox(cr, pokerGui, 100, 100); //draws the player's box 

    drawDealerCards(widget, cr, images, dealerCards, cardsToDeal); //cards to deal aka what turn
    drawPlayer1Cards(widget, cr, images, player1Cards); 
    drawPlayer2Cards(widget, cr, images, NULL); 
    drawPlayer3Cards(widget, cr, images, player1Cards);
    drawPlayer4Cards(widget, cr, images, NULL); 
    drawPlayer5Cards(widget, cr, images, NULL); 
    drawPlayer6Cards(widget, cr, images, player1Cards);


    //if end turn draw everyone's cards 

    //have to determine what to draw when someone has folded 
    //draw pop ups for when someone calls, checks, raises, goes all in etc 
    //table ?

    //need to add some style to the button box
    //dealer character ? --> w/ dialogue ?

}


//enum --> first to enter into client socket --> aka index 0 --> is player 1