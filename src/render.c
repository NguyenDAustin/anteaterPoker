
#include "render.h"

const char* CARDS_RESOURCES[MAX_CARDS] = {"resources/jack_of_hearts.png"};
#define MAX_PLAYER_CARDS 2
#define MAX_DEALER_CARDS 5


typedef cairo_surface_t Icon; 


float getDealerCardSize(){ 
    return WINDOW_HEIGHT * 0.15; 
}

float getPlayerCardSize(){ 
    return WINDOW_HEIGHT * 0.1; 
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

void drawCard(cairo_t* cr, Icon* img, int xPos, int yPos){ 
    float targetSize = getPlayerCardSize(); 
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

void drawPlayerCards(cairo_t* cr, Icon** cards){
    //x pos , ypos --> ypos doesn't change
    for(int i = 0; i < MAX_PLAYER_CARDS; i++){
        //drawCard(cr, cards[i]); 
    }
}

Icon* getCardImage(Icon** images, Card card){  //will deal with anteater cards later... cause idk what the anteater is
    int suit = card.suit; 
    int index = suit * 4 + card.rank; 
    return images[index]; 
}


//given a card array draw those cards
void drawDealerCards(cairo_t* cr, Icon** images, Card* dealerCards, int cardsToDeal){ 
    Icon* image; 

    float targetSize = getDealerCardSize(); 
    float xPos = (float)WINDOW_WIDTH / 2.0 - (cardsToDeal * targetSize/2.0); 

    for(int i = 0; i < cardsToDeal && i < MAX_DEALER_CARDS; i++){
        //image = getCardImage(dealerCards[i]);  
        //drawCard(cr, images[0], xPos, yPos); 
        drawDealerCard(cr, images[0], xPos); 
        xPos = getNextPos(xPos, targetSize);
    }
    //dealerCards[]
}

gboolean drawPokerTable(GtkWidget *widget, cairo_t *cr, gpointer user_data){
    Icon** images = user_data; 

    Card card1 = cardCtor(HEARTS, JACK); 
    Card card2 = cardCtor(HEARTS, JACK); 
    Card card3 = cardCtor(HEARTS, JACK); 
    Card card4 = cardCtor(HEARTS, JACK); 
    Card card5 = cardCtor(HEARTS, JACK); 

    Card dealerCards[MAX_DEALER_CARDS] = {card1, card2, card3, card4, card5}; //temp solution for testing
    int cardsToDeal = MAX_DEALER_CARDS; 
    drawDealerCards(cr, images, dealerCards, cardsToDeal); //cards to deal aka what turn

    //drawCard(cr, images[0]); 
}


