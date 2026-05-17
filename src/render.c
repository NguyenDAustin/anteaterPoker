
#include "render.h"

/*

typedef enum {
    HEARTS = 0,
    DIAMONDS = 1,
    CLUBS = 2,
    SPADES = 3,
    ANTEATER_SUIT = 4
} Suit;*/

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
    "resources/king_of_spades.png"
};

#define MAX_PLAYER_CARDS 2
#define MAX_DEALER_CARDS 5


typedef cairo_surface_t Icon; 


float getDealerCardSize(){ 
    return WINDOW_WIDTH * 0.15; 
}

float getPlayerCardSize(){ 
    return WINDOW_WIDTH * 0.1; 
}

Icon* imageToSurface(const char *filename)
{
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

void createImages(Icon** imgs, int numOfImgs)
{
    for(int i = 0; i < numOfImgs; i++){ 
        imgs[i] = imageToSurface(CARDS_RESOURCES[i]); 

        if(imgs[i] == NULL) 
        printf("ERROR: failed to load resource - %s\n", CARDS_RESOURCES[i]); 
    }
}


void scale(cairo_t *cr, Icon *img, float targetSize)
{ // scales image to the desired size
  int imgWidth = cairo_image_surface_get_width(img);
  int imgHeight = cairo_image_surface_get_height(img);
  double yScaleFactor = targetSize / (float)imgHeight;
  double xScaleFactor = targetSize / (float)imgWidth;
  cairo_scale(cr, xScaleFactor, yScaleFactor);
}


void drawImg(cairo_t* cr, Icon *img, float xPos, float yPos, float targetSize)
{
    cairo_save(cr);
    cairo_translate(cr, xPos,  yPos);  //center
    scale(cr, img, targetSize); 
    cairo_set_source_surface(cr, img, 0, 0);
    cairo_paint(cr);
    cairo_restore(cr);
}

void drawPlayerCard(cairo_t* cr, Icon* img, int xPos){ 
    float targetSize = getPlayerCardSize();
    float yPos = (float)WINDOW_HEIGHT * 0.8; 
    drawImg(cr, img, xPos, yPos, targetSize); 
}

void drawDealerCard(cairo_t* cr, Icon* img, int xPos){ 
    float targetSize = getDealerCardSize(); 
    float yPos = (float)WINDOW_HEIGHT / 2.0 - targetSize/2.0; 
    drawImg(cr, img, xPos, yPos, targetSize); 
}

int getNextPos(int xPos, int imageWidth){ 
    xPos += imageWidth; 
    return xPos; 
}

void drawPlayerCards(cairo_t* cr, Icon** images, Card* playerCards){
    Icon* image; 

    float targetSize = getPlayerCardSize(); 
    float xPos = (float)WINDOW_WIDTH / 2.0 - (MAX_PLAYER_CARDS * targetSize/2.0); 

    for(int i = 0; i < MAX_PLAYER_CARDS; i++){
        image = getCardImage(images, playerCards[i]); 
        drawPlayerCard(cr, image, xPos); 
        xPos = getNextPos(xPos, targetSize); 
    }
}

Icon* getCardImage(Icon** images, Card card){  //will deal with anteater cards later... cause idk what the anteater is
    int suit = card.suit; //
    int index = suit * NUM_OF_RANKS + card.rank - 1; 
    return images[index]; 
}


//given a card array draw those cards
void drawDealerCards(cairo_t* cr, Icon** images, Card* dealerCards, int cardsToDeal){ 
    Icon* image; 

    float targetSize = getDealerCardSize(); 
    float xPos = (float)WINDOW_WIDTH / 2.0 - (cardsToDeal * targetSize/2.0); 

    for(int i = 0; i < cardsToDeal && i < MAX_DEALER_CARDS; i++){
        image = getCardImage(images, dealerCards[i]);  
        drawDealerCard(cr, image, xPos); 
        xPos = getNextPos(xPos, targetSize);
    }
}

gboolean drawPokerTable(GtkWidget *widget, cairo_t *cr, gpointer user_data){
    Icon** images = user_data; 

    //temp solu for testing --> future dealer cards should be passed in from user_data //////////////////////////
    Card card1 = cardCtor(CLUBS, JACK); 
    Card card2 = cardCtor(DIAMONDS, KING); 
    Card card3 = cardCtor(CLUBS, ACE); 
    Card card4 = cardCtor(SPADES, NINE); 
    Card card5 = cardCtor(HEARTS, FIVE); 

    Card dealerCards[MAX_DEALER_CARDS] = {card1, card2, card3, card4, card5}; 
    int cardsToDeal = MAX_DEALER_CARDS;  

    ////////////////////////////////////////////////////

    //temp solu for testing --> future player cards should be passed in from user_data //////////////////////////

    card1 = cardCtor(HEARTS, NINE); 
    card2 = cardCtor(DIAMONDS, SIX); 

    Card playerCards[MAX_PLAYER_CARDS] = {card1, card2}; 

    ////////////////////////////////////////////////////

    drawDealerCards(cr, images, dealerCards, cardsToDeal); //cards to deal aka what turn
    drawPlayerCards(cr, images, playerCards); 
    

    //if end turn draw everyone's cards 

    //have to determine what to draw when someone has folded 
    //draw pop ups for when someone calls, checks, raises, goes all in etc 
    //table ?

}


