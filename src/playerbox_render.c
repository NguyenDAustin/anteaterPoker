#include "playerbox_render.h"
#include "render.h"

//player box dimensions 
#define P_BOX_HEIGHT_PERCENTAGE 0.15 
#define P_BOX_WIDTH_PERCENTAGE 3
#define P_BOX_HORIZ_PADDING 0.05

//avatar box dimensions 

//PLAYER BOX FUNCTIONS
double getPlayerBoxHeight(GtkWidget* pokerTable){ 
    double areaH = gtk_widget_get_allocated_height(pokerTable); 
    return (areaH * P_BOX_HEIGHT_PERCENTAGE);
}

double getPlayerBoxWidth(GtkWidget* pokerTable){  
    return getPlayerBoxHeight(pokerTable) * P_BOX_WIDTH_PERCENTAGE; 
}

double getPlayerBoxHorizPadding(GtkWidget* pokerTable){ 
    return getPlayerBoxHeight(pokerTable) * P_BOX_HORIZ_PADDING; 
}

//AVATAR BOX FUNCTIONS

double getAvatarHeight(GtkWidget* pokerTable){ 
    return getPlayerBoxHeight(pokerTable) * 0.85; 
}

double getAvatarWidth(GtkWidget* pokerTable){ 
    return getAvatarHeight(pokerTable); 
}

void drawRoundedBoxPath(cairo_t* cr, double x, double y, double boxWidth, double boxHeight)
{
    double r = 8.0; // corner radius

    cairo_new_path(cr);
    cairo_move_to(cr, x + r, y); // Start at top left, after the corner
    cairo_line_to(cr, x + boxWidth - r, y);  // Top edge
    cairo_arc(cr, x + boxWidth - r, y + r, r, -G_PI / 2.0, 0); // Top right corner
    cairo_line_to(cr, x + boxWidth, y + boxHeight- r); // Right edge
    cairo_arc(cr, x + boxWidth - r, y + boxHeight - r, r, 0, G_PI / 2.0); // Bottom right corner
    cairo_line_to(cr, x + r, y + boxHeight); // Bottom edge
    cairo_arc(cr, x + r, y + boxHeight - r, r, G_PI / 2.0, G_PI); // Bottom left corner
    cairo_line_to(cr, x, y + r);  // Left edge
    cairo_arc(cr, x + r, y + r, r, G_PI, 3.0 * G_PI / 2.0);  // Top left corner
    cairo_close_path(cr);
}

void drawAvatar(cairo_t* cr, Icon* avatarImg, double xPos, double yPos, double avatarWidth, double avatarHeight, double inset){ 
    cairo_save(cr);  
    drawRoundedBoxPath(cr, xPos + inset, yPos + inset, avatarWidth, avatarHeight); 
    cairo_clip(cr);
    drawImg(cr, avatarImg, xPos + inset, yPos+inset, avatarWidth, avatarHeight);  
    cairo_restore(cr);; 
}

void moneyBuilder(char* text, int chipCount){ //123456 --> 123456 //     ,456   // ,345 
    char number[MAX_NUMBER_LENGTH];
    snprintf(number, sizeof(number), "%d", abs(chipCount));

    int digits = strlen(number); //102000 --> 6 
    int commas = (digits - 1)/3; //1
    int backIndex = digits + commas - 1; //6 
    int count = 0; 
    int append; 
    
    for(int i = backIndex; i >= 0; i--, count++){ //123456  
        if(count == 3){
            text[i] = ',';
            count = -1; 
        }
        else{
            append = chipCount % 10;  //     02,000
            text[i] = append + '0'; 
            chipCount /= 10; 
        }
    }
    text[backIndex + 1] = '\0';
}

