
#include "render.h"

const char* CARDS_RESOURCES[MAX_CARDS] = {"resources/jack_of_hearts.png"};

typedef cairo_surface_t Icon; 

Icon* imageToSurface(const char *filename)
{
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, &error);

    if (pixbuf == NULL) {
        fprintf(stderr, "Failed to load image %s: %s\n", filename, error->message);
        g_error_free(error);
        return NULL;
    }

    cairo_surface_t *surface = gdk_cairo_surface_create_from_pixbuf(pixbuf,0, NULL);

    g_object_unref(pixbuf);
    return surface;
}

void createImages(cairo_surface_t **imgs, int numOfImgs)
{
    for(int i = 0; i < numOfImgs; i++){ 
        imgs[i] = imageToSurface(CARDS_RESOURCES[i]); 

        if(imgs[i] == NULL) 
        printf("ERROR: failed to load resource - %s\n", CARDS_RESOURCES[i]); 
    }
}


void scale(cairo_t *cr, Icon *img, float targetSize)
{ // scales piece to the desired chess square size
  int imgWidth = cairo_image_surface_get_width(img);
  int imgHeight = cairo_image_surface_get_height(img);
  double yScaleFactor = targetSize / (float)imgHeight;
  double xScaleFactor = targetSize / (float)imgWidth;
  cairo_scale(cr, xScaleFactor, yScaleFactor);
}


void drawImg(cairo_t* cr, Icon *img)
{
  cairo_save(cr);
  //cairo_translate(cr, indexToPix(col) + (SQUARE_SIZE - targetSize) / 2.0, indexToPix(row) + (SQUARE_SIZE - targetSize) / 2.0); // center
 // scale(cr, img, targetSize);
  cairo_set_source_surface(cr, img, 0, 0);
  cairo_paint(cr);
  cairo_restore(cr);
}

gboolean drawPokerTable(GtkWidget *widget, cairo_t *cr, gpointer user_data){
    Icon** images = user_data; 
    drawImg(cr, images[0]); 
}


