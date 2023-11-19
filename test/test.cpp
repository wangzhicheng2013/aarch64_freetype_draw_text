#include "qnx_text_draw.hpp"
#include <fcntl.h>
#define WIDTH   80
#define HEIGHT  80
 
 
/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];
/* Replace this function with something useful. */
void draw_bitmap(FT_Bitmap*  bitmap,
				 FT_Int      x,
				 FT_Int      y) {
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
  	FT_Int  y_max = y + bitmap->rows;
	for (i = x, p = 0; i < x_max; i++, p++ ) {
		for ( j = y, q = 0; j < y_max; j++, q++ ) {
			if ( i < 0      || j < 0       ||
           i >= WIDTH || j >= HEIGHT )
        continue;
 
      image[j][i] |= bitmap->buffer[q * bitmap->width + p];
	  // mask_[width_ * j + i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}
void
show_image( void )
{
  int  i, j;
 
 
  for ( i = 0; i < HEIGHT; i++ )
  {
    for ( j = 0; j < WIDTH; j++ )
      putchar( image[i][j] == 0 ? ' '
                                : image[i][j] < 128 ? '+'
                                                    : '*' );
    putchar( '\n' );
  }
}
void uyvy_draw(unsigned char *pic, 
				int pic_w,
				int pic_h, 
				int rect_x, 
				int rect_y, 
				int rect_w,
				int rect_h,
				int R,
				int G,
				int B) {
	/* RGB convert YUV */
	int Y =  0.299  * R + 0.587  * G + 0.114  * B;
	int U = -0.1687 * R + 0.3313 * G + 0.5    * B + 128;
	int V =  0.5    * R - 0.4187 * G - 0.0813 * B + 128;
	/* Locking the scope of rectangle border range */
	int j = 0, k = 0;
	int n = 0;
	int y_index = 0;
	for (j = 0;j < HEIGHT;j++) {
		for (k = 0;k < WIDTH;k++) {
				/* Components of YUV's storage address index */
				if (0 == image[j][k]) continue;
				n = j * pic_w + k;
				y_index = (n << 1) - 1;
				pic[y_index] = Y;
				if (1 == y_index % 4) {
					pic[y_index - 1] = U;
					pic[y_index + 1] = V;
				}
		}
	}
}
int test_uyvy() {
    const int pic_w  = 1600;
    const int pic_h  = 1300;
    const int dest_pic_w  = pic_w;
    const int dest_pic_h  = pic_h;

    /* Set up test data's path and output data's path */
    const char *inputPathname  = "./UYVY_946684877_1_1600x1300.uyvy";
    const char *outputPathname = "./UYVY_946684877_2_1600x1300.uyvy";
    FILE *fin  = fopen(inputPathname , "rb+");
    if (!fin) {
        return -1;
    }
    FILE *fout = fopen(outputPathname, "wb+");
    if (!fout) {
        fclose(fin);
        return -1;
    }
    size_t size = pic_w * pic_h * 2;
    size_t dest_size = dest_pic_w * dest_pic_h * 2;
    /* Allocate memory for uyvy */
    unsigned char *src_img = (unsigned char *)malloc(size * sizeof(unsigned char));
    /* Read file data to buffer */
    fread(src_img, size, 1, fin);
    fclose(fin);
	uyvy_draw(src_img, pic_w, pic_h, 0, 0, WIDTH, HEIGHT, 255, 0, 0);
	fwrite(src_img, dest_size, 1, fout);
    /* Write data of buf to fout */
   
    /* Close the file */
    fclose(fout);
    return 0;        
}
int main(int     argc,
      char**  argv) {
	FT_Library    library;
  FT_Face       face;
 
  FT_GlyphSlot  slot;
  FT_Matrix     matrix;                 /* transformation matrix */
  FT_Vector     pen;                    /* untransformed origin  */
  FT_Error      error;
 
  char*         filename;
  char*         text;
 
  double        angle;
  int           target_height;
  int           n, num_chars;
 if ( argc != 3 )
  {
    fprintf ( stderr, "usage: %s font sample-text\n", argv[0] );
    exit( 1 );
  }
 
  filename      = argv[1];                           /* first argument     */
  text          = argv[2];                           /* second argument    */
  num_chars     = strlen( text );
  angle         = ( 0.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
  target_height = HEIGHT;
   error = FT_Init_FreeType( &library );              /* initialize library */
  /* error handling omitted */
 
  error = FT_New_Face( library, argv[1], 0, &face ); /* create face object */
  /* error handling omitted */
 
#if 0
  /* use 50pt at 100dpi */
  error = FT_Set_Char_Size( face, 50 * 64, 0,
                            100, 0 );                /* set character size */
 
	/* pixels = 50 /72 * 100 = 69  */
#else
	FT_Set_Pixel_Sizes(face, 14, 0);
	//FT_Set_Pixel_Sizes(face_, font_size_, font_size_);
#endif
  /* error handling omitted */
 
  slot = face->glyph;
 
  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (0,40) relative to the upper left corner  */
  pen.x = 0 * 64;
  pen.y = ( target_height - 40 ) * 64;
  //pen.y = (target_height - (font_size_ * 7) / 10) * 32;
 
  for ( n = 0; n < num_chars; n++ )
  {
    /* set transformation */
    FT_Set_Transform( face, &matrix, &pen );
 
    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
    if ( error )
      continue;                 /* ignore errors */
 
    /* now, draw to our target surface (convert position) */
    draw_bitmap( &slot->bitmap,
                 slot->bitmap_left,
                 target_height - slot->bitmap_top );
 
    /* increment pen position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }
 
  show_image();
  FT_Done_Face    ( face );
  FT_Done_FreeType( library );
  test_uyvy();

	return 0;
}