void drawPlayerInfoBox(cairo_t* cr, GtkWidget* drawArea, Poker_Gui* pokerGui, Player_Info* playerInfo, double xPos, double yPos){ //the x and y pos that you want to start drawing at (left corner)
    printf("drawing player info box\n");
    int borderWidth = 3.0;
    int avatarBorderWidth = borderWidth * 0.5;  

    //loading gui stuff
    Icon** images = getImages(pokerGui);
    const char* playerName = playerInfo->name;
    Icon* avatarImg = playerInfo->avatarImg;
    Card* playerCards = playerInfo->playerCards;

    printf("finished loading all info for player box\n");

    //loading heights and width
    double boxHeight = getPlayerBoxHeight(drawArea);
    double boxWidth = getPlayerBoxWidth(drawArea); 
    double avatarHeight = getAvatarHeight(drawArea);
    double avatarWidth = getAvatarWidth(drawArea); 
    double cardH = boxHeight  * 0.6; 
    double cardW = cardH * 0.71; 
    double chipH = boxHeight * 0.1; 
    double chipW = chipH;

    //loading margins and padding
    double topTextMargin = borderWidth + (boxHeight * 0.05); 
    double leftTextMargin = borderWidth + (boxWidth * 0.05); 
    double sidePadding = boxHeight * 0.055; 

    cairo_save(cr); 

    //fill the box
    drawRoundedBoxPath(cr, xPos, yPos, boxWidth, boxHeight);  //xPos and yPos is top left corner
    cairo_set_source_rgba(cr, getRed(PLAYER_BOX_COLOR), getGreen(PLAYER_BOX_COLOR), getBlue(PLAYER_BOX_COLOR), 1);  
    cairo_fill_preserve(cr);

    //draw player box border
    drawBorder(cr, PLAYER_BOX_BORDER_COLOR, borderWidth, boxWidth, boxHeight, xPos, yPos); 

    //draw the avatar photo + border
    double avatarYPos = getCenter(yPos + borderWidth, yPos + boxHeight - borderWidth) - (avatarHeight/2.0); 
    double avatarXPos = xPos + sidePadding; 
    drawAvatar(cr, avatarImg, avatarXPos, avatarYPos, avatarWidth, avatarHeight, 0); 
    drawBorder(cr, PLAYER_BOX_BORDER_COLOR, 3.0, avatarWidth, avatarHeight, avatarXPos, avatarYPos); 
   
    //add the player name 
    double nameYPos = avatarYPos + topTextMargin;
    double nameXPos = avatarXPos + avatarWidth + leftTextMargin; 
    double playerFontSize = boxHeight * 0.2;   
    drawText(cr, PLAYER_TEXT_COLOR, playerName, playerFontSize, nameXPos, nameYPos);  

    //draw chip icon
    double chipYPos = nameYPos + playerFontSize + boxHeight * 0.1; 
    double chipXPos = avatarXPos + avatarWidth + leftTextMargin;  
    drawImg(cr, pokerGui->chipIcon, chipXPos, chipYPos, chipW, chipH); 

    //add the word chips 
    double chipFontSize = boxHeight * 0.1; 
    drawText(cr, PLAYER_TEXT_COLOR, "CHIPS", chipFontSize, chipXPos + chipW + 3.0, chipYPos); 

    //add the player chip count
    double chipTextYPos = chipYPos + boxHeight * 0.15; 
    double chipTextXPos = chipXPos; 
    double chipTextFontSize = boxHeight * 0.2; 
    char chipCount[MAX_NUMBER_LENGTH]; //build the text
    moneyBuilder(chipCount, playerInfo->chips);
    drawText(cr, CHIP_COUNT_TEXT_COLOR, chipCount, chipTextFontSize, chipTextXPos, chipTextYPos); 

    //add the cards -
    double cardsTotalW = MAX_PLAYER_CARDS * cardW + (MAX_PLAYER_CARDS - 1) * getCardGap(drawArea);
    double cardYPos = getCenter(yPos + borderWidth, yPos + boxHeight - borderWidth) - (cardH/2.0); //center --> make helper function
    double cardXPos = xPos + boxWidth - cardsTotalW - sidePadding;  //card width //double cardsTotalW = MAX_PLAYER_CARDS * cardW + (MAX_PLAYER_CARDS - 1) * cardGap;
    drawCards(cr, images, MAX_PLAYER_CARDS, playerCards, cardW, cardH, cardXPos, cardYPos); 
    
    cairo_restore(cr);
}

void drawPlayerBoxes(cairo_t* cr, GtkWidget* pokerTable, Poker_Gui* pokerGui, Seat_Info* seats){ 
    Icon** avatarImages = getAvatarImages(pokerGui); 
    GameState* gameState = getGameState(pokerGui);

    for(int i = 0; i < MAX_PLAYERS; i++){ //prob will have to change to # of players joined then add something to draw robots
        drawPlayerInfoBox(cr, pokerTable, pokerGui, getPlayerInfo(gameState, i), seats[i].xPos, seats[i].yPos); //probably add some bool to say whether active player or not
    }
}
