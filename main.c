/* créer un buffer circulaire.
 
  * ce buffer contiendra des entiers sur 8 bits.
  * Le but est de pouvoir gérer une zone tampon permettant un asynchronisme
  * entre la récupèration d'une donnée et son utilisation.
  * le but du développement et de permettre de gérer simplement 
  * l'introduction d'une donnée et son utilisation
  * cela se fera par l'intermédiaire de variables logiquement contenues dans 
  * structure. 
  * Ces variables seront le head pour le point d'introduction dans le buffer, 
  * tail pour le point de récupèration dans le buffer.

 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#define TAILLE_BUFFER 1024

/* head et tail pourraient être de format size_t.
 * Cela permettrait simplement de créer le mouvement circulaire
 * en utilisant le modulo % TAILLE_BUFFER.
 * L'utilisation des pointeurs est moins appropriée mais ça fonctionne
 * aussi.
 */
 
struct st_buffer {
    uint8_t *buffer;
    uint8_t head;
    uint8_t tail;
    size_t buffer_size;
    bool full;
};

/* size_t est normalement capable de contenir n'importe
  * quelle taille d'objet */

/* envoyer une structure ou un pointeur sur une structure ?
* a un moment donné, il faudra envoyer l'adresse de la structure
* dans une fonction pour que celle-ci puisse modifier les données
* à l'intérieure de la structure...
* Par ailleurs, il serait trés intéressant de conserver l'adresse
* du buffer dans la structure. Dans ce cas on limitera le nombre de paramètres
* d'appel.
* full :
* */
struct st_buffer *buffer_init (uint8_t *buffer, size_t size) {
    
    assert (buffer && size);
// obligé d'allouer sinon warning lors de la compilation.
    struct st_buffer *fbuf = malloc (sizeof(struct st_buffer));
    
    fbuf->buffer = buffer;
    fbuf->head = 0;
    fbuf->tail = 0;
    fbuf->buffer_size = size;
    fbuf->full = false;
    return fbuf;
}


void buffer_put(struct st_buffer *buffer, uint8_t data) {

    assert(buffer);
    
    buffer->buffer[buffer->head] = data;
    
    buffer->head += 1 % TAILLE_BUFFER; 
    buffer->full = buffer->head == buffer->tail;
    
    }
    
int buffer_get (struct st_buffer *buffer) {
    
    assert (buffer);
    
    printf ("valeur %d\n", buffer->buffer[buffer->tail]);
    
    buffer->tail += 1 % TAILLE_BUFFER  ;
    
    buffer->full = false;
    
    return 0;
}

/* buffer is full if head < tail
*
*/

 int buffer_full (struct st_buffer *buffer) {
    
    return (buffer->full);
}

void buffer_free (struct st_buffer *buffer) {
    assert (buffer);
    free(buffer);
} 

int main(int argc, char **argv)
{
    
    uint8_t i = 2;
    i = i % 10;
    printf("i : %d", i);
    
    uint8_t *buffer = malloc (sizeof(uint8_t) * TAILLE_BUFFER);
    struct st_buffer *cbuf = buffer_init(buffer, TAILLE_BUFFER);

    buffer_put(cbuf, 15);
    buffer_put(cbuf, 255);
    buffer_get(cbuf);
    buffer_get(cbuf);
    
    return 0;
}
